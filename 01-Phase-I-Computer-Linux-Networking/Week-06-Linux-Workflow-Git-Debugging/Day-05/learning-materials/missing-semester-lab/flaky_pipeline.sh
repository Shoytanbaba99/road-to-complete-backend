#!/usr/bin/env bash
# Generates a random number between 1 and 50
VAL=$(( RANDOM % 50 ))

echo "[INFO] Processing batch payload... Step 1 OK"
echo "[INFO] Processing batch payload... Step 2 OK"

if [ "$VAL" -eq 42 ]; then
    echo "[CRITICAL ERROR] Thread memory corrupt at 0xDEADBEEF! Math logic crashed." >&2
    exit 1
fi

echo "[SUCCESS] Batch completed cleanly."
exit 0
