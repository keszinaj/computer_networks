#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip_icmp.h> //change
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

int send_packet(int sockfd, char *ip, int ttl)
{
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = 69;
    header.icmp_hun.ih_idseq.icd_seq= 69; //some number for checks
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum ((u_int16_t*)&header, sizeof(header));
    
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &recipient.sin_addr);
    //set ttl
    setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));


    ssize_t bytes_sent = sendto (
    sockfd,
    &header,
    sizeof(header),
    0,
    (struct sockaddr*)&recipient,
    sizeof(recipient)
    );
    printf("sent");
    return 0;
}
int send_pipe(int sockfd, char *ip, int ttl)
{
    //send 3 packet
    for(int i=0; i< 3; i++)
    {
        send_packet(sockfd, ip, ttl);

    }
    return 0;
}