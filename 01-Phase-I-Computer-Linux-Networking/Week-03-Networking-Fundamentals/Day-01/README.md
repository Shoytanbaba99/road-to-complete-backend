# Week 3 - Day 1: Network Layers, Ethernet Frames, MAC Addresses & ARP

---

## 📋 Objectives
- [x] Network layers (4-Layer TCP/IP model vs 7-Layer OSI model)
- [x] Ethernet frame structure (Header, Destination MAC, Source MAC, EtherType, Payload, CRC Trailer)
- [x] 48-bit Hardware MAC Addresses
- [x] Address Resolution Protocol (ARP) broadcast (`ff:ff:ff:ff:ff:ff`) and unicast replies
- [x] Kernel ARP caching (`ip neighbor` / `arp -an`)
- [x] Build C raw Ethernet frame sniffer ([`my_sniffer.c`](learning-materials/my_sniffer.c))

---

## 🗺️ Day 1 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Network Layers, Ethernet framing, ARP broadcast/unicast, and raw sockets. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, IEEE 802.3 Ethernet standards, and POSIX raw packet socket specifications. |
| 🛠️ [**`learning-materials/my_sniffer.c`**](learning-materials/my_sniffer.c) | **Capstone:** C Raw Ethernet Frame Sniffer using `AF_PACKET` / `SOCK_RAW` to capture Layer 2 traffic. |
| 🔬 [**`learning-materials/arp_experiment.sh`**](learning-materials/arp_experiment.sh) | Bash script demonstrating ARP cache inspection, clearing (`ip neigh flush`), and ping ARP resolution. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Inspect local Linux kernel ARP cache
ip neighbor show
# or legacy command:
arp -an

# Compile and run Raw Ethernet Frame Sniffer (Requires root / sudo for raw network socket)
gcc my_sniffer.c -o my_sniffer
sudo ./my_sniffer
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Network Layers, Ethernet Frames, MAC Addresses & ARP Resolution]]` in `Engineers-Playbook/02 Permanent/`
