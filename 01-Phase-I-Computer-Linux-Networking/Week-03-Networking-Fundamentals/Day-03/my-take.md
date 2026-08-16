So, to connect everything. From the data link layer, you have frames which contain the source MAC address and destination MAC address, while the IP packet inside the frame contains the destination IP address. The frame is sent to the router, if the destination IP address is on the same network, meaning the network address is the same, then ARP sends a special frame to identify which local computer has the destination MAC address.

Then, if they are not on the same local network, the router will send the packet to the next router that it finds in its routing table, and this process continues until the packet reaches the destination computer.

At each router, the old frame is discarded and the IP packet is encapsulated in a new frame for the next hop.

In the destination computer, how will it identify which application requires this packet? For that, we use ports. Each application that is communicating over the network uses a port number, and the destination computer will check the destination port number in the TCP or UDP header to determine which application should receive the data.

Socks are file descriptors that os allocates and gives so that the application can use them to send and receive data. The application will bind to specific port number and the operating system will use that port number to identify which application should receive the data.
