## 🧠 Core Mental Model

Flow control, is where the receiving device through rwnd (receive window) communicates its available buffer space to the sending device. This allows the sender to adjust its sending rate based on the receiver's capacity.

Congestion Control is the ability of sending devices to adjust their sending rate based on the intermediate routers capability of processing the packets. One way is exponentially increasing until packet falls, then decreasing it dramatically and sending packets linearly to find the perfect window which allows for highest throughput.

Then you have the concept of 4 way handshake. the side responsible for initiaing the FIN has to wait for the other side to acknowledge it, then the other side has to send its own FIN and wait for the first side to acknowledge it. This is done to ensure that both sides have finished sending data before closing the connection.

And there is the TCP KEEP ALIVE signal which sends packages around every 2 hour to ensure the connection is still alive.
