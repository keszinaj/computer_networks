#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int check_correctnes(int fd, int rn, struct icmp* icmp_header)
{
    //ICMP_TIME_EXCEEDED - when ttl == 0
    //ICMP_ECHOREPLY when reach ip
    if(icmp_header->icmp_type != ICMP_TIME_EXCEEDED)
    {
        if(icmp_header->icmp_type == ICMP_ECHOREPLY)
        {
            return 2;
        }
        return -1;
    }
    if(ntohs(icmp_header->icmp_hun.ih_idseq.icd_id) != getpid())
    {
        printf("\n%d\n", icmp_header->icmp_hun.ih_idseq.icd_seq);
        return -1;
    }
    if(ntohs(icmp_header->icmp_hun.ih_idseq.icd_seq) != rn)
    {
        printf("rn");
        return -1;
    }
    return 0;
}
int receive(int fd, int rn)
{
    int received = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    for(int i = 0; i<3;i++)
    {
        struct sockaddr_in 	sender;	
		socklen_t sender_len = sizeof(sender);
		u_int8_t buffer[IP_MAXPACKET];
        int ready = select(fd+1, &readfds, NULL, NULL, &timeout);
        //chceck if timeout
        if(ready == 0)
        {
            break;
        }
        //other errors
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
        
        //code from presentation{41}
        //read icmp header
        struct ip* ip_header = (struct ip*) buffer;
        u_int8_t* icmp_packet = buffer + 4 * ip_header->ip_hl;
        struct icmp* icmp_header = (struct icmp*) icmp_packet;
        printf("\n%d\n", icmp_header->icmp_hun.ih_idseq.icd_seq);
        printf("\n%d\n", icmp_header->icmp_hun.ih_idseq.icd_id);
       // printf("nie jes zle");
      // if(check_correctnes(fd, rn, icmp_header) == 0)
      //  {
      //      printf("nie jes zle\n");
      //  }

    
    }
    return EXIT_SUCCESS;

}