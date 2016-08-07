#ifndef _HEAD_CONNECTION_
#define _HEAD_CONNECTION_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define CONN_INIT_PHASE           1
#define CONN_RECV_PHASE           2
#define CONN_RECV_FINISH_PHASE    4
#define CONN_CLOSE_PHASE          8
#define HTTP_METHOD_PHASE         64
#define HTTP_URL_PHASE            128
#define HTTP_CONTENT_LENGTH_PHASE 256
#define HTTP_CONTENT_RECV_PHASE   512

#define METHOD_GET  1
#define METHOD_HEAD 2
#define METHOD_POST 3

typedef struct bb_pos_s {
    int start;
    int end;
    int len;
} bb_pos_t;

typedef struct bb_http_s {
    int method;
    int content_length;

    bb_pos_t url;
    bb_pos_t content_buff_pos;
    bb_pos_t content_length_pos;
} bb_http_t;


typedef struct bb_conn_s {
    int                fd;
    struct sockaddr_in addr;

    int                phase;

    bb_http_t          http;

    int (*epoll_in_handle)(struct bb_conn_s *conn);
    int (*epoll_out_handle)(struct bb_conn_s *conn);
    int (*epoll_err_handle)(struct bb_conn_s *conn);
} bb_conn_t;

bb_conn_t *bb_conn_creat();
void       bb_conn_destroy();
void       bb_conn_init();

#endif
