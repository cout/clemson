CCC= g++
#CCFLAGS=  -Wall -fhandle-exceptions -O2 -DDEBUG
CCFLAGS=   -Wall -fhandle-exceptions -O2

OBJS = main.o ternary.o speller.o
	
spell:  $(OBJS)
	$(CCC) $(CCFLAGS) -o spell $(OBJS)
	strip spell

main.o: main.cpp ternary.h
	$(CCC) $(CCFLAGS) -c main.cpp

ternary.o: ternary.cpp ternary.h
	$(CCC) $(CCFLAGS) -c ternary.cpp

speller.o: ternary.h speller.h speller.cpp
	$(CCC) $(CCFLAGS) -c speller.cpp

makebal: makebal.cpp
	$(CCC) $(CCFLAGS) makebal.cpp -o makebal

clean:
	rm -f *.o spell makebal core
