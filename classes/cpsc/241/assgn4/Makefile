CCC= g++
#CCFLAGS= -Wall -fno-implicit-templates  -fhandle-exceptions
#CCFLAGS= -DDEBUG -Wall  -fhandle-exceptions
CCFLAGS = -Wall -fhandle-exceptions -O2
    

phone:	main.o Menu.o IteratorString.o Person.o List.o
	$(CCC) $(CCFLAGS) -o phone main.o Menu.o IteratorString.o \
		Person.o List.o
	strip phone

main.o: main.cpp Menu.h Person.h List.h
	$(CCC) $(CCFLAGS) -c main.cpp

Menu.o: Menu.cpp Menu.h
	$(CCC) $(CCFLAGS) -c Menu.cpp

IteratorString.o: IteratorString.cpp IteratorString.h
	$(CCC) $(CCFLAGS) -c IteratorString.cpp

Person.o: Person.cpp Person.h
	$(CCC) $(CCFLAGS) -c Person.cpp

List.o: List.cpp List.h Person.h
	$(CCC) $(CCFLAGS) -c List.cpp

clean:
	rm -f *.o phone core
