#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>

int
epoll_init() {
    return epoll_create(1);
}

int
__epoll_manage(int epoll_fd, int fd, void *conn, int ep_event, int epoll_action) {
    struct epoll_event event;
    int    i;

    memset(&event, '\0', sizeof(struct epoll_event));
    if (conn != NULL) {
        event.data.ptr = conn;
    } else {
        event.data.fd = fd;
    }
    event.events = ep_event;

    i = epoll_ctl(epoll_fd, epoll_action, fd, &event);

    return i;
}

int
epoll_add(int epoll_fd, int fd, void *conn, int ep_event) {
    return __epoll_manage(epoll_fd, fd, conn, ep_event, EPOLL_CTL_ADD);
}

int
epoll_mod(int epoll_fd, int fd, void *conn, int ep_event) {
    return __epoll_manage(epoll_fd, fd, conn, ep_event, EPOLL_CTL_MOD);
}

int
epoll_del(int epoll_fd, int fd) {
    return __epoll_manage(epoll_fd, fd, NULL, EPOLLIN, EPOLL_CTL_DEL);
}
