#ifndef SEND_H
# define SEND_H
#include <netinet/ip_icmp.h> 
u_int16_t compute_icmp_checksum (const void *buff, int length);
int send(int sockfd, char *ip);
#endif