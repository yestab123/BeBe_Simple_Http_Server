#ifndef _H_BB_HTTP_
#define _H_BB_HTTP_

#include "bb_conn.h"

int http_method_parse(bb_conn_t *conn, char *buff, int length);
int http_url_position(bb_conn_t *conn, char *buff, int length);
int http_content_length_phase(bb_conn_t *conn, char *buff, int length);

#endif
