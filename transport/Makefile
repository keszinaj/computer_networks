transport: main.o download.o
	gcc main.o download.o -o transport 
download.o: download.c download.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror download.c	
main.o: main.c 
	gcc -c -std=gnu99 -Wall -Wextra -Werror main.c 
clean: 
	rm *.o 
distclean:
	rm *.o transport