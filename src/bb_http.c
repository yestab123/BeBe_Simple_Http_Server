#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

/*
  HTTP PHASE FIND Function.
  If length not enough, return -2, and try parse again.
  If not find any method, return -1, and close connection.
  If find method, return 0.
 */
int
http_method_parse(bb_conn_t *conn, char *buff, int length) {
    if (length < 3) {
        return -2;
    }

    if (strncmp(buff, "GET", 3) == 0) {
        conn->http.method = METHOD_GET;
        goto METHOD_PHASE_CHANGE;
    }

    if (length < 4) {
        return -2;
    }

    if (strncmp(buff, "POST", 4) == 0) {
        conn->http.method = METHOD_POST;
        goto METHOD_PHASE_CHANGE;
    }

    if (strncmp(buff, "HEAD", 4) == 0) {
        conn->http.method = METHOD_HEAD;
        goto METHOD_PHASE_CHANGE;
    }

    return -1;

 METHOD_PHASE_CHANGE:
    conn->phase &= ^HTTP_METHOD_PHASE;
    conn->phase &= HTTP_URL_PHASE;
    return 0;
}


/*
  HTTP URL POSITION FIND Function
  Find url start and end position idx.
  If method ERROR, return -1, and close connection.
  If not find url start or end blank position, return -2, and try it again when recv more buff.
  If find all start and end position, return 0. Goto Next Phase.
 */
int
http_url_position(bb_conn_t *conn, char *buff, int length) {
    int start;

    switch(conn->phase) {
    case METHOD_GET:
        start = 3;
        break;
    case METHOD_HEAD:
    case METHOD_POST:
        start = 4;
        break;
    default:
        return -1;
    }

    int pos = start;
    int flag = 0;
    while (pos < length) {
        if (buff[pos] != ' ') {
            if (flag == 0) {
                flag = 1;
                conn->http.url.start = pos;
            }
        } else {
            if (flag == 1) {
                flag = 2;
                conn->http.url.end = pos - 1;
                conn->http.url.len = conn->http.url.end - conn->http.url.start + 1;
                conn->phase &= ^HTTP_URL_PHASE;
                conn->phase &= HTTP_CONTENT_LENGTH_PHASE;
                return 0;
            }
        }
    }

    return -2;
}

/*
  Find Content-Length on Buff.
  If method is GET or HEAD, Finish this phase, and goto send result phase
 */
int
http_content_length_phase(bb_conn_t *conn, char *buff, int length) {
    if (conn->http.method == GET || conn->http.method == HEAD) {
        conn->phase = 0;
        conn->phase = CONN_RECV_FINISH_PHASE;
        return 0;
    }
}
