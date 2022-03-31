traceroute: main.o traceroute.o send.o receive.o 
	gcc main.o traceroute.o receive.o send.o -o traceroute 
receive.o: receive.c receive.h
	gcc -c -std=c99 -Wall -Wextra -Werror -lm receive.c
send.o: send.c send.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror -lm send.c	
traceroute.o: traceroute.c traceroute.h
	gcc -c -std=c99 -Wall -Wextra -Werror -lm traceroute.c 
main.o: main.c 
	gcc -c -std=c99 -Wall -Wextra -Werror -lm main.c 
clean: 
	rm *.o traceroute