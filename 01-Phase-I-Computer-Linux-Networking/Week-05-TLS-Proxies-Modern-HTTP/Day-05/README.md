# Week 5 - Day 5: Proxies, Load Balancers, TLS Termination & Forwarded Headers

---

## 📋 Objectives
- [x] Forward Proxy vs. Reverse Proxy architectural boundaries
- [x] Load Balancer concepts & traffic distribution
- [x] TLS Termination offloading at reverse proxy boundary
- [x] Proxy headers (`X-Forwarded-For`, `X-Forwarded-Proto`)
- [x] Study system cluster architecture script ([`system_cluster.py`](learning-materials/system_cluster.py))

---

## 🗺️ Day 5 Pathways & Files

| File / Artifact | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | My personal mental model synthesizing Load Balancers, Reverse Proxies, and TLS Termination. |
| 🤖 [**`learning-materials/ai-summary.md`**](learning-materials/ai-summary.md) | Formal technical reference of Reverse Proxies, Load Balancing strategies, TLS Offloading, and `X-Forwarded-*` headers. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Raw textbook theory on Nginx/HAProxy reverse proxy architecture, load balancing algorithms, and TLS termination. |
| 🛠️ [**`learning-materials/system_cluster.py`**](learning-materials/system_cluster.py) | **Reference Script:** Reverse proxy & backend cluster simulation demonstrating TLS termination and load balancing. |

---

## 📝 Obsidian Vault Link
- **Concept Note:** `[[Forward vs Reverse Proxies, Load Balancers & TLS Termination]]` in `Engineers-Playbook/02 Permanent/`
