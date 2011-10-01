CCC= g++
CCFLAGS=  -Wall -fhandle-exceptions -O2

OBJS = main.o Node.o List.o Bignum.o
	
run:   $(OBJS)
	$(CCC) $(CCFLAGS) -o run $(OBJS)
	strip run

main.o: main.cpp List.h Bignum.h
	$(CCC) $(CCFLAGS) -c main.cpp

Node.o: Node.cpp Node.h
	$(CCC) $(CCFLAGS) -c Node.cpp

List.o: List.cpp List.h
	$(CCC) $(CCFLAGS) -c List.cpp

Bignum.o: Bignum.cpp Bignum.h
	$(CCC) $(CCFLAGS) -c Bignum.cpp

clean:
	rm -f *.o run core
