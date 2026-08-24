# Week 6 - Day 6 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Terminal Productivity: Shell Aliases, Parameterized Shell Functions (`mkcd()`, `port_owner()`), and `tmux` Terminal Multiplexing Architecture ([`productivity_suite.sh`](learning-materials/productivity_suite.sh)).

---

## 🌐 Shell Aliases vs. Functions & `tmux` Architecture

```text
[ SHELL SHORTCUT SPECTRUM ]
  Alias:   alias ll='ls -laFh' (Simple text replacement, no positional args)
  Function: mkcd() { mkdir -p "$1" && cd "$1"; } (Accepts positional arguments, logic, return codes)

[ TMUX ARCHITECTURE ]
  Tmux Server (Background Process)
    └── Session ("backend_dev")
          ├── Window 0 ("editor")
          │     ├── Pane 0 (Top: Code Editor / Shell)
          │     ├── Pane 1 (Bottom-Left: System Monitor / top)
          │     └── Pane 2 (Bottom-Right: Server Logs)
          └── Window 1 ("tests")
```

---

## 1. Global Shell Configuration Setup

To make `mkcd()` available globally in all terminal sessions for your user account:

```bash
# Add mkcd to ~/.bashrc
cat << 'EOF' >> ~/.bashrc

# Create a directory and immediately move into it
mkcd() {
    if [ -z "$1" ]; then
        echo "[ERROR] mkcd requires a directory path argument." >&2
        return 1
    fi
    mkdir -p "$1" && cd "$1"
}
EOF

# Reload config immediately
source ~/.bashrc
```
