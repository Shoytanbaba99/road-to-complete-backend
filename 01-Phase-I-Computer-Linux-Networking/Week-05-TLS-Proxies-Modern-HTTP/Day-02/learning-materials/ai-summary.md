# Week 5 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Symmetric Encryption (AES-GCM), Asymmetric Cryptography (RSA/ECC), Hybrid Encryption Architecture, and Diffie-Hellman Key Exchange (`ECDHE`).

---

## 🌐 Hybrid Encryption Architecture (TLS Paradigm)

```text
[ PHASE 1: ASYMMETRIC KEY EXCHANGE (Handshake Phase - CPU Heavy, Solves Key Sharing) ]
  Client                                           Server
    ├── Public Key Exchange (ECDHE / X25519) ──────►│
    │◄── Public Key Exchange (ECDHE / X25519) ──────┤
    │
    ▼ Both independently compute identical Secret Key ▼
    [ 256-bit Symmetric Session Key Generated ]

[ PHASE 2: SYMMETRIC BULK ENCRYPTION (Data Phase - High Speed, Minimal CPU) ]
  Client                                           Server
    ├── Encrypted HTTP Payload (AES-256-GCM) ──────►│ Decrypts using Session Key
    │◄── Encrypted Response Payload (AES-256-GCM) ──┤ Encrypts using Session Key
```

---

## 1. Symmetric vs. Asymmetric Trade-Off Matrix

| Feature | Symmetric (AES-GCM / ChaCha20) | Asymmetric (RSA / ECC / X25519) |
|---|---|---|
| **Keys** | Single Shared Secret Key | Public / Private Key Pair |
| **Speed** | ⚡ Extremely Fast (Hardware AES-NI) | 🐢 Slow (~1000x slower computation) |
| **Use Case** | Bulk Data Encryption (HTTP Body/Stream) | Identity Verification & Key Exchange |
| **Key Distribution** | Difficult (requires secure channel) | Easy (Public Key is freely distributed) |

---

## 2. Diffie-Hellman Key Exchange (ECDHE)
- **Ephemeral Keys:** Single-use key pairs generated per connection.
- **Perfect Forward Secrecy (PFS):** Even if a server's long-term private key is compromised in the future, past recorded sessions cannot be decrypted because each session key was ephemeral.
