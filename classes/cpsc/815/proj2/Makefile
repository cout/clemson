SRC=imtrans.cpp
OBJ=$(SRC:.cpp=.o)
OUT=imtrans

INCLUDES=-I/usr/include/g++ -I/usr/local/include -I.
CFLAGS=-Wall -O2 -g `libglade-config --cflags` `gtk-config --cflags` \
	`gdk-pixbuf-config --cflags`
CCFLAGS=$(CFLAGS)
CC=gcc
CCC=g++

LIBS=-lm
LDFLAGS=`gtk-config --libs` `libglade-config --libs` \
	`gdk-pixbuf-config --libs`

##### RULES #####

.SUFFIXES: .c .c .ln

.cpp.o: 
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

clean:
	/bin/rm -f $(OBJ) $(OUT)
