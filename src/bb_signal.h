#ifndef _H_BB_SIGNAL_
#define _H_BB_SIGNAL_

#include <signal.h>

int signal_handle_set(int signo, void (*call_back)(int signo));
void signal_SIGTERM_handle(int signo);

#endif
