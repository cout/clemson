/* An example main(), reads the input polygon and prints
   out the trapezoids found. */

#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "func.h"

main (int argc, char *argv[]) {

	int  no;
	Vertex vts;
	TrapezoidL traplist;
	int oogl=0;   /* default mode non-debug, oogle output */
	int practical=0;  /* default mode theoretical (O(n lg*n) alg) */
	int inandout = 0; /* default mode: interior decomposition */

	for (++argv; --argc > 0; argv++) {
	  if (strcmp(*argv, "-o")==0)
	    oogl=1;
	  else if (strcmp(*argv, "-p")==0)
	    practical=1;
	  else if (strcmp(*argv, "-a") == 0)
	    inandout = 1;    
	  else {

	    fprintf(stderr,"\
TrapDec [-o] [-p] [-a]\n\
Trapezoidal decomposition of simple polygons.\n\
-o Output in oogl format \n\
-p Use O(n lg n) subroutine which is faster on small problems \n\
-a Decomposes the interior and the exterior of the input polygon \n\
   (by deault only the interior is decomposed) \n");

            exit(1);
          }
	}
  


	  vts = readinputvertex(&no);
		if (no == 0) {
			scanf("%d", &no);
			vts = makepoly(no);
		}
	  
/*
	if (checkcrossings(no, vts) )  {
		printf("error in input");
		exit(1);
	}
	checkycoords(no, vts);
*/

	  if (practical) 
	    /* Use O(n lg n) subroutine for the whole thing;
	       probably faster in practice on most polygons */
	    traplist = trap_dec_log(no, vts);
	  /* Use theoretical O(n lg*n) algorithm */
	  else traplist = trap_dec(no, vts);

	  if (oogl) outputtraps(traplist, 1, inandout);
	  else outputtraps(traplist, 0, inandout); 
      }
      

void outputtraps (TrapezoidL traplist, int OutFormat, int mode)
  {
    if (OutFormat == OOGL) 
      printf("QUAD  \n");  /* keyword means a list of quadrilaterals follows */
    if (mode)
    	reportalltraps(traplist->upm, OutFormat);
    else
    	reportintraps(traplist, OutFormat);
  }

