#define _DEFAULT_SOURCE
#define __FAVOR_BSD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define PACKET_BUFFER_SIZE 4096
#define SOURCE_IP "127.0.0.1"
#define TARGET_IP "127.0.0.1"



struct pseudo_header {
    uint32_t source_address;
    uint32_t dest_address;
    uint8_t  placeholder;
    uint8_t  protocol;
    uint16_t tcp_length;
};


unsigned short in_cksum(unsigned short *ptr, int nbytes) {
    long sum = 0;
    unsigned short oddbyte = 0;
    unsigned short answer = 0;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        *(unsigned char *)(&oddbyte) = *(unsigned char *)ptr;
        sum += oddbyte;
    }

    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    answer = (unsigned short)(~sum);
    return answer;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Target Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int target_port = atoi(argv[1]);
    if (target_port <= 0 || target_port > 65535) {
        fprintf(stderr, "Invalid port number: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    uint16_t src_port = (rand() % (65535 - 1024 + 1)) + 1024;
    uint32_t seq_num = rand();
    int sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock_raw < 0) {
        perror("Socket creation failed (Must run as root / sudo)");
        exit(EXIT_FAILURE);
    }

    int one = 1;
    if (setsockopt(sock_raw, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("Error setting IP_HDRINCL");
        close(sock_raw);
        exit(EXIT_FAILURE);
    }

    char packet[PACKET_BUFFER_SIZE];
    memset(packet, 0, PACKET_BUFFER_SIZE);

    struct iphdr *iph = (struct iphdr *)packet;
    struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr));
    iph->id = htons(rand() % 65535);
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->saddr = inet_addr(SOURCE_IP);
    iph->daddr = inet_addr(TARGET_IP);
    iph->check = in_cksum((unsigned short *)iph, sizeof(struct iphdr));
    
    tcph->source = htons(src_port);
    tcph->dest = htons(target_port);
    tcph->seq = htonl(seq_num);
    tcph->ack_seq = 0;
    tcph->doff = 5; 
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons(5840); 
    tcph->check = 0;         
    tcph->urg_ptr = 0;

    struct pseudo_header psh;
    psh.source_address = inet_addr(SOURCE_IP);
    psh.dest_address = inet_addr(TARGET_IP);
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr));

    int psize = sizeof(struct pseudo_header) + sizeof(struct tcphdr);
    char *pseudogram = malloc(psize);
    if (!pseudogram) {
        perror("Allocation failed");
        close(sock_raw);
        exit(EXIT_FAILURE);
    }

    memcpy(pseudogram, (char *)&psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), tcph, sizeof(struct tcphdr));

    tcph->check = in_cksum((unsigned short *)pseudogram, psize);
    free(pseudogram);


    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(target_port);
    dest_addr.sin_addr.s_addr = inet_addr(TARGET_IP);

    if (sendto(sock_raw, packet, ntohs(iph->tot_len), 0,
               (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto failed");
        close(sock_raw);
        exit(EXIT_FAILURE);
    }

    printf("SYN packet transmitted: %s:%u -> %s:%u (Seq=%u)\n",
           SOURCE_IP, src_port, TARGET_IP, target_port, seq_num);


    int recv_sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (recv_sock < 0) {
        perror("Failed to create receiving raw socket");
        close(sock_raw);
        exit(EXIT_FAILURE);
    }


    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(recv_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));

    unsigned char recv_buf[PACKET_BUFFER_SIZE];
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    while (1) {
        ssize_t data_size = recvfrom(recv_sock, recv_buf, sizeof(recv_buf), 0,
                                     (struct sockaddr *)&from_addr, &from_len);
        if (data_size < 0) {
            printf("Scan timed out: No response received.\n");
            break;
        }

        struct iphdr *recv_iph = (struct iphdr *)recv_buf;
        unsigned short ip_header_len = recv_iph->ihl * 4;

        if (recv_iph->protocol == IPPROTO_TCP) {
            struct tcphdr *recv_tcph = (struct tcphdr *)(recv_buf + ip_header_len);

            
            if (ntohs(recv_tcph->dest) == src_port &&
                ntohs(recv_tcph->source) == target_port) {

                if (recv_tcph->syn == 1 && recv_tcph->ack == 1) {
                    printf("PORT IS OPEN (Received SYN-ACK, Ack=%u)\n", ntohl(recv_tcph->ack_seq));
                    break;
                } else if (recv_tcph->rst == 1) {
                    printf("PORT IS CLOSED (Received RST)\n");
                    break;
                }
            }
        }
    }

    close(sock_raw);
    close(recv_sock);
    return 0;
}