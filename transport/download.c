#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// implementacj samego okna
// TO TRZEBA DEFINE!!!!
int const window_size = 1000;
int const packet_length = 1000;
int free_space = window_size;
int start_global = 0; //głupia nazwa
int filled[window_size] = {0};
char window[window_size][packet_length] ={0};
int next()
{
    if(start == window_size-1)
    {
        return 0;
    }
    else
    {
        return start+1;
    }
}

int send_request(size_t start, size_t length)
{
    char request_str[100];
    sprintf(request_str, "GET %ld %ld\n", start, length);
    int check = sendto(sockfd, request_str, message_len, 0, (struct sockaddr*) &server_address, sizeof(server_address)) != message_len);
    if(check< 0)
    {
        printf("add error msg");
    }

}
receive_data(int number_of_sent)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
     char *buffer = malloc(1000);
    while(number_of_sent > 0)
    {
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
         else{
            ssize_t packet_len = recvfrom (fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
		    if (packet_len < 0) {
			    fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
			    return EXIT_FAILURE;
            }
            //check czy to dobre ip
            
         }

    }
    
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



    int amount_of_packets = 1;
    int next_in_window = 0;
    int amount_of_packet_to_send =1;
    //pre config
    if(size>1000)
    {
        amount_of_packets = ceil(size/1000);//sufit 
        
        else if(amount_of_packets > 1000)
        {
            next_in_window = 1000;
            amount_of_packet_to_send= 1000;
        }
        else{
            amount_of_packet_to_send= amount_of_packets
        }
    }
    int downloaded = 0;
    while(downloaded < size)
    {
        while(amount_of_packet_to_send)
        {
            send_request(start_global, next_in_window);
        }
        receive_data();
    }
}