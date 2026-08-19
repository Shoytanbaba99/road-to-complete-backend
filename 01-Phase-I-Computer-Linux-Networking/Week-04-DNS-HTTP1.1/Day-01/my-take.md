## 🧠 Core Mental Model

So, we need a way for humans to remember the ip addresses of the servers, and for that we use DNS (Domain Name System). DNS is a hierarchical and decentralized naming system for computers, services, or other resources connected to the Internet or a private network. the way it works is the following: The recursor like (1.1.1.1 or 8.8.8.8) will ask the root server (.(13 servers)) for the TLD server (like .com, .bd, etc.) and then the TLD server will ask the authoritative nameserver for the domain (like example.com) and then the authoritative nameserver will give the ip address of the domain to the recursor and then the recursor will give it back. those www are nothing but subdomains.

CNam points to other another DNS record, and it is used to alias one name to another. as the shell doesnt have any identity, other records can't exist with the CName.

# Cheat

| Term                         | Meaning                                          | Example                                    |
| ---------------------------- | ------------------------------------------------ | ------------------------------------------ |
| **Root (`.`)**               | Top of DNS hierarchy / root zone                 | `.`                                        |
| **Root server**              | Server serving the root zone                     | `a.root-servers.net`                       |
| **TLD**                      | Top-level domain                                 | `.com`, `.bd`                              |
| **Domain name**              | A name in the DNS hierarchy                      | `acme.bd`                                  |
| **Nameserver**               | DNS server that answers queries                  | `hera.ns.cloudflare.com`                   |
| **Authoritative nameserver** | Nameserver holding authoritative data for a zone | `hera.ns.cloudflare.com` for `example.com` |
| **Zone**                     | Portion of DNS namespace administered together   | `example.com` zone                         |
| **Recursive resolver**       | Server that chases DNS referrals for you         | `8.8.8.8`                                  |
| **DNS record**               | Actual piece of DNS information                  | `A → 104.20.23.154`                        |
