/* sparcsim.c */

#include <stdio.h>
#include "sparc.h"
#include <strings.h>

/*
** The following variables provide information about implemented
** instructions.
*/
char instr_format[100];
char instr_label[100];
char instrname[100][10];
unsigned long instrvalu[100];
unsigned long instrmask[100];
unsigned long instrcnt[100];
int num_instr=0;

/*
** Variables defined in Lab 2.
*/
int num_instr_loaded=0;
int toti = 0;
long IP=0;
unsigned long rammem[2000];

/*
** The register file.
*/ 
unsigned long registers[32];

/*
** Decoded pieces of the current instruction.
*/
long rdst;			/* Destination register, from lab 2 */
long rsrc1;			/* Source register 1 */
long rsrc2;			/* Source register 2 */
long simm13;		/* 13-bit signed immediate for format 3c */
long reg_or_imm;	/* is the second source a register or immediate? */
long cond;			/* Condition bits */
long disp22;		/* 22-bit displacement for format 2 */
long disp30;		/* 30-bit displacement for format 1 */
long effaddr;		/* effective address - format 3 */
long branchaddr;	/* branch address - format 2 */
long calladdr;		/* call address - format 1 */


/*
** load_instr: this function is finished.
*/
void load_instr(filename)
char *filename;
{
FILE *f;
char  linebuf[100];
int n;

   num_instr = 0;
   f = fopen(filename, "r");
	if(!f){
		printf("ERROR: I couldn't find the sparc.ins file!\n");
		exit(-1);
	}
   fgets(linebuf,99,f);
   while (!feof(f)) {
      instr_format[num_instr] = linebuf[32];
      instr_label[num_instr] = linebuf[34];
      linebuf[7] = 0;          /*null terminate string name */
      strcpy(instrname[num_instr],linebuf);
 
      linebuf[19]= 0;          /* instruction value as hex  */
      sscanf(&linebuf[8], "%x", &instrvalu[num_instr]);
 
      linebuf[30]= 0;          /* instruction mask          */
      sscanf(&linebuf[20], "%x", &instrmask[num_instr]);

      instrcnt[num_instr] = 0;
      num_instr++;
      fgets(linebuf,99,f);
   }

   fclose(f);
}
 
/*
** load_prog: this function is finished.
*/ 
void load_prog(filename)
char *filename;
{
FILE *f;
char  linebuf[100];
unsigned long instr;
int i = 0;
 
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
   num_instr_loaded = i;
   printf("\n\n");

}

/*
** fetch - this function doesn't need to be modified.
*/
unsigned long fetch() {
   return rammem[IP++];
}

/*
** decode: this function needs to be modified to completely decode
** the given instruction.  Currently, this only computes the destination
** register for a given instruction.
*/
int decode (unsigned long instr)
{ 
   int i;

   rdst       = (instr >> 25) & 0x0000001f; 
   rsrc1      = (instr >> 14) & 0x0000001f;
   rsrc2      = (instr)       & 0x0000001f;
   simm13     = (instr)       & 0x00001fff;
   reg_or_imm = (instr >> 13) & 0x00000001;
   cond       = (instr >> 25) & 0x0000000f;
   disp22     = (instr)       & 0x003fffff;
   disp30     = (instr)       & 0x3fffffff;

   /* Compute the effective address for loads and stores - format 3 */
   if(reg_or_imm){
      /* Second source is an immediate value */
      if(simm13 >> 12){
         /* immediate value is negative; perform sign extension */
         effaddr = registers[rsrc1] + (0xffffe000 | simm13);
      } else {
         effaddr = registers[rsrc1] + simm13;
      }
   } else {
      /* Second source is a register */
      effaddr = registers[rsrc1] + registers[rsrc2];
   }

   /* branch address - format 2 */
   if(disp22 >> 21){
      /* displacement is negative; perform sign extension */
      branchaddr = 0xffc00000 | disp22;
   } else {
      branchaddr = disp22;
   }

   /* Call address - format 1 */
   calladdr = (disp30 << 2);  /* Call displacements are always left-shifted */

   for (i=0; (instr & instrmask[i])!=instrvalu[i] && i<num_instr; i++)
	; /* do nothing */ 

   if (i==num_instr) {
     printf("ERROR: could not id opcode: %lx\n", instr);
   } else {

    printf("%s ", instrname[i]);

	switch(instr_format[i]){
	case FORMAT1:
		printf("0x%08X", calladdr);
		break;
	case FORMAT2:
		if(instr_label[i] == INSTR_sethi)
			printf("0x%08X R%d", branchaddr, rdst);
		else
			printf("0x%01X 0x%08X", cond, branchaddr);
		break;
	case FORMAT3:
		if(reg_or_imm)
			printf("R%d R%d 0x%08X", rdst, rsrc1, effaddr);
		else
			printf("R%d R%d R%d", rdst, rsrc1, rsrc2);
		break;
	}

	printf("\n");
 
    instrcnt[i]++;           /* How many times that instr occurred */
    toti++;
 
   }
  
   return i;
}

/*
** Execute - this function doesn't need to be modified for
** this lab.
*/
void Execute(){
	int cycles = 0;
    IP = 0;
    while (cycles<num_instr_loaded) {
      decode(fetch());    
      cycles++;
    }
}

/*
** main - this function also does not need to be modified for
** this lab.
*/
/* void main is BAD!!! */
/* void main(argc, argv)
int argc;
char **argv; */
int main(int argc, char *argv[])
{

   if (argc < 2) {
      printf("ERROR:  %s sparc.prg\n", argv[0]);
   }
   else {
      load_instr("sparc.ins");
      load_prog(argv[1]);
	  Execute();
   }
   return 0;
}

