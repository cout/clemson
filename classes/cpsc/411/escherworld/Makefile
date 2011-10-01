SRC=main.c md2.c pcx.c scene.c _string.c fixnums.c crater.c box.c \
	npr_draw.c texgen.c lighting.c
OBJ=$(SRC:.c=.o)
LIBS=-L/usr/disney/users1/vernon/pub/lib -L/usr/lib32/internal -lglut -lGLU -lGL -lm -L/usr/X11/lib -lX11 -lXmu
OUT=ew
CFLAGS=-g -DSGI -O2
CC=cc
INCLUDES=-I/usr/local/include
#CC=gcc
#INCLUDES=-I/usr/include/g++-2 -I/usr/local/include

##### RULES #####

.SUFFIXES: .c .o .ln

.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) $(INCLUDES) -c $< -o $@

default: dep $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(LIBS)

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	/bin/rm -f $(OBJ) $(OUT)

# DO NOT DELETE

main.o: /usr/include/sys/signal.h /usr/include/standards.h
main.o: /usr/include/sys/types.h /usr/include/sgidefs.h
main.o: /usr/include/sys/stat.h /usr/include/sys/timespec.h
main.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h /usr/include/stdio.h
main.o: /usr/include/sys/ioctl.h /usr/include/sys/ioccom.h
main.o: /usr/include/net/soioctl.h /usr/include/sys/termio.h
main.o: /usr/include/sys/termios.h /usr/include/sys/ttydev.h
main.o: /usr/include/math.h /usr/local/include/GL/glut.h /usr/include/GL/gl.h
main.o: /usr/include/GL/glu.h /usr/include/assert.h /usr/include/stdlib.h
main.o: pcx.h md2.h texgen.h scene.h _string.h
md2.o: /usr/include/stdio.h /usr/include/standards.h /usr/include/sgidefs.h
md2.o: /usr/include/stdlib.h /usr/include/string.h
md2.o: /usr/local/include/GL/glut.h /usr/include/GL/gl.h
md2.o: /usr/include/GL/glu.h /usr/include/assert.h /usr/include/math.h md2.h
md2.o: texgen.h pcx.h fixnums.h anorms.h
pcx.o: /usr/include/GL/glu.h /usr/include/GL/gl.h /usr/include/assert.h
pcx.o: /usr/include/stdlib.h /usr/include/standards.h /usr/include/sgidefs.h
pcx.o: /usr/include/stdio.h pcx.h
scene.o: /usr/include/assert.h /usr/include/stdio.h /usr/include/standards.h
scene.o: /usr/include/sgidefs.h /usr/include/string.h /usr/include/math.h
scene.o: /usr/include/stdlib.h /usr/local/include/GL/glut.h
scene.o: /usr/include/GL/gl.h /usr/include/GL/glu.h scene.h md2.h texgen.h
scene.o: crater.h _string.h box.h npr_draw.h ew.h
_string.o: /usr/include/ctype.h /usr/include/standards.h
_string.o: /usr/include/locale_attr.h /usr/include/sgidefs.h
_string.o: /usr/include/string.h
fixnums.o: /usr/include/assert.h /usr/include/string.h
fixnums.o: /usr/include/standards.h /usr/include/math.h
fixnums.o: /usr/include/sgidefs.h fixnums.h
crater.o: /usr/local/include/GL/glut.h /usr/include/GL/gl.h
crater.o: /usr/include/GL/glu.h /usr/include/math.h /usr/include/sgidefs.h
crater.o: /usr/include/standards.h /usr/include/stdlib.h /usr/include/stdio.h
crater.o: ew.h
box.o: /usr/include/assert.h /usr/include/stdio.h /usr/include/standards.h
box.o: /usr/include/sgidefs.h /usr/local/include/GL/glut.h
box.o: /usr/include/GL/gl.h /usr/include/GL/glu.h box.h ew.h texgen.h
npr_draw.o: /usr/local/include/GL/glut.h /usr/include/GL/gl.h
npr_draw.o: /usr/include/GL/glu.h npr_draw.h md2.h /usr/include/stdio.h
npr_draw.o: /usr/include/standards.h /usr/include/sgidefs.h texgen.h
texgen.o: /usr/include/math.h /usr/include/sgidefs.h /usr/include/standards.h
texgen.o: /usr/local/include/GL/glut.h /usr/include/GL/gl.h
texgen.o: /usr/include/GL/glu.h texgen.h ew.h pcx.h
lighting.o: /usr/local/include/GL/glut.h /usr/include/GL/gl.h
lighting.o: /usr/include/GL/glu.h lighting.h
