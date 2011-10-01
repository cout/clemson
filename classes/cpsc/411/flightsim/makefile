COPTS = -g
SRC=fly.c objects.c gltext.c spin.c scene.c display.c input.c reshape.c compass.c ppm.c gettime.c midi.c
OBJS=${SRC:.c=.o}
HEADERS=gltext.h display.h spin.h objects.h input.h reshape.h point.h freeze.h compass.h ppm.h gettime.h midi.h
INCS=-I/usr/local/include -I/usr/disney/users1/pbranna/include
LIBS=-L/usr/disney/users1/vernon/pub/lib -L/usr/lib32/internal -lglut -lGLU -lGL -lm -L/usr/X11/lib -lX11 -lXi -lXt -lXmu
OUT=fly
CFLAGS = -O2 -mips4 -DSGI


##### RULES #####

.SUFFIXES: .c .o .ln

.c.o: 
	${CC} ${INCS} ${CFLAGS} -c ${INCDIR} $< -o $@

default: dep sim

sim: ${OBJS}
	${CC} ${CFLAGS} -o ${OUT} ${OBJS} ${LIBS}

dep:
	makedepend $(INCS) ${SRC}

clean: ${HEADERS}
	/bin/rm -f ${OBJS} ${OUT}

########################################################################
# DO NOT DELETE THIS LINE

fly.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/sgidefs.h
fly.o: /usr/include/stdlib.h /usr/disney/users1/pbranna/include/GL/glut.h
fly.o: /usr/include/GL/gl.h /usr/include/GL/glu.h /usr/include/assert.h
fly.o: scene.h gltext.h spin.h display.h input.h reshape.h freeze.h compass.h
fly.o: gettime.h midi.h
objects.o: /usr/include/assert.h /usr/include/stdlib.h
objects.o: /usr/include/standards.h /usr/include/sgidefs.h
objects.o: /usr/include/time.h /usr/disney/users1/pbranna/include/GL/glut.h
objects.o: /usr/include/GL/gl.h /usr/include/GL/glu.h /usr/include/math.h
objects.o: /usr/include/stdio.h objects.h reshape.h input.h ppm.h point.h
gltext.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
gltext.o: /usr/include/GL/glu.h gltext.h
spin.o: /usr/include/sys/time.h /usr/include/standards.h
spin.o: /usr/include/sgidefs.h /usr/include/unistd.h
spin.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
spin.o: /usr/include/GL/glu.h spin.h gettime.h
scene.o: /usr/include/math.h /usr/include/sgidefs.h /usr/include/standards.h
scene.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
scene.o: /usr/include/GL/glu.h objects.h reshape.h scene.h spin.h input.h
display.o: /usr/include/math.h /usr/include/sgidefs.h
display.o: /usr/include/standards.h
display.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
display.o: /usr/include/GL/glu.h display.h fsim.h
input.o: /usr/include/ctype.h /usr/include/standards.h
input.o: /usr/include/locale_attr.h /usr/include/sgidefs.h
input.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
input.o: /usr/include/GL/glu.h /usr/include/stdio.h /usr/include/stdlib.h
input.o: /usr/include/assert.h /usr/include/math.h spin.h reshape.h freeze.h
input.o: input.h fsim.h gettime.h compass.h
reshape.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
reshape.o: /usr/include/GL/glu.h /usr/include/stdlib.h
reshape.o: /usr/include/standards.h /usr/include/sgidefs.h
reshape.o: /usr/include/assert.h reshape.h input.h
compass.o: /usr/include/stdlib.h /usr/include/standards.h
compass.o: /usr/include/sgidefs.h /usr/include/assert.h
compass.o: /usr/disney/users1/pbranna/include/GL/glut.h /usr/include/GL/gl.h
compass.o: /usr/include/GL/glu.h compass.h display.h input.h reshape.h
ppm.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/sgidefs.h
ppm.o: /usr/include/stdlib.h /usr/disney/users1/pbranna/include/GL/glut.h
ppm.o: /usr/include/GL/gl.h /usr/include/GL/glu.h ppm.h
gettime.o: /usr/include/sys/time.h /usr/include/standards.h
gettime.o: /usr/include/sgidefs.h /usr/include/unistd.h
midi.o: /usr/include/stdlib.h /usr/include/standards.h /usr/include/sgidefs.h
midi.o: /usr/include/string.h midi.h
