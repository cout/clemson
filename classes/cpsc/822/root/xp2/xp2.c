/*
 * xpsc.c:  petri net simulator
 * 
 * Rev 2.0  -RMG, Columbus day, '93
syntax:

xpsc [-r -v noise_level -t simulation_time -i input_file] 

-r call interactive file reviewer before execution; default is don't
-v verbose flag; default is 2
	2: yakity yak
	1: don't talk back
	0: shut up 
-t simulated time: default is 0
-i input file: default is stdin
		

net specifcation language:

p <place name> <intial token count> [list of token colors]
t <trans. name> <fire time distrib.> [prmtrs] <group name> <group corr.>
   <color fct.> [prmtrs] <number of enabling restrictions> 
   [place color place color ...]
a <place name> <transition name> <inhibitor | no>
a <transition name> <place name> <probabilistic | no> [branch prob.]
o <place name | transition name>
s <place name> <filename>
% comment
d

notes: 

1) if transition firing time distribution is chosen to be "subnet",
the one parameter called for is the subnet name.  If this subnet
has not yet been specified, it's specification must be done
immediately, i.e. prior to <group name>

2) special group names "halt" and "restart" are recognized; a good
convention: use trans. name for group name, if transition is 
independent of all others.  Correlation is real in range -1.0 to 1.0.

3) There are several semantic difficulties associated with simultaneously
enabled conflicting transitions when an enabling place contains multiple
tokens.  We now disable (for the instant) all concurrently enabled 
conflicting transitions that were not disabled by having a place emptied.  
They are automatically re-enabled after 0 time with a new sample from 
the firing time distribution.  This way, we can count on the enabling list 
still being correct at firing time, so we remove the correct colors from 
the correct places.

4) We've also added "place   color" pairs as enabling restrictions
attached to transitions.  Multiple restrictions on the same place
are allowed; the effect is the logical & of the restrictions with
the default, which is "any color".  Named places from which inhibitor
arcs emanate must NOT contain the named color ... other colors ok.

Magic negative number colors are also recognized as restrictions:

placename -0x24_bit_color

All other input places to this transition must enable with colors
that match the color grabbed from "placename", but they need only match
on the "on" bits of the "24_bit_color", e.g. -ffffff means match 
all bits, and -ff means match mod 256, etc.  Places from inhibitor 
arcs must NOT match.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <malloc.h>
#include <errno.h>
#include <asm/unistd.h>

_syscall1(int, turbo, int, pid);
int turbo_on = 0;

/*
define this to disable re-sampling by conflicting 
transitions when one of them fires
#define D_AND_D 1
*/

/*
 * Macros
 */
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

/*
 * Define Null pointers
 */
#define PNIL ((struct place *)0)
#define TNIL ((struct trans *)0)
#define ANIL ((struct arc *)0)
#define NNIL ((struct net *)0)
#define TOKENNIL ((struct token *)0)
#define ENIL ((struct color_enable *)0)

/*
 * Arc constants
 */
#define STD_ARC 0
#define INHIB_ARC 1
#define BIFUR_ARC 2

/*
 * Resources
 */
#define NPLACES 1000
#define NTRANS 1000
#define NARCS 3000
#define NNETS 30
#define NTOKENS 115000
#define NENABLES 500

#define MAX_NAME_LEN 170
#define NOTIFY_INTERVAL 2000000

struct net {
	char name[MAX_NAME_LEN];
	struct place *plist;
	struct trans *tlist;
	struct net *next;
	} net[NNETS];

struct token {
	int color;
	int arrival;
	struct token *next;
	} token[NTOKENS];
	
struct color_enable {
	int color;
	char placename[MAX_NAME_LEN];
	struct color_enable *next;
	} color_enable[NENABLES];

struct place {
	char name[MAX_NAME_LEN];
	int token_count;
	int orig_token;
	struct token *tokenlist_front;
	struct token *tokenlist_rear;
	struct token *origlist;
	struct place *next;
	int output;
	int distrib_index;
	int total_visit_time;
	int num_visitors;
	int util_start;
	int util_total;
	} place[NPLACES];

#define MAX_PARMS 6
#define MAX_PARM_LEN 64
/* 
This is a cute switcheroo to make parameter passing easy;
wherever you see PARMLIST(i) think parmlist[i], and you'll
be happy.
*/
#define PARMLIST(i) ((char *)(parmlist + MAX_PARM_LEN*i))

struct trans {
	char name[MAX_NAME_LEN];
	int (*rand_ftime)();
	char dparameter[MAX_PARMS][MAX_PARM_LEN];
	int rttf;
	struct arc *inlist;
	struct arc *outlist;
	int enabled;
	struct color_enable *enabling;
	struct color_enable *restrict;
	struct trans *next;
	char groupname[MAX_NAME_LEN];
	double correlation;
	double lastrand; 
	int (*color_out)();
	char cparameter[MAX_PARMS][MAX_PARM_LEN];
	int output;
	int firecount;
	} trans[NTRANS];

struct arc {
	struct place *pptr;
	struct trans *tptr;
	int inhibifur;
	double bifur_prob;
	struct arc *next;  /* link of arcs in transition inlist or outlist */
	} arc[NARCS];

struct place *pfreelist;
struct trans *tfreelist;
struct arc *afreelist;
struct net *nfreelist, *nlist;
struct token *tokenfreelist;
struct color_enable *enablefreelist;

struct place *pfind();
struct trans *tfind();
struct net *nfind();
struct token *toefind();
struct token *toefind_bitmatch();
struct place *getplace();
struct trans *gettrans();
struct arc *getarc();
struct net *getnet();
struct token *gettoken();
struct color_enable *getenable();


/*
 * transition firing time distributions
 */

int instant();
int constnt();
int unif();
int expo();
int empirical();
int run();
int diskseek();
int expold();

struct dstrb {
	char *name;
	int (*fction)();
	int paramcount;
	};

struct dstrb distrib[] = {
"instantaneous",instant,0,
"constant",constnt,1,
"uniform",unif,1,
"exponential",expo,1,
"empirical_from_file",empirical,1,
"subnet",run,1,
"color_distance",diskseek,5,
"expo_load_dep",expold,2,
0};


/*
 *  color output functions
 */
int black();
int constant_color();
int myselect();
int markov();
int color_io();
int color_stream();
int usercolor();

struct dstrb cdistrib[] = {
"black",black,0,
"constant_color",constant_color,1, 		/* param is the color to be output */
"from_selected_place",myselect,1,
"color_markov_from_file",markov,2,     		/* params are filename, placename */
"color_io_from_file",color_io,2,
"color_stream_from_file",color_stream,1,   	/* parameter is filename */
"user_defined",usercolor,0,
0};


#define MAXTTF 1000000

double correl_rn();
double genrand();
double cgenrand();


/*
 * empirical distribution files
 */
#define MAX_NUM_HISTS 2
#define MAX_HIST 30

struct histcollection {
	int loaded;
	int size;
	struct emp {
		double prob;
		int value;
		} hist[MAX_HIST];
	} histcollection[MAX_NUM_HISTS];

/*
 * color Markov from file 
 */
#define MAX_NUM_MARKOVS 1
#define MAX_MARKOV 44

struct markovcollection {
	int loaded;
	int size;
	float nextstate[MAX_MARKOV*MAX_MARKOV];
	} markovcollection[MAX_NUM_MARKOVS];


/*
 * color I/O from file
 */
#define MAX_NUM_COLORIOS 2
#define MAX_COLORMAP 1760

struct coloriocollection {
	int loaded;
	int size;
	int colormapstate[MAX_COLORMAP];
	} coloriocollection[MAX_NUM_COLORIOS];


/*
 * color stream from file
 */
#define MAX_NUM_COLORSTREAMS 2
#define BUFSIZE 4096

struct colorstreamcollection {
	int open;
	int fd;
	int buffer[BUFSIZE];
	int buff_pos;
	int bytes_read;
	} colorstreamcollection[MAX_NUM_COLORSTREAMS];


/*
 * Place distribution recording stuff
 */
#define MAX_PLACES_DIST 10
#define MAX_DISTRIBUTION_TIME 500

struct distributions {
	int dist[MAX_DISTRIBUTION_TIME];
	char outfile[100];
	} place_dist[MAX_PLACES_DIST];

/*
 *  Some global variables
 */
int maxtime;
int verbose;
int review_input;
char input_file[MAX_NAME_LEN];

main(argc,argv)
int argc;
char **argv;
{
struct net *nptr;
int elapsedtime;

process_params(argc,argv,&review_input,&verbose,&maxtime,input_file);

initenable();
inittoken();
initplace();
inittrans();
initarc();
initnet();
initfilestructures();
srandom(123456789);

nlist=NNIL;
nptr=getnet();
sprintf(nptr->name,"main");
nptr->next = nlist;
nlist = nptr;

iinput(nptr,input_file);

if (review_input) interactive(nptr);

elapsedtime=run(0.0,ENIL,nptr->name);

printf("OUTPUT: runtime: %d\n",elapsedtime);
if (turbo_on) printf("Turbo Enabled!\n");
outputstats(elapsedtime,nptr);
}

int d_emp_index = 0;
int c_markov_index = 0;
int c_io_index = 0;
int c_stream_index = 0;
int place_distrib_index = 0;

/*
 * Input petri-net specification
 **********/
iinput(nptr,filename)
struct net *nptr;
char *filename;
{
struct place *pptr;
struct trans *tptr, *tptr2;
struct arc *aptr;
struct net *nptr2;
struct token *tokenptr1, *tokenptr2;
struct color_enable *egob;
char buf[260];
int toks,i;
int hwmany;
int color;
char c, b;
FILE *fp;

if(*filename) fp = fopen(filename,"r");
else fp = fdopen(0,"r");

c='z';
while(c!='d'){
	if(verbose == 2) fprintf(stderr,"INPUT: net %s: p, t, a, o, s, or d?\n",nptr->name);
	/*
	 * eat characters until a p,t,a,o, or d is input 
	 */
	fscanf(fp,"%c",&c);
	while(c!='p' && c!='t' && c!='a' && c!='o' && c!='d' && c!='%' && c!='s'){
		if(verbose == 2) fprintf(stderr,"INPUT: p, t, a, o, s, or d?\n");
		fscanf(fp,"%c",&c);
		}

	if (c != '%') fscanf(fp,"%*c");
	switch(c){
		case '%': 
			/*
			 * Comment, so eat characters until end of line
			 */
			b = c;
			while (b !='\n') fscanf(fp,"%c",&b);
			break;

		case 'p':
			/*
			 * Place.  Need name, token count, colors 
			 */
			pptr=getplace();
			if(verbose == 2) fprintf(stderr,"INPUT: place name:\n");
			sprintf(buf,"                   ");
			fscanf(fp,"%s",buf);
			fscanf(fp,"%*c");
			sprintf(pptr->name,buf);
			if(verbose == 2) fprintf(stderr,"INPUT: initial token count:\n");
			fscanf(fp,"%d",&toks);
			fscanf(fp,"%*c");
			pptr->token_count=toks;
			pptr->orig_token=toks;
			while(toks>0){
				tokenptr1=gettoken();
				tokenptr2=gettoken();
				if(verbose == 2) fprintf(stderr,"INPUT: token color (in hex):\n");
				fscanf(fp,"%x",&color);
				fscanf(fp,"%*c");

				/* need copies for reset */

				tokenptr1->color = color;
				tokenptr1->next = TOKENNIL;
				tokenptr2->color = color;
				/* maintain origlist in LIFO ...others FIFO */
				if(pptr->tokenlist_front==TOKENNIL){
					pptr->tokenlist_front = tokenptr1;
					}
				else{
					(pptr->tokenlist_rear)->next = tokenptr1;
					}
				pptr->tokenlist_rear = tokenptr1;
				tokenptr2->next = pptr->origlist;
				pptr->origlist = tokenptr2;
				toks--;
				}
			if (verbose == 2) dump_tokenlist(pptr);
			pptr->next = nptr->plist;
			nptr->plist = pptr;
			break;

		case 't':
			/*
			 * Transition.
	 		 */
			tptr=gettrans();
			if(verbose == 2) fprintf(stderr,"INPUT: transition name:\n");
			sprintf(buf,"                   ");
			fscanf(fp,"%s",buf);
			fscanf(fp,"%*c");
			sprintf(tptr->name,buf);
			if(verbose == 2) {
				fprintf(stderr,"INPUT: select distribution number:\n");
				i=0;
				while(distrib[i].name) {
					fprintf(stderr,"INPUT: %d %s\n",i,distrib[i].name);
					i++;
					}
				}
			fscanf(fp,"%d",&toks);
			fscanf(fp,"%*c");
			tptr->rand_ftime = distrib[toks].fction;
			if((hwmany=distrib[toks].paramcount)>0){
				if(verbose == 2) fprintf(stderr,"INPUT: %d parameters required\n",hwmany);

				i=0;
				while(i<hwmany){
					if(verbose == 2) fprintf(stderr,"INPUT: parameter %d:\n",i+1);
					fscanf(fp,"%s",tptr->dparameter[i]);
					fscanf(fp,"%*c");
					i++;
					}
				}
			/*
			make index to empirical file the last parameter
			*/
			if(strcmp(distrib[toks].name,"empirical")==0){
				if(d_emp_index == MAX_NUM_HISTS){
					fprintf(stderr,"out of histograms\n");
					exit(1);
					}
				else{
					sprintf(tptr->dparameter[i],"%d",d_emp_index);
					d_emp_index++;
					}
				}
			if(strcmp(distrib[toks].name,"subnet")==0){
				nptr2=nfind(tptr->dparameter[0]);
				if(nptr2==NNIL){
					if(verbose == 2) {
						fprintf(stderr,"INPUT: subnet %s ",tptr->dparameter[0]);
						fprintf(stderr,"INPUT: does not exist; specify now:\n");
						}
					nptr2=getnet();
					sprintf(nptr2->name,tptr->dparameter[0]);
					nptr2->next = nlist;
					nlist = nptr2;
					iinput(nptr2,0);
					if(verbose == 2) fprintf(stderr,"INPUT: return from subnet specification\n");
					}
				}
			if(verbose == 2) fprintf(stderr,"INPUT: group name:\n");
			sprintf(buf,"                   ");
			fscanf(fp,"%s",buf);
			fscanf(fp,"%*c");
			sprintf(tptr->groupname,buf);
			if(verbose == 2) fprintf(stderr,"INPUT: correlation to group (-1.0,1.0):\n");
			fscanf(fp,"%lf",&(tptr->correlation));
			if(verbose == 2) {
				fprintf(stderr,"INPUT: select color_out function number:\n");
				i=0;
				while(cdistrib[i].name) {
					fprintf(stderr,"INPUT: %d %s\n",i,cdistrib[i].name);
					i++;
					}
				}
			fscanf(fp,"%d",&toks);
			fscanf(fp,"%*c");
			tptr->color_out = cdistrib[toks].fction;
			if((hwmany=cdistrib[toks].paramcount)>0){
				if(verbose == 2) fprintf(stderr,"INPUT: %d parameters required\n",hwmany);
				i=0;
				while(i<hwmany){
					if(verbose == 2) fprintf(stderr,"INPUT: parameter %d:\n",i+1);
					fscanf(fp,"%s",tptr->cparameter[i]);
					fscanf(fp,"%*c");
					i++;
					}
				}
			/*
			use last parameter as index into file structure
			*/
			if(strcmp(cdistrib[toks].name,"color_markov_from_file")==0){
				if(c_markov_index==MAX_NUM_MARKOVS){
					fprintf(stderr,"out of markovs\n");
					exit(1);
					}
				else{
					sprintf(tptr->cparameter[i],"%d",c_markov_index);
					c_markov_index++;
					}
				}
			if(strcmp(cdistrib[toks].name,"color_io_from_file")==0){
				if(c_io_index==MAX_NUM_COLORIOS){
					fprintf(stderr,"out of color ios\n");
					exit(1);
					}
				else{
					sprintf(tptr->cparameter[i],"%d",c_io_index);
					c_io_index++;
					}
				}
			if(strcmp(cdistrib[toks].name,"color_stream_from_file")==0){
				if(c_stream_index==MAX_NUM_COLORSTREAMS){
					fprintf(stderr,"out of colorstreams\n");
					exit(1);
					}
				else{
					sprintf(tptr->cparameter[i],"%d",c_stream_index);
					c_stream_index++;
					}
				}
			if(verbose == 2) fprintf(stderr,"INPUT: number of enabling restrictions:\n");
			fscanf(fp,"%d",&toks);
			fscanf(fp,"%*c");
			while(toks--){
				egob = getenable();
				if(verbose == 2) fprintf(stderr,"INPUT: place name:\n");
				sprintf(buf,"                   ");
				fscanf(fp,"%s",buf);
				fscanf(fp,"%*c");
				sprintf(egob->placename,buf);
				if(verbose == 2) fprintf(stderr,"INPUT: required color");
				fscanf(fp,"%x",&(egob->color));
				fscanf(fp,"%*c");
				egob->next = tptr->restrict;
				tptr->restrict = egob;
				}

			tptr->next = nptr->tlist;
			nptr->tlist = tptr;
			break;

		case 'a':
			/*
			 * Arc.  From place/transition, to place/transition
			 */
			if(verbose == 2) fprintf(stderr,"INPUT: from (node name):\n");
			sprintf(buf,"                   ");
			fscanf(fp,"%s",buf);
			fscanf(fp,"%*c");
			if(pptr=pfind(nptr,buf)){
				if(verbose == 2) fprintf(stderr,"INPUT: to (transition name):\n");
				sprintf(buf,"                   ");
				fscanf(fp,"%s",buf);
				fscanf(fp,"%*c");
				if(tptr=tfind(nptr,buf)){
					/* link up */
					aptr=getarc();
					aptr->pptr = pptr;
					aptr->tptr = tptr;
					if(verbose == 2) fprintf(stderr,"INPUT: inhibitor arc (y/n)?\n");
					fscanf(fp,"%c",&c);
					while(c!='y' && c!='n'){
						if(verbose == 2) fprintf(stderr,"INPUT: inhibitor arc (y/n)?\n");
						fscanf(fp,"%c",&c);
						}
					fscanf(fp,"%*c");
					if(c=='y') aptr->inhibifur=INHIB_ARC;
					else aptr->inhibifur=STD_ARC;
					aptr->next = tptr->inlist;
					tptr->inlist = aptr;
					}
				else{
					fprintf(stderr,"INPUT: %s: no such transition\n",buf);
					exit(1);
					}
				}
			else{
				if(tptr=tfind(nptr,buf)){
					if(verbose == 2) fprintf(stderr,"INPUT: to (place name):\n");
					sprintf(buf,"                   ");
					fscanf(fp,"%s",buf);
					fscanf(fp,"%*c");
					if(pptr=pfind(nptr,buf)){
						/* link up */
						aptr=getarc();
						aptr->pptr = pptr;
						aptr->tptr = tptr;
						if(verbose == 2) fprintf(stderr,"INPUT: probabilistic arc (y/n)?\n");
						fscanf(fp,"%c",&c);
						while(c!='y' && c!='n'){
							if(verbose == 2) fprintf(stderr,"INPUT: probabilistic arc (y/n)?\n");
							fscanf(fp,"%c",&c);
							}
						fscanf(fp,"%*c");
						if(c=='y') {
							aptr->inhibifur=BIFUR_ARC;
							if(verbose == 2) fprintf(stderr,"INPUT:branching probability for this arc:\n");
							fscanf(fp,"%lf",&(aptr->bifur_prob));
							fscanf(fp,"%*c");
							} 
						else {
							aptr->inhibifur=STD_ARC;
							}
						aptr->next = tptr->outlist;
						tptr->outlist = aptr;
						}
					else{
						fprintf(stderr,"INPUT: %s: no such place\n",buf);
						exit(1);
						}
					}
				else{
					fprintf(stderr,"INPUT: %s: no such node in either list\n",buf);
					exit(1);
					}
				}
				
			break;
		case 'o':
			/*
			 * Output.  call for output of a place or a transition
			 */
			if(verbose == 2) fprintf(stderr,"INPUT: output for (node name):\n");
			sprintf(buf,"                   ");
			fscanf(fp,"%s",buf);
			fscanf(fp,"%*c");
			if(pptr=pfind(nptr,buf)) pptr->output=1;
			 else {
				if(tptr=tfind(nptr,buf)) tptr->output=1;
				else  fprintf(stderr,"ERROR: no such node %s\n",buf);
				}
			break;
		case 's':
			/*
			 * Output distribution of holding time for place
			 */
	      		if(verbose == 2) fprintf(stderr,"INPUT: output for (place name):\n");
			sprintf(buf,"                   ");
			fscanf(fp,"%s",buf);
			fscanf(fp,"%*c");
			if(pptr=pfind(nptr,buf)) {
				pptr->output=1; 
				pptr->distrib_index = place_distrib_index++;
				if(pptr->distrib_index >= MAX_PLACES_DIST) {
					fprintf(stderr,"ERROR: exceeded MAX_PLACES_DIST");
					exit(1);
					}
				}
			else {
				fprintf(stderr,"ERROR: no such place %s\n",buf);
				exit(1);
				}
			sprintf(buf,"                   ");
			fscanf(fp,"%s",place_dist[pptr->distrib_index].outfile);
			fscanf(fp,"%*c");

			break;
		case 'd':
			/*
			 * Done with net input.
			 */
			break;
		default:
			if(verbose == 2) fprintf(stderr,"ERROR: wise up\n");
		} /* switch (c) */
	} /* while (c != 'd')     'd' is end of net character */
} /* end of iinput() */

/*
 * Run petri-net or sub-net
 *
 * parmlist[0] is the name fo the petri net to be run. 
 * inseed and elist are not used.
 * elapsed time is returned.
 ******/
int 
run(inseed,elist,parmlist)
double inseed;
struct color_enable *elist;
char *parmlist;
{
int firecount;
int minttf;
struct trans *tptr;
struct trans *tptr2;
struct place *pptr;
int whichone,counter;
int clock, last_clock;
struct net *nptr;
double rnum;

nptr=nfind(PARMLIST(0));
if(nptr==NNIL){
	fprintf(stderr,"ERROR: no such net\n");
	exit(1);
	}
clock=last_clock=0;
while(clock<maxtime){
	/* 
	 * enable exactly those deserving
	 */
	firecount=0;
	minttf=MAXTTF;
	tptr = nptr->tlist;
	while(tptr!=TNIL){
		if(tptr->enabled==0){
			/* 
			 * try to build list of colors-places that 
			 * enable; this can fail due to color restrictions
			 * or missing tokens or tokens present
			 */
			if(buildenable(tptr)){
				/* skip most calls to correl_rn */
				if(tptr->correlation==0.0) rnum = genrand();
				else rnum = correl_rn(tptr,nptr);
				tptr->lastrand = rnum;
				tptr->enabled = 1;
#ifdef D_AND_D
				if (tptr->rttf == -1 ) 
#endif
				tptr->rttf = (*(tptr->rand_ftime))(rnum,tptr->enabling,tptr->dparameter);
				if(verbose) fprintf(stderr,"ENABLE: enabling %s at time %d\n",tptr->name,clock);
				}
			}
		if (tptr->enabled){
			if(tptr->rttf == minttf) firecount++;
			if(tptr->rttf < minttf){
				minttf = tptr->rttf;
				firecount=1;
     				}
			}
		tptr=tptr->next;
		}
	if(firecount<1){
		fprintf(stderr,"END: NET %s LOCKED\n",nptr->name);
		reset(nptr,clock);
		return(clock);
		}
	/*
	 * adjust all times 
	 */
	clock += minttf;
	if (clock-last_clock > NOTIFY_INTERVAL) {
		fprintf(stderr,"Current time: %d\n",clock);
		last_clock = clock;
		}
	tptr = nptr->tlist;
	while(tptr != TNIL){
		if(tptr->enabled) tptr->rttf -= minttf;
		tptr = tptr->next;
		}
	/*
	 * fire ONE of those eligible ... random choice
	 */

	whichone = (int)(genrand()*(double)(firecount)) + 1;
	tptr = nptr->tlist;
	counter=0;
	while(tptr!=TNIL){
		if(tptr->enabled && tptr->rttf==0) counter++;
		if(counter==whichone) break;
		tptr = tptr->next;
		}
	if(counter==whichone){
		if(verbose) fprintf(stderr,"FIRE: firing %s at local time %d\n",tptr->name,clock);
		fire(tptr,clock);
		tptr->enabled=0;
#ifdef D_AND_D
		tptr->rttf= -1;
#endif
		releaseenable(tptr);

		/* 
		 * disable, for the moment, conflicting enabled transitions 
		 */
		tptr2=nptr->tlist;
		while(tptr2 != TNIL){
			if(tptr2->enabled){
				if(share_inplace(tptr,tptr2)){
					tptr2->enabled=0;
					releaseenable(tptr2);
					}
				}
			tptr2 = tptr2->next;
			}

		if(strcmp(tptr->groupname,"halt")==0){
			reset(nptr,clock);
			printf("OUTPUT: Net halted by halt transition.\n");
			return(clock);
			}
		if(strcmp(tptr->groupname,"restart")==0){
			reset(nptr,clock);
			firecount=0;
			}
		}
	else{
		fprintf(stderr,"ERROR: oops firecount %d but no fire!\n",firecount);
		exit(1);
		}

	} /* while (clock < maxtime) */
/* timer maxes out: */
reset(nptr,clock);
return(clock);
}

/*
 *
 ************/
share_inplace(t1,t2)
struct trans *t1, *t2;
{
struct arc *aptr1, *aptr2;
aptr1 = t1->inlist;
while(aptr1!=ANIL){
	if((aptr1->inhibifur)==STD_ARC){
		aptr2 = t2->inlist;
		while(aptr2!=ANIL){
			if((aptr2->inhibifur)==STD_ARC){
				/* short-cut check ... should test names */
				if(aptr1->pptr == aptr2->pptr) return(1);
				}
			aptr2 = aptr2->next;
			}
		}
	aptr1 = aptr1->next;
	}
return(0);
}

/* 
 * build enabling list for the transition pointed to by
 * tptr, subject to restrictions.
 **********/
buildenable(tptr)
struct trans *tptr;
{
struct color_enable *egob;
struct color_enable *rstrct;
struct arc *aptr, *aptr2;
struct token *toeptr;
int found;
int magiccolor;

/* 
 * quick check of all incoming standard arcs for tokens 
 */
aptr = tptr->inlist;
while(aptr!=ANIL){
	if((aptr->pptr)->token_count==0 && (aptr->inhibifur)==STD_ARC ){
		tptr->enabled = 0;
		releaseenable(tptr);
		return(0);
		}
	aptr=aptr->next;
	}

/*
 * If a restriction has a color < 0, then it is a special "matching" restriction.
 * An example provides the best explanation: assume restriction is
 * "place37 -blue"
 * The first color in place37 is grabbed, call it "red".  Now, all OTHER input
 * places must match the color red, up to the 24-bit pattern in blue, e.g.
 * if -blue is -ffffff then match on all bits.
 * All places at head of incoming inhibitor arcs must NOT have a color that 
 * matches red up to bit pattern in blue.
 * 
 * We assume that a matching restriction, if it exists, is the only restriction
 * on that transition.
 */

rstrct = tptr->restrict;
if(rstrct != ENIL){
	/* first check for magic restriction */
	if(rstrct->color < 0){
		/* find the named place on the input list */ 
		aptr = tptr->inlist;
		while(aptr!=ANIL){
			if(strcmp(rstrct->placename,(aptr->pptr)->name)==0){
				/* found place; get color to be matched */
				magiccolor = ((aptr->pptr)->tokenlist_front)->color;

				/*
				 * now build enabling list matching this magic color 
				 * but inhibitors should NOT have this color 
				 */
				aptr2 = tptr->inlist;
				while(aptr2 != ANIL){
					toeptr = toefind_bitmatch((aptr2->pptr)->tokenlist_front,magiccolor,-(rstrct->color));
					if((aptr2->inhibifur==STD_ARC && toeptr==TOKENNIL)|| (aptr2->inhibifur==INHIB_ARC && toeptr!=TOKENNIL)){
						/* 
						 * color missing from where it should be or
						 * present where it shouldn't

						 */
						tptr->enabled = 0;
						releaseenable(tptr);
						return(0);
						}
					/*
					 * place is ok ... add to enabling list
					 */
					if(aptr2->inhibifur==STD_ARC){

						if (verbose) fprintf(stderr,"ENABLE: Adding place %s, token 0x%x to enabled list\n",(aptr2->pptr)->name,toeptr->color);

						egob = getenable();
						/* set to the color actually found */
						egob->color = toeptr->color;
						sprintf(egob->placename,(aptr2->pptr)->name);
						egob->next = tptr->enabling;
						tptr->enabling = egob;
						}
					aptr2 = aptr2->next;
					} /* end while (aptr2 != ANIL) */

				/* matched it ! */
				return(1);
				}
			aptr = aptr->next;
			}
		fprintf(stderr,"ERROR: oops can't find named place to match \n");
		exit(1);
		} /*  if(rstrct->color < 0) */
	} /* if (rstrct != ENIL) */

/* 
 * no magic matching restriction 
 * process ordinary restrictions
 * consider each input place ...
 */

aptr = tptr->inlist;
while(aptr!=ANIL){
	/* 
	 * check restriction list for this place; 
	 * check entire list since multiple ordinary restrictions legal
	 */
	found = 0;
	rstrct = tptr->restrict;
	while(rstrct != ENIL){
		if(strcmp(rstrct->placename,(aptr->pptr)->name)==0){
			/* 
			 * place is restricted ... must contain color
			 * rstrct->color if std, ... must not contain it
			 * if inhib; look for it on tokenlist
			 */
			toeptr = toefind((aptr->pptr)->tokenlist_front,rstrct->color);
			if((toeptr==TOKENNIL && aptr->inhibifur==STD_ARC)||(toeptr!=TOKENNIL && aptr->inhibifur==INHIB_ARC)){
				/* 
				 * color missing from where it should be or present
				 * where it shouldn't be
				 */
				releaseenable(tptr);
				tptr->enabled = 0;
				return(0);
				}
			/* restricted color found ... add to elist */
			if(aptr->inhibifur==STD_ARC){
				egob = getenable();
				egob->color = rstrct->color;
				sprintf(egob->placename,rstrct->placename);
				egob->next = tptr->enabling;
				tptr->enabling = egob;
				}
			found = 1;
			}
		rstrct = rstrct->next;
		}
				
	if(!found){
		/* place was unrestricted */
		if(aptr->inhibifur==INHIB_ARC && (aptr->pptr)->token_count>0){
			releaseenable(tptr);
			tptr->enabled = 0;
			return(0);
			}
		if(aptr->inhibifur==STD_ARC){
			egob = getenable();
			sprintf(egob->placename,(aptr->pptr)->name);
			egob->color = ((aptr->pptr)->tokenlist_front)->color;
			egob->next = tptr->enabling;
			tptr->enabling = egob;
			}
		}
	aptr = aptr->next;
	} 
return(1);
}


/*
 *
 ************/
releaseenable(tptr)
struct trans *tptr;
{
struct color_enable *egob;

while(tptr->enabling != ENIL){
	egob = tptr->enabling;
	tptr->enabling = (tptr->enabling)->next;
	returnenable(egob);
	}
}



/*
 * fire.
 *
 * remove tokens from incoming places, and put one in each outgoing place 
 ***************/
fire(trns,clock)
struct trans *trns;
int clock;
{
struct arc *aptr;
struct trans *tptr;
double sum_bifur;
double bi_prob;
double rshot;
int bi_fired;
struct token *toeptr;
struct color_enable *ce_ptr;
int tmp_color;
/*
 * Count the number of times this transition fires.
 */
(trns->firecount)++;

aptr=trns->inlist;
while(aptr!=ANIL){
	if(aptr->inhibifur!=INHIB_ARC){
		/*
		 * run down enabling list looking for instances of this place;
		 * for each instance, extract that color from token list
		 */
		ce_ptr = trns->enabling;
		while(ce_ptr != ENIL){
			if(strcmp(ce_ptr->placename,(aptr->pptr)->name)==0){ 
				
				(aptr->pptr)->token_count--;
				toeptr = toefind((aptr->pptr)->tokenlist_front,ce_ptr->color);
				tokenextract(aptr->pptr,toeptr);

				if (verbose) fprintf(stderr,"FIRE: Removing token 0x%x from place %s\n", toeptr->color,(aptr->pptr)->name);
				if((aptr->pptr)->output){

					/*
					 * output on ... token leaving place
					 */
					((aptr->pptr)->num_visitors)++;
					(aptr->pptr)->total_visit_time += (clock - toeptr->arrival);

					if((aptr->pptr)->distrib_index) { 

						/* distribution output for this place */
						if ((clock - toeptr->arrival) < MAX_DISTRIBUTION_TIME) place_dist[(aptr->pptr)->distrib_index].dist[clock - toeptr->arrival]++;
						else fprintf(stderr,"ERROR: Exceeded MAX_DISTRIBUTION_TIME ");
						}
					}

				if((aptr->pptr)->token_count == 0){
					/*
					 * place just emptied
					 */
					(aptr->pptr)->util_total += (clock - (aptr->pptr)->util_start);
					}
				returntoken(toeptr);
				}
			ce_ptr = ce_ptr->next;
			}

		} /* if arc in not an inhibitor */

	if(verbose) {
		fprintf(stderr,"FIRE: %d ",(aptr->pptr)->token_count);
		dump_tokenlist(aptr->pptr);
		fflush(stderr);
		}
	aptr = aptr->next;
	} /* step through all incomming arcs */

/*
tokens all extracted from incoming ... now dump in outgoing 
*/
sum_bifur=0.0;
bi_prob=genrand();
bi_fired=0;
rshot = genrand();
tmp_color = (*(trns->color_out))(rshot,trns->enabling,trns->cparameter);

if(tmp_color == -1) {
	/* 
	probably hit eof on a color stream; use Darren's cute trick to halt net
	*/
	sprintf(trns->groupname,"halt");
	return;
	}
aptr = trns->outlist;
while(aptr!=ANIL){
	if(aptr->inhibifur != BIFUR_ARC){
		(aptr->pptr)->token_count++;
		if((aptr->pptr)->token_count == 1){
			/* 
			 * just started some utilization
			 */
			(aptr->pptr)->util_start = clock;
			}
		toeptr = gettoken();
		toeptr->color = tmp_color;
		toeptr->arrival = clock;
		toeptr->next = TOKENNIL;
		if((aptr->pptr)->tokenlist_front==TOKENNIL){
			(aptr->pptr)->tokenlist_front = toeptr;
			}
		else{
			(aptr->pptr)->tokenlist_rear->next = toeptr;
			}
		(aptr->pptr)->tokenlist_rear = toeptr;
		}

	else{
		/* bifurcated output arc */
		if(bi_fired==0){
			sum_bifur += aptr->bifur_prob;
			if(bi_prob<=sum_bifur){
				(aptr->pptr)->token_count++;
				if((aptr->pptr)->token_count == 1){
					/* just started some utilization */
					(aptr->pptr)->util_start = clock;
					}
				toeptr = gettoken();
				toeptr->color = tmp_color; 
				toeptr->arrival = clock;
				toeptr->next = TOKENNIL;
				if((aptr->pptr)->tokenlist_front==TOKENNIL){
					(aptr->pptr)->tokenlist_front = toeptr;
					}
				else{
					(aptr->pptr)->tokenlist_rear->next = toeptr;
					}
				(aptr->pptr)->tokenlist_rear = toeptr;
				bi_fired=1;
				}
			}
		}

	if(verbose) { 
		fprintf(stderr,"FIRE: %d ", (aptr->pptr)->token_count);
		dump_tokenlist(aptr->pptr);
		fflush(stderr);
		}
	aptr = aptr->next;
	} /* while(aptr!=ANIL)  step through all arcs */
}




/*
 * reset is nasty because it must calculate place stats
 ***************/
reset(nptr,clock)
struct net *nptr;
int clock;
{
struct trans *tptr;
struct place *pptr;
struct token *toeptr, *toeptr2;

if(verbose){
	fprintf(stderr,"RESET: begin reset\n");
	fprintf(stderr,"RESET: Current token listing...\n");
	}

tptr=nptr->tlist;
while(tptr!=TNIL){
	tptr->enabled=0;
	releaseenable(tptr);
	tptr = tptr->next;
	} 
pptr=nptr->plist;
while(pptr!=PNIL){
	/*
	 * first release tokens and calculate stats
	 */
	if(pptr->token_count>0) {
		if(verbose) dump_tokenlist(pptr);
		/*
		 * this place just now will be emptied
		 */
		pptr->util_total += (clock - pptr->util_start);
		}

	while(pptr->tokenlist_front != TOKENNIL){
		toeptr = pptr->tokenlist_front;
		pptr->tokenlist_front = (pptr->tokenlist_front)->next;
		/*
		 * output on ... token leaving place 
		 */
		pptr->total_visit_time += (clock - toeptr->arrival);
		(pptr->num_visitors)++;
		if(verbose) {
			fprintf(stderr,"RESET: returning token of color %d ",toeptr->color);
			fprintf(stderr,"from place %s\n",pptr->name);
			}
		fflush(stderr);
		returntoken(toeptr);
		}
		
	/* 
	 * now rebuild token lists from originals
	 * origlist is LIFO, and rebuilt list is FIFO
	 */
	pptr->token_count = pptr->orig_token;
	pptr->util_start = clock;
	toeptr = pptr->origlist;
	while(toeptr != TOKENNIL){
		toeptr2 = gettoken();
		toeptr2->color = toeptr->color;
		toeptr2->arrival = clock;
		toeptr2->next = pptr->tokenlist_front;
		pptr->tokenlist_front = toeptr2;
		toeptr = toeptr->next;
		}
	pptr = pptr->next;
	} /* while(pptr!=PNIL)  step through all places */
}


/*
 *
 ******/
double genrand()
{
	return(((double)(random()+1))/2147483649.);
}



/************************************************************************
 * find functions
 *
 * find a place, transition, net...
 * in the internal linked lists holding the net data.
 */

/*
 * pfind
 *
 * search for a place by name
 ******************/
struct place *pfind(nptr,name)
	struct net *nptr;
	char *name;
{
	struct place *ptr;
	ptr=nptr->plist;
	while(ptr!=PNIL){
		if (strcmp(ptr->name,name)==0) return(ptr);
		ptr=ptr->next;
	}
	return(0);
}

/*
 * tfind
 *
 * search for a transition by name
 ******************/
struct trans *tfind(nptr,name)
	struct net *nptr;
	char *name;
{
	struct trans *ptr;
	ptr=nptr->tlist;
	while(ptr!=TNIL){
		if (strcmp(ptr->name,name)==0) return(ptr);
		ptr=ptr->next;
	}
	return(0);
}

/*
 * nfind
 * 
 * search for a net by name
 ****************/
struct net *nfind(name) 
	char *name;
{
	struct net *ptr;
	ptr=nlist;
	while(ptr!=NNIL){
		if (strcmp(ptr->name,name)==0) return(ptr);
		ptr=ptr->next;
	}
	return(0);
}


/*
 * Find a token in the given tokenlist that has the specified color.
 * Return 0 if a token of that color is not found.
 ********************/
struct token *toefind(tokenlist,color)
	struct token *tokenlist;
	int color;
{
	struct token *toeptr;

	toeptr = tokenlist;
	while(toeptr != TOKENNIL){
		if(toeptr->color == color) return(toeptr);
		toeptr = toeptr->next;
	}
	return(0);
}

/*
find a token on the list that matches color on bits in bitpattern
*/
struct token *toefind_bitmatch(tokenlist,mcolor,bitpattern)
struct token *tokenlist;
int mcolor,bitpattern;
{
struct token *toeptr;

if(bitpattern<0 || bitpattern>0x00ffffff) {
	fprintf(stderr,"illegal bit pattern\n");
	exit(1);
	}
toeptr = tokenlist;

/* really nasty here: == binds tighter than & */
while(toeptr != TOKENNIL){
	if(((toeptr->color)&bitpattern) == (mcolor&bitpattern)) return(toeptr);
	toeptr = toeptr->next;
	}
return(TOKENNIL);
}



/*
 *
 ***********/
tokenextract(placepointer,tokenptr)
	struct token *tokenptr;
	struct place *placepointer;
{
	struct token *toeptr1;

	/* extract from empty list is error */
	if(placepointer->tokenlist_front == TOKENNIL){
		fprintf(stderr,"ERROR: extract attempt from empty token list\n");
		exit(1);
		}

	/* extract from list with 1 item */
	if(placepointer->tokenlist_front == placepointer->tokenlist_rear){
		placepointer->tokenlist_front = placepointer->tokenlist_rear = TOKENNIL;
		return;
		}

	/* extract from front of list with >=2 items */
	if(placepointer->tokenlist_front == tokenptr){
		placepointer->tokenlist_front = (placepointer->tokenlist_front)->next;
		return;
		}

	/* extract from non-front of list with >= 2 items */
	toeptr1 = placepointer->tokenlist_front;
	while(toeptr1->next != TOKENNIL){
		if(toeptr1->next == tokenptr){
			toeptr1->next = (toeptr1->next)->next;
			if(tokenptr == placepointer->tokenlist_rear){
				placepointer->tokenlist_rear = toeptr1;
				}
			return;
			}
		toeptr1 = toeptr1->next;
		}
	fprintf(stderr,"ERROR: oops ... couldn't find color on token list\n");
	exit(1);
}



/*
 *
 ********/
dump_arcs(nptr)
	struct net *nptr;
{
	struct trans *tptr;
	struct arc *aptr;

	tptr = nptr->tlist;
	while(tptr!=TNIL){
		fprintf(stderr,"%s:\n",tptr->name);
		fprintf(stderr,"%s:\n",tptr->groupname);
		fprintf(stderr,"arcs in from places:\n");
		aptr=tptr->inlist;
		while(aptr!=ANIL){
			fprintf(stderr,"%s\n",(aptr->pptr)->name);
			aptr= aptr->next;
		}
		fprintf(stderr,"arcs out to places:\n");
		aptr=tptr->outlist;
		while(aptr!=ANIL){
			fprintf(stderr,"%s\n",(aptr->pptr)->name);
			aptr= aptr->next;
		}
		tptr=tptr->next;
	}
	fprintf(stderr,"\n");
	fflush(stderr);
}


/*
 *
 *********/
dump_plist(nptr)
	struct net *nptr;
{
	struct place *ptr;

	ptr = nptr->plist;
	while(ptr!=PNIL){
		fprintf(stderr,"%s: %d\n",ptr->name,ptr->token_count);
		ptr = ptr->next;
	}
	fprintf(stderr,"\n");
}


/*
 *
 *************/
dump_tokenlist(placeptr)
	struct place *placeptr;
{
	int count;
	struct token *toeptr;

	count = 0;
	fprintf(stderr,"tokens in %s: ",placeptr->name);
	toeptr = placeptr->tokenlist_front;
	while(toeptr != TOKENNIL){
		if (count++ > 10) {
			fprintf(stderr,"\n");
			count = 0;
		}
		fprintf(stderr,"0x%x ",toeptr->color);
		toeptr = toeptr->next;
	}
	fprintf(stderr,"\n");
}


/*
 *
 *********************/
struct place *getplace()
{
	struct place *ptr;
	if(pfreelist==PNIL){
		fprintf(stderr,"ERROR: out of places\n");
		exit(1);
	}
	else{
		ptr = pfreelist;
		pfreelist = pfreelist->next;
		return(ptr);
	}
}


/*
 *
 *********************/
struct token *gettoken()
{
	struct token *tokenptr;
	if(tokenfreelist==TOKENNIL){
		fprintf(stderr,"ERROR: out of tokens\n");
		exit(1);
	}
	else{
		tokenptr = tokenfreelist;
		tokenfreelist = tokenfreelist->next;
		return(tokenptr);
	}
}


/*
 *
 **********/
returntoken(tokenid)
	struct token *tokenid;
{
	tokenid->next = tokenfreelist;
	tokenfreelist = tokenid;
}


/*
 *
 *****************************/
struct color_enable *getenable()
{
	struct color_enable *enableptr;
	if(enablefreelist==ENIL){
		fprintf(stderr,"ERROR: out of enable units\n");
		exit(1);
	}
	else{
		enableptr = enablefreelist;
		enablefreelist = enablefreelist->next;
		return(enableptr);
	}
}


/*
 *
 ***********/
returnenable(enableid) 
	struct color_enable *enableid;
{
	enableid->next = enablefreelist;
	enablefreelist = enableid;
}

/*
 *
 ********************/
struct trans *gettrans()
{
	struct trans *ptr;
	if(tfreelist==TNIL){
		fprintf(stderr,"ERROR: out of transitions\n");
		exit(1);
	}
	else{
		ptr = tfreelist;
		tfreelist = tfreelist->next;
		return(ptr);
	}
}


/*
 *
 *****************/
struct arc *getarc()
{
	struct arc *ptr;
	if(afreelist==ANIL){
		fprintf(stderr,"ERROR: out of arcs\n");
		exit(1);
	}
	else{
		ptr = afreelist;
		afreelist = afreelist->next;
		return(ptr);
	}
}


/*
 *
 ****************/
struct net *getnet()
{
	struct net *ptr;
	if(nfreelist==NNIL){
		fprintf(stderr,"ERROR: out of nets\n");
		exit(1);
	}
	else{
		ptr = nfreelist;
		nfreelist = nfreelist->next;
		return(ptr);
	}
}


/**************************************************************************
 * Init Data Structures
 *
 *
 ********/
initplace()
{
	int i;

	for(i=0;i<NPLACES;i++){
		sprintf(place[i].name,"                   ");
		place[i].token_count = 0;
		place[i].orig_token = 0;
		place[i].next = &place[i+1];
		place[i].tokenlist_front = TOKENNIL;
		place[i].tokenlist_rear = TOKENNIL;
		place[i].origlist = TOKENNIL;
		place[i].output = 0;
		place[i].distrib_index = 0;
		place[i].total_visit_time = 0;
		place[i].num_visitors = 0;
		place[i].util_start = 0;
		place[i].util_total = 0;
	}
	pfreelist = &place[0];
	place[NPLACES-1].next = PNIL;
}


/*
 *
 ********/
inittrans()
{
	int i,j;

	for(i=0;i<NTRANS;i++){
		sprintf(trans[i].name,"                     ");
		sprintf(trans[i].groupname,"                     ");
		trans[i].rand_ftime = instant;
		for(j=0;j<3;j++) sprintf(trans[i].dparameter[j],""); 
		trans[i].rttf = -1;
		trans[i].enabled = 0;
		trans[i].enabling = ENIL;
		trans[i].restrict = ENIL;
		trans[i].inlist = ANIL;
		trans[i].outlist = ANIL;
		trans[i].next = &trans[i+1];
		trans[i].correlation = 0.0;
		trans[i].lastrand = 0.0;
		trans[i].color_out = black;
		for(j=0;j<2;j++) sprintf(trans[i].cparameter[j],""); 
		trans[i].output = 0;
		trans[i].firecount= 0;
	}
	tfreelist = &trans[0];
	trans[NTRANS-1].next = TNIL;
}


/*
 *
 ******/
initarc()
{
	int i;

	for(i=0;i<NARCS;i++){
		arc[i].pptr = PNIL;
		arc[i].tptr = TNIL;
		arc[i].inhibifur = STD_ARC;
		arc[i].bifur_prob=1.0;
		arc[i].next = &arc[i+1];
	}
	afreelist = &arc[0];
	arc[NARCS-1].next = ANIL;
}


/*
 *
 ******/
initnet()
{
	int i;

	for(i=0;i<NNETS;i++){
		net[i].plist = PNIL;
		net[i].tlist = TNIL;
		net[i].next = &net[i+1];
	}
	nfreelist = &net[0];
	net[NNETS-1].next = NNIL;
}


/*
 *
 ********/
inittoken()
{
	int i;

	for(i=0;i<NTOKENS;i++){
		token[i].color = 0;
		token[i].arrival = 0;
		token[i].next = &token[i+1];
	}
	tokenfreelist = &token[0];
	token[NTOKENS-1].next = TOKENNIL;
}



/*
 *
 *********/
initenable()
{
	int i;

	for(i=0;i<NENABLES;i++){
		color_enable[i].color = 0;
		sprintf(color_enable[i].placename,"                   ");
		color_enable[i].next = &color_enable[i+1];
	}
	enablefreelist = &color_enable[0];
	color_enable[NENABLES-1].next = ENIL;
}

initfilestructures()
{
int i;

/* empirical distributions: size 0,  not loaded */
for(i=0;i<MAX_NUM_HISTS;i++){
	histcollection[i].size = 0;
	histcollection[i].loaded = 0;
	}
/* color markov from file: size 0, not loaded */
for(i=0;i<MAX_NUM_MARKOVS;i++){
	markovcollection[i].size = 0;
	markovcollection[i].loaded = 0;
	}
/* color io from file: size 0, not loaded */
for(i=0;i<MAX_NUM_COLORIOS;i++){
	coloriocollection[i].size = 0;
	coloriocollection[i].loaded = 0;
	}
/* color stream from file: not open, buff_pos = 0*/
for(i=0;i<MAX_NUM_COLORSTREAMS;i++){
	colorstreamcollection[i].buff_pos = 0;
	colorstreamcollection[i].open = 0;
	colorstreamcollection[i].bytes_read = 0;
	}
}
/**************************************************************************/



/**************************************************************************
 * Transition fireing time distributions
 *
 *
 *********/
int instant()
{
	return(0);
}

/*
 *
 **********/
int constnt(inseed,elist,parmlist)
	double inseed;
	struct color_enable *elist;
	char *parmlist;
{
	int res;

	res = atol(PARMLIST(0));
	return(res);
}


/*
 *
 *******/
int unif(inseed,elist,parmlist)
	double inseed;
	struct color_enable *elist;
	char *parmlist;
{
	int outtime;
	double res;

	res=atof(PARMLIST(0));
	outtime = (int)(inseed*res);
	return(outtime);
}


/*
 *
 *******/
int expo(inseed,elist,parmlist)
	double inseed;
	struct color_enable *elist;
	char *parmlist;
{
	int res;
	double xtra;

	xtra = atof(PARMLIST(0));
	res = (int) ((log(1.0-inseed)/(-xtra))+0.5);
	return(res);
}


/*
 *
 *********/
int expold(inseed,elist,parmlist)
double inseed;
struct color_enable *elist;
char *parmlist;
{
	int res;
	double xtra;
	int pload;
	struct net *npt;
	struct place *ppt;

	npt=nlist;
	while(npt != NNIL){
		if(ppt=pfind(npt,PARMLIST(1))) break;
		npt = npt->next;
	}
	if(npt==NNIL){
		fprintf(stderr,"ERROR: what place?\n");
		exit(1);
	}
	pload = ppt->token_count;
	xtra = atof(PARMLIST(0));
	xtra *= (double)(pload);
	res = (int) ((log(1.0-inseed)/(-xtra))+0.5);
	return(res);
}

/*
 *
 ************/
int empirical(inseed,elist,parmlist)
double inseed;
struct color_enable elist;
char *parmlist;
{
/*
PARMLIST(0) is filename
PARMLIST(1) is index into histcollection
*/
FILE *fp, *fopen();
int i,j;
double probsum, rnum;
int index,size;

index = atoi(PARMLIST(1));
if(!histcollection[index].loaded){
		fp=fopen(PARMLIST(0),"r");
		fscanf(fp,"%d",&size);
		histcollection[index].size = size;
		if(size>MAX_HIST) {
			fprintf(stderr,"ERROR: Empirical array too small\n");
			exit(1);
			}
		for(i=0;i<size;i++){
			fscanf(fp,"%lf %d",&histcollection[index].hist[i].prob,&histcollection[index].hist[i].value);
			}
		histcollection[index].loaded=1;
		fclose(fp);
		}
probsum = 0.0;
rnum = genrand();
size = histcollection[index].size;
for(j=0;j<size;j++) {
	probsum += histcollection[index].hist[j].prob;
	if(rnum <= probsum) return(histcollection[index].hist[j].value);
	}
return(histcollection[index].hist[size-1].value);
}


/*
 *
 ***********/
int diskseek(inseed,elist,parmlist)
double inseed;
struct color_enable *elist;
char *parmlist;
{
/*
PARMLIST(0) is placename where we find current head location
PARMLIST(1) is placename where we find desired head location
PARMLIST(2) is rotation
PARMLIST(3) is startup
PARMLIST(4) is seek/cyl
*/
int res;
int trk1, trk2;
float rotate;
float startup;
float seek_per_cyl;
float fres;
struct color_enable *ceptr;

rotate = atof(PARMLIST(2));
startup = atof(PARMLIST(3));
seek_per_cyl = atof(PARMLIST(4));

trk1 = trk2 = -1;
ceptr = elist;
while(ceptr != ENIL){
	if(strcmp(ceptr->placename,PARMLIST(0))==0){
		trk1 = ceptr->color;
		}
	if(strcmp(ceptr->placename,PARMLIST(1))==0){
		trk2 = ceptr->color;
		}
	ceptr = ceptr->next;
	}

if((trk1 == -1) || (trk2 == -1)){
	fprintf(stderr,"eek ... can't find track\n");
	}
res = abs(trk1-trk2);

if(res==0) res = (int)(rotate + 0.5);
else res = (int)(rotate + startup + seek_per_cyl*(float)(res) + 0.5);
if(verbose) fprintf(stderr,"%f %f %f res: %d\n",rotate,startup,seek_per_cyl,res);
return(res);
}
/**************************************************************************/



/**************************************************************************
 * Color output of transitions
 *
 *
 ********/
int black()
{
	return(0);
}


/*
 *
 */
int myselect(rnum,elist,parmlist)
double rnum;
struct color_enable *elist;
char *parmlist;
{
/*
PARMLIST(0) is placename where we grab color
*/
struct color_enable *eptr;

eptr = elist;
while(eptr != ENIL){
	if(strcmp(eptr->placename,PARMLIST(0))==0) return(eptr->color); 
	eptr = eptr->next;
	}
fprintf(stderr,"NET ERROR: select function:\n");
exit(1);
}


/*
undecided issue here: is Markov file in regular or .sqz format ?
code for both loads is below, but some resolution would help
*/
int
markov(rnum,elist,parmlist)
double rnum;
struct color_enable *elist;
char *parmlist;
{
/*
PARMLIST(0) is filename containing markov matrix
PARMLIST(1) is placename where we get input state
PARMLIST(2) is index into Markov array collection
*/
int fda;
struct color_enable *eptr;
int i,j;
int row;
float probsum;
char *ptr;
FILE *fp, *fopen();
int index,size;

index = atoi(PARMLIST(2));
if(!markovcollection[index].loaded){
	/*
	two file formats used ... regular and .sqz
	*/
	fp=fopen(PARMLIST(0),"r");
	fscanf(fp,"%d",&size);
	/*
	fda = open(PARMLIST(0),0);
	ptr = (char *)(&size);
	read(fda,ptr,4);
	*/
	markovcollection[index].size = size;
	if(size>MAX_MARKOV) {
		fprintf(stderr,"Markov array too small\n");
		fprintf(stderr,"size read is %d\n",size);
		exit(1);
		}
	/*
	ptr = (char *)(markovcollection[index].nextstate);
	read(fda,ptr,size*size*4);
	close(fda);
	*/
	for(i=0;i<size*size;i++){
		fscanf(fp,"%f",&markovcollection[index].nextstate[i]);
		}
	fclose(fp);
	markovcollection[index].loaded=1;
	}
size = markovcollection[index].size;
eptr = elist;
while(eptr != ENIL){
	if(strcmp(eptr->placename,PARMLIST(1))==0){
		row = eptr->color;
		probsum = 0.0;
		for(j=0;j<size;j++) {
			probsum += markovcollection[index].nextstate[size*row+j];
			/* 
			 * patch markov-float inaccuracies;
			 * a row sum < 1.0 (e.g. 0.9999999) 
			 * will cause errors; we cannot simply return
			 * final column, since there may be no
			 * exit from there; small row found is 
			 * .99943
			 */
			if(probsum>0.9994) probsum = 1.0;

			if(rnum <= (double)(probsum)) return(j);
			}
		fprintf(stderr,"ERROR: could not find column in Markov\n");
		exit(1);
		}
	eptr = eptr->next;
	}
fprintf(stderr,"ERROR: oops ... no row in Markov\n");
exit(1);
}


/*
 *
 ***********/
int color_io(rnum,elist,parmlist)
double rnum;
struct color_enable *elist;
char *parmlist;
{
/*
PARMLIST(0) is file name
PARMLIST(1) is place name where we get input color
PARMLIST(2) is index into collection of colorio files
*/
FILE *fp, *fopen();
struct color_enable *eptr;
int i,j;
int row;
int index,size;

index=atoi(PARMLIST(2));
if(!coloriocollection[index].loaded){
		fp=fopen(PARMLIST(0),"r");
		fscanf(fp,"%d",&size);
		coloriocollection[index].size = size;
		if(size>MAX_COLORMAP) {
			fprintf(stderr,"ERROR: color map array too small\n");
			exit(1);
			}
		for(i=0;i<size;i++) fscanf(fp,"%d",&coloriocollection[index].colormapstate[i]);
		coloriocollection[index].loaded=1;
		fclose(fp);
		}
eptr = elist;
while(eptr != ENIL){
	if(strcmp(eptr->placename,PARMLIST(1))==0){
		row = eptr->color;
		return(coloriocollection[index].colormapstate[row]);
		}
	eptr = eptr->next;
	}
fprintf(stderr,"ERROR: oops ... no row in color map\n");
exit(1);
}


int color_stream(rnum,elist,parmlist)
double rnum;
struct color_enable *elist;
char *parmlist;
{
/*
PARMLIST(0) is file name where we find stream
PARMLIST(1) is index into stream file collection
*/
struct color_enable *eptr;
int index;

index = atoi(PARMLIST(1));
if(!colorstreamcollection[index].open){
	colorstreamcollection[index].fd = open(PARMLIST(0),0);
	colorstreamcollection[index].open = 1;
	}
if(colorstreamcollection[index].buff_pos==colorstreamcollection[index].bytes_read){
	colorstreamcollection[index].bytes_read = read(colorstreamcollection[index].fd,(char *)(colorstreamcollection[index].buffer),4*BUFSIZE);
	if(colorstreamcollection[index].bytes_read==0) return(-1);
	colorstreamcollection[index].buff_pos = 0;
	}
return(colorstreamcollection[index].buffer[colorstreamcollection[index].buff_pos++]);
}



int constant_color(rnum,elist,parmlist)
double rnum;
struct color_enable *elist;
char *parmlist;
{
return (atoi(PARMLIST(0)));
}

int usercolor()
{
return(0);
}

/**************************************************************************/

/* this dude doesn't get called unless correlation != 0.0 */
double correl_rn(transptr,netptr)
struct trans *transptr;
struct net *netptr;
{
struct trans *tptr;
int hwmny,whichone;
double rn;

hwmny=0;
rn=genrand();

/* count how many others in group are enabled */
tptr=netptr->tlist;
while(tptr!=TNIL){
	if (tptr->enabled==1 && tptr!=transptr && strcmp(tptr->groupname,transptr->groupname)==0) hwmny++;
	tptr = tptr->next;
	}
if(hwmny==0) return(rn);

/* choose one of the others */
rn *= (double)(hwmny);
whichone = (int)(rn) + 1;
hwmny=0;
tptr=netptr->tlist;
while(tptr!=TNIL){
	if (tptr->enabled==1 && tptr!=transptr && strcmp(tptr->groupname,transptr->groupname)==0) hwmny++;
	if(hwmny==whichone){
		rn = cgenrand(transptr->correlation,tptr->lastrand);
		return(rn);
		}
	tptr = tptr->next;
	}
fprintf(stderr,"ERROR: oops ... didn't find group\n");
exit(1);
}

/*
 *
 **************/
double cgenrand(k,r)
	double k,r;
{
	double localr;

	if(k>=0.0){
		localr=genrand();
		if(localr<=k) return(r);
		localr=genrand();
		return(localr);
	}
	else{
		localr=genrand();
		if(localr<= -k) return(1.0-r);
		localr=genrand();
		return(localr);
	}
}


/*
 * process params
 *
 */
/*****************/
int 
process_params(argcount,arguments,ireview,verb,mtime,ifilename)
int argcount;
char **arguments;
int *ireview, *verb, *mtime;
char *ifilename;
/*****************/
{
/* 
defaults: 
don't review input file
blabber
execute for 0 time
read from stdin
*/
int i;
char *cptr;

*ireview = 0;
*verb = 2;
*mtime = 0;
*ifilename = 0;

for(i=1;i<argcount;i++){
	cptr = arguments[i];
	switch(*(++cptr)){
		case 't': 
				*mtime = atoi(arguments[++i]);
				if(*mtime > MAXTTF){
					fprintf(stderr,"too much time\n");
					exit(1);
					}
		  		break;
		case 'v': 
				*verb = atoi(arguments[++i]);
				break;
		case 'r': 
				*ireview = 1;
				break;
		case 'i':
				sprintf(ifilename,arguments[++i]);
				break;
		case 'u':
				turbo(getpid());
				turbo_on = 1;
				break;
		default:  
				fprintf(stderr,"no such option\n");
				exit(1);
		}
	}
}

/*
 * INTERACTIVE INTERFACE
 */

/* 
 * Provide an interactive interface that provides a way to "look" into
 * the net description.
 */
int interactive(nptr)
	struct net *nptr;
{
	char search_string[MAX_NAME_LEN];
	int done;
	struct place *place_ptr;
	struct trans *trans_ptr;

	fprintf(stderr,"\n\n\n INTERACTIVE MODE \n\n");
	done = 0;
	while (!done) {
		fprintf(stderr,"Enter command: ");
		scanf("%s",search_string);
		fprintf(stderr,"\n\n");

		/* exit */
		if (strcmp(search_string,"exit") == 0) {
			done = 1;
			continue;
		}

		/* quit */
		if (strcmp(search_string,"quit") == 0) {
			exit(0);
		}

		/* help */
		if (strcmp(search_string,"help") == 0) {
		 fprintf(stderr,"Help stuff\n");
		 continue;
		}

		/* 
		 * Name search
		 */
		place_ptr=nptr->plist;
		while(place_ptr!=PNIL){
			if (substr(place_ptr->name,search_string)) {
				print_place_info(nptr, place_ptr);
			}
			place_ptr=place_ptr->next;
		}

		trans_ptr=nptr->tlist;
		while(trans_ptr!=TNIL){
			if (substr(trans_ptr->name,search_string)) {
				print_trans_info(nptr, trans_ptr);
			}
			trans_ptr=trans_ptr->next;
		}

		fprintf(stderr,"\n\n");
	}
}

int print_place_info(nptr, place_ptr)
	struct net *nptr;
	struct place *place_ptr;
{
	struct trans *trans_ptr;
	struct arc *arc_ptr;

	/*
	 * Print info about place
	 */
	fprintf(stderr,"\nPlace name: %s  Token Count: %d\n",place_ptr->name,
		place_ptr->token_count);

	/*
	 * Run down all transitions, and print ones with arcs to this place
	 */
	fprintf(stderr,"-- Incoming arcs from transitions --\n");
	trans_ptr = nptr->tlist;
	while (trans_ptr != TNIL) {
		arc_ptr = trans_ptr->outlist;
		while (arc_ptr != ANIL) {
			if (arc_ptr->pptr == place_ptr) {
				fprintf(stderr,"%s\n",trans_ptr->name);
			}
			arc_ptr = arc_ptr->next;
		}
		trans_ptr = trans_ptr->next;
	}

	/*
	 * Run down all transitions, and print ones with arcs from this place
	 */
	fprintf(stderr,"-- Outgoing arcs to transitions --\n");
	trans_ptr = nptr->tlist;
	while (trans_ptr != TNIL) {
		arc_ptr = trans_ptr->inlist;
		while (arc_ptr != ANIL) {
			if (arc_ptr->pptr == place_ptr) {
				fprintf(stderr,"%s\n",trans_ptr->name);
			}
			arc_ptr = arc_ptr->next;
		}
		trans_ptr = trans_ptr->next;
	}
}

int print_trans_info(nptr, trans_ptr)
	struct net *nptr;
	struct trans *trans_ptr;
{
	struct place *place_ptr;
	struct arc *arc_ptr;

	/*
	 * Print info about place
	 */
	fprintf(stderr,"\nTransition name: %s\n",trans_ptr->name);

	/*
	 * Run down incoming list
	 */
	fprintf(stderr,"-- Incoming arcs from places --\n");

	arc_ptr = trans_ptr->inlist;
	while (arc_ptr != ANIL) {
		fprintf(stderr,"%s\n",arc_ptr->pptr->name);
		arc_ptr = arc_ptr->next;
	}

	/*
	 * Run down all outgoing arcs
	 */
	fprintf(stderr,"-- Outgoing arcs to places --\n");

	arc_ptr = trans_ptr->outlist;
	while (arc_ptr != ANIL) {
		fprintf(stderr,"%s\n",arc_ptr->pptr->name);
		arc_ptr = arc_ptr->next;
	}
}

int substr(source,sub)
	char *source, *sub;
{
	int i, j, equal;
	int sub_length, source_length;

	source_length = strlen(source);
	sub_length = strlen(sub);

	for (i=0;i<source_length-sub_length+1;i++) {
		equal = 1;
		for (j=i;j<sub_length+i;j++) {
			if (source[j] != sub[j-i]) {
				equal = 0;
				break;
			}
		}
		if (equal) return (1);
	}
	return(0);
}

outputstats(et,nptr)
int et;
struct net *nptr;
{
struct place *pptr;
struct trans *tptr;
double holding_time;
/*
 * Output place statistics
 */
FILE *fp, *fopen();
int i;
pptr=nptr->plist;
while(pptr != PNIL){
	if(pptr->output){
		if (pptr->num_visitors == 0) holding_time = 0.0;
		else holding_time = (double)(pptr->total_visit_time)/(double)(pptr->num_visitors);
		printf("place: %s holding_time: %f utilization: %f\n",pptr->name,holding_time,(double)(pptr->util_total)/(double)(et));
      		if(pptr->distrib_index) {  /* output distribution to file */
        		fp = fopen(place_dist[pptr->distrib_index].outfile,"w");
          		for (i=0;i<MAX_DISTRIBUTION_TIME; i++) fprintf(fp,"%d\n",place_dist[pptr->distrib_index].dist[i]);
			fclose(fp);
        		}
    		}
	pptr = pptr->next;
  	}

/*
 * Output transition statistics
 */
tptr=nptr->tlist;
while(tptr != TNIL){
	if(tptr->output){
		printf("transition: %s throughput: %f total_fires: %d\n",tptr->name,(double)(tptr->firecount)/(double)(et),tptr->firecount);
    		}
	tptr = tptr->next;
	}
}
