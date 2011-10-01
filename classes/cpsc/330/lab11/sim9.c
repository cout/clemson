/* sparcsim.c */

#include <stdio.h>
#include "sparc.h"
#include <strings.h>

/*
** Variables needed for a direct mapped cache.
*/
#define NUM_CACHE_LINES 4 
#define NUM_WORDS_PER_LINE 4
#define TAG_MASK 0xFFFFFFFC
#define WORD_MASK 0x00000003
#define MAX_CYCLES 10000
#define MISS_PENALTY 20
#define HIT_PENALTY 1 
unsigned long cache[NUM_CACHE_LINES][NUM_WORDS_PER_LINE];
unsigned long int tags[NUM_CACHE_LINES];
unsigned long int mem_count[NUM_CACHE_LINES];
unsigned long int valid_bits;
unsigned long int cache_hits;
unsigned long int cache_misses;
#define IS_VALID(x) ((valid_bits >> x) & 0x00000001)

/*
** The following variables provide information about those
** instructions that have been implemented.
*/
char instr_format[100];
char instr_label[100];
char instrname[100][10];
unsigned long instrvalu[100];
unsigned long instrmask[100];
unsigned long instrcnt[100];
int num_instr=0;

/*
** Miscellaneous variables...
*/
int num_instr_loaded=0;
int toti = 0;
unsigned long rammem[4096];
int num_stalls = 0;

/*
** The register file.
*/ 
unsigned long registers[32];

/*
** Some other processer state information
*/
int cond_zero;
int cond_negative;
unsigned long int ALUout;
int PC = 0, NPC = 0;


/*
** Pipeline variables, definitions, etc.
*/
#define NUM_STAGES 4 
#define NEXT_PIPESTAGE(x)  (((x)+1) % NUM_STAGES)
#define PREV_PIPESTAGE(x)  (((x)==0)?(NUM_STAGES-1):((x)-1))
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
void execute_branch();

/*
** Init_Cache - this function initialized the cache and all
** cache variables.  It should be called once at the beginning
** of main.
*/
void Init_Cache(){
	int i,j;

	for(i = 0; i < NUM_CACHE_LINES; i++){
		for(j = 0; j < NUM_WORDS_PER_LINE; j++)
			cache[i][j] = 0; 
		tags[i] = 0;
		mem_count[i] = 0;
	}
	valid_bits = 0;
	cache_hits = cache_misses = 0;
}

/*
** LRU_Access - this takes care of the LRU bookkeeping.
*/
void LRU_Access(unsigned long int cache_line){
	int i;
	for(i = 0; i < NUM_CACHE_LINES; i++){
		mem_count[i]++;
	}	
	valid_bits |= (0x00000001 << cache_line);
	mem_count[cache_line] = 0;
}

/*
** Read_Cache - this function reads a word from the cache, and
** appropriately evicts a line on a miss, if the cache is full.
*/
unsigned long int Read_Cache(unsigned long int address){
	int i,j;
	int invalid_line = -1;
	int victim = 0;
	unsigned long int word;
	unsigned long int block;

	block = address & TAG_MASK;
	word = address & WORD_MASK;
	
	for(i = 0; i < NUM_CACHE_LINES; i++){
		if( !IS_VALID(i) ){
			invalid_line = i;
		}
		else{
			if(block == tags[i])
				break;	
		}
	}
	if(i == NUM_CACHE_LINES){
		/* cache miss -- find a victim */
		cache_misses++;
		if(invalid_line != -1){
			for(j = 0; j < NUM_WORDS_PER_LINE; j++){
				cache[invalid_line][j] = rammem[block + j];
			}
			tags[invalid_line] = block;
			LRU_Access(invalid_line);
			return cache[invalid_line][word];
		}
		else{
			for(j = 0; j < NUM_CACHE_LINES; j++){
				if(mem_count[j] > mem_count[victim]){
					victim = j;	
				}
			}
			for(j = 0; j < NUM_WORDS_PER_LINE; j++){
				cache[victim][j] = rammem[block + j];
			}
			tags[victim] = block;
			LRU_Access(victim);
			return cache[victim][word];
		}
	}
	else{
		/* cache hit - just write the puppy out */
		cache_hits++;
		LRU_Access(i);
		return cache[i][word];
	}
}

/*
** Write_Cache - this writes a word to the cache, and also writes
** the value out to memory.
*/
void Write_Cache(unsigned long int address, unsigned long int data){
	int i,j;
	int invalid_line = -1;
	int victim = 0;
	unsigned long int word;
	unsigned long int block;

	block = address & TAG_MASK;
	word = address & WORD_MASK;
	
	for(i = 0; i < NUM_CACHE_LINES; i++){
		if( !IS_VALID(i) ){
			invalid_line = i;
		}
		else{
			if(block == tags[i])
				break;	
		}
	}
	if(i == NUM_CACHE_LINES){
		/* cache miss -- find a victim */
		cache_misses++;
		if(invalid_line != -1){
			for(j = 0; j < NUM_WORDS_PER_LINE; j++){
				cache[invalid_line][j] = rammem[block + j];
			}
			tags[invalid_line] = block;
			LRU_Access(invalid_line);
			rammem[block + word] = data;
			cache[invalid_line][word] = data;
		}
		else{
			for(j = 0; j < NUM_CACHE_LINES; j++){
				if(mem_count[j] > mem_count[victim]){
					victim = j;	
				}
			}
			for(j = 0; j < NUM_WORDS_PER_LINE; j++){
				cache[victim][j] = rammem[block + j];
			}
			tags[victim] = block;
			LRU_Access(victim);
			rammem[block + word] = data;
			cache[victim][word] = data;
		}
	}
	else{
		/* cache hit - just write the puppy out */
		cache_hits++;
		LRU_Access(i);
		rammem[block + word] = data;
		cache[i][word] = data;
	}
}

/*
** load_instr: this loads instruction information from the file
** specified as the argument.
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
** load_prog: this loads the program into rammem.
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
    /* printf(linebuf); 
    printf("\n"); */
    sscanf(linebuf, "%x", &instr);
    rammem[i++] = instr;
    fgets(linebuf,99,f);
  }
  
  fclose(f);
  num_instr_loaded = i;
  /* printf("\n\n"); */
  
}

/*
** fetch - this function fetches an instruction from memory.  
** It now correctly handles stalling.
*/
void fetch(int stage_number) {
	static unsigned long int instr;
	static unsigned long rsrc1;
	static unsigned long rsrc2;
	Qelement *S = pipeline_Q + PREV_PIPESTAGE(stage_number);
	static int stalled = 0;

	if(!stalled){
		instr = Read_Cache(NPC);
		rsrc1 = (instr >> 14) & 0x0000001f;
		rsrc2 = instr & 0x0000001f;
	}
	else stalled = 0;

	PC = NPC;
	if((S->rdst == 0)||((S->rdst != rsrc1)&&(S->rdst != rsrc2))){
		NPC++;	
		pipeline_Q[stage_number].instr = instr;
	} 
	else{
		/* this issues a NOP to create the stall */
		pipeline_Q[stage_number].instr = 0x01000000;
		/* printf(".....Stall......\n"); */
		num_stalls++;
		stalled = 1;
	} 
}

/*
** decode - this function decodes fetched instruction in the
** pipestage identified in the argument.
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
	 exit(1);
   } else {

 /*   printf("%s ", instrname[i], local.rdst); 

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
	}*/
	printf("."); fflush(stdout); 
	
    instrcnt[i]++;           /* How many times that instr occurred */
    toti++;
 
   }
	pipeline_Q[stage_number] = local;
}

/*
**	Execute - this function executes the instruction in the
**  pipestage which is passed as an argument.
*/
void Execute(int stage_number){
	int instruction_number;
	Qelement *S = pipeline_Q + stage_number;

	instruction_number = pipeline_Q[stage_number].i;
	  /* Execute the code here */
		switch(instr_label[instruction_number]){
			case INSTR_ldsb:  /* load signed byte */
				ALUout = (Read_Cache(S->effaddr) & 0xff000000) / (256*256*256);
			break;
			case INSTR_ldsh:  /* load signed halfword */
				ALUout = (Read_Cache(S->effaddr) & 0xffff0000) / (256*256);
			break;
			case INSTR_ld:   /* load */
				ALUout = Read_Cache(S->effaddr);
			break;
			case INSTR_ldub:	/* load unsigned byte */
				ALUout = (Read_Cache(S->effaddr) & 0xff000000) >> 24;
			break;
			case INSTR_lduh:	/* load unsigned half-word */
				ALUout = (Read_Cache(S->effaddr) & 0xffff0000) >> 16;
			break;
			case INSTR_st:		/* store */
				Write_Cache(S->effaddr, registers[S->rdst]);
				S->rdst = 0;
			break;
			case INSTR_stb:		/* store byte */
				Write_Cache(S->effaddr, registers[S->rdst] << 24);
				S->rdst = 0;
			break;
			case INSTR_sth:		/* store halfword */
				Write_Cache(S->effaddr,registers[S->rdst] << 16);
				S->rdst = 0;
			break;
			case INSTR_std:		/* store a double word */
				Write_Cache(S->effaddr, registers[S->rdst]);
				Write_Cache(S->effaddr+1, registers[S->rdst+1]);
				S->rdst = 0;
			break;
			case INSTR_swap:	/* swap register with memory */
				{
					unsigned long int tmp;
					tmp = registers[S->rdst];
					ALUout = Read_Cache(S->effaddr);
					Write_Cache(S->effaddr, tmp);
				}
			break;
			case INSTR_sethi:	/* set high-order 22 bits of rd */
				ALUout = S->disp22 << 10;
			break;
			case INSTR_or:		/* or; set condition codes on S bit */
				ALUout = registers[S->rsrc1] | 
					((S->reg_or_imm)?(S->simm13):(registers[S->rsrc2]));
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
				ALUout = registers[S->rsrc1] & 
					((S->reg_or_imm)?(S->simm13):(registers[S->rsrc2]));
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
			case INSTR_sll:
				ALUout = (registers[S->rsrc1] << 
					((S->reg_or_imm)?(S->simm13):(registers[S->rsrc2]))); 
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
				printf("\nUnimplemented instruction: %s\n",
					instrname[instruction_number]);
			break;
			
		}
}

/*
**	Execute_branch - this tests the condition codes to determine
**  whether or not a branch should be taken.
*/
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
		case INSTRC_bg:
		do_branch = !(cond_negative | cond_zero);	
		break;
		case INSTRC_bge:
		do_branch = !cond_negative;
		break;
		default:
		printf("\nUnimplemented branch condition: 0x%08x\n",S->cond);
	}
	if(do_branch){
		NPC += S->branchaddr;
		NPC -= 2;
	}else if(S->anull){
		NPC++;  /* skip past the next instruction */
	}
}

/*
** Write - this carries out the write stage of the pipeline for
** the appropriate pipe stage.
*/
void Write(int stage_number){
	Qelement *S = pipeline_Q + stage_number;

	registers[S->rdst] = ALUout;

}

/*
** print_regs - this just prints out the entire register file.
*/
void print_regs(){
int i;
	printf("\n");
	for(i = 0; i < 16; i++){
		printf("R%-2d: 0x%08x\t\tR%-2d: 0x%08x\n",i,
				registers[i],i+16,registers[i+16]);
	}
}

/*
** main...
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
		Init_Cache(); 
		registers[30] = 0x00000800;  /* sets frame pointer */
	
		printf("Executing"); fflush(stdout);
	
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

			if(cycles > MAX_CYCLES){
				printf("Maximum allowed cycles exceeded.\n");
				printf("You are probably stuck in an infinite loop.\n");
				exit(1);
			}
		}
		Write(stage);
		registers[0] = 0;	
		Execute(NEXT_PIPESTAGE(stage));
		decode(NEXT_PIPESTAGE(NEXT_PIPESTAGE(stage)));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		Write(stage);
		registers[0] = 0;	
		Execute(NEXT_PIPESTAGE(stage));
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		Write(stage);
		registers[0] = 0;	
		stage = NEXT_PIPESTAGE(stage);
		cycles++;

		print_regs();
		cycles += (MISS_PENALTY*cache_misses) + (HIT_PENALTY*cache_hits);
		printf("Total cycles: %d\n",cycles);
		printf("Total hits: %d, Total misses: %d\n",cache_hits,	
			cache_misses);
		printf("Number of stalls: %d\n",num_stalls);
	}
}

