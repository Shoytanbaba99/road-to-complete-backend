## 🧠 Core Mental Model

HTTP/2 is a major revision of the HTTP network protocol, some things it impprovde over HTTP/1.1 are:

- Multiplexing: You can allow multiple requests and responses to be in flight at the same time over a single TCP connection. All of each are called stream with unique stream identifiers. even numbered streams are for server and odd numbered streams are for client.
- Header Compression: HTTP/2 uses HPACK compression to reduce the overhead of HTTP headers, so instead of repeatedly sending teh same headers, it can send an index to the required function of a header. There are static tables and Dynamic tables, static ones are built into the protocol and dynamic ones are built during the connection.
- Server Push: Something about automatically sending promise of dependencies without needing to ask from the side of the clients.
- Moving to Binary: instead of using /r/n/r/n or parsing ascii it uses binary framing, which is more efficient and easier to parse for the CPU.

Then comes HTTP/3 which drops TCP connection entirely and uses QUIC protocol over UDP. Because a single lost package form any stream from a tcp connection would stop the other streams from being processed, QUIC allows for multiplexing without head of line blocking.
QUIC has 0/1 RTT connection establishment, meaning it can establish a connection with 0 or 1 round trip time. Nothing like the tcp connections syn/syn ack/ack and tls and certificate and diffie hellman etc. it has 1.3 TLS built in as well. HTTP/3 also contains connection migration, meaning if a client changes its IP address, it can continue the connection without needing to reestablish it.

Also, the reason why GET is safe for 0 RTT is because it is idempotent, multiple stream requests can be sent without worrying about the server state changing.
