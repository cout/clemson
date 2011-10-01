SRC=main.c md2.c pcx.c scene.c _string.c fixnums.c crater.c box.c \
	npr_draw.c texgen.c lighting.c
OBJ=$(SRC:.c=.o)
# LIBS=-L/usr/disney/users1/vernon/pub/lib -L/usr/lib32/internal -lglut -lGLU -lGL -lm -L/usr/X11/lib -lX11 -lXmu
LIBS=-lglut -lGLU -lGL -lm
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

default: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(LIBS)

clean:
	/bin/rm -f $(OBJ) $(OUT)

CFLAGS += -MMD -MP
DEP_FILES = $(patsubst %.o,%.d,$(OBJS))
-include $(DEP_FILES)

