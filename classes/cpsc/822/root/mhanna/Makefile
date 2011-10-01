all:	calc.o ctest.o
	gcc ctest.o -o ctest
	insmod calc.o

ctest:	ctest.c
	gcc -Wall -c ctest.c

calc.o:	calc.c calc.h
	gcc -Wall -D__KERNEL__ -DMODULE -DCPU=586 -O2 -fomit-frame-pointer -fno-strength-reduce -fvolatile -fvolatile-global -pipe -malign-loops=2 -malign-jumps=2 -malign-functions=2 -c calc.c -o calc.o

clean:
	rmmod calc
	rm *.o core ctest
