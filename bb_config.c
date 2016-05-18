#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char config_save[50][100];
static int  config_count = 0;

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

/* Test Case */
/* int */
/* main() { */
/*     char save[100]; */
/*     char test[] = " abc = \"This is a test # This is a test"; */

/*     int i = bb_config_delete_blank(save, 100, test); */
/*     printf("%d:%s\n", i, save); */
/* } */
