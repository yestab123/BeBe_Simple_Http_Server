#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <error.h>

#include "bb_util.h"
#include "bb_log.h"
#include "bb_conn.h"

int
__creat_listen(char *ip, int port) {
    struct sockaddr_in ser;
    int sock, i;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    ser.sin_port = htons(port);
    ser.sin_family = AF_INET;
    ser.sin_addr.s_addr = inet_addr(ip);

    i = bind(sock, (struct sockaddr*)&ser, sizeof(ser));
    if (i < 0) {
        return i;
    }

    listen(sock, 10);

    return sock;
}

int
bb_creat_listen(char *ip, int port) {
    int i;

    i = __creat_listen(ip, port);
    if (i < 0) {
        return -1;
    }

    /* Socket Option Processing */
    bb_set_nonblock(i);
    bb_set_reuseaddr(i);

    return i;
}

int
__accept(int listen_fd, struct sockaddr_in *cli_addr) {
    int cli_fd;
    socklen_t len;
    
    len = sizeof(struct sockaddr_in);

    while(1) {
        cli_fd = accept(listen_fd, (struct sockaddr*)cli_addr, &len);
        if (cli_fd < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                blog_error("sock:%d accept new connection error %d", listen_fd, cli_fd);
                return -1;
            }
        }
        return cli_fd;
    }
}

bb_conn_t *
bb_accept(int listen_fd) {
    int        cli_port;
    char       cli_ip[32];
    bb_conn_t  *conn;

    conn = bb_conn_creat();
    if (conn == NULL) {
        blog_error("malloc buff for bb_conn_t error %d", errno);
        return NULL;
    }

    conn->fd = __accept(listen_fd, &(conn->addr));
    if (conn->fd == -1) {
        bb_conn_destroy(conn);
        return NULL;
    }

    cli_port = ntohs(conn->addr.sin_port);
    inet_ntop(AF_INET, &(conn->addr.sin_addr), cli_ip, 31);
    blog_info("sock:%d accept new connection(sock:%d %s:%d)", listen_fd, conn->fd, cli_ip, cli_port);

    bb_set_nonblock(conn->fd);
    bb_set_tcp_keepalive(conn->fd);
    bb_set_tcp_nodelay(conn->fd);

    return conn;
}
