all: cuglint.o test.o

clean:
	rm -f cuglint.o test.o

cuglint.o: cuglint.c
	gcc -Wall -D__KERNEL__ -DMODULE -DCPU=586 -DMODVERSIONS \
	    -O2 -fomit-frame-pointer -fno-strength-reduce \
	    -fvolatile -fvolatile-global -pipe -malign-loops=2 \
	    -malign-jumps=2 -malign-functions=2 \
	    -c cuglint.c -o cuglint.o

test.o: test.c
	gcc test.c -o test

install: cuglint.o
	chown root.root cuglint.o
	chmod 644 cuglint.o
	cp cuglint.o /lib/modules/`cat /proc/sys/kernel/osrelease`/video
	depmod -a -e
