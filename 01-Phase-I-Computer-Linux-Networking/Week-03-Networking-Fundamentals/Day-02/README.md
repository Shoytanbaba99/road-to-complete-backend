# Week 3 - Day 2: IP Addressing, Subnet Masks/CIDR, Private IPs & Default Gateways

---

## 📋 Objectives
- [x] IPv4 address structure (32-bit integers, octets)
- [x] Subnet masks & CIDR prefix notation (`/24`, `/16`, `/8`)
- [x] Bitwise AND subnet mask calculations (`IP & Subnet Mask = Network ID`)
- [x] Public vs. Private RFC 1918 IP address ranges
- [x] Default Gateway routing decision (Local target ARP vs. Gateway forwarding)
- [x] Build C subnet route resolution engine capstone ([`router_math.c`](learning-materials/router_math.c))

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and takeaways written during study. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of IPv4 CIDR subnetting, bitwise operations, and default gateways. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, RFC 1918 specifications, and kernel IP routing algorithms. |
| 🛠️ [**`learning-materials/router_math.c`**](learning-materials/router_math.c) | **Capstone:** C Kernel Subnet Routing Engine calculating bitwise subnet masks and route decisions. |
| 🔬 [**`learning-materials/routing_sandbox.sh`**](learning-materials/routing_sandbox.sh) | Bash script demonstrating routing table inspection (`ip route` / `route -n`) and default gateway lookup. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Inspect local kernel routing table and default gateway
ip route show

# Compile and run Subnet Route Engine
gcc router_math.c -o router_math

# Test local subnet match (Action: ARP Broadcast)
./router_math 192.168.1.10 24 192.168.1.50

# Test remote network mismatch (Action: Forward to Default Gateway)
./router_math 192.168.1.10 24 8.8.8.8
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[IP Addressing, CIDR Subnetting & Default Gateway Routing]]` in `Engineers-Playbook/02 Permanent/`
