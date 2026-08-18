# Week 3 - Day 5: Transmission Control Protocol (TCP) & Three-Way Handshake

---

## 📋 Objectives
- [x] Transmission Control Protocol (TCP) header flags (`SYN`, `ACK`, `FIN`, `RST`, `PSH`, `URG`)
- [x] TCP Three-Way Handshake (`SYN` ➔ `SYN-ACK` ➔ `ACK`)
- [x] Byte sequence numbering & cumulative acknowledgements
- [x] Dynamic RTT estimation & Retransmission Timeouts (RTO)
- [x] Flow control (Window Size) & Congestion control principles
- [x] Build C Raw TCP Stealth SYN Scanner ([`syn_scan.c`](learning-materials/syn_scan.c))

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of TCP framing, 3-way handshake, RTT/RTO timers, and raw SYN scanning. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 793 TCP specifications, and flag header layouts. |
| 🛠️ [**`learning-materials/syn_scan.c`**](learning-materials/syn_scan.c) | **Capstone:** Raw TCP Stealth SYN Scanner crafting custom IP/TCP headers to probe port status. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Compile Raw TCP SYN Scanner (Requires root / sudo for SOCK_RAW IP_HDRINCL)
gcc syn_scan.c -o syn_scan

# Run SYN scan against local SSH port 22 or HTTP port 80
sudo ./syn_scan 22
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[TCP Three-Way Handshake, Sequence Numbers & Retransmission]]` in `Engineers-Playbook/02 Permanent/`
