# Road to Complete Backend

> **North Star:** Become a strong, backend-heavy full-stack software engineer with real systems, Linux, networking, database, infrastructure, security, concurrency, observability, and distributed-systems understanding.

---

## Why I'm Pivoting

Check out [this blog post](https://hashnode.com/edit/cmspmjg1900010akwccpqdq6e) on why I pivoted to this roadmap.

---

## Repository Architecture & Obsidian Integration

This repository houses all code labs, shell experiments, benchmarks, terminal logs, and hands-on projects. It works hand-in-hand with my Obsidian Vault (`Engineers-Playbook`).

```text
Road to Complete Backend/  (GitHub Code Workspace)
├── 01-Phase-I-Computer-Linux-Networking/
│   ├── Week-01-Computer-Fundamentals-Shell/
│   ├── Week-02-Memory-Files-Syscalls/
│   ├── Week-03-Networking-Fundamentals/
│   ├── Week-04-DNS-HTTP1.1/
│   ├── Week-05-TLS-Proxies-Modern-HTTP/
│   └── Week-06-Linux-Workflow-Git-Debugging/
├── 02-Phase-II-Go-As-Laboratory/
├── 03-Phase-III-PostgreSQL-Unmasked/
├── 04-Phase-IV-Production-Backend-Engineering/
├── 05-Phase-V-Deployment-DevOps/
├── 06-Phase-VI-Redis-Concurrency-Background-Processing/
├── 07-Phase-VII-Realtime-Observability/
├── 08-Phase-VIII-System-Design-Distributed-Systems/
├── 09-Phase-IX-Rust-Systems-Paradigm/
├── Projects/
├── Engineering Materials.md         # Reading list & reference library
├── Guide.md                         # The 52-Week Master Curriculum
├── Guide Log.md                     # Daily tracking journal
└── README.md
```

---

## The Two-Pillar System: Code Workspace vs. Obsidian Vault

| Pillar | Location | Primary Purpose | What Lives Here |
|---|---|---|---|
| **Code Workspace** | `Road to Complete Backend/` | **Execution & Evidence** | Bash scripts, Go/Rust code, `strace` logs, SQL benchmarks, Dockerfiles, Git commits. |
| **Obsidian Vault** | `Engineers-Playbook/` | **Theory & Synthesis** | Literature notes, atomic permanent notes, Mermaid diagrams, mental models, personal explanations. |

---

## Daily 5-Step Learning Ritual

For each day (e.g., Week 1, Day 1):

1. **Study & Unpack (AI Pairing):** Take the daily topic from [`Guide.md`](file:///mnt/Shared/Projects/Github/Road%20to%20Complete%20Backend/Guide.md) and explore the concepts, mechanisms, and edge cases with Gemini.
2. **Predict & Draw (Obsidian):** Open `Engineers-Playbook/02 Permanent/`, draw the architecture or data flow using Mermaid/Excalidraw, and write the concept in your own words.
3. **Build & Lab (`Road to Complete Backend`):** Go to the matching Phase/Week directory (e.g., `01-Phase-I-.../Week-01-...`), create a day lab folder, and write code, terminal commands, or benchmarks to prove the concept empirically.
4. **Break & Observe:** Deliberately induce failures (race conditions, memory leaks, invalid inputs, network timeouts) and inspect logs, socket states, or stack traces.
5. **Log & Lock:** Open [`Guide Log.md`](file:///mnt/Shared/Projects/Github/Road%20to%20Complete%20Backend/Guide%20Log.md) and fill out the daily 1-line log:
   `Day X: Today I learned __; I proved it by __; I broke __; the failure taught me __; tomorrow I will __.`

---

## Core Rules
- **Rule 1:** One Main Road (No derailment by trendy frameworks).
- **Rule 2:** AI is an accelerator, understanding is mandatory.
- **Rule 3:** Build before collecting notes.
- **Rule 4:** Learn the abstraction AND the underlying mechanism.
