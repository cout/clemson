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
/* long IP=0; */
long PC, NPC;
unsigned long rammem[2000];



/*
** Pipeline variables, definitions, etc, for lab 7.
*/
#define NUM_STAGES 3
#define NEXT_PIPESTAGE(x) (((x)+1) % NUM_STAGES)
typedef struct Qelement Qelement;  /* this is a single pipeline stage */
struct Qelement{
	int i;          /* which instruction is this? */
	unsigned long instr;    /* the undecoded instruction */
	long rdst;  		/* Destination register for this instruction */
	long rsrc1;             /* Source register 1 */
	long rsrc2;             /* Source register 2 */
	long simm13;            /* 13-bit signed immediate for format 3c */
	long reg_or_imm;        /* is the second source a register or immediate? */
	long cond;              /* Condition bits */
	long disp22;            /* 22-bit displacement for format 2 */
	long disp30;            /* 30-bit displacement for format 1 */
	long effaddr;           /* effective address */
	long branchaddr;        /* branch address */
	long calladdr;          /* call address */
	int anull;      /* is the anull bit on for this instruction? */
	int sbit;       /* is the sbit set for this instruction? */
};
Qelement pipeline_Q[NUM_STAGES];  /* this is the complete pipeline */

/*
** Branching variables (with condition codes) defined in Lab 5.
*/
int cond_zero;
int cond_negative;

/*
** The register file.
*/ 
unsigned long registers[32];

/* prototype */
void execute_branch();

/*
** load_instr: this function is finished.
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
void fetch(unsigned long stage) {
	PC = NPC++;
	pipeline_Q[stage].instr = rammem[PC];
}

/*
** decode - this function decodes a fetched instruction.  This doesn't
** need to be modified for lab 4.
*/
void decode (unsigned long stage)
{ 
   Qelement *s = pipeline_Q + stage;
   int donegate;
   unsigned long instr = s->instr;

   s->rdst = (instr >> 25) & 0x0000001f; 
   s->rsrc1 = (instr >> 14) & 0x0000001f;
   s->rsrc2 = instr & 0x0000001f;
   s->simm13 = instr & 0x00001fff;
   s->reg_or_imm = (instr >> 13) & 0x00000001;
   s->cond = (instr >> 25) & 0x0000000f;	
   s->disp22 = instr & 0x003fffff;
   s->disp30 = instr & 0x3fffffff;
   s->anull = (instr >> 29) & 0x00000001;
   s->sbit = (instr >> 23) & 0x00000001;
	

   for (s->i=0; (instr & instrmask[s->i])!=instrvalu[s->i] && s->i<num_instr; s->i++)
	; /* do nothing */ 

	/* Compute the effective address or alu operation */
	donegate = (instr_label[s->i]==INSTR_sub || instr_label[s->i]==INSTR_subx);
	if(s->reg_or_imm){ 
		if(s->simm13 >> 12){  /* displacement is negative */
			if(s->rsrc1 != 0){
				if(donegate){
					s->effaddr = registers[s->rsrc1] - (0xffffe000 | s->simm13);
				} else {
					s->effaddr = registers[s->rsrc1] + (0xffffe000 | s->simm13);
				}
			} else {
				s->effaddr = 0xffffe000 | s->simm13;
			}
		} else {	/* displacement is postive */
			if(s->rsrc1 != 0){
				if (donegate){
					s->effaddr = registers[s->rsrc1] - s->simm13;
				} else {
					s->effaddr = registers[s->rsrc1] + s->simm13;
				}
			} else {
				s->effaddr = s->simm13;	
			}	
		}
	} else {	/* second operand is a register */
		if(donegate)
			s->effaddr = registers[s->rsrc1] - registers[s->rsrc2];
		else
			s->effaddr = registers[s->rsrc1] + registers[s->rsrc2];
	}	

	/* branch address */
	if (s->disp22 >> 21) {
		s->branchaddr = 0xffc00000 | s->disp22;
	}else{
		s->branchaddr = s->disp22;
	}
	
	/* Call address */
	s->calladdr = (s->disp30 << 2);


   if (s->i==num_instr) {
     printf("ERROR: could not id opcode: %lx\n", instr);
   } else {

    printf("%s ", instrname[s->i], s->rdst);

	switch(instr_format[s->i]){
	case FORMAT1:
		printf("0x%08X\n",s->disp30);
		break;
	case FORMAT2:
		if(instr_label[s->i] == INSTR_sethi)
			printf("0x%08X, R%d\n",s->disp22,s->rdst);
		else
			printf("0x%08X, 0x%08X\n",s->cond,s->disp22);
		break;
	case FORMAT3:
		if(s->reg_or_imm){
			printf("R%d R%d 0x%08X\n",s->rdst,s->rsrc1,s->simm13);
		}else{
			printf("R%d R%d R%d\n",s->rdst,s->rsrc1,s->rsrc2);
		}
		break;
	}
 
    instrcnt[s->i]++;           /* How many times that instr occurred */
    toti++;
 
   }
}

/*
**	Execute - this function needs to be filled in.
*/
void Execute(unsigned long stage){
	Qelement *s = pipeline_Q + stage;
	int instruction_number = s->i;
	  /* Execute the code here */
		switch(instr_label[instruction_number]){
			case INSTR_ldsb:  /* load signed byte */
				registers[s->rdst] = (rammem[s->effaddr] & 0xff000000) / (256*256*256);
			break;
			case INSTR_ldsh:  /* load signed halfword */
				registers[s->rdst] = (rammem[s->effaddr] & 0xffff0000) / (256*256);
			break;
			case INSTR_ld:   /* load */
				registers[s->rdst] = rammem[s->effaddr];
			break;
			case INSTR_ldub:	/* load unsigned byte */
				registers[s->rdst] = (rammem[s->effaddr] & 0xff000000) >> 24;
			break;
			case INSTR_lduh:	/* load unsigned half-word */
				registers[s->rdst] = (rammem[s->effaddr] & 0xffff0000) >> 16;
			break;
			case INSTR_ldd:		/* load doubleword */
				registers[s->rdst] = rammem[s->effaddr];
				registers[s->rdst+1] = rammem[s->effaddr+1];
			break;
			case INSTR_st:		/* store */
				rammem[s->effaddr] = registers[s->rdst];
			break;
			case INSTR_stb:		/* store byte */
				rammem[s->effaddr] = registers[s->rdst] << 24;
			break;
			case INSTR_sth:		/* store halfword */
				rammem[s->effaddr] = registers[s->rdst] << 16;
			break;
			case INSTR_std:		/* store a double word */
				rammem[s->effaddr] = registers[s->rdst];
				rammem[s->effaddr+1] = registers[s->rdst+1];
			break;
			case INSTR_swap:	/* swap register with memory */
				{
					unsigned long int tmp;
					tmp = registers[s->rdst];
					registers[s->rdst] = rammem[s->effaddr];
					rammem[s->effaddr] = tmp;
				}
			break;
			case INSTR_sethi:	/* set high-order 22 bits of rd */
				registers[s->rdst] = s->disp22 << 10;
			break;
			case INSTR_or:		/* or; set condition codes on S bit */
				registers[s->rdst] = registers[s->rsrc1] | 
					((s->reg_or_imm)?(s->simm13):(registers[s->rsrc2]));
				if(s->sbit){
					cond_negative = (registers[s->rdst] >> 31);
					cond_zero = (registers[s->rdst] == 0);
				}
			break;
			case INSTR_add:		/* add; set condition codes on S bit */
				registers[s->rdst] = s->effaddr; 
				if(s->sbit){
					cond_negative = (registers[s->rdst] >> 31);
					cond_zero = (registers[s->rdst] == 0);
				}
			break;
			case INSTR_sub:
				registers[s->rdst] = s->effaddr;
				if(s->sbit){
					cond_negative = (registers[s->rdst] >> 31);
					cond_zero = (registers[s->rdst] == 0);
				}
			break;
			case INSTR_and:
				registers[s->rdst] = registers[s->rsrc1] & 
					((s->reg_or_imm)?(s->simm13):(registers[s->rsrc2]));
				if(s->sbit){
					cond_negative = (registers[s->rdst] >> 31);
					cond_zero = (registers[s->rdst] == 0);
				}
			break;
			case INSTR_andn:
				registers[s->rdst] = ~(registers[s->rsrc1] &
					((s->reg_or_imm)?(s->simm13):(registers[s->rsrc2])));
				if(s->sbit){
					cond_negative = (registers[s->rdst] >> 31);
					cond_zero = (registers[s->rdst] == 0);
				}
			break;
			case INSTR_orn:
				registers[s->rdst] = ~(registers[s->rsrc1] |
					((s->reg_or_imm)?(s->simm13):(registers[s->rsrc2])));
				if(s->sbit){
					cond_negative = (registers[s->rdst] >> 31);
					cond_zero = (registers[s->rdst] == 0);
				}
			break;
			case INSTR_bcc:
			execute_branch(stage);
			break;
			default:
				printf("Unimplemented instruction: %s\n",
					instrname[instruction_number]);
			break;
		}
}

void execute_branch(unsigned long stage){
	int do_branch = 0;
	Qelement *s = pipeline_Q + stage;


	switch(s->cond){
		case INSTRC_ba:
		do_branch = 1;
		break;
		case INSTRC_bl:
		do_branch = cond_negative;	
		break;
		case INSTRC_ble:
		do_branch = cond_negative | cond_zero;
		break;
		case INSTRC_be:
		do_branch = cond_zero;
		break;
		case INSTRC_bne:
		do_branch = !cond_zero;
		break;
		default:
		printf("Unimplemented branch condition\n");
	}
	if(do_branch){
		NPC += s->branchaddr;
		NPC -= 2;
	}else if(s->anull){
		NPC++;  /* skip past the next instruction */
	}
}

void print_regs(){
int i;
	printf("\n");
	for(i = 0; i < 16; i++){
		printf("R%d: %08x\t\tR%d: %08x\n",i,registers[i],i+16,registers[i+16]);
	}
}

/*
** main - this function also does not need to be modified for
** this lab.
*/
void main(argc, argv)
int argc;
char **argv;
{
	unsigned long stage, cycles;

	if (argc < 2) {
		printf("ERROR:  %s sparc.prg\n", argv[0]);
		exit(1);
	}

	load_instr("sparc.ins");
	load_prog(argv[1]);
	NPC = PC = 0;

	/* first cycle */
	fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));
	stage = NEXT_PIPESTAGE(stage);
	cycles++;       

	/* second cycle */
	decode(NEXT_PIPESTAGE(stage));
	fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));
	stage = NEXT_PIPESTAGE(stage);
	cycles++;

	/* all remaining cycles to program termination */
	while( INSTR_restore != instr_label[pipeline_Q[stage].i]){
		Execute(stage);
		decode(NEXT_PIPESTAGE(stage));
		fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));

		stage = NEXT_PIPESTAGE(stage);
		registers[0] = 0;
		cycles++;
	}
	print_regs();
	printf("Total cycles: %d\n",cycles);
}

