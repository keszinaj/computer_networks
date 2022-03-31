traceroute: main.o traceroute.o receive.o send.o
	gcc main.o traceroute.o receive.o send.o -o traceroute 
send.o: send.c send.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror send.c	
receive.o: receive.c receive.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror receive.c
traceroute.o: traceroute.c traceroute.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror traceroute.c 
main.o: main.c 
	gcc -c -std=gnu99 -Wall -Wextra -Werror main.c 
clean: 
	rm *.o traceroute