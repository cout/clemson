SRC=main.c md2.c pcx.c tankdef.c _string.c fixnums.c
OBJ=$(SRC:.c=.o)
LIBS=-L/usr/disney/users1/vernon/pub/lib -L/usr/lib32/internal -lglut -lGLU -lGL -lm -L/usr/X11/lib -lX11 -lXmu
OUT=md2
CFLAGS=-g -DSGI
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

main.o: /usr/local/include/GL/glut.h /usr/include/GL/gl.h
main.o: /usr/include/GL/glu.h /usr/include/assert.h /usr/include/stdlib.h
main.o: /usr/include/standards.h /usr/include/sgidefs.h pcx.h md2.h
main.o: /usr/include/stdio.h tankdef.h
md2.o: /usr/include/stdlib.h /usr/include/standards.h /usr/include/sgidefs.h
md2.o: /usr/include/string.h /usr/local/include/GL/glut.h
md2.o: /usr/include/GL/gl.h /usr/include/GL/glu.h /usr/include/assert.h md2.h
md2.o: /usr/include/stdio.h pcx.h fixnums.h anorms.h
pcx.o: /usr/include/GL/glu.h /usr/include/GL/gl.h /usr/include/assert.h
pcx.o: /usr/include/stdlib.h /usr/include/standards.h /usr/include/sgidefs.h
pcx.o: /usr/include/stdio.h pcx.h
tankdef.o: /usr/include/stdio.h /usr/include/standards.h
tankdef.o: /usr/include/sgidefs.h /usr/include/string.h tankdef.h _string.h
_string.o: /usr/include/ctype.h /usr/include/standards.h
_string.o: /usr/include/locale_attr.h /usr/include/sgidefs.h
_string.o: /usr/include/string.h
fixnums.o: /usr/include/assert.h /usr/include/string.h
fixnums.o: /usr/include/standards.h /usr/include/math.h
fixnums.o: /usr/include/sgidefs.h fixnums.h
