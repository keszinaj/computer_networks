traceroute: main.o traceroute.o prettyprint.o receive.o send.o
	gcc main.o traceroute.o prettyprint.o receive.o send.o -o traceroute 
send.o: ./src/send.c ./src/send.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror ./src/send.c	
receive.o: ./src/receive.c ./src/receive.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror ./src/receive.c
prettyprint.o: ./src/prettyprint.c ./src/prettyprint.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror ./src/prettyprint.c
traceroute.o: ./src/traceroute.c ./src/traceroute.h
	gcc -c -std=gnu99 -Wall -Wextra -Werror ./src/traceroute.c 
main.o: ./src/main.c 
	gcc -c -std=gnu99 -Wall -Wextra -Werror ./src/main.c 
clean: 
	rm *.o traceroute