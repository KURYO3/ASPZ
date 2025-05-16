#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FS_FILENAME   "filesystem.dat"
#define MAX_FILES     128
#define MAX_NAME_LEN  32
#define BUFFER_SIZE   4096

typedef struct {
    char     name[MAX_NAME_LEN];
    long     offset;
    long     size;
    int      active;
} FileEntry;

typedef struct {
    int        fileCount;
    FileEntry  entries[MAX_FILES];
} FSHeader;

FSHeader load_header(FILE *fs) {
    FSHeader hdr;
    fseek(fs, 0, SEEK_END);
    if (ftell(fs) == 0) {
        hdr.fileCount = 0;
        for (int i = 0; i < MAX_FILES; i++) hdr.entries[i].active = 0;
        fseek(fs, 0, SEEK_SET);
        fwrite(&hdr, sizeof(hdr), 1, fs);
        fflush(fs);
    } else {
        fseek(fs, 0, SEEK_SET);
        fread(&hdr, sizeof(hdr), 1, fs);
    }
    return hdr;
}

void save_header(FILE *fs, FSHeader *hdr) {
    fseek(fs, 0, SEEK_SET);
    fwrite(hdr, sizeof(*hdr), 1, fs);
    fflush(fs);
}

void cmd_init() {
    FILE *fs = fopen(FS_FILENAME, "wb+");
    if (!fs) { perror("init: fopen"); exit(1); }
    FSHeader hdr = {0};
    fwrite(&hdr, sizeof(hdr), 1, fs);
    fclose(fs);
    printf("The file system is initialized.\n");
}

// Створити файл
void cmd_create(const char *name, const char *srcpath) {
    FILE *fs = fopen(FS_FILENAME, "rb+");
    if (!fs) { fprintf(stderr, "FS not found - run init first\n"); exit(1); }

    FSHeader hdr = load_header(fs);
    if (hdr.fileCount >= MAX_FILES) {
        fprintf(stderr, "Maximum files reached\n"); fclose(fs); exit(1);
    }
    for (int i = 0; i < MAX_FILES; i++)
        if (hdr.entries[i].active && strcmp(hdr.entries[i].name, name) == 0) {
            fprintf(stderr, "File '%s' already exists\n", name);
            fclose(fs); exit(1);
        }

    FILE *src = fopen(srcpath, "rb");
    if (!src) { perror("create: fopen source"); fclose(fs); exit(1); }
    struct stat st; stat(srcpath, &st);
    long size = st.st_size;

    int idx = 0;
    while (hdr.entries[idx].active) idx++;

    fseek(fs, 0, SEEK_END);
    long offset = ftell(fs);
    char buf[BUFFER_SIZE];
    size_t r;
    while ((r = fread(buf,1,sizeof(buf),src)) > 0)
        fwrite(buf,1,r,fs);
    fclose(src);

    strncpy(hdr.entries[idx].name, name, MAX_NAME_LEN-1);
    hdr.entries[idx].offset = offset;
    hdr.entries[idx].size   = size;
    hdr.entries[idx].active = 1;
    hdr.fileCount++;

    save_header(fs, &hdr);
    fclose(fs);
    printf("Created '%s' (%ld bytes)\n", name, size);
}

void cmd_delete(const char *name) {
    FILE *fs = fopen(FS_FILENAME, "rb+");
    if (!fs) { fprintf(stderr, "FS not found\n"); exit(1); }
    FSHeader hdr = load_header(fs);
    int found = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if (hdr.entries[i].active && strcmp(hdr.entries[i].name, name)==0) {
            hdr.entries[i].active = 0;
            hdr.fileCount--;
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "File '%s' not found\n", name);
        fclose(fs); exit(1);
    }
    save_header(fs, &hdr);
    fclose(fs);
    printf("Deleted '%s'\n", name);
}

void cmd_read(const char *name, const char *dest) {
    FILE *fs = fopen(FS_FILENAME, "rb");
    if (!fs) { fprintf(stderr, "FS not found\n"); exit(1); }
    FSHeader hdr = load_header(fs);
    FileEntry *e = NULL;
    for (int i = 0; i < MAX_FILES; i++)
        if (hdr.entries[i].active && strcmp(hdr.entries[i].name, name)==0) {
            e = &hdr.entries[i];
            break;
        }
    if (!e) {
        fprintf(stderr, "File '%s' not found\n", name);
        fclose(fs); exit(1);
    }
    FILE *out = stdout;
    if (dest) {
        out = fopen(dest, "wb");
        if (!out) { perror("read: fopen dest"); fclose(fs); exit(1); }
    }
    // копіюємо
    fseek(fs, e->offset, SEEK_SET);
    long remaining = e->size;
    char buf[BUFFER_SIZE];
    while (remaining > 0) {
        size_t toread = remaining>BUFFER_SIZE?BUFFER_SIZE:remaining;
        fread(buf,1,toread,fs);
        fwrite(buf,1,toread,out);
        remaining -= toread;
    }
    if (out!=stdout) fclose(out);
    fclose(fs);
    if (!dest) printf("\n");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr,
            "Using:\n"
            "  %s init\n"
            "  %s create <name> <source>\n"
            "  %s delete <name>\n"
            "  %s read <name> [<dest>]\n", argv[0],argv[0],argv[0],argv[0]);
        return 1;
    }

    if (strcmp(argv[1],"init")==0) {
        cmd_init();
    } else if (strcmp(argv[1],"create")==0 && argc==4) {
        cmd_create(argv[2], argv[3]);
    } else if (strcmp(argv[1],"delete")==0 && argc==3) {
        cmd_delete(argv[2]);
    } else if (strcmp(argv[1],"read")==0 && (argc==3||argc==4)) {
        cmd_read(argv[2], argc==4?argv[3]:NULL);
    } else {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    return 0;
}
