## 🧠 Core Mental Model

The Load balancer is a service/server that processes teh incoming requests and distributes them to the backend servers. It can also handle TLS termination, meaning it can decrypt incoming TLS traffic and forward it to the backend servers in plain HTTP. This is useful for offloading the TLS processing from the backend servers. Its also a reverse proxy ;3

Proxy servers are used to expose a single endpoint to the outside world while routing requests to multiple backend servers.
