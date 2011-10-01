CC=gcc
CFLAGS=-Wall -g
LDFLAGS=

all: buf insdel

clean:
	rm buf
	rm insdel
	rm *.o

buf: buf.o bufmain.o
	$(CC) $(CFLAGS) buf.o bufmain.o $(LDFLAGS) -o $@

insdel: buf.o insdel.o insdelmain.o
	$(CC) $(CFLAGS) buf.o insdel.o insdelmain.o $(LDFLAGS) -o $@

buf.o: buf.c buf.h
	$(CC) $(CFLAGS) -c $<

bufmain.o: bufmain.c buf.h
	$(CC) $(CFLAGS) -c $<

insdel.o: insdel.c insdel.h
	$(CC) $(CFLAGS) -c $<

insdelmain.o: insdelmain.c insdel.h
	$(CC) $(CFLAGS) -c $<

insdel.h: buf.h

