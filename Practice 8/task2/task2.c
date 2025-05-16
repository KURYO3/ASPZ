#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *filename = "testfile.bin";

    unsigned char data[10] = {4, 5, 2, 2, 3, 3, 7, 9, 1, 5};

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, data, 10);
    close(fd);

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (lseek(fd, 3, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    unsigned char buffer[4];
    ssize_t bytesRead = read(fd, buffer, 4);
    if (bytesRead != 4) {
        perror("read");
        close(fd);
        return 1;
    }

    printf("Read bytes: ");
    for (int i = 0; i < 4; ++i) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}
