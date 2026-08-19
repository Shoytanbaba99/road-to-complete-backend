import json
import socket

HOST = "postman-echo.com"
PORT = 80
payload = json.dumps({"name": "John Doe", "age": 30, "city": "New York"})
payload_bytes = payload.encode("ascii")

request = (
    f"POST /post HTTP/1.1\r\n"
    f"Host: {HOST}\r\n"
    f"Content-Type: application/json\r\n"
    f"Content-Length: {len(payload_bytes)}\r\n"
    f"Connection: close\r\n\r\n"
).encode("ascii") + payload_bytes


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))
sock.sendall(request)

def read_line(s):
    line = bytearray()
    while True:
        char = s.recv(1)
        if not char:
            break
        line.extend(char)
        if char == b'\n':
            break
    return line.decode('utf-8')

status_line = read_line(sock)
protocol, status_code, *reason = status_line.strip().split(" ", 2)

headers = {}
while True:
    header_line = read_line(sock).strip()
    if not header_line:
        break
    key, value = header_line.split(":", 1)
    headers[key.strip().lower()] = value.strip()

content_length = int(headers.get("content-length", 0))
body = bytearray()
while len(body) < content_length:
    chunk = sock.recv(min(4096, content_length - len(body)))
    if not chunk:
        break
    body.extend(chunk)
sock.close()

print(f"Status Code: {status_code}")
print("Response Payload:")
print(json.dumps(json.loads(body.decode("utf-8")), indent=2))
