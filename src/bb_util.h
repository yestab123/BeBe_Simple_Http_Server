#ifndef _HEAD_UTIL_
#define _HEAD_UTIL_

void bb_set_nonblock(int fd);
int  bb_set_tcp_keepalive(int fd);
int  bb_set_tcp_nodelay(int fd);
int  bb_set_reuseaddr(int fd);


#endif
