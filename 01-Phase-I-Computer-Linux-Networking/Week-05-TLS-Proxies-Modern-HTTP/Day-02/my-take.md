## 🧠 Core Mental Model

Symmetric keys are fast, assymmetric keys are heavy on computation, to ensure at the initialisation of a secure connection, we use asymmetric keys to exchange symmetric keys. Once the symmetric key is exchanged, we can use it for fast encryption and decryption of data. the way we use it is diffie hellman key exchange, where both begins with a shared public key, and then they each generate a private key, mixed with the public key and gives each other the mixed key, then they again include their private key to the mixed key they received, and they both end up with the same symmetric key.

Diffie man hellman key exchange is susceptible to man in the middle attacks, where the attacker could act as a buffer between client and server and instead send its own keys.
