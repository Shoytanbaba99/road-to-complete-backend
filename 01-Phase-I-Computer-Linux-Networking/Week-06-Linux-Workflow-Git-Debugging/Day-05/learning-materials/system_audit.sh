#!/usr/bin/env bash
set -eo pipefail

LOGFILE="audit.log"
CRASH_REPORT="crash_dump.txt"

# Clean previous runs
rm -f "$LOGFILE" "$CRASH_REPORT"

echo "[1] Initializing background worker simulation..."

# Background worker that intermittently outputs malformed records
python3 - << 'EOF' &
import time
import random

with open("audit.log", "w", buffering=1) as f:
    for i in range(100):
        time.sleep(0.02)
        status = 200
        if random.random() < 0.10:
            status = 500
        latency = random.randint(10, 450)
        f.write(f"REQ_ID={i:04d} STATUS={status} LATENCY={latency}ms\n")
EOF
WORKER_PID=$!

echo "[2] Monitoring worker stream (PID: $WORKER_PID)..."
wait $WORKER_PID

echo "[3] Processing Metrics & Aggregating 500 Errors..."
echo "--- TOTAL FAILED REQUEST COUNT ---"
grep "STATUS=500" "$LOGFILE" | wc -l

echo "--- AVERAGE LATENCY CALCULATION VIA AWK ---"
awk -F'LATENCY=' '{print $2}' "$LOGFILE" | sed 's/ms//' | awk '{sum+=$1; count++} END {if (count > 0) print "Avg Latency:", sum/count, "ms"}'

echo "--- HIGH LATENCY SAMPLES (>300ms) ---"
awk -F'LATENCY=' '$2+0 > 300 {print $0}' "$LOGFILE" | head -n 5
