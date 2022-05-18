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
int proba(int size)
{
    //preprocesing dla pierwszej pętli while
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
    //_______________________________________
        RECEIVE_DATA();
    //_______________________________________

    }
}