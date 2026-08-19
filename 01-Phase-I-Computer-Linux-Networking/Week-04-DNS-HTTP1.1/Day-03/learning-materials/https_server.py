import socket

def parse_http_request(raw_data):
    """
    Parses a raw TCP byte stream into:
    - Method, Path, Version
    - Headers Dictionary
    - Body Bytes
    """
    # 1. Locate the header delimiter (\r\n\r\n or \n\n)
    delimiter = b"\r\n\r\n"
    delim_pos = raw_data.find(delimiter)
    
    header_offset = 4
    if delim_pos == -1:
        delimiter = b"\n\n"
        delim_pos = raw_data.find(delimiter)
        header_offset = 2
        if delim_pos == -1:
            return None # Incomplete request headers

    header_bytes = raw_data[:delim_pos]
    body_bytes = raw_data[delim_pos + header_offset:]

    # 2. Decode headers as ASCII
    header_text = header_bytes.decode('iso-8859-1')
    lines = header_text.splitlines()
    if not lines:
        return None

    # 3. Parse Request Line
    request_line = lines[0].split(" ")
    if len(request_line) != 3:
        return ("BAD_REQUEST", None, None, None)
    
    method, path, version = request_line

    # 4. Parse Headers into Dictionary
    headers = {}
    for line in lines[1:]:
        if ":" in line:
            key, val = line.split(":", 1)
            headers[key.strip().lower()] = val.strip()

    return (method, path, version, headers, body_bytes)

def run_server():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', 8080))
    srv.listen(5)
    print("[*] HTTP/1.1 Server listening on http://127.0.0.1:8080")

    while True:
        client_sock, addr = srv.accept()
        raw_buffer = b""
        
        # Read from socket until header boundary is reached
        while b"\r\n\r\n" not in raw_buffer and b"\n\n" not in raw_buffer:
            chunk = client_sock.recv(1024)
            if not chunk:
                break
            raw_buffer += chunk

        parsed = parse_http_request(raw_buffer)
        
        if not parsed or parsed[0] == "BAD_REQUEST":
            response = b"HTTP/1.1 400 Bad Request\r\nContent-Length: 15\r\nConnection: close\r\n\r\n400 Bad Request"
            client_sock.sendall(response)
            client_sock.close()
            continue

        method, path, version, headers, initial_body = parsed

        # Enforce RFC 7230: Host header mandatory in HTTP/1.1
        if version == "HTTP/1.1" and "host" not in headers:
            response = b"HTTP/1.1 400 Bad Request\r\nContent-Length: 21\r\nConnection: close\r\n\r\nMissing Host Header"
            client_sock.sendall(response)
            client_sock.close()
            continue

        # Handle Content-Length for Body Extraction
        content_length = int(headers.get("content-length", 0))
        body = initial_body
        while len(body) < content_length:
            remaining = content_length - len(body)
            chunk = client_sock.recv(min(remaining, 4096))
            if not chunk:
                break
            body += chunk

        print(f"[{method}] {path} -> Read {len(body)} body bytes")

        # Construct Compliant HTTP/1.1 Response
        if path == "/":
            payload = b"<h1>Hello from Bare-Metal HTTP/1.1</h1>"
            status_line = "HTTP/1.1 200 OK\r\n"
            content_type = "text/html; charset=utf-8"
        elif path == "/echo" and method == "POST":
            payload = b'{"echo": "' + body + b'"}'
            status_line = "HTTP/1.1 200 OK\r\n"
            content_type = "application/json"
        else:
            payload = b"404 Resource Not Found"
            status_line = "HTTP/1.1 404 Not Found\r\n"
            content_type = "text/plain"

        response_headers = (
            f"{status_line}"
            f"Content-Type: {content_type}\r\n"
            f"Content-Length: {len(payload)}\r\n"
            f"Connection: close\r\n"
            f"\r\n"
        ).encode('ascii')

        client_sock.sendall(response_headers + payload)
        client_sock.close()

if __name__ == "__main__":
    run_server()