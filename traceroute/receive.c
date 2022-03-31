#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define EXIT_END 2;
int check_correctnes(int id, int seq, struct icmp* icmp_header, u_int8_t *icmp_len)
{
    if(icmp_header->icmp_type != ICMP_TIME_EXCEEDED && icmp_header->icmp_type != ICMP_ECHOREPLY)
        return EXIT_FAILURE;  
    if(icmp_header->icmp_type == ICMP_ECHOREPLY)
        return EXIT_END;
    struct ip *ip_header_inside = (struct ip*)(icmp_len + 8);
    u_int8_t *icmp_len_inside = icmp_len + 8 + 4 * ip_header_inside->ip_hl;
    struct icmp *icmp_header_inside = (struct icmp*) icmp_len_inside;
    if((int)icmp_header_inside->icmp_hun.ih_idseq.icd_id != id)
    {
        return EXIT_FAILURE;
    }
    uint_16_t seq_received = icmp_header_inside->icmp_hun.ih_idseq.icd_seq;
    if(seq_received < 4*seq || seq_received > 4*seq+4)
    {
        return EXIT_FAILURE;
    }
        //return 0;
    return EXIT_SUCCESS;
}


int receive(int fd, int id, int seq)
{
    int received = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    char sender_ip_str[3][20];
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int time_to_responde = 0;
    int pom;
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
        if(ready < 0){
            fprintf(stderr, "Error: Select error %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        ssize_t packet_len = recvfrom (fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
		if (packet_len < 0) {
			fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
			return EXIT_FAILURE;
		}
        //Encapsulation
        struct ip *ip_header = (struct ip*)buffer;
        u_int8_t *icmp_len = buffer + 4 * ip_header->ip_hl;
        struct icmp *icmp_header = (struct icmp*) icmp_len;
        pom= check_correctnes(id, seq, icmp_header, icmp_len);
        if(pom == 1)
        {
            inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str[received], sizeof(sender_ip_str[received]));
            received++;
        }
        else if(pom == 2)
        {
            inet_ntop(AF_INET, &(sender.sin_addr), sender_ip_str[received], sizeof(sender_ip_str[received]));
            printf("%s %ld",sender_ip_str[0], 1000000 - timeout.tv_usec);
            return 1;
        }
        if(received == 1)
        {
            time_to_responde = 1000000 - timeout.tv_usec;
        }
    }
    if(received == 0)
    {
        printf("* *\n");
        return 0;
    }
    if(received == 1)
    {
        printf("%s ?\n", sender_ip_str[0]);
        return 0;
    }
    int same = strcmp(sender_ip_str[0], sender_ip_str[1]);
    if(received == 2)
    {
        if(same == 0)
        {
            printf("%s ?\n", sender_ip_str[0]);
        }
    }
    else if(received == 3)
    {
        if(same == 0)
        {
            if(strcmp(sender_ip_str[0], sender_ip_str[2])==0)
                printf("%s %d\n", sender_ip_str[0], time_to_responde);
            else
            {
                printf("%s ?\n", sender_ip_str[0]);
                printf("%s ?\n", sender_ip_str[2]); ///eeee an pewno?
            }
        }
        else if(strcmp(sender_ip_str[0], sender_ip_str[2])==1 && strcmp(sender_ip_str[1], sender_ip_str[2])==1)
        {
            printf("%s ?\n", sender_ip_str[0]);
            printf("%s ?\n", sender_ip_str[1]);
            printf("%s ?\n", sender_ip_str[2]);
        }
        else{
            printf("%s ?\n", sender_ip_str[0]);
            printf("%s ?\n", sender_ip_str[1]);
        }
        
    }
    return EXIT_SUCCESS;

}