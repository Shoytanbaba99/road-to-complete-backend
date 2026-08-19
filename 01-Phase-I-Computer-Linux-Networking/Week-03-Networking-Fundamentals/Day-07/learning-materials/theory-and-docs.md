Welcome to Day 7. This is the synthesis of everything you’ve studied in Phase I of your "Guide.md" roadmap. Today, you are moving from theoretical concepts into the terminal to prove you understand the mechanics.

Here is how you execute today's laboratory work.

### 1. Inspect Sockets with `ss`

The `ss` (socket statistics) command is the modern, much faster replacement for `netstat`. It queries the Linux kernel directly to reveal exactly what your network stack is doing.

To understand your machine's state, combine these flags:

- `ss -t`: Show only **T**CP connections.
- `ss -l`: Show **L**istening sockets (servers waiting for a connection).
- `ss -n`: Force **N**umeric output (stops `ss` from doing slow DNS lookups to resolve IPs and port names).
- `ss -p`: Show the **P**rocess ID and name holding the socket (often requires `sudo`).
- `ss -o`: Show timer information (this is how you catch a socket in the `TIME_WAIT` state you learned about yesterday).

**Your exercise:** Run `sudo ss -tlpn`. This will output every listening TCP server on your machine and the exact process attached to it.

### 2. Create a TCP Conversation with `nc`

Netcat (`nc`) is the Swiss Army knife of networking. It allows you to create raw TCP or UDP connections without writing a single line of code, bypassing the HTTP layer entirely.

1. **Start the Server:** Open a terminal and run `nc -l 8080`. Your OS immediately allocates port 8080 and enters a listening state.
2. **Start the Client:** Open a second terminal window and run `nc 127.0.0.1 8080`. This simple command forces your kernel to execute the full TCP three-way handshake locally.
3. **The Conversation:** Type a message into the client terminal and hit Enter. The OS pushes that text payload through the TCP stack, across the loopback network interface, and up to the server terminal. You are now speaking raw TCP. Press `Ctrl+C` on the client to initiate the four-way connection close.

### 3. A Packet’s Journey (Machine A to Machine B)

When you send a payload to a remote server, it traverses the entire networking stack before it even leaves your house. Here is the exact path:

- **Application (Layer 7):** Your tool (like `nc` or a browser) takes your data and hands it to an OS file descriptor (the socket).
- **Transport (Layer 4):** The TCP layer wraps the data in a segment. It adds the Source Port, Destination Port, the Sequence Number, and the Checksum.
- **Network (Layer 3):** The IP layer wraps that segment in a packet. It slaps on your machine's Source IP and the target server's Destination IP. The OS consults its routing table and realizes the Destination IP is outside your local network, meaning it must go to your default gateway (your home router).
- **Data Link (Layer 2):** Your OS uses ARP (Address Resolution Protocol) to find your router's physical MAC address. It wraps the IP packet in an Ethernet frame, setting the Source MAC as your machine and the Destination MAC as your router.
- **Physical (Layer 1):** Your network card translates the frame into electrical pulses down a wire or radio waves via Wi-Fi.
- **The Transit:** Your router receives the frame, rips off the MAC header, looks at the Destination IP, determines the next hop to your ISP, wraps it in a _new_ MAC header, and sends it out. This unpacking and repacking of Layer 2 frames happens at every single router across the internet.
- **The Arrival:** The target server receives the frame, unpacks it layer by layer up the stack, validates the TCP sequence number, and hands the raw data to the receiving application.
