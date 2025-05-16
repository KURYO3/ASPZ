#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define UID_THRESHOLD 1000

int main() {
    FILE *fp;
    char line[1024];

    uid_t current_uid = getuid();
    struct passwd *pw = getpwuid(current_uid);
    if (pw == NULL) {
        perror("getpwuid");
        return 1;
    }
    const char *current_user = pw->pw_name;

    fp = popen("getent passwd", "r");
    if (fp == NULL) {
        perror("popen");
        return 1;
    }

    printf("Regular users (UID > %d), except current (%s):\n", UID_THRESHOLD, current_user);
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *username = strtok(line, ":");
        strtok(NULL, ":");
        char *uid_str = strtok(NULL, ":");

        if (!username || !uid_str)
            continue;

        int uid = atoi(uid_str);

        if (uid >= UID_THRESHOLD && strcmp(username, current_user) != 0) {
            printf("  %s (UID: %d)\n", username, uid);
        }
    }

    pclose(fp);
    return 0;
}
