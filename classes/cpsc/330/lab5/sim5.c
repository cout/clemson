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
long effaddr;		/* effective address */
long branchaddr;	/* branch address */
long calladdr;		/* call address */


/*
** load_instr: this function is finished.  One line was added to initialize
** register 0 to 0.
*/
void load_instr(filename)
char *filename;
{
FILE *f;
char  linebuf[100];

   registers[0] = 0;
	
   num_instr = 0;
   f = fopen(filename, "r");
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
** fetch - this function fetches an instruction from memory.  This doesn't
** need to be modified for lab 4.
*/
unsigned long fetch() {
   return rammem[IP++];
}

/*
** decode - this function decodes a fetched instruction.  This doesn't
** need to be modified for lab 4.  The switch statement was modified 
** slightly; this only affects the printed output.
*/
int decode (unsigned long instr)
{ 
   int i;
   rdst = (instr >> 25) & 0x0000001f; 
   rsrc1 = (instr >> 14) & 0x0000001f;
   rsrc2 = instr & 0x0000001f;
   simm13 = instr & 0x00001fff;
   reg_or_imm = (instr >> 13) & 0x00000001;
   cond = (instr >> 25) & 0x0000000f;	
   disp22 = instr & 0x003fffff;
   disp30 = instr & 0x3fffffff;

	/* Compute the effective address */
	if(reg_or_imm){ 
		if(simm13 >> 12){
			effaddr = registers[rsrc1] + (0xffffe000 | simm13);
		} else {
			effaddr = registers[rsrc1] + simm13;
		}
	} else {
		effaddr = registers[rsrc1] + registers[rsrc2];
	}	

	/* branch address */
	if (disp22 >> 21) {
		branchaddr = 0xffc00000 | disp22;
	}else{
		branchaddr = disp22;
	}
	
	/* Call address */
	calladdr = (disp30 << 2);

   for (i=0; (instr & instrmask[i])!=instrvalu[i] && i<num_instr; i++)
	; /* do nothing */ 

   if (i==num_instr) {
     printf("ERROR: could not id opcode: %lx\n", instr);
   } else {

    printf("%s ", instrname[i], rdst);

	switch(instr_format[i]){
	case FORMAT1:
		printf("0x%08X\n",disp30);
		break;
	case FORMAT2:
		if(instr_label[i] == INSTR_sethi)
			printf("0x%08X, R%d\n",disp22,rdst);
		else
			printf("0x%08X, 0x%08X\n",cond,disp22);
		break;
	case FORMAT3:
		if(reg_or_imm){
			printf("R%d R%d 0x%08X\n",rdst,rsrc1,simm13);
		}else{
			printf("R%d R%d R%d\n",rdst,rsrc1,rsrc2);
		}
		break;
	}
 
    instrcnt[i]++;           /* How many times that instr occurred */
    toti++;
 
   }
  
   return i;
}

/*
**	Execute - this function needs to be filled in.
*/
void Execute(){
	int cycles = 0;
	int instruction_number;
    IP = 0;
    while (cycles<num_instr_loaded) {
      instruction_number = decode(fetch());    
	  /* Execute the code here */
		switch(instr_label[instruction_number]){
			case INSTR_ld:   /* load a whole word */
			break;
				registers[rdst] = (rammem[effaddr]);
			case INSTR_ldub:	/* load unsigned byte */
				registers[rdst] = (rammem[effaddr] & 0xff000000) >> 24;
			break;
			case INSTR_lduh:	/* load unsigned half-word */
				registers[rdst] = (rammem[effaddr] & 0xffff0000) >> 16;
			break;
			case INSTR_ldsb:  /* load signed byte */
				registers[rdst] = (rammem[effaddr] & 0xff000000)/(256*256*256);
			break;
			case INSTR_ldsh:  /* load signed halfword */
				registers[rdst] = (rammem[effaddr] & 0xffff0000)/(256*256);
			break;
			case INSTR_ldd:		/* load doubleword */
				registers[rdst] = rammem[effaddr];
				registers[rdst+1] = rammem[effaddr+1];
			break;
			case INSTR_st:		/* store */
				rammem[effaddr] = registers[rdst];
			break;
			case INSTR_stb:		/* store byte */
				rammem[effaddr] = registers[rdst] << 24;
			break;
			case INSTR_sth:		/* store halfword */
				rammem[effaddr] = registers[rdst] << 16;
			break;
			case INSTR_std:		/* store a double word */
				rammem[effaddr]   = registers[rdst];
				rammem[effaddr+1] = registers[rdst+1];
			break;
			case INSTR_swap:	/* swap register with memory */
				{
					unsigned long tmp;
					tmp = rammem[effaddr];
					rammem[effaddr] = registers[rdst];
					registers[rdst] = tmp;
				}
			break;
			case INSTR_sethi:	/* set high-order 22 bits of rd */
				registers[rdst] = disp22 << 10;
			break;
			case INSTR_or:		/* or; set condition codes on S bit */
				registers[rdst] = registers[rsrc1] | 
					((reg_or_imm)?(simm13):(registers[rsrc2]));
				/* save the condition codes for next lab */
			break;
			case INSTR_add:		/* add; set condition codes on S bit */
				registers[rdst] = effaddr; 
				/* save the condition codes for next lab */
			break;
			default:
				printf("Unimplemented instruction: %s\n",
					instrname[instruction_number]);
			break;
			
		}
      cycles++;
    }
}

/*
** print_regs - this function just prints out all registers; this still
** needs to be written.
*/
void print_regs(){
	int j;
	for(j = 0; j < 31; j++) printf("R%d:\t0x%08x\n", j, registers[j]);
}

/*
** main - this function also does not need to be modified for
** this lab; print_regs was added after Execute.
*/
void main(argc, argv)
int argc;
char **argv;
{

   if (argc < 2) {
      printf("ERROR:  %s sparc.prg\n", argv[0]);
   }
   else {
      load_instr("sparc.ins");
      load_prog(argv[1]);
	  Execute();
	  print_regs();
   }
}

