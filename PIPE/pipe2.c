#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void read_from_pipe(int fd)
{
    FILE *stream;
    int c;
    stream = fdopen(fd, "r");
    while((c=fgetc(stream)) != EOF)
        putchar(c);
    fclose(stream);
}

void write_to_pipe(int fd)
{
    FILE *stream;
    stream = fdopen(fd, "w");
    fprintf(stream, "Hello ");
    fprintf(stream, "World!");
    fclose(stream);
}

int main(void)
{
    pid_t pid;
    int pipefd[2];

    if (pipe(pipefd)) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid == (pid_t) 0) {
        close(pipefd[1]);
        read_from_pipe(pipefd[0]);
        return EXIT_SUCCESS;
    }
    else if (pid < (pid_t) 0) {
        fprintf(stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    } else {
        close(pipefd[0]);
        write_to_pipe(pipefd[1]);
        return EXIT_SUCCESS;
    }
    return 0;
}

