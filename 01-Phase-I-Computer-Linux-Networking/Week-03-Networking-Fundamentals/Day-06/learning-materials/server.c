#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 9000

int main(){
    int server_fd, client_fd;
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int rcvbuf = 2048;
    setsockopt(server_fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));

    int opt  = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);
    printf("[Server] Listening on port %d\n", PORT);
    client_fd = accept(server_fd, NULL, NULL);
    printf("[Server] Client connected. Holding recv() for 8 seconds\n");

    sleep(8);
    char buffer[1024];
    printf("[Server] Waking up. Calling recv()...\n");
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
    printf("[Server] recv() returned %zd bytes\n", bytes_read);
    sleep(2);
    close(client_fd);
    close(server_fd);
    return 0;

}