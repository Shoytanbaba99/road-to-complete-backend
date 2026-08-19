#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 9000
#define CHUNK_SIZE 512

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("connect");
        exit(EXIT_FAILURE);
    }
    char payload[CHUNK_SIZE];
    memset(payload, 'A', sizeof(payload));
    size_t total_sent = 0;
    struct timespec ts;
    printf("[Client] Sending data in chunks of %d bytes...\n", CHUNK_SIZE);
   
    while(1){
        clock_gettime(CLOCK_REALTIME, &ts);
        printf("[%ld.%06ld] Sending %d bytes (Total: %zu)...\n", 
               ts.tv_sec, ts.tv_nsec / 1000, CHUNK_SIZE, total_sent);
        ssize_t sent = send(sock, payload, sizeof(payload), 0);
        if(sent < 0){ break; }
        total_sent += sent;
               
    } close (sock);
    return 0;
}