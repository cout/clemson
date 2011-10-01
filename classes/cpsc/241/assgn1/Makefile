CCC= g++
CCFLAGS= -Wall -O2

main:   main.o Hex.o
	$(CCC) $(CCFLAGS) -o main main.o Hex.o
	strip main

main.o: main.cpp Hex.h
	$(CCC) $(CCFLAGS) -c main.cpp

Hex.o: Hex.cpp Hex.h
	$(CCC) $(CCFLAGS) -c Hex.cpp
 
clean:
	rm -f *.o main core
