#include <cstdio>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

constexpr size_t MAX_SOCKETS   = 16;
constexpr size_t MAX_TMPFILES  = 16;

static int   g_sockets[MAX_SOCKETS];
static size_t g_num_sockets = 0;

static char g_tmpfilenames[MAX_TMPFILES][256];
static size_t g_num_tmpfiles = 0;

static void safe_log(const char *msg) {
    size_t len = strlen(msg);
    write(2, msg, len);
}

bool register_socket(int sockfd) {
    if (g_num_sockets >= MAX_SOCKETS) return false;
    g_sockets[g_num_sockets++] = sockfd;
    return true;
}

bool register_tmpfile(const char *path) {
    if (g_num_tmpfiles >= MAX_TMPFILES) return false;
    strncpy(g_tmpfilenames[g_num_tmpfiles], path, sizeof(g_tmpfilenames[0]) - 1);
    g_tmpfilenames[g_num_tmpfiles][sizeof(g_tmpfilenames[0]) - 1] = '\0';
    ++g_num_tmpfiles;
    return true;
}

static void signal_handler(int signo, siginfo_t *info, void *ucontext) {
    (void)ucontext;
    safe_log(">>> signal_handler: caught fatal signal\n");
    for (size_t i = 0; i < g_num_sockets; ++i) {
        int fd = g_sockets[i];
        if (fd >= 0) close(fd);
    }
    for (size_t i = 0; i < g_num_tmpfiles; ++i) {
        const char *path = g_tmpfilenames[i];
        if (path[0] != '\0') unlink(path);
    }
    _exit(EXIT_FAILURE);
}

static bool setup_signal_handlers() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);
    if (sigaction(SIGINT,  &sa, nullptr) == -1) return false;
    if (sigaction(SIGTERM, &sa, nullptr) == -1) return false;
    if (sigaction(SIGSEGV, &sa, nullptr) == -1) return false;
    if (sigaction(SIGBUS,  &sa, nullptr) == -1) return false;
    if (sigaction(SIGFPE,  &sa, nullptr) == -1) return false;
    return true;
}

int create_and_register_tmpfile() {
    char template_name[] = "/tmp/myapp_tmp_XXXXXX";
    int fd = mkstemp(template_name);
    if (fd == -1) return -1;
    register_tmpfile(template_name);
    return fd;
}

int create_and_register_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return -1;
    register_socket(sockfd);
    return sockfd;
}

int main() {
    if (!setup_signal_handlers()) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    int tmpfd = create_and_register_tmpfile();
    if (tmpfd < 0) {
        perror("mkstemp");
    } else {
        const char *msg = "Hello from temporary file\n";
        write(tmpfd, msg, strlen(msg));
    }

    int sockfd = create_and_register_socket();
    if (sockfd < 0) {
        perror("socket");
    }

    safe_log(">>> Main loop is running. Try sending SIGINT (Ctrl+C) or use a segfault.\n");

    while (true) {
        pause();
    }

    return EXIT_SUCCESS;
}
