#ifndef RUDP_H
#define RUDP_H

#include <stdint.h>

#define PAYLOAD_SIZE 1024
#define DEFAULT_PORT 8080
typedef struct{
    uint32_t seq_num;
    uint32_t length;
    char payload[PAYLOAD_SIZE];

}  rudp_packet_t;


typedef struct {
    uint32_t ack_num;
} rudp_ack_t;

#endif