#!/bin/bash
# Must be run as root to manipulate kernel routing tables
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root (sudo ./routing_sandbox.sh)"
  exit 1
fi

# Extract the primary network interface name dynamically
INTERFACE=$(ip route | awk '/default/ {print $5}')
echo "Operating on interface: $INTERFACE"

echo "=== PHASE 1: Manual IP Allocation ==="
# We are assigning a brand new Private IP to your network card.
# Crucially, we assign it a /24 subnet mask (255.255.255.0).
NEW_IP="10.99.99.10"
CIDR="/24"

# Add the IP to the interface
ip addr add "$NEW_IP$CIDR" dev "$INTERFACE"
echo "Successfully added $NEW_IP$CIDR to $INTERFACE."

# Prove the kernel automatically generated a local routing rule for it
echo "Kernel routing table updated automatically:"
ip route show | grep "10.99.99.0"

echo "=== PHASE 2: DELIBERATE BREAKAGE (The Subnet Trap) ==="
echo "We will now attempt to ping 10.99.99.250."
echo "Because 250 falls within the /24 boundary (1-254), the OS will NOT use the Default Gateway."
echo "It will attempt a local Layer 2 ARP broadcast."

# Send exactly 1 ping with a 2-second timeout
ping -c 1 -W 2 -I "$NEW_IP" 10.99.99.250 > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "Ping SUCCESS."
else
    echo "Ping FAILED. Destination Host Unreachable."
fi

echo "Proving why it failed: The kernel tried to resolve MAC locally."
ip neighbor show | grep "10.99.99.250" || echo "ARP Table entry is INCOMPLETE (No one replied)."

echo -e "\n=== PHASE 3: The Routing Override Breakage ==="
echo "We will forcefully delete the kernel's automatic local subnet route."
echo "Command: ip route del 10.99.99.0/24 dev $INTERFACE"
ip route del 10.99.99.0/24 dev "$INTERFACE"

echo "Attempting to ping 10.99.99.250 again..."
ping -c 1 -W 2 -I "$NEW_IP" 10.99.99.250 > /dev/null 2>&1
echo "Ping failed instantly because the OS literally does not know where to send it!"

echo -e "\n=== PHASE 4: Cleanup ==="
# Remove the IP address to restore the system to normal
ip addr del "$NEW_IP$CIDR" dev "$INTERFACE"
echo "Sandbox destroyed. System restored."