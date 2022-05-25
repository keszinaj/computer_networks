#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
int const window_size = 1000;
int const packet_length = 1000;
int free_space = window_size;
int filled[window_size] = {0};
char window[window_size][packet_length];
int first_to_save = 0;
int last_to_save = 0;
int all_memory_square = 1;
int start_cash= 0;
int length_of_last_packet = 0;

int get(int sockfd, struct sockaddr_in addr, int start, int length)
{
    char buffer[40];
    sprintf(buffer, "GET %ld %ld\n", start, length);
    size_t buffer_len = strlen(buffer);
    if (sendto(sockfd, buffer, length, 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "ERROR: Problem with get %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    free(buffer);
    return 0;
}
int receive_data(int sent, int fd, struct sockaddr_in server_address)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    //set time
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    struct sockaddr_in 	sender;	
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];
    int pos, len, add ,k;
    for(int i = 0; i<sent; i++)
    {
        int ready = select(fd+1, &readfds, NULL, NULL, &timeout);
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
        if (sender.sin_addr.s_addr != server_address.sin_addr.s_addr || sender.sin_port != server_address.sin_port) {
            break;
        }
        sscanf(buffer, "DATA %ld %ld\n", &pos, &len);
        //okej teraz trzeba znalesc gdzie dac ten datagram i go tam zapisac
        if(len ==1000)
        {
            add = pos-start_cash;
            add = add/1000;
            k = first_to_save + add;
            if(k >= window_size)
            {
                k -= window_size; 
            }
            if(filled[k]==0)
            {
                memcpy(window[k][0], strchr(buffer, '\n') + 1, len);
                filled[k]=1;
            }

        }
        else{
            if(filled[last_to_save]==0)
            {
                memcpy(window[last_to_save][0], strchr(buffer, '\n') + 1, len);
                filled[last_to_save]=1;
            }
        }
    }
    save_in_file();
    return 0;
}
int download(char *addr, int port, FILE *file, int size)
{
    // na podsawiw wykładu 4
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
	{
		fprintf(stderr, "Error: Problem with socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
    struct sockaddr_in server_address;
	bzero (&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
    int correct = inet_pton(AF_INET, addr, &server_address.sin_addr);
    if(correct<=0)
    {
        fprintf(stderr, "Error: Wrong Ip address"); 
        return EXIT_FAILURE;
    }
    if(size<packet_length)
    {
        length_of_last_packet = size;
    }
    else
    {
        int cash_needed = ceil(size/1000);
        if(cash_needed>window_size)
        {
            last_to_save = window_size - 1;
            all_memory_square = window_size;
        }
        else
        {
            last_to_save = cash_needed - 1;
            all_memory_square = cash_needed;
        }
        length_of_last_packet = size%packet_length;
        if(length_of_last_packet == 0)
        {
            length_of_last_packet = packet_length;
        }
    }

    int j,i;
    int sent = 0;
    while(size>0)
    {
        for(i =0; i<all_memory_square; i++)
        {
            j = first_to_save+i;
            if(j>=window_size)
            {
               j = j - window_size;
            }
            if(filled[j] == 0)
            {
                get(start_cash+i*packet_length, packet_length);
                sent++;
            }
            if(filled[j] > 0)
            {
                continue;
            }
            //ostatni pakiet
            if(filled[j] == -1)
            {
                continue;
            }
        }
        receive_data(sent);


    }



   
   
}