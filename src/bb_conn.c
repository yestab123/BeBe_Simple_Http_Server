#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "bb_conn.h"
#include "bb_malloc.h"

bb_conn_t *
bb_conn_creat() {
    return (bb_conn_t *)bb_malloc(sizeof(bb_conn_t));
}

void
bb_conn_destroy(bb_conn_t *conn) {
    if (conn->fd == -1) {

    } else {

    }

    bb_free(conn);
}

void
bb_conn_init(bb_conn_t *conn) {
    
}
