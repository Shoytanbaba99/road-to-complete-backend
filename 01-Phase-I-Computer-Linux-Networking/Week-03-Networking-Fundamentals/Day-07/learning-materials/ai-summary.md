# Week 3 - Day 7 Review & Capstone Technical Reference

> **Scope:** Full architectural synthesis of Week 3 Networking Primitives (4-Layer TCP/IP Stack, Ethernet Framing, MAC Addresses, ARP Broadcasts, IPv4 Subnetting & CIDR, Default Gateway Routing, NAT, 16-bit Ports, BSD Sockets, UDP Datagrams, RUDP ARQ, TCP 3-Way Handshake, Flow & Congestion Control, and Complete Packet Traversal).

---

## 🌐 Full Packet Traversal: Host A to Host B across Internet Routers

```text
[ HOST A (Application Layer) ]
  ├── Socket API (FD 3) ──► Generates payload bytes
  ▼
[ TRANSPORT LAYER ]
  ├── TCP Header (Src Port: 52410, Dst Port: 80, Seq #, Ack #, Flags)
  ▼
[ NETWORK LAYER ]
  ├── IP Packet Header (Src IP: 192.168.1.10, Dst IP: 93.184.216.34)
  ├── Subnet Bitwise Test (Dest IP & Mask != Source IP & Mask) ──► Target is REMOTE!
  ▼
[ DATA LINK LAYER ]
  ├── ARP Table Lookup ──► Resolves Default Gateway IP (192.168.1.1) to Gateway MAC
  ├── Ethernet Frame Header (Src MAC: Host A, Dst MAC: Gateway Router MAC, EtherType: 0x0800)
  ▼
[ PHYSICAL LAYER ]
  └── Bits ➔ Electrical Pulses / Wi-Fi Radio Waves ➔ Router Physical NIC Port

[ ROUTER HOPS ]
  ├── 1. Strips Layer 2 Ethernet Frame Header
  ├── 2. Inspects Layer 3 Destination IP Address (93.184.216.34)
  ├── 3. Consults Routing Table for Next Hop (ISP Gateway)
  └── 4. Encapsulates IP Packet in NEW Layer 2 Frame with Next Hop MAC Address!
```

---

## 1. Week 3 Deliverable Summary
- **Complete End-to-End Packet Journey:** Rebuilt mental model of how data moves from user-space application sockets, down the 4-layer TCP/IP stack, across Ethernet switches via ARP, through NAT and Internet router hops, up to the target application port.
- **Terminal Lab Inspections:** Mastered `ss` socket state inspection, `nc` netcat TCP/UDP connection probing, and `ip neighbor` ARP table debugging.
