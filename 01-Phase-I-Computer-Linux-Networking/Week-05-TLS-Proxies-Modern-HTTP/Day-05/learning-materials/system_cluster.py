import socket
import ssl
import threading
import time

def run_backend_node(node_id:int, port:int):
    """Simulates an internal microservice running inside a private subnet."""
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', port))
    srv.listen(10)
    print(f"Backend node {node_id} listening on port {port}")

    while True:
        conn, addr = srv.accept()
        raw_req = b""
        while b"\r\n\r\n" not in raw_req:
            chunk = conn.recv(1024)
            if not chunk:
                break
            raw_req += chunk
        if not raw_req:
            conn.close()
            continue
        header_block = raw_req.split(b"\r\n\r\n")[0].decode("utf-8")
        lines = header_block.splitlines()

        headers = {}
        for line in lines[1:]:
            if ": " in line:
                k, v = line.split(": ", 1)
                headers[k.strip().lower()] = v.strip()
        real_ip = headers.get("x-forwarded-for", "UNKNOWN (DIRECT CONNECTION)")
        proto = headers.get("x-forwarded-proto", "http")

        body = (
            f"Fulfillment Node: Backend-{node_id}\n"
            f"Internal Socket Peer IP: {addr[0]}:{addr[1]}\n"
            f"True Client IP (X-Forwarded-For): {real_ip}\n"
            f"True Client Protocol (X-Forwarded-Proto): {proto}\n"
        ).encode("utf-8")

        response = (
            f"HTTP/1.1 200 OK\r\n"
            f"Content-Type: text/plain\r\n"
            f"Content-Length: {len(body)}\r\n"
            f"Connection: close\r\n"
            f"\r\n"
        ).encode("ascii") + body
        conn.sendall(response)
        conn.close()

class LoadBalancer:
    def __init__(self, backends: list[tuple[str,int]]):
        self.all_backends = backends.copy()  # Keep original list for recovery
        self.active_backends = backends.copy()  # Only healthy ones
        self.current_idx = 0
        self.lock = threading.Lock()
        self.health_check_thread = threading.Thread(target=self._health_check_loop, daemon=True)
        self.health_check_thread.start()
        print(f"[Health Check] Started monitoring {len(self.all_backends)} backends")

    def _health_check_loop(self):
        """Background thread: checks backend health every 2 seconds"""
        while True:
            time.sleep(2)  # Check every 2 seconds
            
            with self.lock:
                new_active = []
                for backend in self.all_backends:
                    host, port = backend
                    if self._check_backend_health(host, port):
                        new_active.append(backend)
                
                # Detect changes
                if set(new_active) != set(self.active_backends):
                    self.active_backends = new_active
                    status = ", ".join([f"{h}:{p} ✅" for h, p in self.active_backends])
                    print(f"[Health Check] Active backends: {status}")

    def _check_backend_health(self, host: str, port: int) -> bool:
        """Attempt to open a TCP connection to the backend"""
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(0.5)  # 500ms timeout
            result = sock.connect_ex((host, port))
            sock.close()
            return result == 0  # 0 = success, other = error
        except Exception:
            return False

    def get_next_backend(self) -> tuple[str,int]:
        """Get the next healthy backend using Round Robin"""
        with self.lock:
            if not self.active_backends:
                raise RuntimeError("No healthy backends available!")
            
            backend = self.active_backends[self.current_idx]
            self.current_idx = (self.current_idx + 1) % len(self.active_backends)
            return backend

def handle_client_proxy(client_conn, client_addr):
    try:
        raw_req = b""
        while b"\r\n\r\n" not in raw_req:
            chunk = client_conn.recv(1024)
            if not chunk:
                break
            raw_req += chunk

        if not raw_req:
            client_conn.close()
            return

        header_bytes, sep, body_bytes = raw_req.partition(b"\r\n\r\n")
        lines = header_bytes.decode('iso-8859-1').splitlines()
        req_line = lines[0]
        
        # Parse and sanitize incoming headers (Strip untrusted client-injected headers)
        forward_headers = []
        existing_xff = None
        for line in lines[1:]:
            if ":" in line:
                k, v = line.split(":", 1)
                k_clean = k.strip()
                # If you trust an upstream CDN, you append; otherwise you overwrite to prevent spoofing
                if k_clean.lower() == "x-forwarded-for":
                    existing_xff = v.strip()
                elif k_clean.lower() in ("x-forwarded-proto", "x-forwarded-host"):
                    continue # Strip client spoof attempts
                else:
                    forward_headers.append(f"{k_clean}: {v.strip()}")

        # Inject authentic Forwarded headers
        client_ip = client_addr[0]
        final_xff = f"{existing_xff}, {client_ip}" if existing_xff else client_ip
        forward_headers.append(f"X-Forwarded-For: {final_xff}")
        forward_headers.append("X-Forwarded-Proto: https")
        forward_headers.append("Connection: close")

        # Build modified request for internal backend
        upstream_req = f"{req_line}\r\n" + "\r\n".join(forward_headers) + "\r\n\r\n"
        upstream_bytes = upstream_req.encode('ascii') + body_bytes

        # Select backend via Round Robin
        target_host, target_port = lb_engine.get_next_backend()

        # Open internal socket to backend
        backend_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        backend_sock.connect((target_host, target_port))
        backend_sock.sendall(upstream_bytes)

        # Stream response back to external client
        while True:
            resp_chunk = backend_sock.recv(4096)
            if not resp_chunk:
                break
            client_conn.sendall(resp_chunk)

        backend_sock.close()
    except Exception as e:
        err_body = f"HTTP/1.1 502 Bad Gateway\r\nContent-Length: {len(str(e))}\r\n\r\n{e}".encode()
        client_conn.sendall(err_body)
    finally:
        client_conn.close()

def run_tls_reverse_proxy(port: int):
    # Setup TLS Context
    ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ctx.load_cert_chain(certfile="proxy.crt", keyfile="proxy.key")

    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', port))
    srv.listen(20)
    print(f"[+] TLS Reverse Proxy & Load Balancer listening publicly on https://127.0.0.1:{port}")

    while True:
        raw_sock, addr = srv.accept()
        try:
            # Terminate TLS at the proxy edge
            tls_conn = ctx.wrap_socket(raw_sock, server_side=True)
            threading.Thread(target=handle_client_proxy, args=(tls_conn, addr), daemon=True).start()
        except ssl.SSLError as e:
            raw_sock.close()

# ==============================================================================
# 3. CLUSTER INITIALIZATION
# ==============================================================================

if __name__ == "__main__":
    # Start Backend 1 and Backend 2 in background threads
    threading.Thread(target=run_backend_node, args=(1, 9001), daemon=True).start()
    threading.Thread(target=run_backend_node, args=(2, 9002), daemon=True).start()
    time.sleep(0.5)

    # Start Edge Reverse Proxy in Main Thread
    run_tls_reverse_proxy(8443)
            
