CC=g++
CFLAGS=-g -Wall -O3
LDFLAGS=

all: test btree

clean:
	rm *.o
	rm test
	rm btree

test: test.o dbfile.o btreenode.o btree.o blockman.o btreedb.o buffer.o secidx.o
	$(CC) $(LDFLAGS) test.o dbfile.o btreenode.o btree.o blockman.o btreedb.o buffer.o secidx.o -o $@

btree: main.o dbfile.o btreenode.o btree.o blockman.o btreedb.o buffer.o secidx.o
	$(CC) $(LDFLAGS) main.o dbfile.o btreenode.o btree.o blockman.o btreedb.o buffer.o secidx.o -o $@

test.o: test.cpp dbfile.h btree.h btreenode.h
	$(CC) $(CFLAGS) -c test.cpp

dbfile.o: dbfile.cpp dbfile.h
	$(CC) $(CFLAGS) -c dbfile.cpp

btreenode.o: btreenode.cpp btreenode.h
	$(CC) $(CFLAGS) -c btreenode.cpp

btree.o: btree.cpp btree.h
	$(CC) $(CFLAGS) -c btree.cpp

blockman.o: blockman.cpp blockman.h
	$(CC) $(CFLAGS) -c blockman.cpp

btreedb.o: btreedb.cpp btreedb.h
	$(CC) $(CFLAGS) -c btreedb.cpp

buffer.o: buffer.cpp buffer.h
	$(CC) $(CFLAGS) -c buffer.cpp

secidx.o: secidx.cpp secidx.h
	$(CC) $(CFLAGS) -c secidx.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
