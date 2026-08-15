## 🧠 Core Mental Model

So, we got the OSI model, which is a framework that was created to standardise the communication protocol, in moder network we use 4 layers mainly.

Data Layer, here frames are created, each frame has a header and a trailer, the header contains the source and destination MAC address, and the trailer contains the error checking information. these packets to be sent to the next layer, requires destination mac address which it can get without communicating right? so it creates a special frame called ARP request, which is broadcasted to all the devices in the network, and the device with the matching IP address will respond with its MAC address. It saves that MAC address in its ARP cache for future communication.
