#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

void errExit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    uid_t user_uid = 1001;
    const char *user_home = "/home/user";
    const char *temp_file = "/tmp/testfile_root.txt";
    char user_file_path[512];

    int fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) errExit("open temp_file");

    write(fd, "Hello from root\n", 16);
    close(fd);

    if (seteuid(user_uid) < 0) errExit("seteuid user");

    const char *user_temp_file = "/tmp/testfile_user.txt";
    fd = open(user_temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) errExit("open user_temp_file");
    write(fd, "Hello from user\n", 17);
    close(fd);

    if (seteuid(0) < 0) errExit("seteuid root");

    snprintf(user_file_path, sizeof(user_file_path), "%s/copied_file.txt", user_home);

    FILE *src = fopen(user_temp_file, "r");
    if (!src) errExit("fopen src");

    FILE *dst = fopen(user_file_path, "w");
    if (!dst) errExit("fopen dst");

    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
        fwrite(buf, 1, n, dst);
    }
    fclose(src);
    fclose(dst);

    if (chown(user_file_path, user_uid, -1) < 0)
        errExit("chown user_file_path");

    if (seteuid(user_uid) < 0) errExit("seteuid user again");

    fd = open(user_file_path, O_WRONLY | O_APPEND);
    if (fd < 0) {
        printf("User can't open file for writing: %s\n", strerror(errno));
    } else {
        write(fd, "User appended this line\n", 24);
        close(fd);
        printf("User successfully wrote to the file.\n");
    }

    if (unlink(user_file_path) == 0) {
        printf("User successfully deleted the file.\n");
    } else {
        printf("User failed to delete the file: %s\n", strerror(errno));
    }

    if (seteuid(0) < 0) errExit("seteuid root final");

    return 0;
}
