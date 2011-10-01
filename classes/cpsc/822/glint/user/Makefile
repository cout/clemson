# Put all your source files here.
SRC=graphics.c vtest.c
SRC3D=graph3d.c 3dtest.c
SRC2D=graph3d.c 2dtest.c
SRCV=viewer.c graph3d.c
OBJ=$(SRC:.c=.o)
OBJ3D=$(SRC3D:.c=.o)
OBJ2D=$(SRC2D:.c=.o)
OBJV=$(SRCV:.c=.o)

# This is the name of your output file
OUT=vtest
OUT3D=3dtest
OUT2D=2dtest
OUTV=viewer

# This specifies all your include directories
INCLUDES=-I/usr/local/include -I.

# Put any flags you want to pass to the C compiler here.
CFLAGS=-g -O2

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

default: dep $(OUT) $(OUT3D) $(OUT2D) $(OUTV)

.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

$(OUT3D): $(OBJ3D)
	$(CC) $(OBJ3D) $(LDFLAGS) $(LIBS) -o $(OUT3D)

$(OUT2D): $(OBJ2D)
	$(CC) $(OBJ2D) $(LDFLAGS) $(LIBS) -o $(OUT2D)

$(OUTV): $(OBJV)
	$(CC) $(OBJV) $(LDFLAGS) $(LIBS) -o $(OUTV)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC) $(SRC3D) $(SRC2D) $(SRCV)

clean:
	/bin/rm -f $(OBJ) $(OBJ3D) $(OUT) $(OUT3D) $(OBJ2D) $(OUT2D)
	/bin/rm -f $(OBJV) $(OUTV)

# DO NOT DELETE

graphics.o: /usr/include/stdio.h /usr/include/features.h
graphics.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
graphics.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
graphics.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
graphics.o: /usr/include/bits/types.h /usr/include/libio.h
graphics.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
graphics.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
graphics.o: /usr/include/sys/types.h /usr/include/time.h
graphics.o: /usr/include/endian.h /usr/include/bits/endian.h
graphics.o: /usr/include/sys/select.h /usr/include/bits/select.h
graphics.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
graphics.o: /usr/include/string.h /usr/include/sys/ioctl.h
graphics.o: /usr/include/bits/ioctls.h /usr/include/asm/ioctls.h
graphics.o: /usr/include/asm/ioctl.h /usr/include/bits/ioctl-types.h
graphics.o: /usr/include/sys/ttydefaults.h /usr/include/math.h
graphics.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
graphics.o: /usr/include/bits/mathcalls.h
graphics.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
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
graph3d.o: /usr/include/stdint.h /usr/include/features.h
graph3d.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
graph3d.o: /usr/include/bits/wordsize.h /usr/include/stdio.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
graph3d.o: /usr/include/bits/types.h /usr/include/libio.h
graph3d.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
graph3d.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
graph3d.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
graph3d.o: /usr/include/bits/endian.h /usr/include/sys/select.h
graph3d.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
graph3d.o: /usr/include/sys/sysmacros.h /usr/include/string.h
graph3d.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
graph3d.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
graph3d.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
graph3d.o: /usr/include/math.h /usr/include/bits/huge_val.h
graph3d.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
graph3d.o: graph3d.h matrix.h uglint3.h
3dtest.o: /usr/include/unistd.h /usr/include/features.h
3dtest.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
3dtest.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
3dtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
3dtest.o: /usr/include/bits/confname.h /usr/include/getopt.h
3dtest.o: /usr/include/stdio.h
3dtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
3dtest.o: /usr/include/libio.h /usr/include/_G_config.h
3dtest.o: /usr/include/bits/stdio_lim.h graph3d.h matrix.h
3dtest.o: /usr/include/math.h /usr/include/bits/huge_val.h
3dtest.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
3dtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
3dtest.o: /usr/include/string.h
graph3d.o: /usr/include/stdint.h /usr/include/features.h
graph3d.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
graph3d.o: /usr/include/bits/wordsize.h /usr/include/stdio.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
graph3d.o: /usr/include/bits/types.h /usr/include/libio.h
graph3d.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
graph3d.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
graph3d.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
graph3d.o: /usr/include/bits/endian.h /usr/include/sys/select.h
graph3d.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
graph3d.o: /usr/include/sys/sysmacros.h /usr/include/string.h
graph3d.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
graph3d.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
graph3d.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
graph3d.o: /usr/include/math.h /usr/include/bits/huge_val.h
graph3d.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
graph3d.o: graph3d.h matrix.h uglint3.h
2dtest.o: /usr/include/unistd.h /usr/include/features.h
2dtest.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
2dtest.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
2dtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
2dtest.o: /usr/include/bits/confname.h /usr/include/getopt.h
2dtest.o: /usr/include/stdio.h
2dtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
2dtest.o: /usr/include/libio.h /usr/include/_G_config.h
2dtest.o: /usr/include/bits/stdio_lim.h graph3d.h matrix.h
2dtest.o: /usr/include/math.h /usr/include/bits/huge_val.h
2dtest.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
2dtest.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
2dtest.o: /usr/include/string.h
viewer.o: /usr/include/unistd.h /usr/include/features.h
viewer.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
viewer.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
viewer.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
viewer.o: /usr/include/bits/confname.h /usr/include/getopt.h
viewer.o: /usr/include/stdio.h
viewer.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
viewer.o: /usr/include/libio.h /usr/include/_G_config.h
viewer.o: /usr/include/bits/stdio_lim.h graph3d.h matrix.h
viewer.o: /usr/include/math.h /usr/include/bits/huge_val.h
viewer.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
viewer.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
viewer.o: /usr/include/string.h
graph3d.o: /usr/include/stdint.h /usr/include/features.h
graph3d.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stddef.h
graph3d.o: /usr/include/bits/wordsize.h /usr/include/stdio.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/stdarg.h
graph3d.o: /usr/include/bits/types.h /usr/include/libio.h
graph3d.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
graph3d.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
graph3d.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
graph3d.o: /usr/include/bits/endian.h /usr/include/sys/select.h
graph3d.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
graph3d.o: /usr/include/sys/sysmacros.h /usr/include/string.h
graph3d.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
graph3d.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
graph3d.o: /usr/include/bits/ioctl-types.h /usr/include/sys/ttydefaults.h
graph3d.o: /usr/include/math.h /usr/include/bits/huge_val.h
graph3d.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
graph3d.o: /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h
graph3d.o: graph3d.h matrix.h uglint3.h
