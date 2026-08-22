it was covered in previous days, but I wanted to summarize the key points here for quick reference.

### 3. Key `curl` TLS Flags for Debugging

| Flag                           | Exact Behavior                                                                                            | When to Use in Backend Debugging                                                       |
| ------------------------------ | --------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| `curl -v https://...`          | Verbose output showing TLS handshake steps, ALPN negotiation (`h2` vs `http/1.1`), and cert verification. | General debugging.                                                                     |
| `curl -k` (or `--insecure`)    | **Bypasses all certificate validation.** Disables chain check, expiration check, and hostname check.      | Local testing with self-signed certs.                                                  |
| `curl --cacert path/to/ca.crt` | Forces curl to use a specific CA file instead of `/etc/ssl/certs/`.                                       | Testing custom internal enterprise or staging CAs.                                     |
| `curl --resolve host:port:ip`  | Overrides DNS for a single request while preserving the SNI and Host header.                              | Testing a specific server/load-balancer IP behind a CDN without altering `/etc/hosts`. |
