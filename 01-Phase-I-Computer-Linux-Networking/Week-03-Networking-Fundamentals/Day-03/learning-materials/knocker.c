#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define NUM_PORTS 3

int main (int argc, char *argv[]){
    if(argc != 5){
        fprintf(stderr, "Usage: %s <IP> <Port1> <Port2> <Port3>\n", argv[0]);
        fprintf(stderr, "Example: %s 127.0.0.1 5000 6000 7000\n", argv[0]);
        return 1;
    }

    const char *target_ip = argv[1];
    struct in_addr ip_addr;
    if(inet_pton(AF_INET, target_ip, &ip_addr) <= 0){
        fprintf(stderr, "Invalid IP address: %s\n", target_ip);
        return 1;
    }

    printf("====================================================\n");
    printf("         PORT KNOCKER CLIENT (TCP SCANNER)          \n");
    printf("====================================================\n");
    printf("Target IP: %s\n\n", target_ip);

    for(int i = 0; i < NUM_PORTS; i++){
        int port = atoi(argv[i + 2]);
        if(port <= 0 || port > 65535){
            fprintf(stderr, "Invalid port number: %d\n", port);
            continue;
        }
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0){
            perror("Socket creation failed");
            continue;
        }
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr = ip_addr;
        printf("[*] Attempting to connect to %s:%d ... ", target_ip, port);
        fflush(stdout);

        if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0){
            printf("Success! Port %d is open.\n", port);
        const char *message = "Knock Knock!";
        send(sock, message, strlen(message), 0);
        close(sock);
        printf("====================================================\n");
            return EXIT_SUCCESS;
    }else{
        printf("Failed. Port %d is closed or filtered. Error: %s\n", port, strerror(errno));
        }
        close(sock);
    }
    printf("\n[-] None of the specified ports were open.\n");
    printf("====================================================\n");
    return EXIT_FAILURE;

}