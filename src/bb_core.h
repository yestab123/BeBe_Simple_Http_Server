#ifndef _H_BEBE_SERVER_CORE_
#define _H_BEBE_SERVER_CORE_

#include <stdio.h>

struct bb_conf_s {
    char     web_listen_ip[32];
    int      web_listen_port;
};
typedef struct bb_conf_s bb_conf_t;

struct bb_core_s {
    bb_conf_t   *conf;
};
typedef struct bb_core_s bb_core_t;

#endif
