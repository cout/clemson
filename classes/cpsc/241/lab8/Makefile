#
# Makefile.aimk for PVM example programs.
#
# Set PVM_ROOT to the path where PVM includes and libraries are installed.
# Set PVM_ARCH to your architecture type (SUN4, HP9K, RS6K, SGI, etc.)
# Set ARCHLIB to any special libs needed on PVM_ARCH (-lrpc, -lsocket, etc.)
# otherwise leave ARCHLIB blank
#
# PVM_ARCH and ARCHLIB are set for you if you use "$PVM_ROOT/lib/aimk"
# instead of "make".
#
# aimk also creates a $PVM_ARCH directory below this one and will cd to it
# before invoking make - this allows building in parallel on different arches.
#

BDIR	=	$(HOME)/pvm3/bin
XDIR	=	$(BDIR)/$(PVM_ARCH)

CC	=       cc 	
CFLAGS	=	-g -I$(PVM_ROOT)/include
LIBS	=	-lpvm3 $(ARCHLIB) -lsocket -lnsl
GLIBS	=	-lgpvm3

LFLAGS	=	-L$(PVM_ROOT)/lib/$(PVM_ARCH)

all: hellomaster helloslave

hellomaster : hellomaster.c hello.h
	$(CC) $(CFLAGS) -o hellomaster hellomaster.c $(LFLAGS) $(LIBS)
	mv hellomaster $(XDIR)
helloslave : helloslave.c hello.h
	$(CC) $(CFLAGS) -o helloslave helloslave.c $(LFLAGS) $(LIBS)
	mv helloslave $(XDIR)

