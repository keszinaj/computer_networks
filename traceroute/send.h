#ifndef SEND_H
# define SEND_H
#include <netinet/ip_icmp.h> 
u_int16_t compute_icmp_checksum (const void *buff, int length);
int send_packet(int sockfd, char *ip, int ttl);
int send_pipe(int sockfd, char *ip, int ttl);
#endif