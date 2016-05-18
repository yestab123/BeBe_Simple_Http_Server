#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

void *
bb_malloc(int size) {
    return malloc(size);
}

void
bb_free(void *point) {
    if (point != NULL) {
        free(point);
    }
}
