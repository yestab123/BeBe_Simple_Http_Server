#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>

#include "bb_epoll.h"
#include "bb_conn.h"

#define MAX_EP_EVENT 200

int
bb_server_loop(int epfd, int timeout) {
    int   i, t;
    struct epoll_event events[MAX_EP_EVENT];

    i = epoll_wait(epfd, events, MAX_EP_EVENT, timeout);
    if (i < 0) {
        if (errno != EINTR) {
            return -1;
        } else {
            return 0;
        }
    }

    if (i == 0) {
        return 0;
    }

    bb_conn_t *conn;

    for(t = 0; t < i; t++) {
        conn = (bb_conn_t *)events[i].data.ptr;
        int ep_event = events[i].events;
        if (ep_event & EPOLLIN) {
            conn->epoll_in_handle(conn);
        }
        if (ep_event & EPOLLOUT) {
            conn->epoll_out_handle(conn);
        }
        if (ep_event & EPOLLERR|| ep_event & EPOLLRDHUP || ep_event & EPOLLHUP) {
            conn->epoll_err_handle(conn);
        }
    }

    return 0;
}
