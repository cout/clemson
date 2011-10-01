# Example Makefile for CpSc L241 lab6

# Define variables for the include and source directories (outside the local
# directory).
INCLUDES = -I/u3/cs241/lib2
SOURCE = /u3/cs241/lib2

# "all" denotes the default executables to be made.  (a.out could be changed
# as desired, but must agree with the name used in the remainder of the
# make file.
all: a.out 

# The first line below indicates that a.out depends on main.o and datarec.o.
# The second line specifies the rule (i.e., the command) for creating a.out if
# a.out is older than main.o or datarec.o. (Note that command lines such as
# this MUST begin with a TAB character (rather than the usual "whitespace").

a.out: main.o datarec.o stringClass.o
	CC -o a.out main.o datarec.o stringClass.o  # link together main.o and datarec.o


# Now we must specify how to build the components main.o and datarec.o if they
# should be needed.
# Note the $(INCLUDES) below.  This is replaced with the value of the INCLUDES
# variable given above, and similarly for the SOURCE variable.
# Also, the -c option tells the compiler to not link the object file at
# this stage.

main.o: main.c datarec.h 
	CC -c $(INCLUDES) -o main.o main.c

datarec.o: datarec.c datarec.h
	CC -c $(INCLUDES) -o datarec.o datarec.c

# The following lines will be needed later, and are included here for your
# convenience.
stringClass.o: $(SOURCE)/stringClass.c 
	CC -c $(INCLUDES) -o stringClass.o $(SOURCE)/stringClass.c

clean:
	rm -f *.o
	rm -f a.out
