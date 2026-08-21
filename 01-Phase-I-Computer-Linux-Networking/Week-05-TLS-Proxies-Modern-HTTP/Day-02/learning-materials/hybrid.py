import os
import hashlib
import hmac
from cryptography.hazmat.primitives.asymmetric import x25519
from cryptography.hazmat.primitives.ciphers.aead import AESGCM

# ==============================================================================
# 1. THE ASYMMETRIC KEY EXCHANGE LAYER (ECDHE / X25519)
# ==============================================================================

class Endpoint:
    def __init__(self, name: str):
        self.name = name
        # Generate single-use ephemeral private key
        self._private_key = x25519.X25519PrivateKey.generate()
        self.public_key = self._private_key.public_key()
        self.session_key = None

    def export_public_bytes(self) -> bytes:
        from cryptography.hazmat.primitives import serialization
        return self.public_key.public_bytes(
            encoding=serialization.Encoding.Raw,
            format=serialization.PublicFormat.Raw
        )

    def derive_symmetric_session_key(self, peer_public_bytes: bytes):
        peer_public_key = x25519.X25519PublicKey.from_public_bytes(peer_public_bytes)
        # Compute raw Diffie-Hellman shared secret point
        raw_shared_secret = self._private_key.exchange(peer_public_key)
        
        # HKDF / Key Derivation: Turn raw DH point into a 256-bit AES key
        # Extract & Expand using SHA-256
        self.session_key = hashlib.sha256(raw_shared_secret).digest()
        print(f"[{self.name}] Derived 256-bit Symmetric Session Key: {self.session_key.hex()[:16]}...")

# ==============================================================================
# 2. THE SYMMETRIC AUTHENTICATED ENCRYPTION LAYER (AES-256-GCM)
# ==============================================================================

def encrypt_payload(session_key: bytes, plaintext: bytes) -> tuple[bytes, bytes]:
    """Returns (nonce, ciphertext_with_tag)"""
    aesgcm = AESGCM(session_key)
    # Generate 96-bit (12-byte) unique Nonce/IV
    nonce = os.urandom(12)
    ciphertext = aesgcm.encrypt(nonce, plaintext, associated_data=None)
    return nonce, ciphertext

def decrypt_payload(session_key: bytes, nonce: bytes, ciphertext: bytes) -> bytes:
    aesgcm = AESGCM(session_key)
    return aesgcm.decrypt(nonce, ciphertext, associated_data=None)

# ==============================================================================
# 3. END-TO-END EXECUTION PIPELINE
# ==============================================================================

def run_pipeline():
    print("--- STEP 1: ASYMMETRIC HANDSHAKE (ECDHE) ---")
    alice = Endpoint("Alice (Client)")
    bob = Endpoint("Bob (Server)")

    # Exchange public keys over the simulated open network wire
    wire_alice_pub = alice.export_public_bytes()
    wire_bob_pub = bob.export_public_bytes()

    # Both derive the exact same symmetric key independently
    alice.derive_symmetric_session_key(wire_bob_pub)
    bob.derive_symmetric_session_key(wire_alice_pub)

    assert alice.session_key == bob.session_key, "Keys do not match!"

    print("\n--- STEP 2: BULK SYMMETRIC ENCRYPTION (AES-GCM) ---")
    http_request = b"POST /account/transfer HTTP/1.1\r\nHost: bank.com\r\n\r\nAmount=5000"
    print(f"[Client] Transmitting Plaintext: {http_request.decode()}")

    nonce, encrypted_bytes = encrypt_payload(alice.session_key, http_request)
    print(f"[Wire] Nonce (12B)      : {nonce.hex()}")
    print(f"[Wire] Ciphertext (Hex) : {encrypted_bytes.hex()}")

    print("\n--- STEP 3: SERVER DECRYPTION & VALIDATION ---")
    recovered = decrypt_payload(bob.session_key, nonce, encrypted_bytes)
    print(f"[Server] Decrypted Body : {recovered.decode()}")

if __name__ == "__main__":
    run_pipeline()