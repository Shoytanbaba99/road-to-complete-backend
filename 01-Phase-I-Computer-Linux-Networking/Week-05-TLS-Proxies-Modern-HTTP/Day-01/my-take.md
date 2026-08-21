## 🧠 Core Mental Model

In network security, we require three fundamental guarantees: **Confidentiality** (eavesdropping prevention), **Integrity** (tamper prevention), and **Authenticity** (identity verification).

- **Encryption** (Symmetric/Asymmetric) provides **Confidentiality** by converting plaintext into ciphertext so unauthorized listeners cannot read the data.
- **Hashing** (e.g., SHA-256) is a one-way deterministic algorithm that provides **Integrity** (detecting data changes), but does NOT provide confidentiality.
- **HMAC (Hash-based Message Authentication Code):** Sender and receiver share a secret key. The sender hashes the message with the secret key (`HMAC`). The receiver re-computes the HMAC using the shared key. If they match, it proves both **Integrity** and **Authenticity** (without encryption, HMAC alone does not provide confidentiality).
- **Digital Signatures (Public Key Cryptography):** Provides **Integrity**, **Authenticity**, and **Non-Repudiation**.
  - **Signing:** Sender hashes the message and encrypts the hash with their **Private Key** to create a signature.
  - **Encryption:** The message + signature are encrypted with the recipient's **Public Key** for confidentiality.
  - **Verification:** Recipient decrypts payload with their **Private Key**, decrypts the signature using sender's **Public Key**, and compares the hash with the decrypted message hash. If it matches, confidentiality, integrity, and authenticity are achieved!
