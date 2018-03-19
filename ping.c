/* Standard I/O and C libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Sockets libraries */
#include <sys/types.h>
#include <sys/socket.h>

/* Network interface libraries */
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int i, s, offset, on;
    char buf[400];
    struct ip *ip = (struct ip *)buf;
    struct icpmhdr *icmp = (struct icpmhdr *)(ip + 1);
    struct hostent *hp, *hp2;
    struct sockaddr_in destination;
    int num = 100;

    if (argc < 3)
    {
        printf("\nUsage: %s <source_address> <destination_address> [#_of_packets]\n", argv[0]);
        printf(" - #_of_packets is the number of packets to be sent, 100 is the default value\n");
        exit(1);
    }

    if (argc == 4)
    {
        num = atoi(argv[3]);

        for(i=1; i<=num; i++)
        {
            on = 1;
            bzero(buf, sizeof(buf));

            /* Kill the process if there's an error while trying to create a raw socket */

            if((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
            {
                perror("socket() error");
                exit(1);
            }

            if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0)
            {
                perror("setsockopt() for IP_HDRINCL error");
                exit(1);
            }

            if((hp = gethostbyname(argv[2])) == NULL)
            {
                if((ip->ip_dst.s_addr = inet_addr(argv[2])) == -1)
                {
                    fprintf(stderr, "%s: Unable to resolve unknown host.\n", argv[2] );
                    exit(1);
                }
            }

            else
                bcopy(hp->h_addr_list[0], &ip->ip_dst.s_addr, hp->h_length);
        }
    }

    return 0;
}
