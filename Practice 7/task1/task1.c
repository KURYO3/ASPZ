#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *fp_rwho, *fp_more;
    char buffer[4096];

    fp_rwho = popen("rwho", "r");
    if (fp_rwho == NULL) {
        perror("popen rwho");
        exit(EXIT_FAILURE);
    }

    fp_more = popen("more", "w");
    if (fp_more == NULL) {
        perror("popen more");
        pclose(fp_rwho);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp_rwho) != NULL) {
        fputs(buffer, fp_more);
    }

    if (pclose(fp_rwho) == -1) {
        perror("pclose rwho");
    }
    if (pclose(fp_more) == -1) {
        perror("pclose more");
    }

    return 0;
}
