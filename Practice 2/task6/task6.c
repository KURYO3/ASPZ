#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#define _GNU_SOURCE

int main() {
    size_t page_size = sysconf(_SC_PAGESIZE); // Отримуємо розмір сторінки пам’яті
    void *addr = mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return EXIT_FAILURE;
    }

    printf("Пам’ять виділена за адресою: %p\n", addr);
    
    strcpy(addr, "Hello, memory protection!");
    printf("Дані: %s\n", (char *)addr);

    if (mprotect(addr, page_size, PROT_READ) == -1) {
        perror("mprotect failed");
        return EXIT_FAILURE;
    }
    printf("Права змінені на тільки для читання\n");

    printf("Читаємо дані: %s\n", (char *)addr);

    printf("Пробуємо змінити дані...\n");
    strcpy(addr, "This should fail!");

    munmap(addr, page_size);

    return 0;
}