CFLAGS = -Wall -O2
LIBS = -lm

all: main

main: main.o zeroin.o zerofinder.h
	gcc $(CFLAGS) main.o zeroin.o -o main $(LIBS)

main.o: main.c
	gcc $(CFLAGS) main.c -c

zeroin.o: zeroin.c
	gcc $(CFLAGS) zeroin.c -c

clean:
	rm -f main
	rm -f *.o
