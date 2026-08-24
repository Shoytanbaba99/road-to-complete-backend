import random
import time

ips = ["192.168.1.10", "10.0.0.52", "172.16.0.4", "10.0.0.99", "192.168.1.10"]
statuses = [200, 200, 200, 404, 502, 500, 200]
paths = ["/api/v1/user", "/index.html", "/api/v1/checkout", "/static/app.js"]

with open("access.log", "w") as f:
    for _ in range(2000):
        ip = random.choice(ips)
        status = random.choice(statuses)
        path = random.choice(paths)
        ts = "2026-08-24T15:20:" + f"{random.randint(10, 59):02d}Z"
        f.write(f"{ts} IP={ip} PATH={path} STATUS={status}\n")

print("[+] access.log generated with 2,000 records.")
