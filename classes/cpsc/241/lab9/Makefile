CFLAGS = -O2 -Wall

all: factor test

factor: main.o factor.o
	g++ $(CFLAGS) main.o factor.o -o factor
	strip factor

test: test.o factor.o
	g++ $(CFLAGS) test.o factor.o -o test
	strip test

main.o: main.cpp factor.h
	g++ $(CFLAGS) main.cpp -c 

test.o: test.cpp factor.h
	g++ $(CFLAGS) test.cpp -c

factor.o: factor.cpp factor.h
	g++ $(CFLAGS) factor.cpp -c

clean:
	rm -f factor test main.o test.o factor.o
