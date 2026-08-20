## 🧠 Core Mental Model

So, you got http/1.1 persistent connection that allows you to reuse the same channel to send multiple requests.
the curl -v(verbose) allows you to see application layer protocols and it basically crafts the request for you and sends it to the server and then you can see the response from the server.

the nc is a raw socket that allows you to send raw data to the server and you have to craft the request yourself and then send it to the server and then you can see the response from the server.

While tcpdump is a packet sniffer that allows you to capture packets on the network and then you can see the request and response from the server.

S - SYN
. - ACK
S. - SYN-ACK
P - PSH
F - FIN
F. - FIN-ACK
