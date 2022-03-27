#ifndef RECEIVE_H
# define RECEIVE_H
int receive(int fd, int id, int seq);
int check_correctnes(int fd, int id, int seq, struct icmp* icmp_header, u_int8_t *icmp_len);
#endif