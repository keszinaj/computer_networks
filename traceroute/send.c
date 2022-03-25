#include <stdlib.h>
#include <stdio.h>
#include <netinet/ip_icmp.h> //change
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

int send(int sockfd, char *ip)
{
    struct icmp header;
    header.icmp_type = ICMP_ECHO;
    header.icmp_code = 0;
    header.icmp_hun.ih_idseq.icd_id = getpid();
    header.icmp_hun.ih_idseq.icd_seq= 1;//changge on number of for loop
    header.icmp_cksum = 0;
    header.icmp_cksum = compute_icmp_checksum ((u_int16_t*)&header, sizeof(header));
    
    struct sockaddr_in recipient;
    bzero (&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &recipient.sin_addr);

    int ttl = 42;
    setsockopt (sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));


    ssize_t bytes_sent = sendto (
    sockfd,
    &header,
    sizeof(header),
    0,
    (struct sockaddr*)&recipient,
    sizeof(recipient)
    );
}