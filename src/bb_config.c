#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_COUNT  50
#define CONFIG_LENGTH 100

static char config_save[CONFIG_COUNT][CONFIG_LENGTH];
static int  config_count = 0;

/* Get key idx in config_save */
static int
__config_get_key_idx(char *key) {
    int i;

    if (key == NULL) {
        return -1;
    }

    for (i = 0; i < CONFIG_COUNT; i++) {
        if (strncmp(key, config_save[i], strlen(key)) == 0) {
            return i;
        }
    }

    return -1;
}

/* Get equal symbol idx in config_save[idx] */
static int
__config_get_equal_idx(int idx) {
    int i;

    if (idx >= CONFIG_COUNT) {
        return -1;
    }

    for (i = 0; i < CONFIG_LENGTH; i++) {
        if (config_save[idx][i] == '=') {
            return ++i;
        }
    }

    return -1;
}

/* Get string value from config_save[idx][e_idx] */
/* May have some problem, need to test. */
/* NEED_TO_TEST */
static char *
__config_get_string_value(int idx, int e_idx) {
    int  i, t;
    static char value[CONFIG_LENGTH];

    memset(value, '\0', CONFIG_LENGTH);

    if (config_save[idx][e_idx] != '"') {
        return NULL;
    }

    for (i = e_idx + 1, t = 0; i < CONFIG_LENGTH; i++) {
        if (config_save[idx][i] == '"') {
            if (t <= 0) {
                return NULL;
            } else {
                strncpy(value, config_save[idx]+e_idx+1, t);
                return value;
            }
        } else {
            t++;
        }
    }

    return NULL;
}

/* Delete All Blank key */
int
bb_config_delete_blank(char *save, int save_len, char *string) {
    int count = 0, i;
    int quotation_flag = 0;

    if (strlen(string) > save_len) {
        return -1;
    }

    memset(save, '\0', save_len);
    for (i = 0; i < strlen(string); i++) {
        if (string[i] == '"') {
            quotation_flag++;
        }
        if (quotation_flag == 1) {
            save[count] = string[i];
            count++;
            continue;
        }
        if (string[i] != ' ' && string[i] != '\r' && string[i] != '\n') {
            if (string[i] == '#') {
                break;
            }
            save[count] = string[i];
            count++;
        }
    }
    if (count <= 0 || quotation_flag == 1) {
        return -1;
    } else {
        return 0;
    }
}


/* Get int value */
int
bb_config_get_int(char *key, int *value) {
    int  i, t;

    if (key == NULL || value == NULL) {
        return -1;
    }

    i = __config_get_key_idx(key);
    if (i == -1) {
        return -1;
    }

    t = __config_get_equal_idx(i);
    if (t == -1) {
        return -1;
    }

    *value = atoi(config_save[i] + t);
    return 0;
}

/* Get string value */
int
bb_config_get_string(char *key, char *value, int length) {
    int   i, t;
    char *point;

    i = __config_get_key_idx(key);
    if (i == -1) {
        return -1;
    }

    t = __config_get_equal_idx(i);
    if (t == -1) {
        return -1;
    }

    point = __config_get_string_value(i, t);
    if (point == NULL) {
        return -1;
    }

    strncpy(value, point, length);
    return 0;
}

/* Test Case */
/* int */
/* main() { */
/*     char save[100]; */
/*     char test[] = " abc = \"This is a test # This is a test"; */

/*     int i = bb_config_delete_blank(save, 100, test); */
/*     printf("%d:%s\n", i, save); */
/* } */
