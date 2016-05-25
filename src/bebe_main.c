#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "bb_log.h"

extern char *optarg;
extern int optind, opterr, optopt;

/* Command Option Flag */
static int process_stop = 0;
static int process_reload = 0;
static int process_status = 0;
static char default_conf_file[] = "bebe.conf";
static char *conf_file;

/* Set Command Option -s flag */
static void
__process_flag_set(char *option) {
    if (option == NULL) {
        return;
    }
    if (strncmp("stop", option, 4) == 0) {
        process_stop = 1;
        return;
    }
    if (strncmp("reload", option, 6) == 0) {
        process_reload = 1;
        return;
    }
    if (strncmp("status", option, 6) == 0) {
        process_status = 1;
        return;
    }
}

/* Parse the command input option */
static void
__cmd_parse(int argc, char **argv) {
#define SIGNAL_ACT_SIZE 20
    char   ch;
    int    signal_flag = 0;
    char   signal_act[SIGNAL_ACT_SIZE];


    while((ch = getopt(argc, argv, "s:c:")) != -1) {
        switch(ch) {
        case 's':
            strncpy(signal_act, optarg, SIGNAL_ACT_SIZE - 1);
            signal_flag = 1;
            break;
        case 'c':
            conf_file = bb_malloc(strlen(optarg) + 1);
            strncpy(conf_file, optarg, strlen(optarg));
            conf_file[strlen(optarg)] = '\0';
            break;
        default:
            break;
        }
    }

    if (signal_flag == 1) {
        __process_flag_set(signal_act);
    }

}

/* If pid file exist return 0, else return -1 */
static int
__pid_file_exist(char *pid_file) {
    int  fd;

    fd = open(pid_file, O_CREAT|O_EXCL);
    if (fd < 0) {
        return 0;
    }

    close(fd);
    unlink(pid_file);

    return -1;
}

/* Get pid from file */
static int
__pid_get(char *pid_file) {
    int  fd, pid;

    fd = open(pid_file, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    read(fd, (char *)&pid, sizeof(pid));
    close(fd);

    return pid;
}

/* Save pid to file */
static int
__pid_file_save(char *pid_file, int pid) {
    int fd;

    fd = open(pid_file, O_CREAT|O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    write(fd, (char *)&pid, sizeof(pid));
    close(fd);

    return 0;
}


/* Main */
int
main(int argc, char **argv) {
    char *pid_file = NULL;
    int   pid;

    /* Parse Command Option */
    conf_file = default_conf_file;
    __cmd_parse(argc, argv);

    /* Get pid file path */

    /* Command Option Handle */
    pid = __pid_get(pid_file);
    if (process_stop == 1) {

        exit(0);
    }

    if (process_reload == 1) {

        exit(0);
    }

    if (process_status == 1) {

        exit(0);
    }

    /* Load Conf */

    /* Start Program */
    int  pid, res;
    res = __pid_file_exist(pid_file);
    if (res == 0) {
        blog_error("bebe http server already running.");
        abort();
    }
    pid = getpid();
    res = __pid_file_save(pid_file, pid);
    if (res < 0) {
        blog_error("save pid file error %d", errno);
        abort();
    }

}
