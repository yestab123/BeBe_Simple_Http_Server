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
#include <signal.h>

#include "bb_log.h"
#include "bb_malloc.h"
#include "bb_config.h"
#include "bb_signal.h"
#include "bb_daemon.h"

extern char *optarg;
extern int optind, opterr, optopt;

extern int SIGTERM_FLAG;

/* Command Option Flag */
static int process_stop = 0;
static int process_reload = 0;
static int process_status = 0;
static char default_conf_file[] = "bebe.conf";
static char *conf_file = NULL;

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

    /* s -> signal command option(such as:reload, stop, status) */
    /* c -> setting user define conf file path */
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

static int
__pid_file_delete(char *pid_file) {
    unlink(pid_file);
    return 0;
}

/* Main */
int
main(int argc, char **argv) {
    char  pid_file[100];
    char *_conf_file = NULL;
    int   pid, res;


    signal_handle_set(SIGTERM, signal_SIGTERM_handle);
    signal_handle_set(SIGINT, signal_SIGTERM_handle);

    /* Parse Command Option */
    __cmd_parse(argc, argv);

    /* Setting Config file path */
    if (conf_file == NULL) {
        _conf_file = default_conf_file;
    } else {
        _conf_file = conf_file;
    }
    bb_config_load(_conf_file);

    /* Get pid file path from conf_file */
    res = bb_config_get_string("pid_file", pid_file, 100);
    if (res == -1) {
        blog_error("pid_file item not find in conf file");
        abort();
    }

    /* Command Option Handle */
    pid = __pid_get(pid_file);
    if (process_stop == 1) {
        if (pid == -1) {
            blog_error("program not running");
            exit(0);
        }
        /* Send SIGTERM signal and send SIGKILL signal */
        kill(pid, SIGTERM);
        blog_info("Sending SIGTERM to program(pid:%d)", pid);
        sleep(10);
        kill(pid, SIGKILL);
        blog_info("Sending SIGKILL to program(pid:%d)", pid);
        exit(0);
    }

    if (process_reload == 1) {
        if (pid == -1) {
            blog_error("program not running");
            exit(0);
        }
        /* Use signo 35 as reload signal */
        kill(pid, 35);
        blog_info("Sending reload signal to program(pid:%d)", pid);
        exit(0);
    }

    if (process_status == 1) {
        if (pid == -1) {
            blog_error("program not running");
        } else {
            blog_error("program is running(pid:%d)", pid);
        }
        exit(0);
    }

    /* Load Conf */

    /* Start Program */
    /* 1.Program Exist Judge */
    res = __pid_file_exist(pid_file);
    if (res == 0) {
        blog_error("bebe http server already running.");
        abort();
    }

    bb_set_into_daemon();

    pid = getpid();
    res = __pid_file_save(pid_file, pid);
    if (res < 0) {
        blog_error("save pid file error %d", errno);
        abort();
    }

    /* 2.Init Main Frame */

    /* 3.Start Main Logic */
    while(SIGTERM_FLAG == 0) {
        sleep(1);
    }

    /* 4.Clean and Exit */
    __pid_file_delete(pid_file);

}
