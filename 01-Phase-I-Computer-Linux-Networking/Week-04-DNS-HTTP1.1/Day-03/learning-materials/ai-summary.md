# Week 4 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of HTTP/1.1 Wire Format, Request & Response Anatomy, CRLF (`\r\n`) Line Delimiters, Status Lines, Headers, Content-Length Parsing, and Raw Socket HTTP Client ([`http_client.py`](learning-materials/http_client.py)).

---

## 🌐 HTTP/1.1 Text Wire Format Specification (RFC 7230)

```text
[ HTTP/1.1 REQUEST STRUCTURE ]
  ├── Request Line:   METHOD SP Request-URI SP HTTP-Version CRLF (e.g. POST /post HTTP/1.1\r\n)
  ├── Headers:        Field-Name ":" SP Field-Value CRLF         (e.g. Host: postman-echo.com\r\n)
  ├── Header Delimiter: CRLF (\r\n\r\n - Empty line separating headers from body)
  └── Message Body:   Raw payload bytes (Length governed by Content-Length header or Chunked Transfer)

[ HTTP/1.1 RESPONSE STRUCTURE ]
  ├── Status Line:    HTTP-Version SP Status-Code SP Reason-Phrase CRLF (e.g. HTTP/1.1 200 OK\r\n)
  ├── Headers:        Content-Type, Content-Length, Connection, etc. CRLF
  ├── Header Delimiter: CRLF (\r\n\r\n)
  └── Response Body:  JSON / HTML / Binary payload
```

---

## 1. Raw Socket HTTP Parsing Mechanics
- **CRLF Delimiters:** Lines in HTTP/1.1 wire protocol MUST be delimited by `\r\n` (Carriage Return + Line Feed, ASCII `0x0D 0x0A`).
- **Header Boundary:** The end of the header section is unambiguously signaled by two consecutive CRLFs (`\r\n\r\n`).
- **Framing & Payload Reading:** Sockets are stream-oriented (no message boundaries). The client MUST parse `Content-Length: N` from the header block to determine exactly how many body bytes to read from the socket before closing or reusing the connection.
