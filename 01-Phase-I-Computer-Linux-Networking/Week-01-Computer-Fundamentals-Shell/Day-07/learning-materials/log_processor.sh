#!/bin/bash
# Strict mode: fail on any error, fail on unset variables, fail if any part of a pipe fails.
set -euo pipefail

echo "=== Log Processor Initializing ===" >&2

# 1. Environment Variable Validation
# We demand the OS environment provides a TARGET_LEVEL before we proceed.
if [ -z "${TARGET_LEVEL:-}" ]; then
    echo "CRITICAL ERROR: Environment variable TARGET_LEVEL is not set." >&2
    echo "Usage: TARGET_LEVEL=\"ERROR\" ./log_processor.sh /path/to/logfile.txt" >&2
    exit 1
fi

# 2. File Argument Validation
if [ "$#" -ne 1 ]; then
    echo "CRITICAL ERROR: Must provide exactly one input file." >&2
    exit 1
fi
INPUT_FILE="$1"

# 3. Generating Dummy Data if the file doesn't exist (for testing)
if [ ! -f "$INPUT_FILE" ]; then
    echo "INFO: Input file not found. Generating dummy log data..." >&2
    for i in {1..5000}; do
        echo "2026-10-25 INFO Operation successful" >> "$INPUT_FILE"
        echo "2026-10-25 WARN Memory usage high" >> "$INPUT_FILE"
        echo "2026-10-25 ERROR Database connection timeout" >> "$INPUT_FILE"
    done
fi

# 4. The Pipeline (Pipes, FDs, and Environment)
# We process the file using standard tools.
# We redirect the final output to a new file, but we use 'tee' to simultaneously
# send it to Standard Output so the user can see it.
echo "INFO: Processing logs for level: [$TARGET_LEVEL]" >&2

OUTPUT_FILE="${INPUT_FILE}_processed.log"

# The core pipeline:
# cat (reads file) -> grep (filters via env var) -> sort (organizes) -> tee (writes to file and stdout)
cat "$INPUT_FILE" \
    | grep "$TARGET_LEVEL" \
    | sort -r \
    | tee "$OUTPUT_FILE"

echo "=== Processing Complete. Output saved to $OUTPUT_FILE ===" >&2
exit 0
