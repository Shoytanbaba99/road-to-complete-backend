# Week 5 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of TLS Handshake Architecture, X.509 Certificates, Certificate Authorities (CAs), Certificate Chains, Subject Alternative Name (SAN) Hostname Verification, and Python TLS Server ([`tls_server.py`](learning-materials/tls_server.py)).

---

## 🌐 Certificate Chain & Hostname Verification Protocol

```text
[ TRUST ANCHOR CHAIN (PKI) ]
  Root CA Certificate (Pre-installed in OS / Browser Trust Store)
    └── Signs ──► Intermediate CA Certificate
                    └── Signs ──► Leaf Server Certificate (server.crt)

[ TLS HANDSHAKE & VERIFICATION FLOW ]
  Client                                           Server
    ├── ClientHello (Supported TLS Versions, Cipher Suites, Random) ──►│
    │◄── ServerHello (Selected Cipher Suite, Server Random) ──────────┤
    │◄── Certificate (Leaf Cert + Intermediate CA Chain) ─────────────┤
    │
    ▼ CLIENT VALIDATION STAGE ▼
    ├── 1. Verify Chain: Trace Leaf ➔ Intermediate ➔ Root CA in Trust Store
    ├── 2. Verify SAN: Ensure requested hostname matches SAN extension field
    ├── 3. Verify Validity: Check NotBefore & NotAfter timestamps & revocation
    │
    ├── Key Exchange (ECDHE Parameters) ─────────────────────────────►│
    │◄── Finished (Encrypted Handshake Complete) ─────────────────────┤
    │
    ▼ ENCRYPTED APPLICATION DATA LAYER (HTTP/1.1 or HTTP/2) ▼
```

---

## 1. Public Key Encryption vs. Digital Signature Summary

| Operation | Key Used by Initiator | Goal / Guarantee | Who Can Read / Verify? |
|---|---|---|---|
| **Public Key Encryption** | Recipient's Public Key | 🔒 **Confidentiality** (Privacy) | ONLY the Recipient holding matching Private Key |
| **Digital Signing** | Sender's Private Key | 🪪 **Authenticity & Integrity** | EVERYONE holding matching Public Key |
