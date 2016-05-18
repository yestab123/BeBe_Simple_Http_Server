#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/* Set Fd nonblock */
void
bb_set_nonblock(int fd) {
    int option;

    option = fcntl(fd, F_GETFL);
    option = option | O_NONBLOCK;
    fcntl(fd, F_SETFL, option);
}

/* Set Tcp Keepalive Option */
int
bb_set_tcp_keepalive(int fd) {
    int keep_alive = 1;          /* Open keepalive */
    /* int keep_idle = 60;          /\* 多久时间没有数据往来时开启探测 *\/ */
    /* int keep_interval = 5;       /\* 探测时每次发包的间隔时间 *\/ */
    /* int keep_count = 3;          /\* 探测尝试发包的次数 *\/ */
    int res;

    res = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keep_alive, sizeof(keep_alive));
    /* 若不调用一下三个设置，相关设置会使用系统默认配置 */
    /* 相关设置在 /proc/sys/net/ipv4/tcp_keepalive_* 文件中 */
    /* tcp_keepalive_intvl 探测包发送间隔 */
    /* tcp_keepalive_probes 探测包发送次数 */
    /* tcp_keepalive_time 多久时间后开始探测 */
    /* setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void *)&keep_idle, sizeof(keep_idle)); */
    /* setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keep_interval, sizeof(keep_interval)); */
    /* setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&keep_count, sizeof(keep_count)); */

    return res;
}

/* Set Tcp Nodelay */
int
bb_set_tcp_nodelay(int fd) {
    int nodelay;

    nodelay = 1;
    return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
}

/* Set addr reuse */
int
bb_set_reuseaddr(int fd) {
    int reuse;

    reuse = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}
