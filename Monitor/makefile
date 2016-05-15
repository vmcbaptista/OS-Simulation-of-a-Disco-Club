monitor: monitor.o leEscreve.o sockets.o util.o
	gcc monitor.o leEscreve.o sockets.o util.o -lpthread -o  monitor 

monitor.o: ../lib/header.h monitor.c
	gcc -c -g -std=c99 monitor.c

leEscreve.o: ../lib/header.h ../lib/leEscreve.c
	gcc -c -g -std=c99  ../lib/leEscreve.c

sockets.o: ../lib/sockets.h ../lib/sockets.c
	gcc -c -g -std=c99 ../lib/sockets.c

util.o: ../lib/util.c ../lib/sockets.h
	gcc -c -g -std=c99 ../lib/util.c
clean:
	rm *.o
