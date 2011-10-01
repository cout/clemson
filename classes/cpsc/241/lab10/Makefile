CFLAGS = -O2 -Wall
LIBS = -lgmp

all: factor test

factor: main.o factor.o biggy.o
	g++ $(CFLAGS) main.o factor.o biggy.o -o factor $(LIBS)
	strip factor

main.o: main.cpp factor.h
	g++ $(CFLAGS) main.cpp -c 

test.o: test.cpp factor.h
	g++ $(CFLAGS) test.cpp -c

factor.o: factor.cpp factor.h
	g++ $(CFLAGS) factor.cpp -c

biggy.o: biggy.cpp biggy.h gmp.h
	g++ $(CFLAGS) biggy.cpp -c

factor.h: biggy.h gmp.h

clean:
	rm -f factor test main.o test.o factor.o
