#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>

void die(const char *f)
{
	printf("%s: %s\n", f, strerror(errno));
	exit(1);
}

int main(void)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0)
		die("socket");

	int rcvbuf, sndbuf;
	socklen_t optlen = sizeof(rcvbuf);
	if(getsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &optlen) < 0)
		die("getsockopt (1)");
	if(getsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbuf, &optlen) < 0)
		die("getsockopt (1)");
	printf("initial rcvbuf: %d\n", rcvbuf);
	printf("initial sndbuf: %d\n", sndbuf);

	rcvbuf = sndbuf = INT_MAX / 2;

	if(setsockopt(s, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf)) < 0)
		die("setsockopt");
	if(setsockopt(s, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf)) < 0)
		die("setsockopt");
	printf("set rcvbuf to %d\n", rcvbuf);
	printf("set sndbuf to %d\n", sndbuf);

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.s_addr = INADDR_ANY;
	if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		die("bind");

	if(listen(s, 10) < 0)
		die("listen");

	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int s2 = accept(s, (struct sockaddr *)&client_addr, &addr_len);
	if(s2 < 0)
		die("accept");

	printf("accepted connection\n");
	optlen = sizeof(rcvbuf);
	if(getsockopt(s2, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &optlen) < 0)
		die("getsockopt (2)");
	if(getsockopt(s2, SOL_SOCKET, SO_SNDBUF, &sndbuf, &optlen) < 0)
		die("getsockopt (2)");
	printf("new rcvbuf: %d\n", rcvbuf);
	printf("new sndbuf: %d\n", sndbuf);


	return 0;
}
