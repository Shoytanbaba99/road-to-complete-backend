## Part 1: Exhaustive Explanation of Concepts

To build software that guarantees the pristine, in-order delivery of data across a globally hostile, chaotic, and lossy network, you must abandon the fire-and-forget simplicity of UDP. You must embrace state. The Transmission Control Protocol (TCP) is not just a header on a packet; it is a massive, complex state machine executing concurrently in the operating system kernels of two completely distinct machines.

### TCP and the Reliable Byte Stream Abstraction

- **The Problem it Solves:** As proven yesterday, raw IP and UDP packets are ruthlessly dropped by congested routers, delayed by milliseconds, or take completely different geographic paths resulting in arrival out of order. If you are downloading a binary executable, a single flipped or missing byte will corrupt the entire program. If you are downloading a web page, the HTML text cannot be assembled out of order.
- **The Abstraction:** TCP provides the abstraction of a **Reliable, Ordered Byte Stream**.
- **Byte Stream vs. Datagrams:** In UDP, if you call `write(100 bytes)` and then `write(50 bytes)`, the receiver gets exactly two distinct packets. In TCP, there are no "message boundaries." If you call `write(100)` and `write(50)`, TCP might merge them into a single 150-byte payload, or split them into three 50-byte payloads. The abstraction is simply an unbroken pipe of bytes.
- **Reliability:** The kernel guarantees that every single byte you push into the socket will either physically arrive at the destination in the exact correct order, or the kernel will sever the connection and explicitly return a fatal error to your application.

### The Three-Way Handshake (State Initialization)

- **The Problem it Solves:** Two disconnected kernels across the planet cannot simply start blasting data at each other. They must formally agree to dedicate RAM (receive and send buffers) to each other, and they must mathematically synchronize their internal counting mechanisms.
- **The Abstraction:** The **Three-Way Handshake (SYN, SYN-ACK, ACK)**.

1. **SYN (Synchronize):** The Client kernel sends an empty TCP packet with the `SYN` flag set to 1. It carries a randomly generated 32-bit integer called the Initial Sequence Number (ISN). _Translation: "I want to talk. My starting byte counter is X."_
2. **SYN-ACK (Synchronize-Acknowledge):** The Server kernel receives it, allocates RAM buffers, and replies. It sets both the `SYN` and `ACK` flags. It generates its own random ISN (Y). It acknowledges the client's ISN by sending back `X + 1`. _Translation: "I acknowledge your X, I expect byte X+1 next. I also want to talk, and my starting byte counter is Y."_
3. **ACK (Acknowledge):** The Client receives the SYN-ACK, allocates its own RAM buffers, and sends a final empty packet with the `ACK` flag. It acknowledges the server's ISN by sending back `Y + 1`. _Translation: "I acknowledge your Y, I expect byte Y+1 next. We are now connected."_

- At this exact microsecond, both kernels transition their internal socket state to `ESTABLISHED`. Only now can the `accept()` syscall wake up in User Space.

### Sequence Numbers and Acknowledgements

- **The Problem it Solves:** When you send 10,000 bytes across the internet, they are chopped into smaller pieces (usually 1460 bytes each, the Maximum Segment Size or MSS). If chunk 3 arrives before chunk 2, or chunk 4 is lost entirely, how does the receiver reconstruct the original stream?
- **The Abstraction:** **Byte-Indexed Sequence Numbers** and **Cumulative Acknowledgements**.
- **Sequence Numbers:** TCP assigns a unique 32-bit mathematical index to _every single byte_ of payload. If the ISN was 1000, and you send a packet containing 500 bytes, the TCP header specifies `Seq=1001`. The receiver mathematically knows this packet contains bytes 1001 through 1500.
- _(Note: The ISN is randomized for security. If it always started at 0, an attacker could easily guess the sequence numbers and inject malicious packets into an established connection)._
- **Cumulative Acknowledgements (ACKs):** The receiver must tell the sender what arrived. It does this by setting the `ACK` flag and sending a number. The rule is absolute: **An ACK number means "I have successfully received every single byte up to X-1, and the very next byte I expect to see is X."**
- If the receiver gets chunk 1, chunk 2, and chunk 4, it will reply with an ACK for the end of chunk 2. It will absolutely refuse to ACK chunk 4, because it is missing chunk 3. It will repeatedly scream "I want the start of chunk 3!" on every packet.

### Retransmission

- **The Problem it Solves:** The sender blasted 5 packets into the void. It has no idea if they arrived unless the receiver sends an ACK. How long should it wait?
- **The Abstraction:** The **Retransmission Timeout (RTO)** and **Fast Retransmit**.
- **RTO:** The kernel dynamically calculates the Round Trip Time (RTT) of the connection. It sets a timer. If an ACK does not arrive before the timer expires, the kernel assumes the packet was annihilated by a router. It pulls a copy of the packet out of its Send Buffer and transmits it again. If it fails again, it doubles the timer (Exponential Backoff: 1s, 2s, 4s, 8s...). If it fails repeatedly (usually after 15 retries), the kernel kills the connection.
- **Fast Retransmit:** Waiting for a timer is slow. If the receiver got chunks 1, 2, 4, 5, and 6, it will send duplicate ACKs screaming "I want chunk 3!" over and over. When the sender's kernel sees exactly **3 Duplicate ACKs**, it bypasses the timer entirely and instantly retransmits chunk 3, assuming it was dropped rather than just delayed.

---

## Part 2: Underlying Mechanisms & System Inspections

We will use `tcpdump` to watch the OS kernel execute the Three-Way Handshake in real-time, verifying the ISN synchronization and sequence incrementation.

**1. Capturing the Handshake**
Open two terminal windows.
In Terminal 1, we will set up a raw packet sniffer to watch TCP traffic on port 8080.
**Crucial flag:** The `-S` flag forces `tcpdump` to print the absolute, raw 32-bit sequence numbers. Without it, `tcpdump` tries to be helpful and prints relative numbers starting at 0, which hides the true math.
Run: `sudo tcpdump -i lo -n -S tcp port 8080`

In Terminal 2, start a quick Netcat server and immediately connect to it.
Run: `nc -l 8080 &` (Starts the server in the background)
Run: `nc -vz 127.0.0.1 8080` (The `-z` flag tells the client to execute the 3-way handshake and then immediately disconnect without sending data).

**2. Dissecting the Packet Capture**
Look at the output in Terminal 1. You will see exactly three lines representing the handshake.

- **Packet 1 (SYN):** `IP 127.0.0.1.54321 > 127.0.0.1.8080: Flags [S], seq 2837461928, win 65495, options...`
- `Flags [S]`: This is the SYN flag.
- `seq 2837461928`: This is the Client's completely randomized ISN.

- **Packet 2 (SYN-ACK):** `IP 127.0.0.1.8080 > 127.0.0.1.54321: Flags [S.], seq 987654321, ack 2837461929, win 65483, options...`
- `Flags [S.]`: The `S` is SYN, the `.` is ACK.
- `seq 987654321`: This is the Server's completely randomized ISN.
- `ack 2837461929`: The Server took the Client's ISN and added exactly 1 to it. It expects that byte next.

- **Packet 3 (ACK):** `IP 127.0.0.1.54321 > 127.0.0.1.8080: Flags [.], seq 2837461929, ack 987654322, win 512, options...`
- `Flags [.]`: This is just the ACK flag.
- `seq 2837461929`: The client sends the exact byte the server asked for.
- `ack 987654322`: The client acknowledges the Server's ISN by adding 1.

**3. Inspecting Kernel Retransmission Limits**
Run: `cat /proc/sys/net/ipv4/tcp_syn_retries`

- **Observation:** This will output a number (usually `6`). This is the hardcoded limit defining exactly how many times the Linux kernel will retransmit a SYN packet during a handshake before giving up and throwing a "Connection Timed Out" error to the user-space application.

---

## Part 3: Code Architecture & Deliberate Breakage

To witness the physical reality of Retransmission and State Machines, we will not write C code today; the C code for TCP sockets (from Day 3) operates entirely in User Space and is blind to these kernel-level mechanics. Instead, we will use the OS firewall (`iptables`) to violently assassinate specific packets mid-flight and observe the kernel's desperate attempts to recover the state.

### The Architecture: The Blackhole Environment

We will set up a local client-server connection, and then we will command the Linux kernel's `netfilter` firewall to silently drop packets matching our criteria.

Open three terminal windows.

**Terminal 1 (The Sniffer):**
Run: `sudo tcpdump -i lo -n tcp port 9999`

**Terminal 2 (The Server):**
Run: `nc -l 9999`

**Terminal 3 (The Client & Saboteur):**
We are going to execute a **SYN Blackhole**. We will configure the firewall to drop all incoming TCP packets destined for port 9999.
Run: `sudo iptables -A INPUT -p tcp --dport 9999 -j DROP`

Now, attempt to connect to the server:
Run: `nc -v 127.0.0.1 9999`

### Deliberate Breakage 1: Observing Exponential Backoff

Look at your `tcpdump` output in Terminal 1 while the `nc` command hangs in Terminal 3.

**Observe the Logs:**

```text
14:00:00.000000 IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [S]...
14:00:01.002000 IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [S]...
14:00:03.004000 IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [S]...
14:00:07.008000 IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [S]...
14:00:15.016000 IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [S]...

```

**Why exactly did this break?**
Your `nc` client executed the `connect()` system call. The kernel generated a SYN packet and sent it to the loopback interface.
However, before the packet could reach the server socket, `iptables` intercepted it and brutally dropped it onto the floor. The server never sent a SYN-ACK.
The client kernel's Retransmission Timer (RTO) expired. It sent a duplicate SYN. It waited 1 second. It failed again. It backed off exponentially: waiting 2 seconds, then 4 seconds, then 8 seconds. It will do this exactly 6 times (based on the `tcp_syn_retries` file you inspected earlier) before completely giving up and returning a `-1` (Connection Timed Out) to the `nc` program.

### Deliberate Breakage 2: The Half-Open Connection (Mid-Stream Drop)

Remove the firewall rule: `sudo iptables -D INPUT -p tcp --dport 9999 -j DROP`

Now, establish the connection successfully.
In Terminal 3, run: `nc 127.0.0.1 9999`.
Type "Hello" and hit Enter. You will see it appear on the server. The state is perfectly `ESTABLISHED`.

Now, we execute the sabotage mid-stream. We will tell the firewall to drop packets coming _from_ the server back to the client. This will kill the ACKs.
Run: `sudo iptables -A OUTPUT -p tcp --sport 9999 -j DROP`

Now, go to your `nc` client and type "Second Message" and hit Enter.

**Observe the Logs (`tcpdump`):**

```text
IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [P.], seq 6:21, ack 1 ...
IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [P.], seq 6:21, ack 1 ...
IP 127.0.0.1.55555 > 127.0.0.1.9999: Flags [P.], seq 6:21, ack 1 ...

```

**Why exactly did this break?**
The client sent the data. The server actually received the data (you will see "Second Message" appear on the server's terminal). The server kernel automatically generated an ACK packet and tried to send it back.
But `iptables` intercepted the server's ACK and destroyed it.
Because the client kernel never received the ACK, its timer expired. The client kernel pulled the unacknowledged data out of its Send Buffer and violently retransmitted it. The server received the exact same data again, recognized by the sequence numbers that it was a duplicate, silently discarded the payload so it wouldn't print to the screen twice, and attempted to send another ACK... which was also destroyed. This cycle will repeat until the TCP connection collapses under timeout rules.

Clean up your firewall: `sudo iptables -F`

---

## Part 4: Record What You Learned

### What assumption is this system making?

The TCP architecture makes the massive, fundamental assumption that **any lost packet, lack of acknowledgement, or timeout is exclusively caused by network congestion, not hardware failure or broken paths.**

Because TCP assumes congestion is the culprit, its hardcoded response to failure is to _slow down_ (Exponential Backoff and Congestion Control). It assumes that blasting retransmissions at full speed would only make the overloaded router drop even more packets, exacerbating the problem. Furthermore, TCP assumes that **reliability is universally more important than latency.** It assumes the User Space application is perfectly willing to halt execution and wait 15 seconds for a lost chunk of data to be retransmitted, completely stalling the byte stream, rather than proceeding with missing data.

---

### Capstone Project: Build a "TCP SYN Scanner" (Raw Socket Forgery)

To deeply internalize the anatomy of the TCP header, sequence numbers, and the Three-Way Handshake, you must bypass the OS kernel's TCP stack entirely and manually forge a TCP packet bit by bit.

**Your Assignment:**
Write a C program that performs a "Stealth SYN Scan" against a specific port on your local machine. You will manually construct the Layer 3 (IP) and Layer 4 (TCP) headers.

**Requirements:**

1. **The Raw Socket:** Use `socket(AF_INET, SOCK_RAW, IPPROTO_TCP)`. This tells the kernel: "I will provide the TCP header myself. Do not format this for me." (Requires root).
2. **The IP Header:** Construct a `struct iphdr` (from `<netinet/ip.h>`). Fill in the source IP, destination IP, protocol (`IPPROTO_TCP`), and calculate the IP checksum.
3. **The TCP Header:** Construct a `struct tcphdr` (from `<netinet/tcp.h>`).

- Set the Source Port to a random number (e.g., 43210).
- Set the Destination Port to the target you want to scan (e.g., 8080).
- Set the `seq` (Sequence Number) to a random 32-bit integer. Set `ack_seq` to 0.
- **The Crucial Step:** Set `syn = 1`. Set all other flags to 0.
- Calculate the TCP Checksum (This is mathematically difficult because TCP requires a "Pseudo-Header" containing the Source and Destination IP addresses to be prepended before calculating the checksum. You will need to research how to build a TCP Pseudo-Header in C).

4. **The Execution:** Send the raw packet to the loopback address `127.0.0.1` using `sendto()`.
5. **The Sabotage (The Firewall):** Before you run your program, your OS kernel will ruin the experiment. If your raw socket sends a SYN, the server replies with a SYN-ACK. Your OS kernel will see the SYN-ACK, realize _it_ (the kernel) never initiated a TCP connection for that port, and instantly send a RST (Reset) packet to kill it.

- To prevent this, apply an iptables rule to drop outgoing RST packets on the loopback interface: `sudo iptables -A OUTPUT -p tcp --tcp-flags RST RST -j DROP`.

6. **The Read Loop:** Use `recvfrom()` to read raw packets. Parse the incoming TCP headers.

- If you receive a packet from the target port with `syn == 1` and `ack == 1`, print `"PORT IS OPEN"`.
- If you receive a packet with `rst == 1`, print `"PORT IS CLOSED"`.

**Why this is difficult:** You are writing code at the exact boundary of the operating system's networking stack. Manually calculating the TCP checksum with the Pseudo-Header is a notorious rite of passage for systems programmers; if a single bit is wrong, the receiving kernel will silently drop the packet, and you will receive zero feedback on why it failed. Completing this proves absolute, surgical mastery over the TCP state machine.
