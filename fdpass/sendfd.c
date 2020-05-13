#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#define SOCKET_PATH "test.sock"

static void error(const char *msg) {
	perror(msg);
	exit(0);
}

int send_fd(int sock, int fd)
{
	struct msghdr msg;
	struct iovec iov[1];
	struct cmsghdr *cmsg = NULL;
	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	memset(&msg, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	data[0] = ' ';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_controllen =  CMSG_SPACE(sizeof(int));
	msg.msg_control = ctrl_buf;

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));

	*((int *) CMSG_DATA(cmsg)) = fd;

	printf("Sending fd %d\n", fd);
	return sendmsg(sock, &msg, 0);
}

int main(int argc, char **argv)
{
	struct sockaddr_un serv_addr;
	int sockfd;
	int filefd;
	int connfd;
	int servlen;

	if (argc != 2) {
		printf("Usage: sendfd <file path>\n");
		exit(1);
	}

	filefd = open(argv[1], O_RDWR);
	if (filefd < 0)
		error("can't open file");

	printf("Opened file descriptor %d for %s\n", filefd, argv[1]);

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		error("creating socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, SOCKET_PATH);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if(bind(sockfd, (struct sockaddr *)&serv_addr, servlen) < 0)
		error("binding socket");

	listen(sockfd, 1);
	printf("Listening on %s\n", SOCKET_PATH);

	// send the file descriptor to anyone who connects
	while(1) {
		connfd = accept(sockfd, NULL, 0);
		printf("Accepted connection with fd %d\n", connfd);
		send_fd(connfd, filefd);
		close(connfd);
	}

	return 0;
}
