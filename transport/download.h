#ifndef DOWNLOAD_H
# define DOWNLOAD_H
#include <netinet/ip_icmp.h> 
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
int get_request(int sockfd, struct sockaddr_in addr, int start, int length);
void save_in_file(FILE *fd);
int receive_data(int sent, int fd, struct sockaddr_in server_address, FILE *file);
int download(char *addr, int port, FILE *file, int s);

#endif