#ifndef RECEIVE_H
# define RECEIVE_H
int receive(int fd, int rn);
int check_correctnes(int fd, int rn, struct icmp icmp_header);
#endif