## 💡 Key Takeaways

Http 1.1 is a protocol that allows clients and servers to communicate in the application layer over the web. while tcp stream is a transport layer protocl it does not have any knowledge of like, what the data is, how to handle it, or how to interpret it.

Thats https is responsible for for all these, with its header and status line or request line, the status line contains the method, the url, and the version of the protocol. The url can also contain query parameters, which are used to pass data to the server.

the request general looks like

```
GET / HTTP/1.1
Host: example.com
/r/n/r/n
body
```

and the response is also similar

```HTTP/1.1 200 OK
Content-Type: text/html
<html></html>
<body>
</body>
```
