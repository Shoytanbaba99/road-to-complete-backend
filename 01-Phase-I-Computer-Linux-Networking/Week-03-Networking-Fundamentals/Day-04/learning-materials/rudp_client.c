#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "rudp.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_ip> <port> <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);
    const char *filename = argv[3];

    FILE *infile = fopen(filename, "rb");
    if (!infile) {
        perror("Failed to open source file");
        return EXIT_FAILURE;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        fclose(infile);
        return EXIT_FAILURE;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt SO_RCVTIMEO failed");
        close(sock);
        fclose(infile);
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    printf("====================================================\n");
    printf("            RUDP CLIENT (STOP-AND-WAIT)             \n");
    printf("====================================================\n");
    printf("[*] Sending file '%s' to %s:%d\n\n", filename, server_ip, port);

    uint32_t current_seq = 0;
    rudp_packet_t pkt;
    rudp_ack_t ack;
    int is_eof = 0;

    while (!is_eof) {
        size_t bytes_read = fread(pkt.payload, 1, PAYLOAD_SIZE, infile);
        if (bytes_read < PAYLOAD_SIZE) {
            if (feof(infile)) {
                is_eof = 1; // Mark that this or next is the terminal block
            }
        }

        
        pkt.seq_num = htonl(current_seq);
        pkt.length = htonl((uint32_t)bytes_read);

        
        int ack_received = 0;
        int attempts = 0;

        while (!ack_received) {
            attempts++;
            if (attempts > 1) {
                printf("[RETRANSMIT] Seq %u (Attempt #%d)...\n", current_seq, attempts);
            } else {
                printf("[SEND] Seq %u (%zu bytes)... ", current_seq, bytes_read);
                fflush(stdout);
            }

            ssize_t sent = sendto(sock, &pkt, sizeof(uint32_t) * 2 + bytes_read, 0,
                                  (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (sent < 0) {
                perror("sendto failed");
            }

            socklen_t addr_len = sizeof(server_addr);
            ssize_t recv_bytes = recvfrom(sock, &ack, sizeof(ack), 0,
                                          (struct sockaddr *)&server_addr, &addr_len);

            if (recv_bytes >= (ssize_t)sizeof(rudp_ack_t)) {
                uint32_t ack_num = ntohl(ack.ack_num);
                if (ack_num == current_seq) {
                    printf("ACK %u OK!\n", ack_num);
                    ack_received = 1;
                } else {
                    printf("Ignored stale ACK %u (waiting for %u)\n", ack_num, current_seq);
                }
            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    printf("TIMEOUT! (No ACK received within 1.0s)\n");
                } else {
                    perror("recvfrom error");
                }
            }
        }

        current_seq++;
    }

    pkt.seq_num = htonl(current_seq);
    pkt.length = htonl(0);
    int eof_acked = 0;

    printf("\n[*] Transmitting EOF delimiter...\n");
    while (!eof_acked) {
        sendto(sock, &pkt, sizeof(uint32_t) * 2, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        socklen_t addr_len = sizeof(server_addr);
        ssize_t recv_bytes = recvfrom(sock, &ack, sizeof(ack), 0,
                                      (struct sockaddr *)&server_addr, &addr_len);

        if (recv_bytes >= (ssize_t)sizeof(rudp_ack_t) && ntohl(ack.ack_num) == current_seq) {
            printf("[+] Server confirmed EOF. Transmission finished successfully.\n");
            eof_acked = 1;
        }
    }

    fclose(infile);
    close(sock);
    printf("====================================================\n");
    return EXIT_SUCCESS;
}