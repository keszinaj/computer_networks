#include<stdlib.h>
#include<stdio.h>
#include "download.h"
#include <errno.h>
//gcc -o test main.c download.c 
int main(int argc, char *argv[])
{
	// validation input data
	if(argc != 5)
	{
		fprintf(stderr, "Error: Wrong number of argument. \n");
		return EXIT_FAILURE;
	}
	int port = atoi(argv[2]);
	if(port < 0 || port > 65535)
	{
		fprintf(stderr, "Error: Wrong socket number");
		return EXIT_FAILURE;
	}
	FILE *file;
	file = fopen(argv[3], "ab");
	if(file == NULL)
	{
		fprintf(stderr, "Error: Problem with file\n");
		return EXIT_FAILURE;
	}
	int size = atoi(argv[4]);
	int check = download(argv[1], port, file, size);
	if(check == EXIT_FAILURE)
	{
		fprintf(stderr, "Error: Problem with download\n");
		return EXIT_FAILURE;	
	}

	
	return 0;
}

