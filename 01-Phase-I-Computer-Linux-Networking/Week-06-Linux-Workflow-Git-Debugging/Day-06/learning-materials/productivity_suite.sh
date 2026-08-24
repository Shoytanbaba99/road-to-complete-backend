#!/usr/bin/env bash

# ==============================================================================
# 1. CORE SAFETY ALIASES & ERGONOMIC SHORTCUTS
# ==============================================================================

# Protect against accidental file overwrites and deletions
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# Human-readable disk and directory navigation
alias df='df -h'
alias free='free -m'
alias ll='ls -laFh --color=auto'

# Quick network socket inspections
alias listening='ss -tuln'

# ==============================================================================
# 2. PARAMETERIZED SHELL FUNCTIONS
# ==============================================================================

# Create a directory and immediately move into it
mkcd() {
    if [ -z "$1" ]; then
        echo "[ERROR] mkcd requires a directory path argument." >&2
        return 1
    fi
    mkdir -p "$1" && cd "$1" || return 1
}

# Find any process listening on a specified TCP/UDP port
port_owner() {
    if [ -z "$1" ]; then
        echo "[ERROR] Usage: port_owner <port_number>" >&2
        return 1
    fi
    lsof -i :"$1" || ss -lptn "sport = :$1"
}

# Quick Git save point with custom message (defaults to timestamp)
gsave() {
    local msg="${1:-Auto-save checkpoint $(date +'%Y-%m-%d %H:%M:%S')}"
    git add -A && git commit -m "$msg"
}

# ==============================================================================
# 3. AUTOMATED MULTI-PANE TMUX WORKSPACE GENERATOR
# ==============================================================================

dev_workspace() {
    local session_name="${1:-backend_dev}"

    # Check if session already exists
    tmux has-session -t "$session_name" 2>/dev/null
    if [ $? -eq 0 ]; then
        echo "[*] Attaching to existing tmux session: $session_name"
        tmux attach-session -t "$session_name"
        return 0
    fi

    echo "[+] Creating new multi-pane development environment: $session_name"

    # 1. Create a new detached session with Window 0 named 'editor'
    tmux new-session -d -s "$session_name" -n "editor"

    # 2. Split Window 0 horizontally: Top pane (70%), Bottom pane (30%)
    tmux split-window -v -p 30 -t "${session_name}:0"

    # 3. Split the bottom pane vertically: Left pane (50%), Right pane (50%)
    tmux split-window -h -p 50 -t "${session_name}:0.1"

    # 4. Configure Panes with automated tasks
    # Pane 0 (Top): Main editor or interactive shell
    tmux send-keys -t "${session_name}:0.0" "echo '=== PRIMARY WORKSPACE PANE ==='" C-m

    # Pane 1 (Bottom Left): System Resource Monitor
    tmux send-keys -t "${session_name}:0.1" "top" C-m

    # Pane 2 (Bottom Right): Log / Network Monitor
    tmux send-keys -t "${session_name}:0.2" "echo '=== LOG / METRICS PANE ==='" C-m

    # 5. Create a second Window named 'tests'
    tmux new-window -t "$session_name" -n "tests"
    tmux send-keys -t "${session_name}:1" "echo '=== TEST SUITE RUNNER ==='" C-m

    # 6. Select the first window and attach
    tmux select-window -t "${session_name}:0"
    tmux select-pane -t "${session_name}:0.0"
    tmux attach-session -t "$session_name"
}
