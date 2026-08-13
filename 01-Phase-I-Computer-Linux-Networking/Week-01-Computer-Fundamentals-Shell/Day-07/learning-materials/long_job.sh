#!/usr/bin/env bash
echo "Daemon started."
echo "Current PATH: $PATH"
echo "Daemon Mode Flag: $DAEMON_MODE"
echo "User variable (should be blank): $USER"

# Sleep loop simulating work
for i in {1..5}; do
    echo "Tick $i"
    sleep 1
done
echo "Daemon finished."
