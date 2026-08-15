# Week 3 - Day 2 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of IP Addressing (IPv4), Subnetting & CIDR notation, Bitwise Subnet Math, Default Gateways, Public vs Private RFC 1918 Ranges, and Kernel Subnet Routing Engine (`router_math.c`).

---

## 🌐 Subnet Mask Math & Default Gateway Routing

```text
[ SUBNET MATH (Bitwise AND) ]
Source IP   : 192.168.1.10  (11000000.10101000.00000001.00001010)
Subnet Mask : /24            (11111111.11111111.11111111.00000000)
                              -----------------------------------
Network ID  : 192.168.1.0   (11000000.10101000.00000001.00000000)

[ ROUTING DECISION TREE ]
Process sends IP packet to Destination IP:

                 ┌─────────────────────────────┐
                 │ Is (Dest IP & Mask) equal   │
                 │ to (Source IP & Mask)?      │
                 └──────────────┬──────────────┘
                                │
                  ┌─────────────┴─────────────┐
               YES│                           │NO
                  ▼                           ▼
        [ LOCAL TARGET ]              [ REMOTE TARGET ]
 1. Send ARP Request for       1. Send ARP Request for
    Dest IP MAC Address           Default Gateway MAC Address
 2. Transmit Layer 2 Frame     2. Transmit Layer 2 Frame with
    direct to Dest Host MAC       Gateway Dest MAC & Dest IP!
```

---

## 1. IPv4 Structure & CIDR Notation
- **IPv4 Address:** 32-bit integer formatted as 4 octets (`0.0.0.0` to `255.255.255.255`).
- **CIDR Prefix (`/N`):** Indicates how many leading bits belong to the **Network ID**. Remaining `32 - N` bits belong to the **Host ID**.
- **Bitwise Subnet Mask:** `/24` = `255.255.255.0` (`0xFFFFFF00`).

---

## 2. Private vs Public IP Space (RFC 1918)
- **10.0.0.0/8:** `10.0.0.0` – `10.255.255.255` (Class A Private)
- **172.16.0.0/12:** `172.16.0.0` – `172.31.255.255` (Class B Private)
- **192.168.0.0/16:** `192.168.0.0` – `192.168.255.255` (Class C Private)

---

## 3. Kernel Subnet Decision Engine ([`router_math.c`](learning-materials/router_math.c))
- Converts IPv4 dot-decimal strings into uint32 32-bit integers via bit shifts (`b1 << 24 | b2 << 16 | b3 << 8 | b4`).
- Calculates mask `~((1U << (32 - cidr)) - 1)`.
- Performs bitwise AND `(src_ip & mask)` vs `(dst_ip & mask)` to determine if target is local (ARP directly) or remote (forward to Default Gateway).
