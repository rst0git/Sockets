#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <stdio.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void client(const char *filename)
{
	int sockfd, servlen, n;
	const int msglen = 81;
	struct sockaddr_un serv_addr;
	char buffer[msglen];
	bzero(buffer, msglen);

	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, filename);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
		error("Creating socket");

	if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
		error("Connecting");

	printf("Please enter your message: ");
	fgets(buffer, msglen - 1, stdin);
	write(sockfd, buffer, strlen(buffer));

	n = read(sockfd, buffer, msglen - 1);

	printf("The return message was\n");
	write(STDOUT_FILENO, buffer, n);
	close(sockfd);
}

void server(const char *filename)
{
	int sockfd,  newsockfd,  servlen,  n;
	socklen_t clilen;
	struct sockaddr_un  cli_addr,  serv_addr;
	char buf[80];

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		error("creating socket");

	bzero((char *) &serv_addr,  sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, filename);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

	if(bind(sockfd, (struct sockaddr *)&serv_addr, servlen)<0)
		error("binding socket");

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	if (newsockfd < 0)
		error("accepting");

	n = read(newsockfd, buf, 80);
	printf("A connection has been established\n");

	write(STDOUT_FILENO, buf, n);
	write(newsockfd, "I got your message\n", 19);

	close(newsockfd);
	close(sockfd);
	unlink(filename);
}

void error_usage(const char *executable)
{
	printf("Usage:\n\t%s c|s\n", executable);
	exit(0);
}

int main(int argc,  char *argv[])
{
	const char *sockname = "test.socket";

	if (argc < 2)
		error_usage(argv[0]);

	if (!strcmp(argv[1], "s"))
		server(sockname);
	else if(!strcmp(argv[1], "c"))
		client(sockname);
	else
		error_usage(argv[0]);

	return 0;
}
