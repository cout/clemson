all:	turbo.o

turbo.o: turbo.c
	gcc -Wall -c -D__KERNEL__ turbo.c

dep:
	sync

clean:
	rm -f core *.o
