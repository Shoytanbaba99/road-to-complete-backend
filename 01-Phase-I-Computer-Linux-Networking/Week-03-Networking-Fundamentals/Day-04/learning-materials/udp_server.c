#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int sockfd;  
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("UDP server listening on port %d\n", PORT);
    int len, n;
    len = sizeof(cliaddr);
    int expected_packet = 0;
    while(1) {
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        int  packet_num = atoi(buffer);
        if(packet_num != expected_packet){
            printf("Packet %d received out of order. Expected %d. Discarding.\n", packet_num, expected_packet);
            expected_packet = packet_num + 1; // Update expected packet to the next one
        } else {
            printf("Packet %d received in order.\n", packet_num);
            expected_packet++;
        }
    }
}