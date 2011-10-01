SRC=	main.c matrix.c
OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.cpp=.o)
OUT=main

INCLUDES=-I/usr/local/include -I.
CFLAGS=-g -O2 -Wall
CC=gcc

CCFLAGS=$(CFLAGS)
CCC=CC

LIBS=
LDFLAGS=-lm

##### RULES #####

.SUFFIXES: .c .c .ln

default: dep $(OUT)

.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.cpp.o: 
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@


$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	/bin/rm -f $(OBJ) $(OUT)
# DO NOT DELETE

main.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/sgidefs.h
main.o: matrix.h
matrix.o: /usr/include/stdlib.h /usr/include/standards.h
matrix.o: /usr/include/sgidefs.h /usr/include/string.h /usr/include/math.h
matrix.o: matrix.h
