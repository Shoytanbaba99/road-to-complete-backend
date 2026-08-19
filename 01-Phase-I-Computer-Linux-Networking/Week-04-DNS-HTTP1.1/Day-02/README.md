# Week 4 - Day 2: Practical DNS Inspection, TTLs & Cascading Caching Layers

---

## 📋 Objectives
- [x] Query DNS records using `dig` (`dig @8.8.8.8 wikipedia.org`)
- [x] Inspect Time-To-Live (TTL) countdown mechanics in response headers
- [x] Master 4-tier cascading DNS caching (Browser ➔ OS ➔ Recursor ➔ Authoritative NS)
- [x] Trace DNS resolution traversal with `dig +trace`

---

## 🗺️ Day 2 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model, notes, synthesis, and live `dig` terminal output analysis. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of `dig` commands, TTL trade-offs, and multi-level DNS caching architecture. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory, `dig` flag parameters, and DNS caching invalidation strategies. |

---

## 🔬 Practical Lab & Inspection Commands

```bash
# Query Wikipedia A record using Google Public DNS
dig @8.8.8.8 wikipedia.org A

# Trace DNS resolution from Root (.) down to Authoritative NS
dig +trace wikipedia.org

# Inspect OS resolver status on Linux
systemd-resolve --status || resolvectl status
```

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[DNS Inspection with dig, TTL Countdown & Multi-Tier Caching]]` in `Engineers-Playbook/02 Permanent/`
