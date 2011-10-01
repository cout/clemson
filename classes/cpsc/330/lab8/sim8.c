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
** 
*/
int num_instr_loaded=0;
int toti = 0;
unsigned long rammem[2000];
int PC = 0, NPC = 0;

/*
** Branching variables (with condition codes) defined in Lab 5.
*/
int cond_zero;
int cond_negative;

/*
** The register file.
*/ 
unsigned long registers[32];
unsigned long ALUout;

/*
** Pipeline variables, definitions, etc.
*/
#define NUM_STAGES 4
#define NEXT_PIPESTAGE(x)  (((x)+1) % NUM_STAGES)
#define PREV_PIPESTAGE(x)  ((x==0)?(NUM_STAGES-1):(((x)-1) % NUM_STAGES))
typedef struct Qelement Qelement;
struct Qelement{
  int i;		/* which instruction is this of our ~30 */
  unsigned long instr;	/* the undecoded instruction */
  long rdst;		/* Destination register, from lab 2 */
  long rsrc1;		/* Source register 1 */
  long rsrc2;		/* Source register 2 */
  long simm13;		/* 13-bit signed immediate for format 3c */
  long reg_or_imm;	/* is the second source a register or immediate? */
  long cond;		/* Condition bits */
  long disp22;		/* 22-bit displacement for format 2 */
  long disp30;		/* 30-bit displacement for format 1 */
  long effaddr;		/* effective address */
  long branchaddr;	/* branch address */
  long calladdr;	/* call address */
  int anull;
  int sbit;
};
Qelement pipeline_Q[NUM_STAGES];

/* prototype */
void execute_branch(int stage_number);

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
** fetch - this function fetches an instruction from memory.  
*/
void fetch(int stage_number) {
	Qelement *S = pipeline_Q + PREV_PIPESTAGE(stage_number);
	unsigned long L_instr = rammem[(PC = NPC)];
	long L_rsrc1 = (L_instr >> 14) & 0x0000001f;
	long L_rsrc2 = L_instr & 0x0000001f;

	if (S->rdst == 0 || (S->rdst != L_rsrc1 && S->rdst != L_rsrc2)) {
		NPC++;
		pipeline_Q[stage_number].instr  = L_instr;
	} else {
		pipeline_Q[stage_number].instr = 0x01000000;
	}
}

/*
** decode - this function decodes a fetched instruction. 
*/
void decode (int stage_number)
{ 
   int i;
   int donegate;
	Qelement local;
	local.instr = pipeline_Q[stage_number].instr;

   local.rdst = (local.instr >> 25) & 0x0000001f; 
   local.rsrc1 = (local.instr >> 14) & 0x0000001f;
   local.rsrc2 = local.instr & 0x0000001f;

	local.simm13 = local.instr & 0x00001fff;
	local.reg_or_imm = (local.instr >> 13) & 0x00000001;
	local.cond = (local.instr >> 25) & 0x0000000f;	
	local.disp22 = local.instr & 0x003fffff;
	local.disp30 = local.instr & 0x3fffffff;
	local.anull = (local.instr >> 29) & 0x00000001;
	local.sbit = (local.instr >> 23) & 0x00000001;

   for (i=0; (local.instr & instrmask[i])
		!=instrvalu[i] && i<num_instr; i++)
	; /* do nothing */ 

	local.i = i;

	/* Compute the effective address or alu operation */
	donegate = (instr_label[i]==INSTR_sub || instr_label[i]==INSTR_subx);
	if(local.reg_or_imm){ 
		if(local.simm13 >> 12){ 
			if(local.rsrc1 != 0){
				if(donegate){
					local.effaddr = registers[local.rsrc1] - (0xffffe000 | local.simm13);
				} else {
					local.effaddr = registers[local.rsrc1] + (0xffffe000 | local.simm13);
				}
			} else {
				local.effaddr = 0xffffe000 | local.simm13;
			}
		} else {	/* displacement is postive */
			if(local.rsrc1 != 0){
				if (donegate){
					local.effaddr = registers[local.rsrc1] - local.simm13;
				} else {
					local.effaddr = registers[local.rsrc1] + local.simm13;
				}
			} else {
				local.effaddr = local.simm13;	
			}	
		}
	} else {	/* second operand is a register */
		if(donegate)
			local.effaddr = registers[local.rsrc1] - registers[local.rsrc2];
		else
			local.effaddr = registers[local.rsrc1] + registers[local.rsrc2];
	}	

	/* branch address */
	if (local.disp22 >> 21) {
		local.branchaddr = 0xffc00000 | local.disp22;
	}else{
		local.branchaddr = local.disp22;
	}
	
	/* Call address */
	local.calladdr = (local.disp30 << 2);


   if (i==num_instr) {
     printf("ERROR: could not id opcode: %lx\n", local.instr);
   } else {

    printf("%s ", instrname[i], local.rdst);

	switch(instr_format[i]){
	case FORMAT1:
		printf("0x%08X\n",local.disp30);
		break;
	case FORMAT2:
		if(instr_label[i] == INSTR_sethi)
			printf("0x%08X, R%d\n",local.disp22,local.rdst);
		else
			printf("0x%08X, 0x%08X\n",local.cond,local.disp22);
		break;
	case FORMAT3:
		if(local.reg_or_imm){
			printf("R%d R%d 0x%08X\n",local.rdst,local.rsrc1,local.simm13);
		}else{
			printf("R%d R%d R%d\n",local.rdst,local.rsrc1,local.rsrc2);
		}
		break;
	}
 
    instrcnt[i]++;           /* How many times that instr occurred */
    toti++;
 
   }
	pipeline_Q[stage_number] = local;
}

/*
**	Execute - this function needs to be filled in.
*/
void Execute(int stage_number){
	int instruction_number;
	Qelement *S = pipeline_Q + stage_number;

	instruction_number = pipeline_Q[stage_number].i;
	  /* Execute the code here */
		switch(instr_label[instruction_number]){
			case INSTR_ldsb:  /* load signed byte */
				ALUout = (rammem[S->effaddr] & 0xff000000) / (256*256*256);
			break;
			case INSTR_ldsh:  /* load signed halfword */
				ALUout = (rammem[S->effaddr] & 0xffff0000) / (256*256);
			break;
			case INSTR_ld:   /* load */
				ALUout = rammem[S->effaddr];
			break;
			case INSTR_ldub:	/* load unsigned byte */
				ALUout = (rammem[S->effaddr] & 0xff000000) >> 24;
			break;
			case INSTR_lduh:	/* load unsigned half-word */
				ALUout = (rammem[S->effaddr] & 0xffff0000) >> 16;
			break;
/*			case INSTR_ldd:		/* load doubleword */
/*				registers[S->rdst] = rammem[S->effaddr];
				registers[S->rdst+1] = rammem[S->effaddr+1];
			break;*/
			case INSTR_st:		/* store */
				rammem[S->effaddr] = registers[S->rdst];
				S->rdst = 0;
			break;
			case INSTR_stb:		/* store byte */
				rammem[S->effaddr] = registers[S->rdst] << 24;
				S->rdst = 0;
			break;
			case INSTR_sth:		/* store halfword */
				rammem[S->effaddr] = registers[S->rdst] << 16;
				S->rdst = 0;
			break;
			case INSTR_std:		/* store a double word */
				rammem[S->effaddr] = registers[S->rdst];
				rammem[S->effaddr+1] = registers[S->rdst+1];
				S->rdst = 0;
			break;
			case INSTR_swap:	/* swap register with memory */
				{
					unsigned long int tmp;
					tmp = registers[S->rdst];
					ALUout = rammem[S->effaddr];
					rammem[S->effaddr] = tmp;
				}
			break;
			case INSTR_sethi:	/* set high-order 22 bits of rd */
				ALUout = S->disp22 << 10;
			break;
			case INSTR_or:		/* or; set condition codes on S bit */
				ALUout = registers[S->rsrc1] | 
					((S->reg_or_imm)?(S->simm13):(ALUout));
				if(S->sbit){
					cond_negative = (ALUout >> 31);
					cond_zero = (ALUout == 0);
				}
			break;
			case INSTR_add:		/* add; set condition codes on S bit */
				ALUout = S->effaddr; 
				if(S->sbit){
					cond_negative = (ALUout >> 31);
					cond_zero = (ALUout == 0);
				}
			break;
			case INSTR_sub:
				ALUout = S->effaddr;
				if(S->sbit){
					cond_negative = (ALUout >> 31);
					cond_zero = (ALUout == 0);
				}
			break;
			case INSTR_and:
				ALUout = registers[S->rsrc1] & ((S->reg_or_imm)?(S->simm13):(registers[S->rsrc2]));
				if(S->sbit){
					cond_negative = (ALUout >> 31);
					cond_zero = (ALUout == 0);
				}
			break;
			case INSTR_andn:
				ALUout = ~(registers[S->rsrc1] &
					((S->reg_or_imm)?(S->simm13):(registers[S->rsrc2])));
				if(S->sbit){
					cond_negative = (ALUout >> 31);
					cond_zero = (ALUout == 0);
				}
			break;
			case INSTR_orn:
				ALUout = ~(registers[S->rsrc1] |
					((S->reg_or_imm)?(S->simm13):(registers[S->rsrc2])));
				if(S->sbit){
					cond_negative = (ALUout >> 31);
					cond_zero = (ALUout == 0);
				}
			break;
			case INSTR_bcc:
				execute_branch(stage_number);
				S->rdst = 0;
			break;
			default:
				printf("Unimplemented instruction: %s\n",
					instrname[instruction_number]);
			break;
			
		}
}

void Write(int stage_number) {
	registers[pipeline_Q[stage_number].rdst] = ALUout;
}

void execute_branch(int stage_number){
	int do_branch;
	Qelement *S = pipeline_Q + stage_number;

	do_branch = 0;

	switch(S->cond){
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
		NPC += S->branchaddr;
		NPC -= 2;
	}else if(S->anull){
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
** main 
*/
void main(argc, argv)
int argc;
char **argv;
{
	int stage=0, cycles = 0;

	if (argc < 2) {
      printf("ERROR:  %s sparc.prg\n", argv[0]);
    }
    else {
    	load_instr("sparc.ins");
    	load_prog(argv[1]);
		NPC = PC = 0;
	
		/* first cycle */
		fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage))));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		/* second cycle */
		decode(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));
		fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage))));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		/* third cycle */
		Execute(NEXT_PIPESTAGE(stage));
		decode(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));
		fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage))));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		/* all remaining cycles */
		while (INSTR_jmpl != 
				instr_label[pipeline_Q[NEXT_PIPESTAGE(stage)].i]) {
			Write(stage);
			registers[0] = 0;
			Execute(NEXT_PIPESTAGE(stage));
			decode(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));
			fetch(NEXT_PIPESTAGE(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage))));
	
			stage = NEXT_PIPESTAGE(stage);
			cycles++;
		}

		/* the third to last cycle */
		Write(stage);
		registers[0] = 0;
		Execute(NEXT_PIPESTAGE(stage));
		decode((NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage))));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		/* the next to last cycle */
		Write(stage);
		registers[0] = 0;
		Execute(NEXT_PIPESTAGE(stage));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		/* the last cycle */
		Write(stage);
		registers[0] = 0;
		cycles++;

		print_regs();
		printf("Total cycles: %d\n",cycles);
	}
}

