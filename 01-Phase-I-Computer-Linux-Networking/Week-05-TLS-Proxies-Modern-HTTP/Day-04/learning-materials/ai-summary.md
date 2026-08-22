# Week 5 - Day 4 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of HTTPS Inspection, `curl` Verbose Protocol Debugging (`-v`, `-k`, `--cacert`, `--resolve`), OpenSSL Certificate Chain Examination (`openssl s_client`), and SNI (Server Name Indication).

---

## 🌐 `curl` TLS Debugging Command Matrix

| Command Flag | Exact Behavior | Engineering Use Case |
|---|---|---|
| `curl -v https://example.com` | Prints TLS handshake steps, ALPN negotiation (`h2`/`http1.1`), and cipher suite. | General protocol & header debugging. |
| `curl -k https://example.com` | Bypasses certificate verification (ignores expiry, CA trust, and SAN mismatch). | Local dev/test environments with self-signed certs. |
| `curl --cacert ca.crt https://...` | Forces `curl` to trust a specific root/intermediate CA certificate file. | Staging environments with internal private enterprise CAs. |
| `curl --resolve host:port:ip` | Overrides DNS resolution for host:port to target IP while preserving SNI and Host header. | Testing individual backend nodes behind a CDN/Load Balancer. |

---

## 1. OpenSSL Certificate Inspection
```bash
# Connect and print full TLS certificate chain
openssl s_client -connect example.com:443 -showcerts

# Inspect remote certificate expiration date
openssl s_client -connect example.com:443 2>/dev/null | openssl x509 -noout -dates
```
