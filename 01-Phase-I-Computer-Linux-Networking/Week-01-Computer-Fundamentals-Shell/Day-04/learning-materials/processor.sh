#!/bin/bash

# This script expects exactly one argument.
if [ "$#" -ne 1 ]; then
    # Write to STDERR (FD 2) using redirection
    echo "ERROR: You must provide exactly one argument." >&2
    exit 1
fi

INPUT_STR="$1"

# Write generic diagnostic info to STDERR
echo "INFO: Processing string: '$INPUT_STR'" >&2

if [ "$INPUT_STR" == "fail" ]; then
    echo "CRITICAL: The string 'fail' is forbidden!" >&2
    exit 88
fi

# Write the actual payload data to STDOUT (FD 1)
# We will generate a lot of data to fill pipe buffers later.
for i in {1..10000}; do
    echo "SUCCESS: Payload data $i - $INPUT_STR"
done

exit 0
