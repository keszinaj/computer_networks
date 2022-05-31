#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

/*
   okno ma wielkość 1000 na 1000
*/
#define win_size 1000
#define packet_length 1000

/*
   Okno przesuwne trzymam w tablicy 2D. 
   Mam też strukturę pomocnicza, która trzyma w sobie stan okna
*/
int filled[win_size] = {0};
char window[win_size][packet_length];

/*
    Pomocnicze zmienne globalne
*/  
int first_to_save = 0;//index 1 do zapisania
int all_memory_square = 1;//ilość wolnych miejsc w oknie
int start_cash= 0; //przesunięcie względem początku danych
int size = 0; //rozmiar jeszcze nie zapisany
int old_size =0; //zachowuje rozmiar dany przez użytkownika
int cash_needed = 0; //potrzebne miejsce na dane w oknie

/*
    funkcja która wysyła get request
*/  
int get_request(int sockfd, struct sockaddr_in addr, int start, int length)
{
    if(old_size - start < packet_length)
    {
        length = old_size-start;
    }
    //wyłapuje dziwny bład
    if( length < 0)
        return EXIT_SUCCESS;
    char buffer[50];
    sprintf(buffer, "GET %d %d\n", start, length);
    size_t buffer_len = strlen(buffer);
    if (sendto(sockfd, buffer, buffer_len, 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "ERROR: Problem with get %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/*
    zapisuje do pliku tyle ile się da, oraz zmieniam ważne zmienne globalne
*/  
void save_in_file(FILE *fd)
{
    int i = first_to_save;
    if( filled[i] == 0 )
    {
        return;
    }
    while( filled[i] > 0 )
    {
        fwrite(&window[i][0], sizeof(char), filled[i], fd);
        filled[i]=0;
        size-=packet_length;
        i++;
        cash_needed--;
        start_cash+=packet_length;
        if(i>= win_size)
        {
            i = 0;
        } 
    }
    if(cash_needed > win_size )
    {
        all_memory_square = win_size ;
    }
    else
    {
        all_memory_square = cash_needed;
    }
    first_to_save = i; 
}

/*
    Funkcja odbiera dane
*/
int receive_data(int sent, int fd, struct sockaddr_in server_address, FILE *file)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    struct sockaddr_in sender;	
	socklen_t sender_len = sizeof(sender);
	char buffer[IP_MAXPACKET];
    int pos, len, add ,k;
    for(int i = 0; i<sent; i++)
    {
        int ready = select(fd+1, &readfds, NULL, NULL, &timeout);
        if(ready == 0)
        {
            break;
        }
        if(ready < 0)
        {
            fprintf(stderr, "Error: Select error %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        ssize_t packet_len = recvfrom (fd, buffer, IP_MAXPACKET, 0, (struct sockaddr*)&sender, &sender_len);
        if (packet_len < 0) 
        {
            fprintf(stderr, "recvfrom error: %s\n", strerror(errno)); 
            return EXIT_FAILURE;
        }
        //odrzucanie smieci
        if (sender.sin_addr.s_addr != server_address.sin_addr.s_addr || sender.sin_port != server_address.sin_port) {
            break;
        }

        sscanf(buffer, "DATA %d %d\n", &pos, &len);

        if(len == packet_length  || len < packet_length)
        {
            add = pos-start_cash;
            add = add/packet_length;
            k = first_to_save+add;

           if( k<first_to_save)
              continue;
           if(k >= win_size)
            {
                k -= win_size; 
            }
            if(filled[k]==0)
            {
                memcpy(&window[k][0], strchr(buffer, '\n') + 1, len);
                filled[k]=len;
            }

        }
        
    }
    save_in_file(file);
    return 0;
}

/* 
    Główna logiczna funcja odpowiadajca za pobieranie danych z serwera
*/
int download(char *addr, int port, FILE *file, int s)
{
    
    size = s;
    old_size = s;

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
    if(size>=packet_length)
    {
        cash_needed = size/packet_length;
        if(cash_needed*packet_length <size)
        cash_needed++;
        if(cash_needed>win_size)
        {
            all_memory_square = win_size;
        }
        else
        {
            all_memory_square = cash_needed;
        }
    }
  
    int j,i;
    int sent = 0;
    while(size>0)
    {
        for(i =0; i<all_memory_square; i++)
        {
            j = first_to_save+i;
            int st = start_cash;
            if(j>=win_size)
            {
               j = j - win_size;
               st = start_cash + (packet_length*win_size) ;
            }
            if(filled[j] == 0)
            {
                get_request(sockfd, server_address, st+i*packet_length, packet_length);
                sent++;
            }
            if(filled[j] > 0)
            {
                continue;
            }
        }
        receive_data(sent,sockfd,server_address,  file);
        printf("Do pobrania: %d\n", size);   

    }
    return EXIT_SUCCESS;
}