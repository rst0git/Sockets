// compile with: gcc -static -o recvfd recvfd.c
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCKET_PATH "test.sock"

int recvfd(int socket) {
	int len;
	int fd;
	char buf[1];
	struct iovec iov;
	struct msghdr msg;
	struct cmsghdr *cmsg;
	char cms[CMSG_SPACE(sizeof(int))];

	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);

	msg.msg_name = 0;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_flags = 0;
	msg.msg_control = (caddr_t) cms;
	msg.msg_controllen = sizeof cms;

	len = recvmsg(socket, &msg, 0);

	if (len < 0) {
		perror("recvmsg failed");
		return -1;
	}

	if (len == 0) {
		perror("recvmsg failed no data");
		return -1;
	}

	cmsg = CMSG_FIRSTHDR(&msg);
	memmove(&fd, CMSG_DATA(cmsg), sizeof(int));
	return fd;
}

void error(const char *msg) {
	perror(msg);
	exit(0);
}

int main(int argc, char **argv)
{
	int filefd;
	int sockfd;
	int servlen;
	int written_len;
	struct sockaddr_un serv_addr;

	if (argc != 2) {
		printf("Usage: recvfd <message>\n");
		exit(1);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, SOCKET_PATH);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		error("Creating socket");

	printf("Connecting to %s\n", SOCKET_PATH);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
		error("Connecting");

	filefd = recvfd(sockfd);
	if (filefd < 0)
		error("receiving file descriptor");

	printf("Received fd %d\n", filefd);

	written_len = write(filefd, argv[1], strlen(argv[1]));
	if (written_len < 0)
		error("writing to file");

	printf("Written %d bytes (%s)\n", written_len, argv[1]);

	close(sockfd);
	return 0;
}
