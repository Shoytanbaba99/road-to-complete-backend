# Week 3 - Day 1 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Network Layers (OSI 7-Layer vs. TCP/IP 4-Layer model), Layer 2 Ethernet Frames, 48-bit Hardware MAC Addresses, EtherType Protocol Dispatching, Address Resolution Protocol (ARP) Broadcast/Unicast mechanics, ARP Caching, and Raw Socket Framing (`AF_PACKET` / `SOCK_RAW`).

---

## 🌐 Layer 2 Ethernet Frame Structure & ARP Resolution

```text
[ ETHERNET FRAME (Layer 2 - Data Link) ]
├── Preamble & SFD (8 Bytes)
├── Destination MAC (6 Bytes) ──► e.g. ff:ff:ff:ff:ff:ff (ARP Broadcast) or Unicast MAC
├── Source MAC      (6 Bytes) ──► Hardware NIC Address (e.g. 00:1a:2b:3c:4d:5e)
├── EtherType       (2 Bytes) ──► 0x0800 (IPv4), 0x0806 (ARP), 0x86DD (IPv6)
├── Payload         (46 - 1500 Bytes: IP Packet or ARP Message)
└── FCS / CRC       (4 Bytes: Frame Check Sequence Error Checking)

[ ARP RESOLUTION WORKFLOW ]
Host A (192.168.1.10) wants to send an IP packet to Host B (192.168.1.20):
1. Host A checks local ARP Cache (`ip neighbor` / `arp -an`).
2. If MISS: Host A broadcasts ARP Request: "Who has 192.168.1.20? Tell 192.168.1.10" (Dest MAC: ff:ff:ff:ff:ff:ff).
3. Host B receives broadcast, responds with ARP Unicast Reply: "192.168.1.20 is at 00:50:56:c0:00:08".
4. Host A updates ARP Cache with entry (192.168.1.20 ➔ 00:50:56:c0:00:08) and transmits Ethernet frame!
```

---

## 1. 4-Layer TCP/IP Stack vs 7-Layer OSI Model
- **Layer 4 Application Layer:** HTTP, DNS, SSH (Data).
- **Layer 3 Transport Layer:** TCP, UDP (Ports & Reliability).
- **Layer 2 Network Layer:** IP (Virtual IPv4/IPv6 Addresses & Routing).
- **Layer 1 Link / Physical Layer:** Ethernet Frames, MAC Addresses, Network Interface Cards (NICs), Switches, Wi-Fi.

---

## 2. Address Resolution Protocol (ARP) & Caching
- **The Problem:** IP routing works with virtual IP addresses, but physical Ethernet switches only forward raw electrical signals based on 48-bit **MAC Addresses** (`XX:XX:XX:XX:XX:XX`).
- **ARP Request:** Sent as a **Layer 2 Broadcast** (`ff:ff:ff:ff:ff:ff`) to every device on the local subnet switch segment.
- **ARP Reply:** Target device returns a **Unicast** frame containing its hardware MAC address.
- **ARP Cache (`ip neighbor`):** The Linux kernel caches IP-to-MAC mappings in memory with a short TTL (e.g., 60–300 seconds) to avoid spamming the switch with ARP requests for every packet.

---

## 3. Raw Packet Sniffing in C ([`my_sniffer.c`](learning-materials/my_sniffer.c))
- `socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))`: Bypasses the OS TCP/IP stack completely, allowing user-space code to capture raw Ethernet frames straight off the physical NIC wire.
- **EtherType Parsing:** Extracts destination MAC (bytes 0–5), source MAC (bytes 6–11), and converts EtherType uint16 (`buffer + 12`) using `ntohs()` (Network to Host Byte Order).
