/* COPYRIGHT - Dimitrios Gunopulos, Princeton University, 1994 */
/* this file contains the functions that implement the algorithm
   (except findtraps()), and the output functions.
*/
/* Changes 7/94 by Nina Amenta - OOGL output format */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "def.h"
#include "func.h"

#define TEST 1
#define MAXSIZE 1000

static vertexL cleanvertexL = {{0.0, 0.0, NULL,NULL}, NULL};
int rnd = 100;



Vertex makepoly(int n) {
/* Creates a simple polygon with n vertices. The method used
   is the following: A triangle with 3 rand()m vertices is produced.
   Then, an edge is chosen at rand()m, and is broken in two pieces. 
   Doesn't work very well.   */

	int i, j, k, l, c1, c2, c3, c4, idist, test, ir;
	double dist, x_dif, y_dif;
	VertexL vta, help, kp, new;
	Vertex ret;

	vta = (VertexL) malloc(n*sizeof(vertexL));
	assert( (vta != NULL));
	srand(getrandom() );
	*vta = cleanvertexL;
	*(vta+1) = cleanvertexL;
	*(vta+2) = cleanvertexL;
	vta->v.x = rand() % MAXSIZE;
	vta->v.y = rand() % MAXSIZE;
	(vta+1)->v.x = rand() % MAXSIZE; 
	(vta+1)->v.y = rand() % MAXSIZE; 
	for (;;) {
		if (!samepoint(&(vta->v), &((vta+1)->v)))
			break;
		(vta+1)->v.x = rand() % MAXSIZE; 
		(vta+1)->v.y = rand() % MAXSIZE; 
	}
	(vta+2)->v.x = rand() % MAXSIZE; 
	(vta+2)->v.y = rand() % MAXSIZE; 
	for (;;) {
		if (!samepoint(&(vta->v), &((vta+2)->v)) && 
			!samepoint(&((vta+1)->v), &((vta+2)->v)))
			break;
		(vta+2)->v.x = rand() % MAXSIZE; 
		(vta+2)->v.y = rand() % MAXSIZE; 
	}
	vta->next = (vta+1);
	(vta+1)->next = (vta+2);
	(vta+2)->next = vta;
	printf("%f %f\n",vta->v.x,vta->v.y);
	printf("%f %f\n",(vta+1)->v.x,(vta+1)->v.y);
	printf("%f %f\n",(vta+2)->v.x,(vta+2)->v.y);
	for (i=2; i<(n-1); ) {
		k = (ir = rand()) % (i);
		help = (vta + k);
		if (help == NULL)
			printf("ERROR -- NULL\n");
		dist = length(&(help->v), &(help->next->v));
		idist = 2*dist;
		if (idist < 2) 
			idist = 2;
		*(vta + i+1) = cleanvertexL;
		new = (vta+i+1);
		new->next = help->next;

/*
		for (l = 0 ; l < 4; l++) {
*/
		ir = rand()%idist - dist;
		x_dif = ir;
		ir = rand()%idist - dist;
		y_dif = ir;
		x_dif += (help->v.x + help->next->v.x)/2;
		y_dif += (help->v.y + help->next->v.y)/2;
		(vta + i+1)->v.x = x_dif;
		(vta + i+1)->v.y = y_dif;
		printf("%f %f not yet\n",(vta+i+1)->v.x,(vta+i+1)->v.y);
		sleep(1);
		test = 0;
		if (samepoint(&(new->v), &(help->v)))
			test = 1;

		if (!test)
		 	for (kp = help->next; kp->next != help; kp = kp->next){
				c1 = ccw(&(kp->v),&(kp->next->v),&(help->v));
				c2 = ccw(&(kp->v),&(kp->next->v),&(new->v));
				c3 = ccw(&(help->v),&(new->v),&(kp->v));
				c4 = ccw(&(help->v),&(new->v),&(kp->next->v));
                       		if ((c1*c2 == -1) && (c3*c4 == -1) ||
                        	       	(c1*c2*c3*c4 == 0))
                               		test = 1;
                        	if (ecomp(kp->v.y,new->v.y) == 0)
                                	test = 1;
			}
		if (!test)
			for (kp = help->next->next; kp != help; kp = kp->next){
				c1=ccw(&(kp->v),&(kp->next->v),&(new->v));
				c2=ccw(&(kp->v),&(kp->next->v),&(new->next->v));
				c3=ccw(&(new->v),&(new->next->v),&(kp->v));
				c4=ccw(&(new->v),&(new->next->v),&(kp->next->v));
                       		if ((c1*c2 == -1) && (c3*c4 == -1) ||
                        	       	(c1*c2*c3*c4 == 0))
                               		test = 1;
			}

/*
			if (!test)
				break;
		}
*/
		if (!test) { 
			help->next = (vta + i+1);
			printf("%f %f \n",(vta+i+1)->v.x,(vta+i+1)->v.y);
			sleep(1);
			i++;
		}
	}
	ret = (Vertex) malloc(n*sizeof(vertex));
	assert((ret!=NULL));
	for (help = vta, i=0; i<n; i++, help = help->next) 
		*(ret+i) = help->v;
	(void) free((char *) vta);
	return ret;
}

void reporttrap(TrapezoidL sol, int OutFormat) {
/* Prints information for the trapezoid trap. */

	if (sol == NULL) return;
	if (OutFormat == DEFAULT)
	  {
	printf("trapezoid number: %d\n", sol->t.no);
	printf(" left edge %f %f %f %f \n",
		sol->t.left->e.vu->x, sol->t.left->e.vu->y,
		sol->t.left->e.vd->x, sol->t.left->e.vd->y);
	printf(" left edge number: %d\n", sol->t.left->e.no);
	printf(" right edge %f %f %f %f \n",
		sol->t.right->e.vu->x, sol->t.right->e.vu->y,
		sol->t.right->e.vd->x, sol->t.right->e.vd->y);
	printf(" right edge number: %d\n", sol->t.right->e.no);
	if ((sol->t.up != NULL) && (sol->t.down != NULL) )
		printf(" top line: %f , bottom line: %f\n",
			sol->t.up->y, sol->t.down->y);
	else 
		printf("  error\n");
	printf(" the four vertices are :\n");
	printf(" (%f, %f),(%f, %f),\n (%f, %f),(%f, %f)\n",
		edxposition2(sol->t.right,sol->t.up->y), sol->t.up->y,
		edxposition2(sol->t.left,sol->t.up->y), sol->t.up->y,
		edxposition2(sol->t.right,sol->t.down->y),sol->t.down->y,
		edxposition2(sol->t.left,sol->t.down->y),sol->t.down->y);

	if (sol->upl != NULL)
                printf("  upl %d  ",sol->upl->t.no);
        else
                printf("  upl NULL  ");
        if (sol->upm != NULL)
                printf("upm %d  ",sol->upm->t.no);
        else
                printf("upm NULL  ");
        if (sol->upr != NULL)
                printf("upr %d\n",sol->upr->t.no);
        else
                printf("upr NULL\n");
        if (sol->downl != NULL)
                printf("  downl %d  ",sol->downl->t.no);
        else
                printf("  downl NULL  ");
        if (sol->downm != NULL)
                printf("downm %d  ",sol->downm->t.no);
        else
                printf("downm NULL  ");
        if (sol->downr != NULL)
                printf("downr %d\n",sol->downr->t.no);
        else
                printf("downr NULL\n");
	if (sol->upprev != NULL)
		printf("  upprev %d", sol->upprev->t.no);
	else 
		printf("  upprev NULL  ");
	if (sol->upnext != NULL)
		printf("upnext %d\n", sol->upnext->t.no);
	else 
		printf("upnext NULL\n");
	if (sol->downprev != NULL)
		printf("  downprev %d  ", sol->downprev->t.no);
	else 
		printf("  downprev NULL  ");
	if (sol->downnext != NULL)
		printf("downnext %d\n", sol->downnext->t.no);
	else 
		printf("downnext NULL\n");
      }
	else /* OOGL output */
	  {
	    printf ("%f %f 0.0  %f %f 0.0  %f %f 0.0  %f %f 0.0 \n",
		    edxposition2(sol->t.right,sol->t.up->y), sol->t.up->y,
		    edxposition2(sol->t.left,sol->t.up->y), sol->t.up->y,
		    edxposition2(sol->t.left,sol->t.down->y),sol->t.down->y,
		    edxposition2(sol->t.right,sol->t.down->y),sol->t.down->y
		    );
	  }
}

void reportalltraps(TrapezoidL sol, int OutFormat) {
/* Prints all trapezoids found so far.
   This includes the trapezoids that surround the input polygon. */

	TrapezoidL help;

	if (sol == NULL) return;
	reporttrap(sol, OutFormat);
	if (((help = sol->downl) != NULL) && (sol->downr != NULL) &&
		(sol->downl != sol->downr)) {
		reportalltraps(sol->downl, OutFormat);
		if (sol->downm != NULL) 
			reportalltraps(sol->downm, OutFormat);
		reportalltraps(sol->downr, OutFormat);
	}
	else  {
		if (help == NULL)
			help = sol->downr;
		if ((help != NULL) && ((help->upl == sol) ||
			(help->upl == NULL) && (help->upr == sol)))
			reportalltraps(help, OutFormat);
	}
}

void cleanup(TrapezoidL sol) {
/* Cleans up the trapezoid chains after the execution of the
   algorithm. */

        TrapezoidL help;

        if (sol == NULL) return;

        if ((sol->downl == sol->downm) || (sol->downr == sol->downm))
        /* in the first of the following two cases, sol->downm 
	   crosses the boubdary of the polygon */ 
		if (eq(sol->t.right->e.vd->y, sol->t.left->e.vd->y) && 
			eq(sol->t.right->e.vd->y, sol->t.down->y))
                        sol->downl = sol->downr = NULL;
                else
                        sol->downm = NULL;

        if ((sol->upl == sol->upm) || (sol->upr == sol->upm))
        /* similarly, in the first case sol->upm crosses the boubdary
           of the polygon */
                if (eq(sol->t.right->e.vu->y, sol->t.left->e.vu->y) &&
                        eq(sol->t.right->e.vu->y, sol->t.up->y))
                                sol->upl = sol->upr = NULL;
                else sol->upm = NULL;

        if (((help = sol->downl) != NULL) && (sol->downr != NULL) &&
                (sol->downl != sol->downr)) {
                cleanup(sol->downl);
                if (sol->downm != NULL)
                        cleanup(sol->downm);
                cleanup(sol->downr);
        }
        else  {
                if (help == NULL)
                        help = sol->downr;
                if ((help != NULL) && ((help->upl == sol) ||
                        (help->upl == NULL) && (help->upr == sol)))
                        cleanup(help);
        }
}


void reportintraps(TrapezoidL sol, int OutFormat) {
/* Prints the trapezoid decomposition of the polygon.
   It can be used only after the completion of the algorithm.
   It assumes that the trapezoid sol is in the polygon, and
   uses only the four downl, downr, upl, upr pointers. 
*/

	printf("decomposition:\n");
	reporttraps(sol, -1, OutFormat);
}

void reporttraps(TrapezoidL sol, int in, int OutFormat) {
/* Does a breadth first search in the trapezoid decomposition
   reprezentation. 
   It assumes that the polygon has no holes.
*/

        if (sol == NULL) return;
        reporttrap(sol, OutFormat);

	/* follow all ather links except the one that
	   goes back */
	if ((sol->downl != NULL) && (sol->downl->t.no != in))
                reporttraps(sol->downl, sol->t.no, OutFormat);
	if ((sol->downr != sol->downl) && 
		(sol->downr != NULL) && (sol->downr->t.no != in))
                reporttraps(sol->downr, sol->t.no, OutFormat);
	if ((sol->upl != NULL) && (sol->upl->t.no != in))
                reporttraps(sol->upl, sol->t.no, OutFormat);
	if ((sol->upr != sol->upl) && 
		(sol->upr != NULL) && (sol->upr->t.no != in))
                reporttraps(sol->upr, sol->t.no, OutFormat);
}


void trapsintraps(TrapezoidL sol) {
/* Recursively visits the trapezoids found so far, and for
   each one finds the new trapezoids in the corresponding region. */

	TrapezoidL help;

	if (sol == NULL) return;

	findtraps(sol->er);

	if (((help = sol->downl) != NULL) && (sol->downr != NULL) &&
		(sol->downl != sol->downr)) {
		trapsintraps(sol->downl);
		if (sol->downm != NULL) 
			trapsintraps(sol->downm);
		trapsintraps(sol->downr);
	}
	else  {
		if (help == NULL)
			help = sol->downr;
		if ((help != NULL) && ((help->upl == sol) ||
			(help->upl == NULL) && (help->upr == sol)))
			trapsintraps(help);
	}
}

int mergetwotraps(TrapezoidL up, TrapezoidL lo, int left) {
/* Merges two trapezoids to one, going from left to right, or the reverse */

	int ui, di, i, j, k, n;
	double bottom;
	TrapezoidL help1, help2, keephelp1, keephelp2, hr, hm, hl, lower,upper;

	if ((up == NULL) || (lo == NULL)) {
	
		return 0;
	}
 	if (left) {
		upper = up->er->bottomleft;
		lower = lo->er->topleft;
		bottom = upper->t.down->y;
		for (help1 = upper, help2 = lower, i = 0; 
		     (help1 != NULL) && (help2 != NULL);) {
			if (sameedges(help1->t.left,help2->t.left) &&
				!sameedges(help1->t.right, help2->t.right) ) {
				if (!samepoint(help1->t.right->e.vd,
						help2->t.right->e.vu)) {
					k = ecomp(
					 edxposition(help1->t.right,bottom,&j) ,
				    	 edxposition(help2->t.right,bottom,&j));
					if ( k < 0) {
						help1->downl = help2;
						help1->downm = help2;
						help1->downr = help2;
						help2->upl = help1;
						help1 = help1->downnext;
					}
					else if (k > 0) {
						help2->upl = help1;
						help2->upm = help1;
						help2->upr = help1;
						help1->downl = help2;
						help2 = help2->upnext;
					}
					else {
						fprintf(stderr," input error, edge intersection detected\n");
						exit(1);
					}
				}
				else {
					help2->upl = help1;
					help2->upm = help1;
					help2->upr = help1;
					help1->downl = help2;
					help1->downm = help2;
					help1->downr = help2;
					help1 = help1->downnext;
					help2 = help2->upnext;
				}
			}
			else if (!sameedges(help1->t.left,help2->t.left)  &&
				!sameedges(help1->t.right, help2->t.right)) {
				if (help1->t.left->e.vd==help1->t.right->e.vd) {
					help1->downl = help2;
					help1->downm = help2;
					help1->downr = help2;
					help2->upm = help1;
					help1 = help1->downnext;
				}
				else if (help2->t.left->e.vu ==
					help2->t.right->e.vu) {
					help2->upl = help1;
					help2->upm = help1;
					help2->upr = help1;
					help1->downm = help2;
					help2 = help2->upnext;
				}
				else {
					fprintf(stderr," input error, edge intersection detected\n");
					exit(1);
				}
			}
			else if (sameedges(help1->t.left, help2->t.left) &&
			 	  sameedges(help1->t.right,help2->t.right)) {
				help1->t.down = help2->t.down;
				hl = help1->downl = help2->downl;
				hm = help1->downm = help2->downm;
				hr = help1->downr = help2->downr;
				if (hl != NULL) {
					if (hl->upl == help2)
						hl->upl = help1;
					if (hl->upm == help2)
						hl->upm = help1;
					if (hl->upr == help2)
						hl->upr = help1;
				}
				if (hm != NULL) {
					if (hm->upl == help2)
						hm->upl = help1;
					if (hm->upm == help2)
						hm->upm = help1;
					if (hm->upr == help2)
						hm->upr = help1;
				}
				if (hr != NULL) {
					if (hr->upl == help2)
						hr->upl = help1;
					if (hr->upm == help2)
						hr->upm = help1;
					if (hr->upr == help2)
						hr->upr = help1;
				}
				keephelp1 = help1->downnext;
				keephelp2 = help2->upnext;
				help1->downprev = help2->downprev;
				help1->downnext = help2->downnext;
				if (help1->downprev != NULL)
					help1->downprev->downnext = help1;
				if (help1->downnext != NULL)
					help1->downnext->downprev = help1;
				if (lo->er->bottomleft == help2)
					lo->er->bottomleft = help1;
				if (lo->er->bottomright == help2)
					lo->er->bottomright = help1;
				if (help1->t.left->right == help2)
					help1->t.left->right = help1;
				help1 = keephelp1;
				help2 = keephelp2;
			}
			else {
				if (samepoint(help1->t.left->e.vd,
						help2->t.left->e.vu)) {
					help1->downl = help2;
					help1->downm = help2;
					help1->downr = help2;
					help2->upl = help1;
					help2->upm = help1;
					help2->upr = help1;
					help1 = help1->downnext;
					help2 = help2->upnext;
				}
				else {
					fprintf(stderr," input error, edge intersection detected\n");
					exit(1);
				}
			}
		}
	}
	else  { 
		upper = up->er->bottomright;
		lower = lo->er->topright;
		bottom = upper->t.down->y;
		for (help1 = upper, help2 = lower, i = 0; 
		     (help1 != NULL) && (help2 != NULL);) {
			if (sameedges(help1->t.right, help2->t.right) &&
				!sameedges(help1->t.left,help2->t.left) ) {
				if (!samepoint(help1->t.left->e.vd,
						help2->t.left->e.vu)) {
					k = ecomp(
					 edxposition(help1->t.left,bottom,&j) ,
				         edxposition(help2->t.left,bottom,&j));
					if (k > 0) {
						help1->downl = help2;
						help1->downm = help2;
						help1->downr = help2;
						help2->upr = help1;
						help1 = help1->downprev;
					}
					else if (k < 0) {
						help2->upl = help1;
						help2->upm = help1;
						help2->upr = help1;
						help1->downr = help2;
						help2 = help2->upprev;
					}
					else {
						fprintf(stderr," input error, edge intersection detected\n");
						exit(1);
					}
				}
				else {
					help1->downl = help2;
					help1->downl = help2;
					help1->downl = help2;
					help2->upl = help1;
					help2->upm = help1;
					help2->upr = help1;
					help1 = help1->downprev;
					help2 = help2->upprev;
				}
			}
			else if (!sameedges(help1->t.right,help2->t.right) &&
				!sameedges(help1->t.left,help2->t.left) ) {
				if (help1->t.left->e.vd==help1->t.right->e.vd) {
					help1->downl = help2;
					help1->downm = help2;
					help1->downr = help2;
					help2->upm = help1;
					help1 = help1->downprev;
				}
				else if (help2->t.left->e.vu ==
					help2->t.right->e.vu) {
					help2->upl = help1;
					help2->upm = help1;
					help2->upr = help1;
					help1->downm = help2;
					help2 = help2->upprev;
				}
				else {
	/*	
					reporttrap(help1, DEFAULT);
					reporttrap(help2, DEFAULT);
					reporttrap(up, DEFAULT);
					reporttrap(lo, DEFAULT);
					fprintf(stderr," 2error in trap chains\n");
*/
					fprintf(stderr," input error, edge intersection detected\n");
					exit(1);
				}
			}
			else if (sameedges(help1->t.left,help2->t.left) &&
			 	  sameedges(help1->t.right,help2->t.right)) {
				help1->t.down = help2->t.down;
				hl = help1->downl = help2->downl;
				hm = help1->downm = help2->downm;
				hr = help1->downr = help2->downr;
				if (hl != NULL) {
					if (hl->upl == help2)
						hl->upl = help1;
					if (hl->upm == help2)
						hl->upm = help1;
					if (hl->upr == help2)
						hl->upr = help1;
				}
				if (hm != NULL) {
					if (hm->upl == help2)
						hm->upl = help1;
					if (hm->upm == help2)
						hm->upm = help1;
					if (hm->upr == help2)
						hm->upr = help1;
				}
				if (hr != NULL) {
					if (hr->upl == help2)
						hr->upl = help1;
					if (hr->upm == help2)
						hr->upm = help1;
					if (hr->upr == help2)
						hr->upr = help1;
				}
				keephelp1 = help1->downprev;
				keephelp2 = help2->upprev;
				help1->downprev = help2->downprev;
				help1->downnext = help2->downnext;
				if (help1->downprev != NULL)
					help1->downprev->downnext = help1;
				if (help1->downnext != NULL)
					help1->downnext->downprev = help1;
				if (lo->er->bottomright == help2)
					lo->er->bottomright = help1;
				if (lo->er->bottomleft == help2)
					lo->er->bottomright = help1;
				if (help1->t.right->left == help2)
					help1->t.right->left = help1;
				help1 = keephelp1;
				help2 = keephelp2;
			}
			else {
				if (samepoint(help1->t.right->e.vd,
						help2->t.right->e.vu)) {
					help1->downl = help2;
					help1->downm = help2;
					help1->downr = help2;
					help2->upl = help1;
					help2->upm = help1;
					help2->upr = help1;
					help1 = help1->downprev;
					help2 = help2->upprev;
				}
				else {
					fprintf(stderr," 4problem in trap chains\n");
					reporttrap(help1, DEFAULT);	
					reporttrap(help2, DEFAULT);	
					return 0;
				}
			}
		}
	}
	return 1;
}

void puttogether(TrapezoidL tr) {
/* Reccursively merges the trapezoids along the boundaries that have
  been artificially split. */

	int i, j, test;
	TrapezoidL help1, help2, help3, hp;
	Vertex keep;
	Region re;

	TrapezoidL disaster;

	if (tr == NULL) return;
	help1 = tr->downl;
	help2 = tr->downr;
	help3 = tr->downm;
	re = tr->er;
	if ((help1 == help2) && ((help1 == NULL) || (help1 == help3)) && 
		(help3 != NULL)) {
		keep = tr->t.down;
		for (hp = help3->er->topleft; hp != NULL; hp = hp->upnext) 
			if ((edxposition(hp->t.left,keep->y,&test) < keep->x) &&
			   (edxposition(hp->t.right,keep->y,&test) > keep->x)) {
				hp->upm = tr->er->bottomleft;
				tr->er->bottomleft->downm = hp;
				break;
			}
	}
	if ((help1 == help2) && (help1 != NULL) ||
	    (help1 != NULL) && (help2 == NULL) ||
	    (help2 != NULL) && (help1 == NULL) ) {
		if (help1 == NULL) 
			help1 = help2;
		if  (help1->upl == tr)   {
			if (!mergetwotraps(tr,help1,1)) {
				printf("mergeproblem1 %d %d\n\n",
				tr->t.no, help1->t.no);
				reporttrap(re->bottomleft, DEFAULT);
				reporttrap(help1->er->topleft, DEFAULT);
			}
			puttogether(help1);
		}
		else  if (help1->upr == tr) {
			if (!mergetwotraps(tr,help1,0)) {
				printf("mergeproblem2 %d %d\n\n",
					tr->t.no, help1->t.no);
				reporttrap(re->bottomright, DEFAULT);
				reporttrap(help1->er->topright, DEFAULT);
				}
		}
	}
	else if ((help1 != NULL) && (help2 != NULL) && (help1 != help2)) {
		if (help3 != NULL) {
			keep = help3->t.up;
			for (hp = tr->er->bottomleft;hp!=NULL;hp = hp->downnext) 
				if ((edxposition(hp->t.left,keep->y,&test) <
					keep->x)  &&
				   (edxposition(hp->t.right,keep->y,&test) >
					keep->x)) {
					hp->downm = help3->er->topleft;
					help3->er->topleft->upm = hp;
					break;
				}
		}
		if (!mergetwotraps(tr, help1, 1)) {
				printf("mergeproblem3  %d %d\n\n",
					tr->t.no, help1->t.no);
				reporttrap(re->bottomleft, DEFAULT);
				reporttrap(help1->er->topleft, DEFAULT);
		}
		if (!mergetwotraps(tr, help2, 0)) {
				printf("mergeproblem4 %d %d\n\n",
					tr->t.no, help2->t.no);
				reporttrap(re->bottomright, DEFAULT);
				reporttrap(help2->er->topright, DEFAULT);
		}
		puttogether(help1);
		if (help3 != NULL)
			puttogether(help3);
		puttogether(help2);
	}
}
	


void mergetraps(Region r) {
/* Replaces each trapezoid with the trapezoids inside it,
   it also puts trapezoids together if they are artificially
   divided. This can't happen for the trapezoids that are in the
   top or the bottom row. */

	int i, j, k;
	Region rhelp;
	EdgeL edg;
	TrapezoidL help1, help2;

	help1 = r->bottomleft;
	help2 = r->bottomleft->downnext;
	for (; help2 != NULL; help1 = help2, help2 = help2->downr) {
		help2->er->bottomleft->downprev = help1->er->bottomright;
		help1->er->bottomright->downnext = help2->er->bottomleft;
	}
	r->bottomleft = r->bottomleft->er->bottomleft;
	r->bottomright = r->bottomright->er->bottomright;

	for (help1 = r->topleft; help1 != NULL; help1 = help1->upnext) 
		puttogether(help1);

	help1 = r->topleft;
	help2 = r->topleft->upnext;
	for (; help2 != NULL; help1 = help2, help2 = help2->downr) {
		help2->er->topleft->upprev = help1->er->topright;
		help1->er->topright->upnext = help2->er->topleft;
	}
	r->topleft = r->topleft->er->topleft;
	r->topright = r->topright->er->topright;
}

void makeregion(TrapezoidL sol) {

	Region r;
	TrapezoidL helpl, helpr, first;
	EdgeL newed;

	if (sol == NULL) return;

/* Make a region for each existing trapezoid. */

	sol->er = makeRegion();
	r = sol->er;
	first = makeTrapezoidL();
	r->edges = makeEdgeL();
	newed = makeEdgeL();
	newed->e = sol->t.left->e;
	newed->right = first;
	r->leftedge = newed;
	newed = makeEdgeL();
	newed->e = sol->t.right->e;
	newed->left = first;
	r->rightedge = newed;
	r->topvertex = sol->t.up;
	r->bottomvertex = sol->t.down;
	r->topleft = first;
	r->topright = first;
	first->t.up = sol->t.up;
	first->t.down = sol->t.down;
	first->t.left = sol->t.left;
	first->t.right = sol->t.right;

	helpl = sol->downl;
	helpr = sol->downr;
	if ((helpl !=  NULL) && (helpr != NULL) &&
		(helpl != helpr)) {
		makeregion(helpl);
		if (sol->downm != NULL)
			makeregion(sol->downm);
		makeregion(helpr);
	}
	else  {
		if (helpl == NULL)
			helpl = helpr;
		if ((helpl != NULL) && ((helpl->upl == sol) ||
			(helpl->upl == NULL) && (helpl->upr == sol)) )
			makeregion(helpl);
	}
}


	
void edgesinregions(Region r, EdgeL ed, int now, int phases) {
/* Puts the edges of the new phase into the existing trapezoids. */

	EdgeL help, next, base, oldhelp;
	EdgeL ttt;
	TrapezoidL newt, curt;
	TrapezoidL trap;
	Region  newr, curr;
	int i, j, dir, olddir;

/* Follow the edges between the trapezoids	*/
/* get the first trapezoid. */

	curt = r->topleft->downl;
	for (base = ed->next; base != NULL ; base = base->next) 
		if (sameedges(base,curt->t.right))
			break;

	help = base;
	
	for (newt = nexttrap(curt,help,dir); curt!=newt; curt = newt);
	if (curt == NULL) 
			printf("IN E_I_R PROBLEM\n");

	if ((next = help->next) == NULL)
		next = ed->next;

	if ((next->e.vu == help->e.vu) || (next->e.vd == help->e.vu) )
		dir = 1;
	else
		dir = 0;

	for (; !sameedges(next, base);) {
		newt = nexttrap(curt,help,dir);
		if (newt == NULL) {
			if (sameedges(curt->t.right,help))  
				curt = curt->t.right->right;
			else if (sameedges(curt->t.left,help))
				curt = curt->t.left->left;
			printf(" CHANGE\n");
			if (curt == NULL) {
				printf(" next trap error\n");
				reporttrap(curt, DEFAULT);
                		printf("edge info %d\n",help->e.no);
                		printf("up? %d \n",dir);
                		printf("e  %f %f %f %f\n",
                                        help->e.vu->x,
                                        help->e.vu->y,
                                        help->e.vd->x,
                                        help->e.vd->y);
				ttt = NULL;
				ttt->e.no = 0;
			}
			dir = olddir;
			help = oldhelp;
		}
		else  {
			if (newt != curt) 
				curt = newt;
			else  {
				olddir = dir;
				oldhelp = help;
				if ((next = help->next) == NULL)
					next = ed->next;
				if (dir == 1) {
					if (next->e.vu == help->e.vu)
						dir = 0;
					else
						dir = 1;
				}
				else {
					if (next->e.vu == help->e.vd)
						dir = 0;
					else
						dir = 1;
				}
				help = next;
			}
			if ((help->e.phase == now) &&
			   (ecomp(help->e.vd->y, curt->t.up->y) < 0) &&
			   (ecomp(help->e.vu->y, curt->t.down->y) > 0)) {
				next = makeEdgeL();
				next->e.vu = help->e.vu;
				next->e.vd = help->e.vd;
				next->next = curt->er->edges->next;
				curt->er->edges->next = next;
				(curt->er->edges->e.phase)++;
			}
		}
	}
}


TrapezoidL trap_dec( int no, Vertex vts) {
/* Finds the trapezoid decomposition of the input polygon. */

        int i, k; 
	int phases;           /* number of phases */
        Region in, help;
        EdgeL edges, help11;
        TrapezoidL list;
        Treeroot tree;
        SVertex vtar;


        edges = makeedges(no, vts);
        edges->e.phase = no;
        phases = phaseno(no);  
        makephases(edges, no); /* partitions the edges into phases */

        in = makefirstregion(in, edges);
        findtraps(in);         /* first phase */

        for (i = 2; i <= phases; i++) {

                for (list = in->topleft; list != NULL; list = list->upnext)
                        makeregion(list);
 			/* this operation creates a new region for 
			   each trapezoid */		    

                edgesinregions(in, edges, i, phases);
 			/* puts the new edges to the existing region */

                for (list = in->topleft; list != NULL; list = list->upnext)
                        trapsintraps(list);
			/* partitions each region to trapezoids */

                mergetraps(in);
			/* puts together trapezoids that are artificially
			   split. also connects trapezoids from 
			   neighboring regions */

        }

	cleanup(in->topleft);

        return(in->topleft->downm);

		/* in->topleft->downm is the top trapezoid in the 
		   interior of the polygon */
}

TrapezoidL trap_dec_log( int no, Vertex vts) {
/* Finds the trapezoid decomposition of the input polygon. */
/* Uses makefirstregion1, results to an n log n algorithm. */

        Region in;
        EdgeL edges;

        edges = makeedges(no, vts);
        makephases(edges, 1);
        in = makefirstregion1(in, edges);
        findtraps(in);
        cleanup(in->topleft);
        return(in->topleft->downm);

}

