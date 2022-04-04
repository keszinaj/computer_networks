#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//print all cases
void pretty_print(int received, char sender_ip_str[3][20], double time, int num)
{
    double mtime = time / 1000;
    if(received == 0)
    {
        printf("%d.  *    *\n", num);
        return;
    }
    else if(received == 1)
    {
        printf("%d.  %s    ???\n", num, sender_ip_str[0]);
        return;
    }
    int same = strcmp(sender_ip_str[0], sender_ip_str[1]);
    if(received == 2)
    {
        printf("%d.  %s    ???\n", num, sender_ip_str[0]);
        if(same != 0)
        {
            printf("%d.  %s    ???\n", num, sender_ip_str[1]);
        }
        return;
    }
    else if(received == 3)
    {
        if(same == 0)
        {
            if(strcmp(sender_ip_str[0], sender_ip_str[2])==0)
            {
                printf("%d.  %s    %.3f ms\n", num, sender_ip_str[0], mtime);
            }
            else
            {
                printf("%d.  %s    ???\n", num, sender_ip_str[0]);
                printf("%d.  %s    ???\n", num, sender_ip_str[2]); 
            }
            return;
        }
        printf("%d.  %s    ???\n", num, sender_ip_str[0]);
        printf("%d.  %s    ???\n", num, sender_ip_str[1]);
        if(strcmp(sender_ip_str[0], sender_ip_str[2])==1 && strcmp(sender_ip_str[1], sender_ip_str[2])==1)
        {
            printf("%d.  %s   ???\n", num, sender_ip_str[2]);
        }
        return;
    }
}