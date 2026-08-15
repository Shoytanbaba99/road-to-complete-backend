#!/bin/bash
# Must be run as root to manipulate the ARP tables.
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root (sudo ./arp_experiment.sh)"
  exit 1
fi

# We need a target on the local network. We will use the default gateway (router).
# This extracts the IP address of your router.
GATEWAY_IP=$(ip route | awk '/default/ {print $3}')
INTERFACE=$(ip route | awk '/default/ {print $5}')

echo "=== PHASE 1: Normal Resolution ==="
echo "Target Gateway IP: $GATEWAY_IP on interface $INTERFACE"

# Clear the ARP cache for the gateway to force a fresh ARP Broadcast
ip neighbor flush to "$GATEWAY_IP" dev "$INTERFACE"

# Ping sends exactly 1 ICMP packet. Because the cache is empty, the kernel 
# MUST pause this ping, broadcast an ARP request, wait for the reply, and then ping.
echo "Pinging gateway to trigger ARP resolution..."
ping -c 1 -W 1 "$GATEWAY_IP" > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "Ping SUCCESS."
else
    echo "Ping FAILED."
fi

# Extract the physically resolved MAC address from the kernel table
REAL_MAC=$(ip neighbor show to "$GATEWAY_IP" dev "$INTERFACE" | awk '{print $5}')
echo "The Gateway's True MAC Address is: $REAL_MAC"

echo -e "\n=== PHASE 2: DELIBERATE BREAKAGE (Self-ARP Poisoning) ==="
echo "We are going to manually overwrite the kernel's ARP table."
echo "We will map the Gateway's IP to a completely fake MAC address."

# The Fake MAC Address (Locally Administered, syntactically valid)
FAKE_MAC="02:aa:bb:cc:dd:ee"

# Forcefully change the neighbor table
ip neighbor replace "$GATEWAY_IP" lladdr "$FAKE_MAC" dev "$INTERFACE" nud permanent

echo "ARP Table poisoned. Verifying new state:"
ip neighbor show to "$GATEWAY_IP" dev "$INTERFACE"

echo "Attempting to ping the gateway using the poisoned ARP entry..."
# We try to ping the exact same IP address as before.
ping -c 1 -W 2 "$GATEWAY_IP" > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "Ping SUCCESS. (This should not happen!)"
else
    echo "Ping FAILED! The packet was lost in the void."
fi

# Cleanup
echo -e "\n=== PHASE 3: Cleanup ==="
echo "Restoring the ARP table by deleting the fake permanent entry."
ip neighbor del "$GATEWAY_IP" dev "$INTERFACE"
ping -c 1 -W 1 "$GATEWAY_IP" > /dev/null 2>&1
echo "Network restored."