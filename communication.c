#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2]; // File descriptors for the pipe
    pid_t pid;

    // pipe creation
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // child process
    if ((pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // child process
        close(fd[0]); 

        // redirect stdout to the write end of the pipe
        dup2(fd[1], STDOUT_FILENO);

        // Execute a command ls
        execlp("ls", "ls", NULL);

        // If execlp() fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } else { // parent now
        close(fd[1]); // 

        // redirect stdin to the read end of the pipe
        dup2(fd[0], STDIN_FILENO);

        // execute another command grep
        execlp("grep", "grep", "keyword", NULL);

        // If execlp() fails
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    return 0;
}
