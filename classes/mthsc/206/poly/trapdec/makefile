CC=gcc
CFLAGS=-g
YFLAGS=-d -v
OBJS=main.o ccw.o rbt.o fun.o new.o

TrapDec:		$(OBJS)
		$(CC) $(CFLAGS) -o TrapDec $(OBJS) -lm 

main.o:		def.h func.h 
new.o:		def.h func.h 
fun.o:		def.h func.h
rbt.o:		def.h func.h
ccw.o:		def.h func.h
