#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main(){
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[100];

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("UDP client sending packets to %s:%d\n", inet_ntoa(servaddr.sin_addr), PORT);
    for(int i = 0; i < 1000; i++){
        sprintf(buffer, "%d", i);
        sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("Packet %d sent\n", i);
    }
    printf("Client finished blasting. The client has no idea if the server received them.\n");

    close(sockfd);
    return 0;
}