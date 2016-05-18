#ifndef _HEAD_CONNECTION_
#define _HEAD_CONNECTION_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

typedef struct bb_conn_s {
    int                fd;
    struct sockaddr_in addr;
} bb_conn_t;

bb_conn_t *bb_conn_creat();
void       bb_conn_destroy();
void       bb_conn_init();

#endif
