#ifndef __H_BB_EPOLL__
#define __H_BB_EPOLL__

int  epoll_init();
int  epoll_add(int epoll_fd, int fd, void *conn, int ep_event);
int  epoll_mod(int epoll_fd, int fd, void *conn, int ep_event);
int  epoll_del(int epoll_fd, int fd);

#endif
