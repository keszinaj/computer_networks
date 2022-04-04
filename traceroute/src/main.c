#include <stdio.h>
#include <stdlib.h>
#include "traceroute.h"

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
