#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

void print_mode(mode_t mode) {
    char str[11] = "----------";

    if      (S_ISDIR(mode)) str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISFIFO(mode))str[0] = 'p';
    else if (S_ISSOCK(mode))str[0] = 's';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';

    printf("%s ", str);
}

int main(void) {
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char timebuf[64];

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        if (lstat(entry->d_name, &sb) == -1) {
            fprintf(stderr, "lstat(%s): %s\n", entry->d_name, strerror(errno));
            continue;
        }

        print_mode(sb.st_mode);

        printf("%2lu ", (unsigned long)sb.st_nlink);

        struct passwd *pw = getpwuid(sb.st_uid);
        printf("%-8s ", pw ? pw->pw_name : "?");

        struct group  *gr = getgrgid(sb.st_gid);
        printf("%-8s ", gr ? gr->gr_name : "?");

        printf("%8lld ", (long long)sb.st_size);

        struct tm *tm = localtime(&sb.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);
        printf("%s ", timebuf);

        printf("%s", entry->d_name);
        if (S_ISLNK(sb.st_mode)) {
            char linktarget[PATH_MAX+1];
            ssize_t len = readlink(entry->d_name, linktarget, sizeof(linktarget)-1);
            if (len != -1) {
                linktarget[len] = '\0';
                printf(" -> %s", linktarget);
            }
        }

        printf("\n");
    }

    closedir(dir);
    return 0;
}
