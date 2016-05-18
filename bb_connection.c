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
bb_accept_connect(int listen_fd) {
    int cli_fd;
    int cli_port;
    char cli_ip[32];
    socklen_t len;
    struct sockaddr_in cli_addr;

    while(1) {
        cli_fd = accept(listen_fd, (struct sockaddr*)&cli_addr, &len);
        if (cli_fd < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                blog_error("sock:%d accept new connection error %d", listen_fd, cli_fd);
                return -1;
            }
        }

        cli_port = ntohs(cli_addr.sin_port);
        inet_ntop(AF_INET, &(cli_addr.sin_addr), cli_ip, 31);
        blog_info("sock:%d accept new connection(sock:%d %s:%d)", listen_fd, cli_fd, cli_ip, cli_port);

        bb_set_nonblock(cli_fd);
        bb_set_tcp_keepalive(cli_fd);
        bb_set_tcp_nodelay(cli_fd);

        return cli_fd;
    }
}
