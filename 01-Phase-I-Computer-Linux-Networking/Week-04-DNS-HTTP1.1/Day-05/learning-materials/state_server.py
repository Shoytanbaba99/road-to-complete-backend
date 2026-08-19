import socket
import hashlib
import json

# Master in-memory database
RESOURCE = {
    "title": "System Architecture Specification",
    "version": 1,
    "content": "This document covers transport and application layer invariants."
}

def calculate_etag(data_dict, weak=False):
    """Generates ETag (strong or weak) based on SHA-256 hash."""
    serialized = json.dumps(data_dict, sort_keys=True).encode('utf-8')
    hash_val = hashlib.sha256(serialized).hexdigest()[:16]
    if weak:
        return f'W/"{hash_val}"'
    return f'"{hash_val}"'

def run_server():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', 8082))
    srv.listen(5)
    print("[*] State & Cache HTTP/1.1 Server listening on http://127.0.0.1:8082")

    while True:
        conn, addr = srv.accept()
        raw = b""
        while b"\r\n\r\n" not in raw:
            chunk = conn.recv(1024)
            if not chunk:
                break
            raw += chunk

        if not raw:
            conn.close()
            continue

        header_bytes, _, body_bytes = raw.partition(b"\r\n\r\n")
        lines = header_bytes.decode('iso-8859-1').splitlines()
        if not lines:
            conn.close()
            continue

        method, path, version = lines[0].split(" ")
        headers = {}
        for line in lines[1:]:
            if ":" in line:
                k, v = line.split(":", 1)
                headers[k.strip().lower()] = v.strip()

        # Read remaining body if Content-Length present
        content_len = int(headers.get("content-length", 0))
        body = body_bytes
        while len(body) < content_len:
            body += conn.recv(content_len - len(body))

        # Parse Cookies
        cookies = {}
        if "cookie" in headers:
            for pair in headers["cookie"].split(";"):
                if "=" in pair:
                    ck, cv = pair.strip().split("=", 1)
                    cookies[ck] = cv

        # Route Handling
        resp_status = "200 OK"
        resp_headers = {"Connection": "close"}
        resp_body = b""

        # Route 1: Authentication / Cookie Issuance
        if path == "/login" and method == "POST":
            resp_status = "200 OK"
            resp_headers["Set-Cookie"] = "session_id=secure_token_999; Path=/; HttpOnly; SameSite=Strict"
            resp_body = b'{"auth": "success"}'
            resp_headers["Content-Type"] = "application/json"

        # Route 2: State Inspection via Cookies
        elif path == "/dashboard" and method == "GET":
            if cookies.get("session_id") == "secure_token_999":
                resp_status = "200 OK"
                resp_body = b'{"view": "Authorized Admin Dashboard"}'
            else:
                resp_status = "401 Unauthorized"
                resp_headers["WWW-Authenticate"] = 'Cookie realm="dashboard"'
                resp_body = b'{"error": "Missing or invalid session cookie"}'
            resp_headers["Content-Type"] = "application/json"

        # Route 3: ETag & Conditional Revalidation
        elif path == "/document" and method == "GET":
            current_etag = calculate_etag(RESOURCE, weak=True) 
            client_etag = headers.get("if-none-match")

            resp_headers["ETag"] = current_etag
            resp_headers["Cache-Control"] = "public, max-age=10, must-revalidate"

            if client_etag == current_etag:
                # 304 Not Modified Invariant: ZERO BODY
                resp_status = "304 Not Modified"
                resp_body = b""
            else:
                resp_status = "200 OK"
                resp_body = json.dumps(RESOURCE).encode('utf-8')
                resp_headers["Content-Type"] = "application/json"

        # Route 4: Optimistic Concurrency Control (Mid-Air Collision Guard)
        elif path == "/document" and method == "PUT":
            current_etag = calculate_etag(RESOURCE, weak=True)
            if_match = headers.get("if-match")

            if not if_match:
                resp_status = "428 Precondition Required"
                resp_body = b'{"error": "PUT requires If-Match header with current ETag"}'
                resp_headers["Content-Type"] = "application/json"
            elif if_match != current_etag:
                resp_status = "412 Precondition Failed"
                resp_body = b'{"error": "Mid-air collision detected. Stale ETag provided."}'
                resp_headers["Content-Type"] = "application/json"
            else:
                try:
                    payload = json.loads(body.decode('utf-8'))
                    RESOURCE["title"] = payload.get("title", RESOURCE["title"])
                    RESOURCE["content"] = payload.get("content", RESOURCE["content"])
                    RESOURCE["version"] += 1
                    
                    new_etag = calculate_etag(RESOURCE, weak=True)
                    resp_status = "200 OK"
                    resp_headers["ETag"] = new_etag
                    resp_body = json.dumps(RESOURCE).encode('utf-8')
                    resp_headers["Content-Type"] = "application/json"
                except Exception as e:
                    resp_status = "400 Bad Request"
                    resp_body = str(e).encode('utf-8')
                    resp_headers["Content-Type"] = "text/plain"

        else:
            resp_status = "404 Not Found"
            resp_body = b"Not Found"
            resp_headers["Content-Type"] = "text/plain"

        # Construct Wire Response
        if resp_status != "304 Not Modified":
            resp_headers["Content-Length"] = str(len(resp_body))

        status_line = f"HTTP/1.1 {resp_status}\r\n"
        hdr_lines = "".join(f"{k}: {v}\r\n" for k, v in resp_headers.items())
        final_wire_bytes = status_line.encode('ascii') + hdr_lines.encode('ascii') + b"\r\n" + resp_body

        conn.sendall(final_wire_bytes)
        conn.close()

if __name__ == "__main__":
    run_server()