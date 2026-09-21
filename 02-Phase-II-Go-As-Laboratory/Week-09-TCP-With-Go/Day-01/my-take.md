Generally. the way nic and kernel communicates is, we create a socket, bind the ip aand prot address to the socket, make it start listening, and accepting and after accpeting it automatically creates a new socket for the connection. The kernel then uses the nic to send and receive data through the socket.

in Go you have, net.listen() which creates a socket and binds teh ip and port and starts listening.
Then you have net.accept() which accepts the connection and creates a new socket for the connection returning a net.Conn object which you can use to send and receive data.
