#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int receive(int fd)
{
    int received = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    for(int i = 0; i++; i<3)
    {
        struct sockaddr_in 	sender;	
		socklen_t sender_len = sizeof(sender);
		u_int8_t buffer[IP_MAXPACKET];
        int ready = select(fd, &readfds, NULL, NULL, &timeout);
        if(ready == 0)
        {
            break;
        }
        if(ready < 0){
            fprintf(stderr, "Error: Select error %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        ssize_t packet_len = recvfrom (fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
		if (packet_len < 0) {
			fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
			return EXIT_FAILURE;
		}
        char sender_ip_str[20]; 
		inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str, sizeof(sender_ip_str));
		printf ("Received IP packet with ICMP content from: %s\n", sender_ip_str);
    }
    return EXIT_SUCCESS;

    
}