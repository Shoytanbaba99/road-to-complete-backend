#!/bin/bash

LOG_FILE="$1"

if [ $# -ne 1 ] || [ ! -f "$LOG_FILE" ]; then
    echo "Usage: $0 <log_file>"
    exit 1
fi

echo "============================================================"
echo "                 LOG ANALYZER REPORT                        "
echo "============================================================"

# 1. Unique visitors
UNIQUE=$(grep -oE 'IP=[0-9.]+' "$LOG_FILE" | cut -d'=' -f2 | sort -u | wc -l)
echo -e "\n📊 Unique Visitors: $UNIQUE"

# 2. Top 3 URIs
echo -e "\n📊 Top 3 URIs:"
grep -oE 'PATH=[^ ]+' "$LOG_FILE" | cut -d'=' -f2 | sort | uniq -c | sort -rn | head -3 | \
awk '{printf "  %4d %s\n", $1, $2}'

# 3. Error percentage
TOTAL=$(wc -l < "$LOG_FILE")
ERRORS=$(grep -cE 'STATUS=[45][0-9]{2}' "$LOG_FILE")
PERCENT=$(awk "BEGIN {printf \"%.2f\", ($ERRORS / $TOTAL) * 100}")
echo -e "\n📊 Error Percentage: $PERCENT% ($ERRORS/$TOTAL)"

echo -e "\n============================================================"
