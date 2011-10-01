SRC=	main.c gtkmisc.c \
	widgets/window.c widgets/widgets.c widgets/buttonctrl.c \
	widgets/right_vbox.c widgets/scale.c widgets/status.c \
	gl/gl.c gl/aw.c gl/selection.c gl/textures.c gl/normals.c gl/splines.c \
	gl/trackball/trackball.c gl/glarea2.c gl/ctlpt.c gl/particle.c \
	misc/_string.c misc/io.c misc/dynarray.c misc/_time.c misc/geometry.c \
	menus/filemenu.c menus/editmenu.c menus/optionmenu.c \
	menus/objectmenu.c menus/particlemenu.c \
	math/matrix.c
OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.cpp=.o)
OUT=viewer

INCLUDES=-I/usr/local/include -I.
CFLAGS=-g -O2 -Wall `libglade-config --cflags` `gtk-config --cflags` \
	`glib-config --cflags` `imlib-config --cflags`
CC=gcc

CCFLAGS=$(CFLAGS)
CCC=CC

LIBS=-lGLU -lGL -lgtkgl
LDFLAGS=`gtk-config --libs` `libglade-config --libs` \
	`imlib-config --libs-gdk` -lm

##### RULES #####

.SUFFIXES: .c .c .ln

default: setup dep $(OUT)

# Machine-specific definitions:
setup:
	if test "$(CC)" = "cc"; then \
		LDFLAGS="$(LDFLAGS) -Wl,-woff,84 -Wl,-woff,85 -Wl,-woff,127"; \
	else \
		CFLAGS="$(CFLAGS) -Wall"; \
	fi

.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.cpp.o: 
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@


$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

clean:
	/bin/rm -f $(OBJ) $(OUT)

CFLAGS += -MMD -MP
DEP_FILES = $(patsubst %.o,%.d,$(OBJ))
-include $(DEP_FILES)

