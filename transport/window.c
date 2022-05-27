#include<stdlib.h>
#include<stdio.h>

int const window_size = 1000;
int const packet_length = 1000;
int free_space = window_size;
int filled[window_size] = {0};
char window[window_size][packet_length] ={0};
int first_to_save = 0;
int last_to_save = 0;
int all_memory_square = 1;
int start_cash= 0;
int length_of_last_packet = 0;
void get(int start, int length)
int download(char *addr, int port, FILE *file, int s)
{
    // na podsawiw wykładu 4
    size = s;
    old_size = s;
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
	int cash_needed = size/1000;
        if(cash_needed*1000 <size)
		cash_needed++;
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
    printf("wielkos ostatniego pakietu: %d\n", length_of_last_packet);
    printf("liczba okien: %d\n", all_memory_square);

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
                get(sockfd, server_address, start_cash+i*packet_length, packet_length);
                sent++;
            }
            if(filled[j] > 0)
            {
                continue;
            }
        }
        receive_data(sent,sockfd,server_address,  file);


    }



   
   
}