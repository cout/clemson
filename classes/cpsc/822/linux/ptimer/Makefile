all:	ptimer.o

ptimer.o: ptimer.c
	gcc -Wall -c -D__KERNEL__ ptimer.c

dep:
	sync

clean:
	rm -f core *.o
