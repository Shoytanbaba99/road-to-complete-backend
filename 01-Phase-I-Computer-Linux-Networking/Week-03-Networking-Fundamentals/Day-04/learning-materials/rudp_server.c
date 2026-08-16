#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "rudp.h"

#define DROP_RATE_PERCENT 30


int main(int argc, char *argv[]){
    int port = (argc > 1) ? atoi(argv[1]) : DEFAULT_PORT;
    srand(time(NULL));

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if(bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        close(sock);
        return EXIT_FAILURE;
    }
    FILE *outfile = fopen("received_file.txt", "wb");
    if(!outfile){
        perror("fopen");
        close(sock);
        return EXIT_FAILURE;
    }

    printf("====================================================\n");
    printf("     RUDP SERVER RUNNING (Loss Simulation: %d%%)     \n", DROP_RATE_PERCENT);
    printf("====================================================\n");
    printf("[*] Listening on port %d...\n\n", port);

    uint32_t expected_seq_num = 0;
    rudp_packet_t packet;
    rudp_ack_t ack;

    while(1){
        ssize_t bytes_recv = recvfrom(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len);
        if(bytes_recv < 0){
            perror("recvfrom");
            continue;
        }
        uint32_t seq_num = ntohl(packet.seq_num);
        uint32_t length = ntohl(packet.length);
        if((rand() % 100) < DROP_RATE_PERCENT){
            printf("[!] Simulated packet loss for seq_num: %u\n", seq_num);
            continue;
        }
        if(seq_num == expected_seq_num){
            if(length == 0){
                printf("[*] Received end of transmission signal. Closing connection.\n");
                ack.ack_num = htonl(seq_num);
                sendto(sock, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, client_len);
                break;
            }
            fwrite(packet.payload, 1, length, outfile);
            fflush(outfile);
            printf("[*] Received packet seq_num: %u, length: %u bytes\n", seq_num, length);
            ack.ack_num = htonl(seq_num);
            sendto(sock, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, client_len);
            expected_seq_num++;
        }
        else if(seq_num < expected_seq_num){
            printf("[!] Received out-of-order packet seq_num: %u\n", seq_num);  
        }
    }
    fclose(outfile);
    close(sock);
    printf("====================================================\n");
    printf("[+] File successfully saved to 'received_output.txt'\n");
    return EXIT_SUCCESS;
}