import socket
import ssl
import threading
import time
import json

# ==============================================================================
# 1. ORIGIN BACKEND WORKER FLEET (3 Microservices)
# ==============================================================================

def run_backend(node_id: int, port: int):
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', port))
    srv.listen(10)
    print(f"[*] Backend Worker {node_id} active on 127.0.0.1:{port}")

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

        header_part = raw.split(b"\r\n\r\n")[0].decode('iso-8859-1')
        lines = header_part.splitlines()
        req_line = lines[0]
        path = req_line.split(" ")[1] if len(req_line.split(" ")) > 1 else "/"

        headers = {}
        for line in lines[1:]:
            if ":" in line:
                k, v = line.split(":", 1)
                headers[k.strip().lower()] = v.strip()

        # Health Check Route
        if path == "/healthz":
            resp = b"HTTP/1.1 200 OK\r\nContent-Length: 2\r\nConnection: close\r\n\r\nOK"
            conn.sendall(resp)
            conn.close()
            continue

        # Dynamic Application Logic
        client_ip = headers.get("x-forwarded-for", "UNKNOWN")
        client_proto = headers.get("x-forwarded-proto", "http")

        response_dict = {
            "fulfilled_by": f"Backend-Node-{node_id}",
            "internal_port": port,
            "origin_socket_peer": f"{addr[0]}:{addr[1]}",
            "client_true_ip": client_ip,
            "client_original_proto": client_proto,
            "requested_path": path
        }
        body = json.dumps(response_dict, indent=2).encode('utf-8')

        # Static assets instruct the Edge Proxy to cache; Dynamic assets do not
        cache_directive = "public, max-age=60" if path.startswith("/static") else "no-store"

        response = (
            f"HTTP/1.1 200 OK\r\n"
            f"Content-Type: application/json\r\n"
            f"Content-Length: {len(body)}\r\n"
            f"Cache-Control: {cache_directive}\r\n"
            f"Connection: close\r\n"
            f"\r\n"
        ).encode('ascii') + body

        conn.sendall(response)
        conn.close()

# ==============================================================================
# 2. ORIGIN LOAD BALANCER (Health Checks + Round-Robin on Port 9000)
# ==============================================================================

class OriginLoadBalancer:
    def __init__(self, backend_ports: list[int]):
        self.backend_ports = backend_ports
        self.healthy_ports = list(backend_ports)
        self.idx = 0
        self.lock = threading.Lock()

    def health_check_daemon(self):
        while True:
            time.sleep(2)
            current_healthy = []
            for port in self.backend_ports:
                try:
                    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                    s.settimeout(1.0)
                    s.connect(('127.0.0.1', port))
                    s.sendall(b"GET /healthz HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n")
                    data = s.recv(1024)
                    s.close()
                    if b"200 OK" in data:
                        current_healthy.append(port)
                except Exception:
                    pass
            with self.lock:
                self.healthy_ports = current_healthy

    def get_next_port(self) -> int:
        with self.lock:
            if not self.healthy_ports:
                raise RuntimeError("503: All origin backend workers are unhealthy!")
            port = self.healthy_ports[self.idx % len(self.healthy_ports)]
            self.idx = (self.idx + 1) % len(self.healthy_ports)
            return port

lb = OriginLoadBalancer([9001, 9002, 9003])

def run_origin_lb(port: int):
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', port))
    srv.listen(20)
    print(f"[+] Origin Ingress Load Balancer ready on 127.0.0.1:{port}")

    while True:
        conn, addr = srv.accept()
        try:
            target_port = lb.get_next_port()
            # Bridge connection directly between Edge Proxy and selected Backend
            b_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            b_sock.connect(('127.0.0.1', target_port))
            
            # Read from Edge Proxy
            req = conn.recv(4096)
            b_sock.sendall(req)
            
            # Pipe response back to Edge Proxy
            while True:
                resp = b_sock.recv(4096)
                if not resp:
                    break
                conn.sendall(resp)
            b_sock.close()
        except Exception as e:
            err = f"HTTP/1.1 503 Service Unavailable\r\nContent-Length: {len(str(e))}\r\n\r\n{e}".encode()
            conn.sendall(err)
        finally:
            conn.close()

# ==============================================================================
# 3. EDGE CDN / REVERSE PROXY (TLS Termination + Caching on Port 8443)
# ==============================================================================

CACHE_STORE = {} # Simple in-memory Edge Cache: path -> (cached_bytes, expire_timestamp)
CACHE_LOCK = threading.Lock()

def handle_edge_client(tls_conn, client_addr):
    try:
        raw_req = b""
        while b"\r\n\r\n" not in raw_req:
            chunk = tls_conn.recv(1024)
            if not chunk:
                break
            raw_req += chunk

        if not raw_req:
            tls_conn.close()
            return

        header_part, _, body_part = raw_req.partition(b"\r\n\r\n")
        lines = header_part.decode('iso-8859-1').splitlines()
        req_line = lines[0]
        path = req_line.split(" ")[1] if len(req_line.split(" ")) > 1 else "/"

        # 1. Edge Cache Check
        now = time.time()
        with CACHE_LOCK:
            if path in CACHE_STORE:
                cached_bytes, expire_time = CACHE_STORE[path]
                if now < expire_time:
                    # Serve directly from Edge Cache (Sub-millisecond Edge Hit)
                    tls_conn.sendall(cached_bytes)
                    tls_conn.close()
                    return

        # 2. Cache Miss: Forward to Origin Load Balancer
        # Sanitize & Append Forwarded Headers
        forward_headers = []
        for line in lines[1:]:
            if ":" in line:
                k, v = line.split(":", 1)
                if k.strip().lower() not in ("x-forwarded-for", "x-forwarded-proto", "x-forwarded-host"):
                    forward_headers.append(f"{k.strip()}: {v.strip()}")

        forward_headers.append(f"X-Forwarded-For: {client_addr[0]}")
        forward_headers.append("X-Forwarded-Proto: https")
        forward_headers.append("X-Forwarded-Host: localhost")
        forward_headers.append("Connection: close")

        upstream_payload = (f"{req_line}\r\n" + "\r\n".join(forward_headers) + "\r\n\r\n").encode('ascii') + body_part

        # Connect to Origin LB on Port 9000
        origin_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        origin_sock.connect(('127.0.0.1', 9000))
        origin_sock.sendall(upstream_payload)

        # Read Full Origin Response
        full_origin_resp = b""
        while True:
            chunk = origin_sock.recv(4096)
            if not chunk:
                break
            full_origin_resp += chunk
        origin_sock.close()

        # 3. Inspect Response for Caching Rules
        if b"Cache-Control: public, max-age=" in full_origin_resp:
            # Store in Edge Cache for 30 seconds
            with CACHE_LOCK:
                CACHE_STORE[path] = (full_origin_resp, time.time() + 30)

        # Stream response back across TLS to client
        tls_conn.sendall(full_origin_resp)
    except Exception as e:
        err = f"HTTP/1.1 502 Bad Gateway\r\nContent-Length: {len(str(e))}\r\n\r\n{e}".encode()
        tls_conn.sendall(err)
    finally:
        tls_conn.close()

def run_edge_proxy(port: int):
    ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ctx.load_cert_chain(certfile="edge.crt", keyfile="edge.key")

    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', port))
    srv.listen(20)
    print(f"[+] Edge CDN / Reverse Proxy listening publicly on https://127.0.0.1:{port}")

    while True:
        raw_sock, addr = srv.accept()
        try:
            tls_sock = ctx.wrap_socket(raw_sock, server_side=True)
            threading.Thread(target=handle_edge_client, args=(tls_sock, addr), daemon=True).start()
        except Exception:
            raw_sock.close()

# ==============================================================================
# 4. INITIALIZE FULL CLUSTER
# ==============================================================================

if __name__ == "__main__":
    # Start Backend Workers
    threading.Thread(target=run_backend, args=(1, 9001), daemon=True).start()
    threading.Thread(target=run_backend, args=(2, 9002), daemon=True).start()
    threading.Thread(target=run_backend, args=(3, 9003), daemon=True).start()

    # Start Origin Load Balancer & Health Monitor
    threading.Thread(target=lb.health_check_daemon, daemon=True).start()
    threading.Thread(target=run_origin_lb, args=(9000,), daemon=True).start()
    time.sleep(0.5)

    # Start Edge Reverse Proxy in main thread
    run_edge_proxy(8443)