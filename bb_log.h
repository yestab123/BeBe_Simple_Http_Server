#ifndef _HEAD_LOG_
#define _HEAD_LOG_

#include <stdio.h>
#include <stdlib.h>

#define LOG_DEBUG

#define TRACE  0
#define DEBUG  1
#define INFO   2
#define WARN   3
#define ERROR  4

#ifdef LOG_DEBUG
#define blog_trace(...) do {                    \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
    }while(0)
#define blog_debug(...) do {                    \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
    }while(0)
#define blog_info(...) do {                     \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
    }while(0)
#define blog_warn(...) do {                     \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
    }while(0)
#define blog_error(...) do {                    \
        printf(__VA_ARGS__);                    \
        printf("\n");                           \
    }while(0)
#else
#define blog_trace bb_log(TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define blog_debug bb_log(DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define blog_info bb_log(INFO, __FILE__, __LINE__, __VA_ARGS__)
#define blog_warn bb_log(WARN, __FILE__, __LINE__, __VA_ARGS__)
#define blog_error bb_log(ERROR, __FILE__, __LINE__, __VA_ARGS__)
#endif



#endif
