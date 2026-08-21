import hashlib, hmac, os
from cryptography.fernet import Fernet

key = Fernet.generate_key()
cipher = Fernet(key)
msg = b"Invoice #101: Pay $500 to Alice"

h_val = haslib.sha256(msg).hexdigest()
hmac_val = hmac.new(key, msg, hashlib.sha256).hexdigest()
ciphertext = cipher.encrypt(msg)

tampered_msg = b"Invoice #101: Pay $5000 to Mallory"
fake_hmac_val = hmac.new(key, tampered_msg, hashlib.sha256).hexdigest()
fake_h_val = hashlib.sha256(tampered_msg).hexdigest()

print("--- [Receiver Verification] --- ")

if(fake_h_val == hashlib.sha256(tampered_msg).hexdigest()):
    print("Hash Matches tampered content")
recv_hmac = hmac.new(key, tampered_msg, hashlib.sha256).hexdigest()

if(hmac.compare_digest(recv_hmac, fake_hmac_val)):
    print("HMAC Matches, accepted tempered message.")
else:
    print("HMAC check: REJECTED! Message tampered with.")

decrypted_msg = cipher.decrypt(ciphertext)
print(f"[INFO] Decrypted message: {decrypted_msg.decode()}")