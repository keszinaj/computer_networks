#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip_icmp.h> 
#include <assert.h>
u_int16_t compute_icmp_checksum (const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

int send_pipe(int sockfd, char *ip, int ttl, int pid)
{
    //create ICMP Packet
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = pid;
    header.icmp_hun.ih_idseq.icd_seq= ttl*4; //must be different
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum ((u_int16_t*)&header, sizeof(header));
    
    int correct;
    //structure deskribing socket adres
    struct sockaddr_in recipient;
    memset(&recipient, 0, sizeof(recipient));
    recipient.sin_family = AF_INET;
    
    // convert IPv4 and IPv6 addresses from text to binary form
    correct = inet_pton(AF_INET, ip, &recipient.sin_addr);
    if(correct<=0)
    {
        fprintf(stderr, "Error: Wrong Ip address"); 
        return EXIT_FAILURE;
    }
    //we must set ttl using this function
    correct = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    if(correct < 0) {
		fprintf(stderr, "Error: socket error: %s\n", strerror(errno)); 
		return EXIT_FAILURE;
	}
    //send 3 packet
    for(int i=0; i< 3; i++)
    {
        //seq must be different every time 
        header.icmp_hun.ih_idseq.icd_seq+=i;
        //sent packet
        correct = sendto(sockfd, &header, sizeof(header), 0, (struct sockaddr*)&recipient, sizeof(recipient));
        if(correct < 0) {
		   fprintf(stderr, "Error: sending failed: %s\n", strerror(errno)); 
		   return EXIT_FAILURE;
	    }
    }
    return EXIT_SUCCESS;
}
