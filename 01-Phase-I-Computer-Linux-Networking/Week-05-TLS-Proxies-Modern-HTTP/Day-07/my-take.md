1. The Edge Handshake & Cryptography
   When a browser connects to a global service, it talks to a geographically local Edge CDN rather than the distant origin. The browser initiates a TLS handshake (ClientHello) containing an ephemeral Diffie-Hellman public key and the target domain (SNI). The Edge responds with its own Diffie-Hellman key and a leaf certificate validating its authority over the domain. The client's OS mathematically verifies this certificate chain against its pre-installed Root Trust Store. Once verified, both sides independently calculate an identical Master Shared Secret. This secret is fed into a symmetric AEAD cipher (like AES-GCM or ChaCha20-Poly1305) to encrypt all subsequent HTTP data with sub-millisecond local latency.

2. The Proxy Identity Invariant (Layer 7)
   Because the Edge Proxy terminates the client's TCP connection and opens a brand new TCP connection to the backend origin, the origin server's kernel only sees the proxy's IP address. To prevent the origin application from becoming blind to the user's true identity, the proxy injects HTTP metadata headers—specifically X-Forwarded-For (Client IP) and X-Forwarded-Proto (Original Scheme)—into the plaintext request before forwarding it over the private backend tunnel.

3. The Layer 4 vs. Layer 7 Boundary
