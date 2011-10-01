/* 
**  sim3.c - processor simulator code file.
*/

#include <stdio.h>
#include "sparc.h"
#include <strings.h>

/* 
** These variables are set by load_instr, and contain
** information about each of the instructions implemented 
** by this simulator
*/
char instr_format[100];  /* each instruction is one of 3 basic types */
char instr_label[100];   /* simple identifier for instruction */
char instrname[100][10]; /* assembly code name of instruction */
unsigned long instrvalue[100];  /* hex code value of this instruction */
unsigned long instrmask[100];  /* mask out unwanted bits for instruction */
unsigned long instrcnt[100];   /* number of calls to this instruction */
int num_instr;  /* total number of instructions implemented */

/*
** Student defined variables begin here.
*/
int toti = 0;   /* total number of instructions executed in a trace */ 
#define RAMMAX 2000
unsigned long rammem[RAMMAX]; /* size of memory is 2k 32-bit words */
unsigned long ip; /* instruction pointer */

/*
** load_instr is the function that reads in all of the implemented
** instructions for this simulator, which are stored in sparc.ins.
** You shouldn't need to modify this routine.
*/ 
void load_instr(char *filename)
{
FILE *f;
char  linebuf[100];
int n;

   num_instr = 0;
   f = fopen(filename, "r");
   fgets(linebuf,99,f);
   while (!feof(f)) {
      instr_format[num_instr] = linebuf[32];
      instr_label[num_instr] = linebuf[34];
      linebuf[7] = 0;          /*null terminate string name */
      strcpy(instrname[num_instr],linebuf);
 
      linebuf[19]= 0;          /* instruction value as hex  */
      sscanf(&linebuf[8], "%x", &instrvalue[num_instr]);
 
      linebuf[30]= 0;          /* instruction mask          */
      sscanf(&linebuf[20], "%x", &instrmask[num_instr]);

      instrcnt[num_instr] = 0; /* initialize instruction count */
      num_instr++;
      fgets(linebuf,99,f);
   }

   fclose(f);
}
 
/*
** load_prog is a function that reads a program from an input file
** (passed as the single argument) and loads the program into memory.
** Currently, all this routine does is read the sample program and
** spit it back out at the user, without storing it in memory.
*/ 
void load_prog(char* filename)
{
FILE *f;
char  linebuf[100];
unsigned long instr;
int i = 0;

   memset(rammem, 0, sizeof(rammem)); /* clear out memory */
 
   f = fopen(filename, "r");
   fgets(linebuf,99,f);
   while (!feof(f)) {
      linebuf[10]= 0;
      printf(linebuf);
      printf("\n");
      sscanf(linebuf, "%x", &instr);
      rammem[i++] = instr;
      fgets(linebuf,99,f);
   }
    
   fclose(f);
   printf("\n\n");

   ip = 0; /* reset instruction pointer */
}

unsigned long fetch() {
	return rammem[ip++];
}

void decode(unsigned long instr) {
	int rdest = (instr & 0x3e000000) >> 25;
	unsigned long i;

	/* There MUST be a faster way of doing this!  No time for it now */
	for(i = 0; i < num_instr && (instr &instrmask[i]) !=
		instrvalue[i]; i++);

	if(i == num_instr) printf("invalid instruction %ld", i);
	else printf("%s r%d\n", instrname[i], rdest);

	/* no need to return any values yet */
	return;
}

void execute() {
	unsigned long instr;
	instr = fetch();
	decode(instr);
}

void go() {
	while(rammem[ip] != 0) execute();
}

/*
** main - the main program.
*/
int main(argc, argv)
int argc;
char **argv;
{
   if (argc < 2) {
      printf("ERROR:  %s sparc.prg\n", argv[0]);
      exit(1);
   }
   else {
      load_instr("sparc.ins");
      load_prog(argv[1]);
      go();
   }
   return 0;
}

