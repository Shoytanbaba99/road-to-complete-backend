import socket

def run():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(('127.0.0.1', 9090))
    srv.listen(5)
    print("[*] Server ready on 127.0.0.1:9090")

    while True:
        conn, addr = srv.accept()
        print(f"[+] TCP connection established from {addr}")
        
        while True:
            # Buffer incoming request
            raw = b""
            while b"\r\n\r\n" not in raw:
                chunk = conn.recv(1024)
                if not chunk:
                    break
                raw += chunk
            
            if not raw:
                print("[-] Client closed connection")
                break
            
            lines = raw.decode('iso-8859-1').splitlines()
            req_line = lines[0]
            print(f"    [REQ] {req_line}")
            
            headers = {}
            for line in lines[1:]:
                if ":" in line:
                    k, v = line.split(":", 1)
                    headers[k.strip().lower()] = v.strip()

            body = b"Payload for: " + req_line.encode()
            
            # If client asked to close or HTTP/1.0 without keep-alive
            should_close = (headers.get("connection") == "close")
            conn_header = "close" if should_close else "keep-alive"
            
            response = (
                f"HTTP/1.1 200 OK\r\n"
                f"Content-Type: text/plain\r\n"
                f"Content-Length: {len(body)}\r\n"
                f"Connection: {conn_header}\r\n"
                f"\r\n"
            ).encode('ascii') + body

            conn.sendall(response)
            
            if should_close:
                break
                
        conn.close()

if __name__ == "__main__":
    run()