#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


int SIGTERM_FLAG = 0;

void
signal_SIGTERM_handle(int signo) {
    SIGTERM_FLAG = 1;
}

int
signal_handle_set(int signo, void (*call_back)(int signo)) {
    struct sigaction sa;

    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = call_back;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);

    return sigaction(signo, &sa, NULL);
}
