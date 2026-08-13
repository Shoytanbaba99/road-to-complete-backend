#!/usr/bin/env bash


if [ "$#" -lt 1 ]; then
    echo "Error: No command provided." >&2
    echo "Usage: $0 <command> [args...]" >&2
    exit 1
fi

env -i PATH="/usr/bin:/bin" DAEMON_MODE="ACTIVE" "$@" \
    </dev/null \
    >/tmp/daemon_out.log \
    2>/tmp/daemon_err.log &

DAEMON_PID=$!

echo "$DAEMON_PID" > /tmp/daemon.pid

echo "Daemon spawned successfully."
echo "  PID:        $DAEMON_PID (written to /tmp/daemon.pid)"
echo "  STDOUT log: /tmp/daemon_out.log"
echo "  STDERR log: /tmp/daemon_err.log"