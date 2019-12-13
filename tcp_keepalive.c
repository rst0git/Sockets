/* tcp_keepalive.c
 *
 * References:
 * http://tldp.org/HOWTO/TCP-Keepalive-HOWTO/usingkeepalive.html
 * http://www.tldp.org/HOWTO/html_single/TCP-Keepalive-HOWTO/
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int main()
{
	int s;
	int optval;
	socklen_t optlen = sizeof(optval);

	/* Create the socket */
	if((s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	/*****************************************************************/

	/* Check the status for the keepalive option */
	if(getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("SO_KEEPALIVE is %s\n", (optval ? "ON" : "OFF"));

	/* Set the option active */
	optval = 1;
	optlen = sizeof(optval);
	if(setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("SO_KEEPALIVE is set to ON\n");

	/* Check the status again */
	if(getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("SO_KEEPALIVE is %s\n\n", (optval ? "ON" : "OFF"));

	/*****************************************************************/

	/* Check tcp_keepalive_probes */
	if(getsockopt(s, SOL_TCP, TCP_KEEPCNT, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPCNT is %d\n", optval);

	optval = 5;
	optlen = sizeof(optval);
	if(setsockopt(s, SOL_TCP, TCP_KEEPCNT, &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPCNT is set to %d\n", optval);

	/* Check tcp_keepalive_time */
	if(getsockopt(s, SOL_TCP, TCP_KEEPCNT, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPCNT is %d\n\n", optval);

	/*****************************************************************/

	/* Check tcp_keepalive_time */
	if(getsockopt(s, SOL_TCP, TCP_KEEPIDLE, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPIDLE is %d\n", optval);

	optval = 10;
	optlen = sizeof(optval);
	if(setsockopt(s, SOL_TCP, TCP_KEEPIDLE, &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPIDLE is set to %d\n", optval);

	/* Check tcp_keepalive_time */
	if(getsockopt(s, SOL_TCP, TCP_KEEPIDLE, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPIDLE is %d\n\n", optval);

	/*****************************************************************/

	/* Check tcp_keepalive_intvl */
	if(getsockopt(s, SOL_TCP, TCP_KEEPINTVL, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPINTVL is %d\n", optval);

	optval = 5;
	optlen = sizeof(optval);
	if(setsockopt(s, SOL_TCP, TCP_KEEPINTVL, &optval, optlen) < 0) {
		perror("setsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPINTVL is set to %d\n", optval);

	/* Check tcp_keepalive_intvl */
	if(getsockopt(s, SOL_TCP, TCP_KEEPINTVL, &optval, &optlen) < 0) {
		perror("getsockopt()");
		close(s);
		exit(EXIT_FAILURE);
	}
	printf("TCP_KEEPINTVL is %d\n\n", optval);

	/*****************************************************************/

	close(s);

	exit(EXIT_SUCCESS);
}
