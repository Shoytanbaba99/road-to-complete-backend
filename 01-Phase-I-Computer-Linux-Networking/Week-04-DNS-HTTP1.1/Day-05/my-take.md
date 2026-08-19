## 🧠 Core Mental Model

1xx represents informational responses, 2xx represents success, 3xx represents redirection, 4xx represents client errors, and 5xx represents server errors.

Cookies are a stateful header mechanism (`Set-Cookie` from server, `Cookie` from client) used to persist session tokens and stateful user information on the client side across stateless HTTP requests.

HTTP Caching allows clients and proxies to store resource representations locally. 
- **`If-None-Match` (Conditional GET):** Client sends its cached ETag string to the server. If the server's current resource ETag matches, the server returns `304 Not Modified` with an empty response body, instructing the client to reuse its local cache copy.
- **`If-Match` (Concurrency Control):** Client sends the known ETag during `PUT`/`PATCH` requests. If the server resource was modified by another client in the meantime (ETags don't match), the server rejects the update with `412 Precondition Failed` to prevent mid-air collisions.
