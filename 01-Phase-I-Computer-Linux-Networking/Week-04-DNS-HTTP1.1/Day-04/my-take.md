## 🧠 Core Mental Model

You got, GET, POST, PUT, DELETE, PATCH, HEAD, OPTIONS.

GET AND HEAD AND OPTIONS are safe methods. They are read-only methods. They do not change the state of the server. while PUT and DELETE are idempotent it means that if you make the same request multiple times, the result will be the same.
and then you have patch and POST which are not idempotent. It means that if you make the same request multiple times, the result will stack.

Then there is also information about preference of the client on how he would like his response to look like. It is called content negotiation. The client can specify the type of response he wants to receive from the server. The server can then respond with the appropriate content type. text/plain;q=0.5, text/html, application/json;q=0.9, _/_;q=0.1
etc

there is also the header vary which is used by the server to indicate that the response may vary based on the value of certain request headers.
