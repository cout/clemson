COPTS = -g
SRC=fly.c objects.c gltext.c spin.c scene.c display.c input.c reshape.c compass.c ppm.c gettime.c midi.c
OBJS=${SRC:.c=.o}
HEADERS=gltext.h display.h spin.h objects.h input.h reshape.h point.h freeze.h compass.h ppm.h gettime.h midi.h
INCS=-I/usr/local/include
LIBS=-L/usr/disney/users1/vernon/pub/lib -L/usr/lib32/internal -lglut -lGLU -lGL -lm -L/usr/X11R6/lib -lX11 -lXi -lXt -lXmu
OUT=fly
CFLAGS = -O2


##### RULES #####

.SUFFIXES: .c .o .ln

.c.o: 
	${CC} ${INCS} ${CFLAGS} -c ${INCDIR} $< -o $@

default: dep sim

sim: ${OBJS}
	${CC} ${CFLAGS} -o ${OUT} ${OBJS} ${LIBS}

dep:
	makedepend -I/usr/local/include ${SRC}

clean: ${HEADERS}
	/bin/rm -f ${OBJS} ${OUT}

########################################################################
# DO NOT DELETE THIS LINE

fly.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
fly.o: /usr/include/gnu/stubs.h
fly.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
fly.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
fly.o: /usr/include/bits/types.h /usr/include/libio.h
fly.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
fly.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
fly.o: /usr/include/endian.h /usr/include/bits/endian.h
fly.o: /usr/include/sys/select.h /usr/include/bits/select.h
fly.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
fly.o: /usr/include/alloca.h /usr/local/include/GL/glut.h
fly.o: /usr/local/include/GL/gl.h /usr/local/include/GL/glu.h
fly.o: /usr/include/assert.h scene.h gltext.h spin.h display.h input.h
fly.o: reshape.h freeze.h compass.h gettime.h midi.h
objects.o: /usr/include/assert.h /usr/include/features.h
objects.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
objects.o: /usr/include/stdlib.h
objects.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
objects.o: /usr/include/sys/types.h /usr/include/bits/types.h
objects.o: /usr/include/time.h /usr/include/endian.h
objects.o: /usr/include/bits/endian.h /usr/include/sys/select.h
objects.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
objects.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
objects.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
objects.o: /usr/local/include/GL/glu.h /usr/include/math.h
objects.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
objects.o: /usr/include/bits/mathcalls.h
objects.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
objects.o: /usr/include/stdio.h
objects.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
objects.o: /usr/include/libio.h /usr/include/_G_config.h
objects.o: /usr/include/bits/stdio_lim.h objects.h reshape.h input.h ppm.h
objects.o: point.h
gltext.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
gltext.o: /usr/local/include/GL/glu.h gltext.h
spin.o: /usr/include/sys/time.h /usr/include/features.h
spin.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h /usr/include/time.h
spin.o: /usr/include/bits/types.h
spin.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
spin.o: /usr/include/sys/select.h /usr/include/bits/select.h
spin.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
spin.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
spin.o: /usr/include/bits/confname.h /usr/include/getopt.h
spin.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
spin.o: /usr/local/include/GL/glu.h spin.h gettime.h
scene.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
scene.o: /usr/include/gnu/stubs.h /usr/include/bits/huge_val.h
scene.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
scene.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
scene.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
scene.o: /usr/local/include/GL/glu.h objects.h reshape.h scene.h spin.h
scene.o: input.h
display.o: /usr/include/math.h /usr/include/features.h
display.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
display.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
display.o: /usr/include/bits/mathcalls.h
display.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
display.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
display.o: /usr/local/include/GL/glu.h display.h fsim.h
input.o: /usr/include/ctype.h /usr/include/features.h
input.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
input.o: /usr/include/bits/types.h
input.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
input.o: /usr/include/endian.h /usr/include/bits/endian.h
input.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
input.o: /usr/local/include/GL/glu.h /usr/include/stdio.h
input.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
input.o: /usr/include/libio.h /usr/include/_G_config.h
input.o: /usr/include/bits/stdio_lim.h /usr/include/stdlib.h
input.o: /usr/include/sys/types.h /usr/include/time.h
input.o: /usr/include/sys/select.h /usr/include/bits/select.h
input.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
input.o: /usr/include/alloca.h /usr/include/assert.h /usr/include/math.h
input.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
input.o: /usr/include/bits/mathcalls.h
input.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h spin.h
input.o: reshape.h freeze.h input.h fsim.h gettime.h compass.h
reshape.o: /usr/local/include/GL/glut.h /usr/local/include/GL/gl.h
reshape.o: /usr/local/include/GL/glu.h /usr/include/stdlib.h
reshape.o: /usr/include/features.h /usr/include/sys/cdefs.h
reshape.o: /usr/include/gnu/stubs.h
reshape.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
reshape.o: /usr/include/sys/types.h /usr/include/bits/types.h
reshape.o: /usr/include/time.h /usr/include/endian.h
reshape.o: /usr/include/bits/endian.h /usr/include/sys/select.h
reshape.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
reshape.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
reshape.o: /usr/include/assert.h reshape.h input.h
compass.o: /usr/include/stdlib.h /usr/include/features.h
compass.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
compass.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
compass.o: /usr/include/sys/types.h /usr/include/bits/types.h
compass.o: /usr/include/time.h /usr/include/endian.h
compass.o: /usr/include/bits/endian.h /usr/include/sys/select.h
compass.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
compass.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
compass.o: /usr/include/assert.h /usr/local/include/GL/glut.h
compass.o: /usr/local/include/GL/gl.h /usr/local/include/GL/glu.h compass.h
compass.o: display.h input.h reshape.h
ppm.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
ppm.o: /usr/include/gnu/stubs.h
ppm.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
ppm.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
ppm.o: /usr/include/bits/types.h /usr/include/libio.h
ppm.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
ppm.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
ppm.o: /usr/include/endian.h /usr/include/bits/endian.h
ppm.o: /usr/include/sys/select.h /usr/include/bits/select.h
ppm.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
ppm.o: /usr/include/alloca.h /usr/local/include/GL/glut.h
ppm.o: /usr/local/include/GL/gl.h /usr/local/include/GL/glu.h ppm.h
gettime.o: /usr/include/sys/time.h /usr/include/features.h
gettime.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gettime.o: /usr/include/time.h /usr/include/bits/types.h
gettime.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gettime.o: /usr/include/sys/select.h /usr/include/bits/select.h
gettime.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
gettime.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
gettime.o: /usr/include/bits/confname.h /usr/include/getopt.h
midi.o: /usr/include/stdlib.h /usr/include/features.h
midi.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
midi.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
midi.o: /usr/include/sys/types.h /usr/include/bits/types.h
midi.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
midi.o: /usr/include/sys/select.h /usr/include/bits/select.h
midi.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
midi.o: /usr/include/alloca.h /usr/include/string.h midi.h
