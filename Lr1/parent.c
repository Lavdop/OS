#include "utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    char filename[256];
    int fn_len = read_line(STDIN_FILENO, filename, sizeof(filename));
    if (fn_len <= 0) return 1;
    if (filename[fn_len - 1] == '\n') {
        filename[fn_len - 1] = '\0';
    }

    int fd_file = open(filename, O_RDONLY);
    if (fd_file < 0) {
        write_str(STDERR_FILENO, "Cannot open file\n");
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        close(fd_file);
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        close(fd_file);
        close(pipefd[0]);
        close(pipefd[1]);
        return 1;
    }

    if (pid == 0) {
        close(pipefd[0]);
        dup2(fd_file, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(fd_file);
        close(pipefd[1]);
        execl("./child", "child", NULL);
        _exit(1);
    }

    close(fd_file);
    close(pipefd[1]);

    char buf[1024];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }

    close(pipefd[0]);
    wait(NULL);
    return 0;
}