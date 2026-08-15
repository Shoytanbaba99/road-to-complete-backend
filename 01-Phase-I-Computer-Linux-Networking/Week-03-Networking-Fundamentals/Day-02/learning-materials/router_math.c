#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

bool parse_ipv4(const char *ip_str, uint32_t *out_ip){
    unsigned int b1, b2, b3, b4;
    if (sscanf(ip_str, "%u.%u.%u.%u", &b1, &b2, &b3, &b4) != 4) {
        return false;
    }
    if (b1 > 255 || b2 > 255 || b3 > 255 || b4 > 255) {
        return false;
    }
    *out_ip = ((uint32_t)b1 << 24) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 8) | (uint32_t)b4;
    return true;
}

uint32_t cidr_to_subnet_mast(int cidr){
    if (cidr == 0) return 0x00000000;
    if(cidr == 32) return 0xFFFFFFFF;
    return ~((1U << (32 - cidr)) - 1 );
}

void print_ip_dotted(uint32_t ip){
    printf("%u.%u.%u.%u\n", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);
}

void print_ip_binary(uint32_t val){
    for(int i = 31; i >= 0; i--){
        printf("%d", (val >> i) & 1);
        if(i % 8 == 0 && i != 0){
            printf(".");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <Source IP> <CIDR> <Destination IP>\n", argv[0]);
        fprintf(stderr, "Example: %s 192.168.1.1 24 10.0.0.1\n", argv[0]);
        return 1;
    }
    const char *src_str = argv[1];
    int cidr = atoi(argv[2]);
    const char *dst_str = argv[3];

    if(cidr < 0 || cidr > 32){
        fprintf(stderr, "Invalid CIDR value. Must be between 0 and 32.\n");
        return 1;
    }
    uint32_t src_ip = 0, dst_ip = 0;
    if (!parse_ipv4(src_str, &src_ip) || !parse_ipv4(dst_str, &dst_ip)) {
        fprintf(stderr, "Error: Invalid IPv4 address format.\n");
        return 1;
    }
        uint32_t subnet_mask = cidr_to_subnet_mast(cidr);
        uint32_t network_a = src_ip & subnet_mask;
        uint32_t network_b = dst_ip & subnet_mask;

        printf("=================================================================\n");
    printf("                  KERNEL ROUTE RESOLUTION ENGINE                 \n");
    printf("=================================================================\n");

    printf("Source IP       : ");
    print_ip_dotted(src_ip);
    printf("  [");
    print_ip_binary(src_ip);
    printf("]\n");

    printf("Subnet Mask (/%d): ", cidr);
    print_ip_dotted(subnet_mask);
    printf("  [");
    print_ip_binary(subnet_mask);
    printf("]\n");

    printf("Dest IP         : ");
    print_ip_dotted(dst_ip);
    printf("  [");
    print_ip_binary(dst_ip);
    printf("]\n");

    printf("-----------------------------------------------------------------\n");
    printf("Source Network  : ");
    print_ip_dotted(network_a);
    printf("  (Source IP & Mask)\n");

    printf("Dest Network    : ");
    print_ip_dotted(network_b);
    printf("  (Dest IP & Mask)\n");
    printf("-----------------------------------------------------------------\n");

    if (network_a == network_b) {
        printf("Verdict : MATCH (Network ID: ");
        print_ip_dotted(network_a);
        printf("/%d)\n", cidr);
        printf("Action  : ARP Broadcast (Target is LOCAL)\n");
    } else {
        printf("Verdict : MISMATCH\n");
        printf("Action  : Forward to Default Gateway (Target is REMOTE)\n");
    }
    printf("=================================================================\n");

    return EXIT_SUCCESS;
}