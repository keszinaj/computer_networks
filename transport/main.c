#include<stdlib.h>
#include<stdio.h>

int main(int argc, char *argv[])
{
	// validation input data
	if(argc != 5)
	{
		fprintf(stderr, "Error: Wrong number of argument. \n");
		return EXIT_FAILURE;
	}
	int port = atoi(argv[2]);
	if(port < 0 || socket > 65535)
	{
		fprint(stderr, "Error: Wrong socket number");
		return EXIT_FAILURE;
	}
	FILE *file;
	file = fopen(argv[3], "ab");
	if(file == NULL)
	{
		fprintf(stderr, "Error: Problem with file: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	int size = atoi(argv[4]);
	
	return 0;
}

