#include <stdlib.h>
#include <stdio.h>
#include <regex.h>
#include "traceroute.h"
//gcc -o my_app main.c traceroute.c receive.c send.c
int main(int argc, char* argv[])
{
	//validation input data
	if( argc != 2)
	{
		fprintf(stderr, "Error: Wrong number of argument \n");
	    return EXIT_FAILURE;
	}
	return traceroute(argv[1]); 

}
