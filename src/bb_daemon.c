#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "bb_log.h"

/* From Nginx */
int
bb_set_into_daemon() {
    int  fd;

    switch (fork()) {
    case -1:
        blog_error("fork error %d", errno);
        abort();
    case 0:
        break;
    default:
        exit(0);
    }

    if (setsid() == -1) {
        blog_error("setsid() error %d", errno);
        abort();
    }

    umask(0);

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        blog_error("open /dev/null error %d", errno);
        abort();
    }
    if (dup2(fd, STDIN_FILENO) == -1) {
        blog_error("dup2(STDIN) error %d", errno);
        abort();
    }
    if (dup2(fd, STDOUT_FILENO) == -1) {
        blog_error("dup2(STDOUT) error %d", errno);
        abort();
    }
#if 0
    if (dup2(fd, STDERR_FILENO) == -1) {
        blog_error("dup2(STDERR) error %d", errno);
        abort();
    }
#endif

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            blog_error("close() error %d", errno);
            abort();
        }
    }

    return 0;
}
