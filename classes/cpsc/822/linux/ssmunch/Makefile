all:	ssmunch.o

ssmunch.o: ssmunch.c
	gcc -Wall -c -D__KERNEL__ ssmunch.c

dep:
	sync

clean:
	rm -f core *.o
