#!/bin/bash
# The shebang (#!/bin/bash) is an absolute path. It ignores the $PATH variable completely.
# When the kernel executes this file, it sees the magic bytes '#!' and passes the rest
# of the file to the binary located exactly at /bin/bash.

echo "=== Deployment Script Initializing ==="
echo "Running as PID: $$"

# 1. Enforce Environment Inheritance
# We expect the parent process to have provided a DB_PASSWORD.
if [ -z "$DB_PASSWORD" ]; then
    echo "CRITICAL ERROR: DB_PASSWORD environment variable is not set."
    echo "The parent process failed to pass the required credentials via envp."
    exit 1
fi

echo "SUCCESS: Inherited DB_PASSWORD: [ HIDDEN FOR LOGS, length: ${#DB_PASSWORD} ]"

# 2. PATH Override Execution
# We want to run a custom 'build_tool', but we want to ensure we run the one
# in our specific project directory, NOT a system-wide one that might exist.
echo "Original PATH: $PATH"

# Prepending to PATH. First match wins.
export PATH="/opt/my_project/bin:$PATH"
echo "Modified PATH: $PATH"

# 3. Attempt to mutate the parent's environment (This is a trap)
echo "Attempting to set DEPLOY_STATUS=SUCCESS for the parent shell..."
export DEPLOY_STATUS="SUCCESS"

echo "=== Deployment Complete ==="
exit 0
