COPTS = -g
TESTSRC=objects.c gltext.c spin.c scene.c test.c display.c ground.c input.c reshape.c time.c
TESTOBJS=${TESTSRC:.c=.o}
FSIMSRC=objects.c gltext.c spin.c scene.c fsim.c display.c ground.c input.c reshape.c time.c
FSIMOBJS=${FSIMSRC:.c=.o}
HEADERS=gltext.h draw.h display.h spin.h objects.h ground.h input.h reshape.h point.h freeze.h
LIBS=-lglut -lGLU -lGL -lm -L/usr/X11/lib -lX11 -lXi -lXt -lXmu
TESTOUT=sim
FSIMOUT=fsim
CFLAGS = -O2

##### RULES #####

.SUFFIXES: .c .o .ln

.c.o: 
	${CC} ${CFLAGS} -c ${INCDIR} $< -o $@

default: dep sim fsim

sim: ${TESTOBJS}
	${CC} ${CFLAGS} -o ${TESTOUT} ${TESTOBJS} ${LIBS}

fsim: $(FSIMOBJS)
	$(CC) $(CFLAGS) -o $(FSIMOUT) $(FSIMOBJS) $(LIBS)

dep:
	makedepend -I/usr/local/include ${SRC}

clean: ${HEADERS}
	/bin/rm -f ${OBJS} ${OUT}

########################################################################
# DO NOT DELETE THIS LINE
