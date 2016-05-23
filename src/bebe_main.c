#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

extern char *optarg;
extern int optind, opterr, optopt;

/* Command Option Flag */
static int process_stop = 0;
static int process_reload = 0;
static int process_status = 0;

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
static int
__cmd_parse(int argc, char **argv) {
#define SIGNAL_ACT_SIZE 20
    char  *ch;
    int    signal_flag = 0;
    char   signal_act[SIGNAL_ACT_SIZE];

    while((ch = getopt(argc, argv, "s:")) != -1) {
        switch(ch) {
        case 's':
            strncpy(signal_act, optarg, SIGNAL_ACT_SIZE - 1);
            signal_flag = 1;
            break;
        default:
            break;
        }
    }

    if (signal_flag == 1) {
        __process_flag_set(signal_act);
    }

}

/* Main */
int
main(int argc, char **argv) {

    __cmd_parse(argc, argv);

}
