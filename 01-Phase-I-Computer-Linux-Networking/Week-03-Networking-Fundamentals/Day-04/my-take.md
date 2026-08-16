## 🧠 Core Mental Model

UDP is a very conventional and minimal protocal that is used to send data over the network. It is a connectionless protocol, meaning that it does not establish a connection before sending data. It is also unreliable, meaning that it does not guarantee that the data will be delivered to the destination. It consists of a header and a payload. The header contains the source port, destination port, length, and checksum. The payload contains the data that is being sent.

There is no established connection, the server opens a port and waits, and the clients blasts their datagrams into the port. The server will receive the datagrams and process them. The server does not know how many clients are sending data, and the clients do not know if the server is receiving the data.
