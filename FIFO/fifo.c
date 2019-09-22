#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO "testfifo"

int main()
{
	int fd;
	pid_t pid;
	char buf[80];

	if (mkfifo(FIFO, 0666)) {
		perror("mkfifo");
		return -1;
	}

	if (fork() == 0) {
		fd = open(FIFO, O_WRONLY);
		write(fd, "Hello World!\n", 14);
	} else {
		fd = open(FIFO, O_RDONLY);
		read(fd, buf, 14);
		write(STDOUT_FILENO, buf, 14);
	}

	close(fd);
	unlink(FIFO);
	return 0;
}

