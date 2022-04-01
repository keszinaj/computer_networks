#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "send.h"
#include "receive.h"

int traceroute(char *ip)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   // printf("traceroute");
	if (sockfd < 0) {
		fprintf(stderr, "socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}
    int pid = getpid();
    //int correct;
    for(int ttl = 1; ttl <= 30; ttl++)
    {
       if(!send_pipe(sockfd, ip, ttl, pid))
       {
           if(receive(sockfd, ip, pid, ttl)== 2)
              return EXIT_SUCCESS;
       }
       else{
           return EXIT_FAILURE;
       }
    }
    return EXIT_SUCCESS;
}