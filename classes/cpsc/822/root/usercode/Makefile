# Put all your source files here.
SRC=graphics.c vtest.c
OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.cpp=.o)

# This is the name of your output file
OUT=vtest

# This specifies all your include directories
INCLUDES=-I/usr/local/include -I.

# Put any flags you want to pass to the C compiler here.
CFLAGS=-g -O2 -Wall

# And put any C++ compiler flags here.
CCFLAGS=$(CFLAGS)

# CC speficies the name of the C compiler; CCC is the C++ compiler.
CC=cc
CCC=CC

# Put any libraries here.
LIBS=-L/usr/local/lib -lm
LDFLAGS=

##### RULES #####

# All rules are in the format:
#     item: [dependency list]
#     <tab>   command
# This means that "item" depends on what's in the dependency list; in other
# words, before "item" can be built, everything in the dependency list must
# be up to date.
# Note that this MUST be a tab, not a set of spaces!

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

graphics.o: /usr/include/fcntl.h /usr/include/features.h
graphics.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
graphics.o: /usr/include/bits/fcntl.h /usr/include/sys/types.h
graphics.o: /usr/include/bits/types.h
graphics.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
graphics.o: /usr/include/time.h /usr/include/endian.h
graphics.o: /usr/include/bits/endian.h /usr/include/sys/select.h
graphics.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
graphics.o: /usr/include/sys/sysmacros.h /usr/include/string.h
graphics.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
graphics.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
graphics.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
graphics.o: graphics.h uglint3.h
vtest.o: /usr/include/unistd.h /usr/include/features.h
vtest.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
vtest.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
vtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
vtest.o: /usr/include/bits/confname.h /usr/include/getopt.h
vtest.o: /usr/include/stdio.h
vtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
vtest.o: /usr/include/libio.h /usr/include/_G_config.h
vtest.o: /usr/include/bits/stdio_lim.h graphics.h
