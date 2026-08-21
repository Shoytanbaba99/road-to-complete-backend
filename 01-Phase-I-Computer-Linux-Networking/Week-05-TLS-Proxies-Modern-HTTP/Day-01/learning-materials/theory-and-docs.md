### 1. Cryptographic Goals (Confidentiality, Integrity, Authenticity)

Modern secure communication (like TLS) is built upon fulfilling three distinct cryptographic properties. If any one of these fails, the entire security model collapses.

- **Confidentiality:** The guarantee that the payload is mathematically illegible to any unauthorized third party observing the wire. This protects data in transit from eavesdropping or packet sniffing (e.g., via `tcpdump`).

- **Integrity:** The guarantee that the payload has not been modified, truncated, or corrupted in transit. If an attacker intercepts a packet and flips a single bit, the receiver must be able to detect the anomaly and reject the payload.

- **Authenticity:** The guarantee that the entity you are communicating with is definitively who they claim to be. It prevents Man-in-the-Middle (MitM) attacks where an adversary intercepts a connection and impersonates the destination server.

### 2. Hashing vs. Encryption vs. Signing

These are the discrete mathematical mechanisms used to achieve the goals listed above. They are frequently confused but serve strictly separate purposes.

**Hashing (Provides Integrity)**
A cryptographic hash function is a strictly **one-way** mathematical algorithm.

- **Mechanism:** It maps an input of arbitrary size (from a 1-byte string to a 10-gigabyte file) to a deterministic, fixed-size bit array (the hash value or digest).
- **Properties:** It must be collision-resistant (computationally infeasible to find two different inputs that produce the same output) and exhibit the avalanche effect (changing a single bit of the input radically changes the entire output).
- **Limitation:** A hash cannot be "decrypted" back into the original plaintext.

**Encryption (Provides Confidentiality)**
Encryption is a **two-way** mathematical algorithm.

- **Mechanism:** It maps plaintext into ciphertext using a cryptographic key, and maps that ciphertext back into plaintext using the corresponding decryption key.
- **Limitation:** Encryption alone does not prove who encrypted the data, nor does it guarantee the data hasn't been maliciously altered (which is why modern protocols use Authenticated Encryption with Associated Data, or AEAD, combining encryption with a Message Authentication Code).

**Signing (Provides Authenticity and Non-Repudiation)**
Digital signing relies on asymmetric (public-key) cryptography.

- **Mechanism:** To sign a payload, the sender first hashes the data. Then, the sender encrypts that hash using their **Private Key**.
- **Verification:** The receiver decrypts the signature using the sender's **Public Key** to retrieve the hash. The receiver then independently hashes the downloaded payload. If the two hashes match perfectly, the receiver has mathematically proven two things: the payload was not altered (Integrity), and it could only have been created by the entity holding the Private Key (Authenticity).

### Phase 1: The Generation Trap

#### The Core Problem Statement

In Week 4, you mastered DNS and HTTP/1.1 over raw TCP streams. You saw that when two machines communicate across the global Internet, their IP packets traverse an uncoordinated, multi-hop mesh of intermediate networks—local Wi-Fi access points, Internet Service Providers (ISPs), national backbones, undersea fiber cables, and transit routers.

Every single router along that path physically reads, buffers, and forwards your packets.

Because the underlying physical and network layers are completely open and shared, any adversary with physical or logical access to any router, switch, Wi-Fi radio wave, or optical tap along the transmission path can perform three distinct hostile actions:

1. **Passive Eavesdropping:** The adversary reads the payload of every passing packet in plaintext (extracting credit card numbers, passwords, session tokens, or confidential emails) without altering the packets or alerting either endpoint.
2. **Active In-Transit Tampering:** The adversary intercepts passing packets, modifies the raw binary payload (e.g., changing a bank account destination number inside a wire transfer or injecting malicious JavaScript into an HTML payload), recalculates the TCP/IP checksums so the operating system accepts them, and forwards the corrupted packet to the destination.
3. **Impersonation / Spoofing:** The adversary crafts brand-new packets claiming to originate from an arbitrary source IP address or domain (e.g., claiming to be `bank.com`), intercepting queries and responding with entirely fabricated records without ever communicating with the true destination server.

To communicate safely across an untrusted, hostile network, we must achieve three distinct security guarantees:

- **Confidentiality:** No unauthorized third party between the sender and receiver can read the message.
- **Integrity:** The receiver can mathematically prove that the message was not altered, truncated, corrupted, or injected in transit.
- **Authenticity:** The receiver can mathematically prove that the message originated from the exact, specific identity it claims to come from, and not an impostor.

Furthermore, we must navigate three distinct mathematical primitives that each address different facets of this triad: **Hashing**, **Encryption**, and **Digital Signing**.

---

### Phase 2: The Isomorphic Anchor & Exhaustive Theory

#### The Isomorphic Physical Analogy: The Royal Courier & Sealed Vault

Imagine a Medieval Kingdom where documents are transported across hostile, bandit-controlled wilderness:

```
+---------------------------------------------------------------------------------------+
| THE CRYPTOGRAPHIC TRIAD ANALOGY                                                       |
|                                                                                       |
| 1. ENCRYPTION (Confidentiality):                                                      |
|    Placing the letter inside a locked steel box. Only someone with the matching key   |
|    can open the box and read the words. Bandits see only solid steel.                 |
|                                                                                       |
| 2. HASHING (Fingerprinting / Collision-Resistant Digest):                             |
|    Taking a document, grinding it into a fine chemical powder, and weighing the exact |
|    molecular residue. Every unique document produces a unique powder weight.          |
|    You cannot turn the powder back into the document (One-Way).                       |
|                                                                                       |
| 3. DIGITAL SIGNATURE (Authenticity + Integrity):                                      |
|    The King grinds the document into a powder (Hash), presses his unique, impossible- |
|    to-forge Royal Signet Ring (Private Key) into hot wax over that powder, creating a  |
|    Seal. Anyone in the realm possessing the public Royal Crest (Public Key) can verify |
|    that the King personally sealed that exact document.                               |
+---------------------------------------------------------------------------------------+

```

---

### Exhaustive Architectural Comparison: Hashing vs. Encryption vs. Signing

```
+---------------------------------------------------------------------------------------------------+
| PRIMITIVE       | REVERSIBLE? | INPUT KEYS?            | PRIMARY GOAL                             |
+-----------------+-------------+------------------------+------------------------------------------+
| Hash Function   | NO (One-Way)| None                   | Integrity / Fixed-length fingerprint     |
+-----------------+-------------+------------------------+------------------------------------------+
| HMAC            | NO (One-Way)| Shared Symmetric Key   | Keyed Integrity & Authentication         |
+-----------------+-------------+------------------------+------------------------------------------+
| Encryption      | YES (Two-Way| Symmetric or Asymmetric| Confidentiality (Hiding data)            |
+-----------------+-------------+------------------------+------------------------------------------+
| Digital Signing | N/A         | Private (Sign) /       | Authenticity, Integrity, Non-Repudiation |
|                 |             | Public (Verify)        |                                          |
+---------------------------------------------------------------------------------------------------+

```

```
                          CRYPTOGRAPHIC PRIMITIVES IN ACTION

1. HASHING:
   Plaintext Message ──► [ SHA-256 Algorithm ] ──► Fixed 256-bit Digest (Irreversible)

2. ENCRYPTION & DECRYPTION (Two-Way):
   Plaintext Message ──► [ Encrypt with Key K ] ──► Ciphertext (Unreadable)
   Ciphertext        ──► [ Decrypt with Key K ] ──► Plaintext Message

3. DIGITAL SIGNING & VERIFICATION:
   Message ──► [ SHA-256 ] ──► Digest ──► [ Encrypt with Sender Private Key ] ──► Signature

   Verification:
   Signature ──► [ Decrypt with Sender Public Key ] ──► Expected Digest (A)
   Message   ──► [ SHA-256 Hash Function ]       ──► Computed Digest (B)
   Check: If (A == B) -> Signature is VALID and Message is UNTAMPERED.

```

#### 1. Cryptographic Hash Functions (`SHA-256`, `SHA-3`, `BLAKE2`)

A mathematical algorithm that maps arbitrary-length binary data to a fixed-size bit string (e.g., SHA-256 outputs 256 bits / 32 bytes) satisfying three mathematical properties:

- **Pre-image Resistance (One-Way):** Given a digest $H$, it is computationally infeasible to find any message $M$ such that $\text{Hash}(M) = H$.
- **Second Pre-image Resistance (Weak Collision Resistance):** Given a specific input $M_1$, it is computationally infeasible to find a different $M_2$ such that $\text{Hash}(M_1) = \text{Hash}(M_2)$.
- **Collision Resistance (Strong):** It is computationally infeasible to find _any_ two arbitrary distinct messages $M_1 \ne M_2$ such that $\text{Hash}(M_1) = \text{Hash}(M_2)$ (defending against the Birthday Paradox attack).
- **The Avalanche Effect:** Changing a single bit in the input message flips approximately 50% of the output bits in the digest pseudorandomly.

#### 2. Encryption (Symmetric & Asymmetric)

- **Goal:** **Confidentiality**.
- Takes plaintext message $P$ and key $K$, producing ciphertext $C = E(K, P)$.
- The ciphertext $C$ reveals zero mathematical information about the original plaintext $P$.
- Given $C$ and key $K$, the decryption function perfectly recovers $P = D(K, C)$.

#### 3. Digital Signatures (RSA, ECDSA, Ed25519)

- **Goal:** **Authenticity, Integrity, and Non-Repudiation**.
- Operates on asymmetric key pairs: **Private Key** (kept secret by sender) and **Public Key** (distributed publicly).
- **Signing Algorithm:**

1. The sender hashes the message: $H = \text{SHA256}(M)$.
2. The sender signs the hash $H$ using their **Private Key**: $S = \text{Sign}(K_{\text{private}}, H)$.
3. The sender transmits both the message $M$ and the signature $S$ across the network.

- **Verification Algorithm:**

1. The receiver receives $M$ and $S$.
2. The receiver independently computes $H' = \text{SHA256}(M)$.
3. The receiver verifies $S$ using the sender's **Public Key**: $\text{Verify}(K_{\text{public}}, H', S)$.
4. If the verification succeeds:

- **Integrity is proven:** Not a single bit of $M$ was altered in transit.
- **Authenticity is proven:** Only the entity possessing $K_{\text{private}}$ could have generated $S$.
- **Non-Repudiation is established:** The sender cannot claim they did not send the message.

---

### Phase 3: The Empirical Proof

Run these diagnostic commands locally using OpenSSL to observe hashing, encryption, signing, and verification directly.

---

#### 1. Proving the Avalanche Effect of Hashing

Create two files differing by only a single character (one letter 'A' vs 'B'):

```bash
echo -n "Transfer $1000 to Account A" | sha256sum
echo -n "Transfer $1000 to Account B" | sha256sum

```

**Output:**

```text
461d36d4f6c44955bca9e663a8a9be12f9b8c049449f80ba0a5d4d38cbde25b4  -
eb72049e29a9972323e07085a53be4d7eec26f04746f332c1c1f54be693fb1db  -

```

_Notice:_ Changing one byte produced two completely uncorrelated 64-character hexadecimal digests.

---

#### 2. Testing Two-Way Symmetric Encryption & Decryption

Encrypt a secret message using AES-256-CBC and decrypt it back:

```bash
# 1. Encrypt plaintext using a passphrase
echo "Secret Credit Card Data: 4111-2222-3333-4444" | openssl enc -aes-256-cbc -pbkdf2 -a -out secret.enc -pass pass:MyPassword123

# 2. Inspect the ciphertext (Completely unreadable gibberish)
cat secret.enc

# 3. Decrypt the ciphertext back to plaintext
openssl enc -d -aes-256-cbc -pbkdf2 -a -in secret.enc -pass pass:MyPassword123

```

---

#### 3. Generating Asymmetric Keys, Signing, and Verifying

Generate an elliptic curve key pair, sign a document, verify the signature, and prove that tampering invalidates the signature:

```bash
# 1. Generate an ECDSA Private Key (Prime256v1)
openssl ecparam -name prime256v1 -genkey -noout -out private_key.pem

# 2. Extract the corresponding Public Key
openssl ec -in private_key.pem -pubout -out public_key.pem

# 3. Create a document
echo "Official Wire Order: Send $50,000 to Supplier" > document.txt

# 4. Sign the document using the Private Key
openssl dgst -sha256 -sign private_key.pem -out document.sig document.txt

# 5. Verify the signature using the Public Key
openssl dgst -sha256 -verify public_key.pem -signature document.sig document.txt
# Output: Verified OK

# 6. TAMPER TEST: Modify the document by 1 byte
echo "Official Wire Order: Send $90,000 to Supplier" > document.txt

# 7. Re-verify the original signature against the tampered document
openssl dgst -sha256 -verify public_key.pem -signature document.sig document.txt
# Output: Verification Failure

```

---

### Phase 4: Architecture & Deliberate Breakage

Here is a self-contained Python script implementing all three cryptographic primitives from scratch using the standard library `hashlib`, `hmac`, and `secrets`.

#### The Cryptographic Triad Implementation (`crypto_demo.py`)

```python
import hashlib
import hmac
import secrets

# 1. HASHING (One-way digest)
def compute_hash(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()

# 2. KEYED INTEGRITY & AUTHENTICITY (HMAC - Hash-based Message Authentication Code)
def create_hmac(secret_key: bytes, message: bytes) -> str:
    return hmac.new(secret_key, message, hashlib.sha256).hexdigest()

def verify_hmac(secret_key: bytes, message: bytes, expected_mac: str) -> bool:
    # Use hmac.compare_digest to prevent timing attacks
    actual_mac = create_hmac(secret_key, message)
    return hmac.compare_digest(actual_mac, expected_mac)

# 3. SYMMETRIC ENCRYPTION (One-Time Pad XOR Stream Demonstration)
def xor_encrypt_decrypt(key: bytes, message: bytes) -> bytes:
    """XOR encryption/decryption: C = P ^ K and P = C ^ K"""
    return bytes(b ^ key[i % len(key)] for i, b in enumerate(message))

def run():
    print("=== 1. HASHING DEMO ===")
    msg = b"Transfer $100"
    h1 = compute_hash(msg)
    print(f"Original Hash : {h1}")

    print("\n=== 2. CONFIDENTIALITY (ENCRYPTION) DEMO ===")
    sym_key = secrets.token_bytes(32)
    plaintext = b"Super Confidential Password: 12345"
    ciphertext = xor_encrypt_decrypt(sym_key, plaintext)
    decrypted = xor_encrypt_decrypt(sym_key, ciphertext)
    print(f"Ciphertext (Hex) : {ciphertext.hex()}")
    print(f"Decrypted Result : {decrypted.decode()}")

    print("\n=== 3. AUTHENTICITY & INTEGRITY (HMAC) DEMO ===")
    shared_key = b"super-shared-secret-key-1234567"
    order = b"Action: GrantAdminRole; User: Alice;"
    mac = create_hmac(shared_key, order)
    print(f"Payload   : {order.decode()}")
    print(f"HMAC Tag  : {mac}")

    is_valid = verify_hmac(shared_key, order, mac)
    print(f"Verification Check: {'PASSED' if is_valid else 'FAILED'}")

if __name__ == "__main__":
    run()

```

---

#### 3 Ways to Inject Failure & Observe the Breakage

```
+-----------------------------------------------------------------------------------------+
| SOWING CHAOS: 3 CRYPTOGRAPHIC BREAKAGE EXPERIMENTS                                      |
+---+-----------------------------+-------------------------------+-----------------------+
| # | Sabotage Action             | Cryptographic Failure Point   | What You Observe      |
+---+-----------------------------+-------------------------------+-----------------------+
| 1 | Tamper with HMAC message    | Integrity Violation           | `verify_hmac()`       |
|   | Modify `order` to           | Received digest does not      | returns `False`;      |
|   | `User: Bob;`                | match computed HMAC.          | forgery rejected.     |
+---+-----------------------------+-------------------------------+-----------------------+
| 2 | Decrypt with Wrong Key      | Confidentiality Failure       | Output is corrupted   |
|   | Pass random key into        | Mathematics of key decryption | unreadable binary     |
|   | `xor_encrypt_decrypt()`     | fails; garbled output.        | noise/garbage.        |
+---+-----------------------------+-------------------------------+-----------------------+
| 3 | Timing Attack Vulnerability | Side-Channel Attack Vector    | Attacker learns MAC   |
|   | Use standard `==` instead   | Early return on mismatch      | byte-by-byte via      |
|   | of `compare_digest()`.      | leaks timing information.     | microsecond analysis. |
+---+-----------------------------+-------------------------------+-----------------------+

```

---

### Phase 5: The Invariant & The Capstone

#### The Non-Negotiable Invariant

> **No single cryptographic primitive can simultaneously provide confidentiality, integrity, and authenticity on its own.**
>
> - **Encryption** hides data, but does not prevent an adversary from flipping bits in ciphertext.
> - **Hashing** creates a unique fingerprint, but provides zero secrecy and can be recomputed by any attacker.
> - **Digital Signatures & HMACs** prove origin and detect tampering, but leave the message completely visible in plaintext unless layered with encryption.
>   Secure protocols (like TLS) work by chaining all three primitives together.

---

#### Day 1 Capstone Challenge

Build a Python script (under 50 lines) that proves you understand the difference between **Hashing**, **Encryption**, and **HMAC/Signing**:

1. **Step 1:** Define a secret shared key and an invoice message `b"Invoice #101: Pay $500 to Alice"`.
2. **Step 2:** Generate the SHA-256 hash of the invoice, an HMAC of the invoice, and an encrypted ciphertext of the invoice.
3. **Step 3:** Simulate an active Man-in-the-Middle attacker who intercepts the network transmission and alters the invoice to `b"Invoice #101: Pay $500 to Mallory"`.
4. **Step 4:** Write the receiver's verification logic to prove that:

- The attacker **can** fool a naive SHA-256 hash check by re-hashing the message.
- The attacker **cannot** fool the HMAC check without knowing the secret key, causing the receiver to reject the tampered payload.
