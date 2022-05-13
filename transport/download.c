#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
int download()
{
    // na podsawiw wykładu 4
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
	{
		fprintf(stderr, "Error: Problem with socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
    struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
    int correct = inet_pton(AF_INET, addr, &server_address.sin_addr);
    if(correct<=0)
    {
        fprintf(stderr, "Error: Wrong Ip address"); 
        return EXIT_FAILURE;
    }
}