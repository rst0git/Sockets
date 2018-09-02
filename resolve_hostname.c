#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
        char *addrString = argv[1];
        char buff[INET6_ADDRSTRLEN];

        struct addrinfo addrCriteria;
        memset(&addrCriteria, 0, sizeof(addrCriteria));
        addrCriteria.ai_family = AF_UNSPEC;
        addrCriteria.ai_socktype = SOCK_STREAM;
        addrCriteria.ai_protocol = IPPROTO_TCP;

        struct addrinfo *addrList;
        int ret = getaddrinfo(addrString, NULL, &addrCriteria, &addrList);
        if(ret != 0) {
                fprintf(stderr, "failed to get address list\n");
                return -1;
        }

        for(struct addrinfo *p = addrList; p != NULL; p = p->ai_next) {
                void *addr;
                char *ipver;

                if (p->ai_family == AF_INET)
                {
                        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                        addr = &(ipv4->sin_addr);
                        ipver = "IPv4";
                }
                else
                {
                        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                        addr = &(ipv6->sin6_addr);
                        ipver = "IPv6";
                }

                inet_ntop(p->ai_family, addr, buff, sizeof(buff));
                printf("%s: %s\n", ipver, buff);
        }

        freeaddrinfo(addrList);
        return 0;
}