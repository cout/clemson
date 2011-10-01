CFLAGS = -Wall -O2
LIBS = -lm

all: main string

main: main.o zeroin.o secant.o bisection.o zerofinder.h
	gcc $(CFLAGS) main.o zeroin.o secant.o bisection.o -o main $(LIBS)

string: string.o zeroin.o secant.o bisection.o zerofinder.h
	gcc $(CFLAGS) string.o zeroin.o secant.o bisection.o -o string $(LIBS)

string.o: string.c
	gcc $(CFLAGS) string.c -c

main.o: main.c
	gcc $(CFLAGS) main.c -c

zeroin.o: zeroin.c
	gcc $(CFLAGS) zeroin.c -c

secant.o: secant.c
	gcc $(CFLAGS) secant.c -c

bisection.o: bisection.c
	gcc $(CFLAGS) bisection.c -c

clean:
	rm -f main
	rm -f string
	rm -f *.o
