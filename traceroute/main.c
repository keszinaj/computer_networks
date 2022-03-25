#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

int main(int argc, char* argv[])
{
	//validation input data
	if( argc != 2)
	{
		fprintf(stderr, "Error: Wrong number of argument \n");
	    return EXIT_FAILURE;
	}
	// I had a minor problem with regex in C but stackoverflow was helpfull as always :)
	// https://stackoverflow.com/questions/21554445/regex-for-ip-address-in-c
	regex_t regex;
	int result;
	result = regcomp(&regex, 
        "^([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))."
         "([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))."
         "([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))."
         "([0-9]|[1-9][0-9]|1([0-9][0-9])|2([0-4][0-9]|5[0-5]))$", REG_EXTENDED);
	result = regexec (&regex, argv[1], 0, NULL, 0);
	//if(!result)
	//{
    //    printf("good ip addres");
	//	return 0;
	//}
	// to do chekc if regcomp output is error
	if(result)
	{
		fprintf(stderr, "Error: IP address is wrong\n");
	    return EXIT_FAILURE;
	}
}
