import socket

# Simple UDP Server for DNS
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('127.0.0.1', 1053))
print("[*] Mock DNS Server listening on 127.0.0.1:1053...")

while True:
    data, addr = sock.recvfrom(512)
    if len(data) < 12:
        continue

    # Extract Transaction ID (Bytes 0-1)
    tx_id = data[0:2]
    
    # Construct DNS Flags: Standard response, No error (0x8180)
    flags = b'\x81\x80'
    
    # Counts: 1 Question, 1 Answer, 0 Auth, 0 Additional
    counts = b'\x00\x01\x00\x01\x00\x00\x00\x00'
    
    # Extract Question section from incoming query
    # Ends at the first zero-byte after offset 12, plus 4 bytes for (Type + Class)
    q_end = 12
    while data[q_end] != 0:
        q_end += 1 + data[q_end]
    q_end += 5 # Skip the 0x00 byte, QTYPE (2B), and QCLASS (2B)
    question = data[12:q_end]

    # Construct Answer Record:
    # Pointer to Question name (0xc00c), Type A (0x0001), Class IN (0x0001)
    # TTL: 60 seconds (0x0000003c), RDLENGTH: 4 bytes (0x0004)
    # IP: 93.184.216.34 (0x5d.0xb8.0xd8.0x22)
    answer = (
        b'\xc0\x0c' +
        b'\x00\x01' +
        b'\x00\x01' +
        b'\x00\x00\x00\x3c' +
        b'\x00\x04' +
        socket.inet_aton('93.184.216.34')
    )

    response = tx_id + flags + counts + question + answer
    sock.sendto(response, addr)