# Week 5 - Day 3: TLS Handshake, Certificates, CA Chains & Hostname Verification

---

## 📋 Objectives
- [x] TLS Handshake protocol & cipher suite negotiation
- [x] X.509 Certificate structure & Public Key Infrastructure (PKI)
- [x] Certificate chains (Leaf ➔ Intermediate CA ➔ Root CA in OS Trust Store)
- [x] Subject Alternative Name (SAN) extension & Hostname Verification
- [x] Run TLS/HTTPS socket wrapper server ([`tls_server.py`](learning-materials/tls_server.py))

---

## 🗺️ Day 3 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and Certificate Chain verification logic. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of TLS handshake stages, PKI chains, and Signing vs. Encryption rules. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory on TLS 1.3 RFC 8446, X.509 RFC 5280, and OpenSSL certificate generation. |
| 🛠️ [**`learning-materials/tls_server.py`**](learning-materials/tls_server.py) | **Lab:** HTTPS server using Python `ssl.SSLContext` wrapping a TCP socket with TLS certificates. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Generate self-signed CA & server certificate chain
openssl req -x509 -newkey rsa:2048 -nodes -keyout my_root_ca.key -out my_root_ca.crt -days 365

# Run TLS wrapped socket server
python3 tls_server.py &

# Query TLS server (use -k to ignore self-signed CA warning)
curl -v -k https://127.0.0.1:8443
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TLS Handshake, X.509 Certificate Chains & Hostname Verification]]` in `Engineers-Playbook/02 Permanent/`
