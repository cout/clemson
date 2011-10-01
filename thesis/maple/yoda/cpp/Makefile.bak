BINAME_SRC=gmpwrapper.cpp biname.cpp biname_test.cpp
BINAME_OBJ=$(BINAME_SRC:.cpp=.o)
BINAME_OUT=biname_test
TREE_SRC=gmpwrapper.cpp biname.cpp tree.cpp tree_test.cpp
TREE_OBJ=$(TREE_SRC:.cpp=.o)
TREE_OUT=tree_test
SUBTREE_SRC=gmpwrapper.cpp biname.cpp tree.cpp subtree.cpp subtree_test.cpp
SUBTREE_OBJ=$(SUBTREE_SRC:.cpp=.o)
SUBTREE_OUT=subtree_test

ALL=$(BINAME_OUT) $(TREE_OUT) $(SUBTREE_OUT)
SRC=$(BINAME_SRC) $(TREE_SRC) $(SUBTREE_SRC)
OBJ=$(BINAME_OBJ) $(TREE_OBJ) $(SUBTREE_OBJ)
OUT=$(ALL)

INCLUDES=-I/usr/local/include -I. -I/usr/include/g++
CFLAGS=-g -O2
CC=gcc

CCFLAGS=$(CFLAGS)
CCC=g++

LIBS=-lm -lgmp
LDFLAGS=

##### RULES #####

.SUFFIXES: .c .cpp .ln .phony

default: dep all

all: $(ALL)

$(BINAME_OUT): $(BINAME_OBJ)
	$(CCC) $(INCLUDES) $(LDFLAGS) $(LIBS) $(BINAME_OBJ) -o $@

$(TREE_OUT): $(TREE_OBJ)
	$(CCC) $(INCLUDES) $(LDFLAGS) $(LIBS) $(TREE_OBJ) -o $@

$(SUBTREE_OUT): $(SUBTREE_OBJ)
	$(CCC) $(INCLUDES) $(LDFLAGS) $(LIBS) $(SUBTREE_OBJ) -o $@

.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.cpp.o: 
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@


depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	/bin/rm -f $(OBJ) $(OUT)

# DO NOT DELETE

gmpwrapper.o: gmpwrapper.h /usr/include/g++/iostream
gmpwrapper.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
gmpwrapper.o: /usr/include/libio.h /usr/include/_G_config.h
gmpwrapper.o: /usr/include/bits/types.h /usr/include/features.h
gmpwrapper.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gmpwrapper.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
gmpwrapper.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
gmpwrapper.o: /usr/include/gmp.h
biname.o: /usr/include/g++/list /usr/include/g++/stl_algobase.h
biname.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
biname.o: /usr/include/bits/types.h /usr/include/features.h
biname.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
biname.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
biname.o: /usr/include/g++/type_traits.h /usr/include/string.h
biname.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
biname.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
biname.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
biname.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
biname.o: /usr/include/bits/endian.h /usr/include/sys/select.h
biname.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
biname.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
biname.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
biname.o: /usr/include/libio.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
biname.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
biname.o: /usr/include/assert.h /usr/include/pthread.h /usr/include/sched.h
biname.o: /usr/include/bits/sched.h /usr/include/signal.h
biname.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
biname.o: /usr/include/g++/stl_construct.h
biname.o: /usr/include/g++/stl_uninitialized.h /usr/include/g++/stl_list.h
biname.o: /usr/include/g++/algorithm /usr/include/g++/stl_tempbuf.h
biname.o: /usr/include/g++/stl_algo.h /usr/include/g++/stl_heap.h
biname.o: /usr/include/g++/iterator biname.h intwrapper.h
biname_test.o: /usr/include/g++/iostream /usr/include/g++/iostream.h
biname_test.o: /usr/include/g++/streambuf.h /usr/include/libio.h
biname_test.o: /usr/include/_G_config.h /usr/include/bits/types.h
biname_test.o: /usr/include/features.h /usr/include/sys/cdefs.h
biname_test.o: /usr/include/gnu/stubs.h
biname_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
biname_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
biname_test.o: biname.h /usr/include/g++/list /usr/include/g++/stl_algobase.h
biname_test.o: /usr/include/g++/stl_config.h /usr/include/g++/stl_relops.h
biname_test.o: /usr/include/g++/stl_pair.h /usr/include/g++/type_traits.h
biname_test.o: /usr/include/string.h /usr/include/limits.h
biname_test.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
biname_test.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
biname_test.o: /usr/include/stdlib.h /usr/include/sys/types.h
biname_test.o: /usr/include/time.h /usr/include/endian.h
biname_test.o: /usr/include/bits/endian.h /usr/include/sys/select.h
biname_test.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
biname_test.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
biname_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
biname_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
biname_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
biname_test.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
biname_test.o: /usr/include/assert.h /usr/include/pthread.h
biname_test.o: /usr/include/sched.h /usr/include/bits/sched.h
biname_test.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
biname_test.o: /usr/include/bits/sigthread.h /usr/include/g++/stl_construct.h
biname_test.o: /usr/include/g++/stl_uninitialized.h
biname_test.o: /usr/include/g++/stl_list.h intwrapper.h
gmpwrapper.o: gmpwrapper.h /usr/include/g++/iostream
gmpwrapper.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
gmpwrapper.o: /usr/include/libio.h /usr/include/_G_config.h
gmpwrapper.o: /usr/include/bits/types.h /usr/include/features.h
gmpwrapper.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gmpwrapper.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
gmpwrapper.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
gmpwrapper.o: /usr/include/gmp.h
biname.o: /usr/include/g++/list /usr/include/g++/stl_algobase.h
biname.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
biname.o: /usr/include/bits/types.h /usr/include/features.h
biname.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
biname.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
biname.o: /usr/include/g++/type_traits.h /usr/include/string.h
biname.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
biname.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
biname.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
biname.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
biname.o: /usr/include/bits/endian.h /usr/include/sys/select.h
biname.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
biname.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
biname.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
biname.o: /usr/include/libio.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
biname.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
biname.o: /usr/include/assert.h /usr/include/pthread.h /usr/include/sched.h
biname.o: /usr/include/bits/sched.h /usr/include/signal.h
biname.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
biname.o: /usr/include/g++/stl_construct.h
biname.o: /usr/include/g++/stl_uninitialized.h /usr/include/g++/stl_list.h
biname.o: /usr/include/g++/algorithm /usr/include/g++/stl_tempbuf.h
biname.o: /usr/include/g++/stl_algo.h /usr/include/g++/stl_heap.h
biname.o: /usr/include/g++/iterator biname.h intwrapper.h
tree.o: /usr/include/g++/list /usr/include/g++/stl_algobase.h
tree.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
tree.o: /usr/include/bits/types.h /usr/include/features.h
tree.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
tree.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
tree.o: /usr/include/g++/type_traits.h /usr/include/string.h
tree.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
tree.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
tree.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
tree.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
tree.o: /usr/include/bits/endian.h /usr/include/sys/select.h
tree.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tree.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
tree.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
tree.o: /usr/include/libio.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
tree.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
tree.o: /usr/include/assert.h /usr/include/pthread.h /usr/include/sched.h
tree.o: /usr/include/bits/sched.h /usr/include/signal.h
tree.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
tree.o: /usr/include/g++/stl_construct.h /usr/include/g++/stl_uninitialized.h
tree.o: /usr/include/g++/stl_list.h /usr/include/g++/vector
tree.o: /usr/include/g++/stl_vector.h /usr/include/g++/stl_bvector.h
tree.o: /usr/include/g++/algorithm /usr/include/g++/stl_tempbuf.h
tree.o: /usr/include/g++/stl_algo.h /usr/include/g++/stl_heap.h biname.h
tree.o: intwrapper.h tree.h edge.h
tree_test.o: tree.h /usr/include/g++/list /usr/include/g++/stl_algobase.h
tree_test.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
tree_test.o: /usr/include/bits/types.h /usr/include/features.h
tree_test.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
tree_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
tree_test.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
tree_test.o: /usr/include/g++/type_traits.h /usr/include/string.h
tree_test.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
tree_test.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
tree_test.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
tree_test.o: /usr/include/sys/types.h /usr/include/time.h
tree_test.o: /usr/include/endian.h /usr/include/bits/endian.h
tree_test.o: /usr/include/sys/select.h /usr/include/bits/select.h
tree_test.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
tree_test.o: /usr/include/alloca.h
tree_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
tree_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
tree_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
tree_test.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
tree_test.o: /usr/include/libio.h
tree_test.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
tree_test.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
tree_test.o: /usr/include/assert.h /usr/include/pthread.h
tree_test.o: /usr/include/sched.h /usr/include/bits/sched.h
tree_test.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
tree_test.o: /usr/include/bits/sigthread.h /usr/include/g++/stl_construct.h
tree_test.o: /usr/include/g++/stl_uninitialized.h /usr/include/g++/stl_list.h
tree_test.o: /usr/include/g++/vector /usr/include/g++/stl_vector.h
tree_test.o: /usr/include/g++/stl_bvector.h biname.h intwrapper.h edge.h
gmpwrapper.o: gmpwrapper.h /usr/include/g++/iostream
gmpwrapper.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
gmpwrapper.o: /usr/include/libio.h /usr/include/_G_config.h
gmpwrapper.o: /usr/include/bits/types.h /usr/include/features.h
gmpwrapper.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gmpwrapper.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
gmpwrapper.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
gmpwrapper.o: /usr/include/gmp.h
biname.o: /usr/include/g++/list /usr/include/g++/stl_algobase.h
biname.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
biname.o: /usr/include/bits/types.h /usr/include/features.h
biname.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
biname.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
biname.o: /usr/include/g++/type_traits.h /usr/include/string.h
biname.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
biname.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
biname.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
biname.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
biname.o: /usr/include/bits/endian.h /usr/include/sys/select.h
biname.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
biname.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
biname.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
biname.o: /usr/include/libio.h
biname.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
biname.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
biname.o: /usr/include/assert.h /usr/include/pthread.h /usr/include/sched.h
biname.o: /usr/include/bits/sched.h /usr/include/signal.h
biname.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
biname.o: /usr/include/g++/stl_construct.h
biname.o: /usr/include/g++/stl_uninitialized.h /usr/include/g++/stl_list.h
biname.o: /usr/include/g++/algorithm /usr/include/g++/stl_tempbuf.h
biname.o: /usr/include/g++/stl_algo.h /usr/include/g++/stl_heap.h
biname.o: /usr/include/g++/iterator biname.h intwrapper.h
tree.o: /usr/include/g++/list /usr/include/g++/stl_algobase.h
tree.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
tree.o: /usr/include/bits/types.h /usr/include/features.h
tree.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
tree.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
tree.o: /usr/include/g++/type_traits.h /usr/include/string.h
tree.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
tree.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
tree.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
tree.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
tree.o: /usr/include/bits/endian.h /usr/include/sys/select.h
tree.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tree.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
tree.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
tree.o: /usr/include/libio.h
tree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
tree.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_alloc.h
tree.o: /usr/include/assert.h /usr/include/pthread.h /usr/include/sched.h
tree.o: /usr/include/bits/sched.h /usr/include/signal.h
tree.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
tree.o: /usr/include/g++/stl_construct.h /usr/include/g++/stl_uninitialized.h
tree.o: /usr/include/g++/stl_list.h /usr/include/g++/vector
tree.o: /usr/include/g++/stl_vector.h /usr/include/g++/stl_bvector.h
tree.o: /usr/include/g++/algorithm /usr/include/g++/stl_tempbuf.h
tree.o: /usr/include/g++/stl_algo.h /usr/include/g++/stl_heap.h biname.h
tree.o: intwrapper.h tree.h edge.h
subtree.o: /usr/include/g++/algorithm /usr/include/g++/stl_algobase.h
subtree.o: /usr/include/g++/stl_config.h /usr/include/_G_config.h
subtree.o: /usr/include/bits/types.h /usr/include/features.h
subtree.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
subtree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
subtree.o: /usr/include/g++/stl_relops.h /usr/include/g++/stl_pair.h
subtree.o: /usr/include/g++/type_traits.h /usr/include/string.h
subtree.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
subtree.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
subtree.o: /usr/include/bits/posix2_lim.h /usr/include/stdlib.h
subtree.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
subtree.o: /usr/include/bits/endian.h /usr/include/sys/select.h
subtree.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
subtree.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
subtree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new.h
subtree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/new
subtree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/exception
subtree.o: /usr/include/g++/iostream.h /usr/include/g++/streambuf.h
subtree.o: /usr/include/libio.h
subtree.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
subtree.o: /usr/include/g++/stl_iterator.h /usr/include/g++/stl_construct.h
subtree.o: /usr/include/g++/stl_tempbuf.h /usr/include/g++/stl_algo.h
subtree.o: /usr/include/g++/stl_heap.h subtree.h /usr/include/g++/vector
subtree.o: /usr/include/g++/stl_alloc.h /usr/include/assert.h
subtree.o: /usr/include/pthread.h /usr/include/sched.h
subtree.o: /usr/include/bits/sched.h /usr/include/signal.h
subtree.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/sigthread.h
subtree.o: /usr/include/g++/stl_uninitialized.h /usr/include/g++/stl_vector.h
subtree.o: /usr/include/g++/stl_bvector.h /usr/include/g++/list
subtree.o: /usr/include/g++/stl_list.h graph.h tree.h biname.h intwrapper.h
subtree.o: edge.h
