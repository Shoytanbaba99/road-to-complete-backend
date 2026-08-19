## 🧠 Core Mental Model

Previous Days theory covered most of things of todays, but did learn that TTL (Time to Live) is the time for which a DNS record is cached in the resolver. It is set by the authoritative nameserver and can be different for different records. The TTL value is important because it determines how long a resolver will cache a record before it needs to query the authoritative nameserver again. and of the caching of dns is a cascading things, webbrowsers has their dns cache, then the OS has its dns cache, then the recursor has its dns cache, and then the authoritative nameserver has its dns cache

```
❯ dig @8.8.8.8 wikipedia.org

; <<>> DiG 9.18.50 <<>> @8.8.8.8 wikipedia.org
; (1 server found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 2043
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 512
;; QUESTION SECTION:
;wikipedia.org. IN A

;; ANSWER SECTION:
wikipedia.org. 162 IN A 103.102.166.224

;; Query time: 58 msec
;; SERVER: 8.8.8.8#53(8.8.8.8) (UDP)
;; WHEN: Wed Aug 19 17:41:55 +06 2026
;; MSG SIZE rcvd: 58
```

| Flag | Means                    | In plain English                                               |
| ---- | ------------------------ | -------------------------------------------------------------- |
| `qr` | **Query Response**       | This packet is a response, not a query                         |
| `rd` | **Recursion Desired**    | "Please recursively chase this answer for me."                 |
| `ra` | **Recursion Available**  | "I support recursive queries."                                 |
| `aa` | **Authoritative Answer** | "This answer comes from a server authoritative for this zone." |
| `ad` | **Authenticated Data**   | DNSSEC validation succeeded                                    |
| `cd` | **Checking Disabled**    | Don't perform DNSSEC validation                                |
| `tc` | **Truncated**            | Response was too large; retry using TCP                        |
