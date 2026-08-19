import socket
import json

# In-memory database
DATABASE = {
    "1": {"id": "1", "name": "System Architecture Manual", "price": 45.00}
}

def parse_q_values(accept_header):
    """
    Parses 'Accept' header into a prioritized list sorted by q-factor.
    Example: 'application/json;q=0.9, text/html;q=1.0' -> [('text/html', 1.0), ('application/json', 0.9)]
    """
    if not accept_header:
        return [("*/*", 1.0)]
    
    preferences = []
    for item in accept_header.split(","):
        parts = item.strip().split(";")
        media_type = parts[0].strip()
        q = 1.0
        for param in parts[1:]:
            param = param.strip()
            if param.startswith("q="):
                try:
                    q = float(param[2:])
                except ValueError:
                    q = 1.0
        preferences.append((media_type, q))
    
    # Sort by q descending
    preferences.sort(key=lambda x: x[1], reverse=True)
    return preferences

def render_representation(data, accepted_types):
    """
    Content Negotiation Engine: Matches client types against available formats.
    """
    available = ["application/json", "text/html", "text/plain"]
    
    for media_type, q in accepted_types:
        if q == 0.0:
            continue
        if media_type == "application/json" or media_type == "*/*":
            return json.dumps(data).encode('utf-8'), "application/json"
        elif media_type == "text/html":
            html = f"<html><body><h1>Product {data.get('id')}</h1><p>Name: {data.get('name')}</p><p>Price: ${data.get('price')}</p></body></html>"
            return html.encode('utf-8'), "text/html; charset=utf-8"
        elif media_type == "text/plain":
            txt = f"ID: {data.get('id')}\nName: {data.get('name')}\nPrice: {data.get('price')}\n"
            return txt.encode('utf-8'), "text/plain; charset=utf-8"
            
    return None, None # 406 Not Acceptable

def start_server():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', 8081))
    srv.listen(5)
    print("[*] REST & ConNeg Server running on http://127.0.0.1:8081")

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

        header_part, _, body_part = raw.partition(b"\r\n\r\n")
        lines = header_part.decode('iso-8859-1').splitlines()
        if not lines:
            conn.close()
            continue

        method, path, version = lines[0].split(" ")
        headers = {}
        for line in lines[1:]:
            if ":" in line:
                k, v = line.split(":", 1)
                headers[k.strip().lower()] = v.strip()

        # Read remaining body if Content-Length specified
        content_len = int(headers.get("content-length", 0))
        body = body_part
        while len(body) < content_len:
            body += conn.recv(content_len - len(body))

        # Routing Logic
        response_code = "200 OK"
        response_headers = {"Vary": "Accept", "Connection": "close"}
        response_body = b""

        if path == "/products/1":
            if method == "OPTIONS":
                response_code = "200 OK"
                response_headers["Allow"] = "GET, HEAD, PUT, PATCH, DELETE, OPTIONS"
                response_headers["Content-Length"] = "0"

            elif method in ("GET", "HEAD"):
                accepted = parse_q_values(headers.get("accept", "*/*"))
                payload, ctype = render_representation(DATABASE.get("1", {}), accepted)
                
                if not DATABASE.get("1"):
                    response_code = "404 Not Found"
                    response_body = b'{"error": "Resource deleted or not found"}'
                    response_headers["Content-Type"] = "application/json"
                elif payload is None:
                    response_code = "406 Not Acceptable"
                    response_body = b"Supported formats: application/json, text/html, text/plain"
                    response_headers["Content-Type"] = "text/plain"
                else:
                    response_code = "200 OK"
                    response_headers["Content-Type"] = ctype
                    response_body = payload if method == "GET" else b""

                response_headers["Content-Length"] = str(len(payload) if payload else len(response_body))

            elif method == "PUT":
                # Strict Idempotent Full Replacement
                try:
                    payload_json = json.loads(body.decode('utf-8'))
                    # Full replacement: All old keys erased except provided ones
                    DATABASE["1"] = {
                        "id": "1",
                        "name": payload_json.get("name"),
                        "price": payload_json.get("price")
                    }
                    response_code = "200 OK"
                    response_body = json.dumps(DATABASE["1"]).encode('utf-8')
                    response_headers["Content-Type"] = "application/json"
                    response_headers["Content-Length"] = str(len(response_body))
                except Exception as e:
                    response_code = "400 Bad Request"
                    response_body = str(e).encode('utf-8')
                    response_headers["Content-Length"] = str(len(response_body))

            elif method == "PATCH":
                # Non-Idempotent Relative Modification
                try:
                    patch_json = json.loads(body.decode('utf-8'))
                    # If action is 'increment_price', each call mutates state further
                    if "price_delta" in patch_json and "1" in DATABASE:
                        DATABASE["1"]["price"] += patch_json["price_delta"]
                    elif "name" in patch_json and "1" in DATABASE:
                        DATABASE["1"]["name"] = patch_json["name"]
                        
                    response_code = "200 OK"
                    response_body = json.dumps(DATABASE.get("1", {})).encode('utf-8')
                    response_headers["Content-Type"] = "application/json"
                    response_headers["Content-Length"] = str(len(response_body))
                except Exception as e:
                    response_code = "400 Bad Request"
                    response_body = str(e).encode('utf-8')
                    response_headers["Content-Length"] = str(len(response_body))

            elif method == "DELETE":
                # Strict Idempotent Purge
                if "1" in DATABASE:
                    del DATABASE["1"]
                    response_code = "204 No Content"
                else:
                    response_code = "404 Not Found"
                response_headers["Content-Length"] = "0"

            else:
                response_code = "405 Method Not Allowed"
                response_headers["Allow"] = "GET, HEAD, PUT, PATCH, DELETE, OPTIONS"
                response_headers["Content-Length"] = "0"
        else:
            response_code = "404 Not Found"
            response_body = b"Not Found"
            response_headers["Content-Length"] = str(len(response_body))

        # Build raw wire response
        status_line = f"HTTP/1.1 {response_code}\r\n"
        hdr_lines = "".join(f"{k}: {v}\r\n" for k, v in response_headers.items())
        final_bytes = status_line.encode('ascii') + hdr_lines.encode('ascii') + b"\r\n" + response_body

        conn.sendall(final_bytes)
        conn.close()

if __name__ == "__main__":
    start_server()