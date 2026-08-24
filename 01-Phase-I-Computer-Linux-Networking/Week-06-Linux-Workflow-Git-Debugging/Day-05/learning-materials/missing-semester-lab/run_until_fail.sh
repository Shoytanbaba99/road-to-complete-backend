#!/usr/bin/env bash
count=0
echo "[*] Starting automated execution loop..."

while true; do
    count=$(( count + 1 ))
    # Run the script, capturing stdout to /dev/null and stderr to crash.log
    ./flaky_pipeline.sh > /dev/null 2> crash.log
    
    # Check the exit status code of the last executed command
    if [ $? -ne 0 ]; then
        echo "[!] CRASH DETECTED on iteration: $count"
        echo "================ CAUGHT ERROR LOG ================"
        cat crash.log
        echo "=================================================="
        break
    fi
done
