SRC=imlib-test.cpp
OBJ=$(SRC:.cpp=.o)
OUT=imlib-test

INCLUDES=-I/usr/include/g++ -I/usr/local/include -I.
CFLAGS=-Wall -O2 -g `libglade-config --cflags` `gtk-config --cflags`
CCC=g++

LIBS=-lm
LDFLAGS=-L/usr/local/lib `gtk-config --libs` `libglade-config --libs` \
	-lgdk_imlib -L/usr/lib/libImlib2.so

##### RULES #####

.SUFFIXES: .c .c .ln

.cpp.o: 
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

clean:
	/bin/rm -f $(OBJ) $(OUT)
