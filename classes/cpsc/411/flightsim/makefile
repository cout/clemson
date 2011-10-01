COPTS = -g
SRC=fly.c objects.c gltext.c spin.c scene.c display.c input.c reshape.c compass.c ppm.c gettime.c midi.c
OBJS=${SRC:.c=.o}
HEADERS=gltext.h display.h spin.h objects.h input.h reshape.h point.h freeze.h compass.h ppm.h gettime.h midi.h
INCS=-I/usr/local/include -I/usr/disney/users1/pbranna/include
# LIBS=-L/usr/disney/users1/vernon/pub/lib -L/usr/lib32/internal -lglut -lGLU -lGL -lm -L/usr/X11/lib -lX11 -lXi -lXt -lXmu
LIBS=-lglut -lGLU -lGL -lm
OUT=fly
# CFLAGS = -O2 -mips4 -DSGI
CFLAGS = -O2


##### RULES #####

.SUFFIXES: .c .o .ln

.c.o: 
	${CC} ${INCS} ${CFLAGS} -c ${INCDIR} $< -o $@

default: sim

sim: ${OBJS}
	${CC} ${CFLAGS} -o ${OUT} ${OBJS} ${LIBS}

CFLAGS += -MMD -MP
DEP_FILES = $(patsubst %.o,%.d,$(OBJS))
-include $(DEP_FILES)

clean: ${HEADERS}
	/bin/rm -f ${OBJS} ${OUT}

