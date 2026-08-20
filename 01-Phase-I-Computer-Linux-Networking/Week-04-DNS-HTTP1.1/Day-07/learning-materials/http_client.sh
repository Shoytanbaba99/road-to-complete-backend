#!/usr/bin/env bash
# ==============================================================================
# Pure Bash HTTP/1.1 Client
# Implements: URL Parsing -> TCP Socket -> Raw HTTP/1.1 Framing -> Stream Parsing
# ==============================================================================
set -euo pipefail

# 1. Parse Command Line Argument
URL="${1:-http://example.com/}"

# 2. Extract Protocol, Host, Port, and Path using Bash Regex
if [[ "$URL" =~ ^(http)://([^/:]+)(:([0-9]+))?(/.*)?$ ]]; then
    SCHEME="${BASH_REMATCH[1]}"
    HOST="${BASH_REMATCH[2]}"
    PORT="${BASH_REMATCH[4]:-80}"
    PATH_PART="${BASH_REMATCH[5]:-/}"
else
    echo "Error: Invalid or unsupported URL (Only HTTP supported): $URL" >&2
    exit 1
fi

echo "[*] Target Host : $HOST" >&2
echo "[*] Target Port : $PORT" >&2
echo "[*] Target Path : $PATH_PART" >&2

# 3. Open Raw TCP Socket via Bash's /dev/tcp Virtual File System
# Assigning File Descriptor 3 for bidirectional I/O
exec 3<>/dev/tcp/"$HOST"/"$PORT"
echo "[+] TCP Connection Established on File Descriptor 3" >&2

# 4. Construct RFC 7230 / RFC 9112 Compliant HTTP/1.1 Request
# CRITICAL: Every line MUST terminate with \r\n (Carriage Return + Line Feed)
CRLF=$'\r\n'
REQUEST="GET ${PATH_PART} HTTP/1.1${CRLF}"
REQUEST+="Host: ${HOST}${CRLF}"
REQUEST+="User-Agent: PureBashClient/1.0${CRLF}"
REQUEST+="Accept: */*${CRLF}"
REQUEST+="Connection: close${CRLF}"
REQUEST+="${CRLF}" # The mandatory empty line delimiter

# 5. Transmit Raw HTTP Request into the TCP Stream
echo -ne "$REQUEST" >&3
echo "[+] HTTP Request Transmitted" >&2

# 6. Parse Response Stream from File Descriptor 3
echo "[*] Reading Response Headers..." >&2

# Read Status-Line (First line)
IFS= read -r -u 3 STATUS_LINE
# Strip trailing Carriage Return (\r)
STATUS_LINE="${STATUS_LINE%$'\r'}"
echo "[<] Status Line: $STATUS_LINE" >&2

CONTENT_LENGTH=0

# Loop through Header Lines until Empty Line (\r\n) is hit
while IFS= read -r -u 3 line; do
    line="${line%$'\r'}"
    # When we encounter an empty line, headers have ended
    if [[ -z "$line" ]]; then
        break
    fi
    
    # Check for Content-Length (case-insensitive)
    if [[ "$line" =~ ^[Cc][Oo][Nn][Tt][Ee][Nn][Tt]-[Ll][Ee][Nn][Gg][Tt][Hh]:[[:space:]]*([0-9]+) ]]; then
        CONTENT_LENGTH="${BASH_REMATCH[1]}"
    fi
    echo "    [HDR] $line" >&2
done

echo "[+] Headers Parsed. Expected Body Length: $CONTENT_LENGTH bytes" >&2
echo "==================== [ RESPONSE BODY START ] ===================="

# 7. Read Body Stream to Standard Output
# Since Connection: close was sent, read until EOF
cat <&3

echo ""
echo "==================== [  RESPONSE BODY END  ] ===================="

# 8. Close File Descriptor 3 (Triggers TCP FIN Teardown)
exec 3>&-
exec 3<&-
echo "[+] Socket Closed Cleanly." >&2