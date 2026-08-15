#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h> // ETH_P_ALL
#include <linux/if_packet.h>  // AF_PACKET

#define BUFFER_SIZE 65536


const char *get_ethertype_name(uint16_t ethertype) {
    switch (ethertype){
        case 0x0800:
            return "IPv4";
        case 0x0806:
            return "ARP";
        case 0x86DD:
            return "IPv6";
        case 0x8100:
            return "VLAN";
        default:
            return "Unknown";
    }
}

int main(void){

    int sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock_raw < 0){
        perror("Socket Error");
        return 1;
    }
    printf("=================================================================\n");
    printf("         RAW ETHERNET FRAME SNIFFER (AF_PACKET / SOCK_RAW)       \n");
    printf("=================================================================\n");
    printf("Listening for Layer 2 frames... (Press Ctrl+C to stop)\n\n");
    unsigned char buffer[BUFFER_SIZE];
    while(1){
        ssize_t packet_size = recvfrom(sock_raw, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if(packet_size < 0){
            perror("Recvfrom Error");
            close(sock_raw);
            return 1;
        }
        if(packet_size < 14){
        continue;
        }
        unsigned char *dest_mac = buffer;
        unsigned char *src_mac = buffer + 6;
        uint16_t raw_ethertype;
        memcpy(&raw_ethertype, buffer + 12, sizeof(uint16_t));
        uint16_t ethertype = ntohs(raw_ethertype);
        printf("Captured Frame (%zd bytes):\n", packet_size);
        printf("  ├── Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               dest_mac[0], dest_mac[1], dest_mac[2],
               dest_mac[3], dest_mac[4], dest_mac[5]);
        printf("  ├── Source MAC:      %02x:%02x:%02x:%02x:%02x:%02x\n",
               src_mac[0], src_mac[1], src_mac[2],
               src_mac[3], src_mac[4], src_mac[5]);
        printf("  └── EtherType:       0x%04x (%s)\n", 
               ethertype, get_ethertype_name(ethertype));
        printf("-----------------------------------------------------------------\n");
    }
    close(sock_raw);
    return EXIT_SUCCESS;
    }
    
    
    

    