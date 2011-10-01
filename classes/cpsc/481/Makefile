# You may have to change the following values:
# GMPDIR=/home/nglover/programs/gmp-crypt/gmp-2.0.2
# INCLUDES=-I/usr/local/include/g++ -I$(GMPDIR)
# LIBS=-lgmp -L$(GMPDIR)
INCLUDES=-I/usr/include/g++-2
LIBS=-lgmp
SRC=test.cpp rsa.cpp gmpwrapper.cpp
OBJ=$(SRC:.cpp=.o)
HEADERS=gltext.h display.h spin.h objects.h input.h reshape.h point.h freeze.h compass.h ppm.h gettime.h midi.h
OUT=rsa
CFLAGS = -O2 -g -Wall
CC=g++

##### RULES #####

.SUFFIXES: .cpp .o .ln

.cpp.o: 
	$(CC) $(INCS) $(CFLAGS) $(INCLUDES) -c $< -o $@

default: dep $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(LIBS)

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	/bin/rm -f $(OBJ) $(OUT)

# DO NOT DELETE

test.o: /usr/include/g++-2/iostream.h /usr/include/g++-2/streambuf.h
test.o: /usr/include/libio.h /usr/include/_G_config.h
test.o: /usr/include/bits/types.h /usr/include/features.h
test.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
test.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
test.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
test.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
test.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
test.o: /usr/include/bits/posix2_lim.h rsa.h /usr/include/g++-2/string
test.o: /usr/include/g++-2/std/bastring.h /usr/include/g++-2/cstddef
test.o: /usr/include/g++-2/std/straits.h /usr/include/g++-2/cctype
test.o: /usr/include/ctype.h /usr/include/endian.h /usr/include/bits/endian.h
test.o: /usr/include/g++-2/cstring /usr/include/string.h
test.o: /usr/include/g++-2/alloc.h /usr/include/g++-2/stl_config.h
test.o: /usr/include/g++-2/stl_alloc.h /usr/include/stdlib.h
test.o: /usr/include/sys/types.h /usr/include/time.h
test.o: /usr/include/sys/select.h /usr/include/bits/select.h
test.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
test.o: /usr/include/alloca.h /usr/include/assert.h /usr/include/pthread.h
test.o: /usr/include/sched.h /usr/include/bits/sched.h /usr/include/signal.h
test.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
test.o: /usr/include/g++-2/iterator /usr/include/g++-2/stl_relops.h
test.o: /usr/include/g++-2/stl_iterator.h /usr/include/g++-2/cassert
test.o: /usr/include/g++-2/std/bastring.cc gmpwrapper.h /usr/include/gmp.h
rsa.o: /usr/include/g++-2/iostream.h /usr/include/g++-2/streambuf.h
rsa.o: /usr/include/libio.h /usr/include/_G_config.h
rsa.o: /usr/include/bits/types.h /usr/include/features.h
rsa.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
rsa.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
rsa.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
rsa.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
rsa.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
rsa.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
rsa.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
rsa.o: /usr/include/bits/endian.h /usr/include/sys/select.h
rsa.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
rsa.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h rsa.h
rsa.o: /usr/include/g++-2/string /usr/include/g++-2/std/bastring.h
rsa.o: /usr/include/g++-2/cstddef /usr/include/g++-2/std/straits.h
rsa.o: /usr/include/g++-2/cctype /usr/include/ctype.h
rsa.o: /usr/include/g++-2/cstring /usr/include/string.h
rsa.o: /usr/include/g++-2/alloc.h /usr/include/g++-2/stl_config.h
rsa.o: /usr/include/g++-2/stl_alloc.h /usr/include/assert.h
rsa.o: /usr/include/pthread.h /usr/include/sched.h /usr/include/bits/sched.h
rsa.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
rsa.o: /usr/include/bits/sigthread.h /usr/include/g++-2/iterator
rsa.o: /usr/include/g++-2/stl_relops.h /usr/include/g++-2/stl_iterator.h
rsa.o: /usr/include/g++-2/cassert /usr/include/g++-2/std/bastring.cc
rsa.o: gmpwrapper.h /usr/include/gmp.h
gmpwrapper.o: /usr/include/limits.h /usr/include/features.h
gmpwrapper.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gmpwrapper.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gmpwrapper.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gmpwrapper.o: gmpwrapper.h /usr/include/g++-2/iostream.h
gmpwrapper.o: /usr/include/g++-2/streambuf.h /usr/include/libio.h
gmpwrapper.o: /usr/include/_G_config.h /usr/include/bits/types.h
gmpwrapper.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gmpwrapper.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gmpwrapper.o: /usr/include/gmp.h
