# Week 5 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Cryptographic Primitives, Confidentiality vs. Integrity vs. Authenticity, Hashing vs. Encryption vs. Digital Signatures, HMAC verification, and Fernet Symmetric Encryption ([`enc.py`](learning-materials/enc.py)).

---

## 🌐 The Cryptographic Security Triad

| Security Goal | Definition | Cryptographic Primitive Used |
|---|---|---|
| 🔒 **Confidentiality** | Prevents unauthorized eavesdropping on payload bytes. | Symmetric (AES, Fernet) or Asymmetric Encryption (RSA/ECC) |
| 🛡️ **Integrity** | Ensures payload has not been modified or corrupted in transit. | Cryptographic Hashes (SHA-256) & MACs (HMAC) |
| 🪪 **Authenticity** | Verifies the true identity of the message originator. | Digital Signatures (Private Key Signing) & HMAC |

---

## 1. Hashing vs. Encryption vs. Signing

```text
[ HASHING (One-Way) ]
  Plaintext ──► SHA-256 ──► Fixed Hash Value (Irreversible, Integrity Only)

[ SYMMETRIC ENCRYPTION (Two-Way) ]
  Plaintext + Secret Key ──► AES-Cipher ──► Ciphertext (Reversible, Confidentiality Only)

[ DIGITAL SIGNATURE (Asymmetric Signing & Verification) ]
  Sender:   Payload ➔ Hash ➔ Encrypt with Sender Private Key ──► Digital Signature
  Receiver: Decrypt Signature with Sender Public Key ➔ Compare Hash ──► Integrity & Authenticity!
```

---

## 2. Lab Verification ([`enc.py`](learning-materials/enc.py))
- Demonstrates Fernet symmetric encryption for payload confidentiality.
- Demonstrates HMAC hash verification (`hmac.compare_digest`) rejecting tampered messages.
