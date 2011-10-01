/* COPYRIGHT - Dimitrios Gunopulos, Princeton University, 1994 */
/* This module iplements the necessary functions 
   for the geometrical manipulations  */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "func.h"

void reporttrap(TrapezoidL, int);

int eq(float x, float y) {

	if (fabs(x-y) < PLUSE) return 1;
	return 0;
}

int samepoint(Vertex a,Vertex b) {
/* If both coordinates differ very little, the two points are 
   assumed to be the same. */

/*
	if ((a == NULL) || (b == NULL))
		return 0;
	if (eq(a->x,b->x) && eq(a->y,b->y))
		return 1;
	return 0;
*/

	if ((a == b) && (a != NULL))
		return 1;
	return 0;

}

int order (Vertex a,Vertex b, Vertex c) { 
/* This function assumes that the three points are colinear, and
   returns 0 if c is between a and b, 2 if b is between a and c, 
   and -2 if a is between b and c.   */

	float ax, ay, bx, by, cx, cy;

	ax = a->x; ay = a->y;
	bx = b->x; by = b->y;
	cx = c->x; cy = c->y;
	if (eq(ax,bx) && eq(ax,cx) && eq(bx,cx))
		if (ay < cy) 
		    if (cy < by) return 0;
		    else if (by < ay) return -2;
		    else return 2;
		else if (cy > by) return 0;
		       	else if (ay < by) return -2;
			else return 2;
	else if (ax < cx)
		    	if (cx <bx) return 0;
			else if (bx < ax) return -2;
			else return 2;
		else if (cx > bx) return 0;
			else if (ax < bx) return -2;
			else return 2;
}

float length(Vertex a,Vertex b) {     
/* Computes the length of the line segment that connects the two points. */

	return (float) sqrt((double) 
			((a->x-b->x)*(a->x-b->x) + (a->y-b->y)*(a->y-b->y)));
}

float distance(Vertex a, Vertex b,Vertex c) {   
/* Computes the distance of point c from the line that passes through
points a and b. */

	float ret;
	float ax, ay, bx, by, cx, cy;

	ax = a->x; ay = a->y;
	bx = b->x; by = b->y;
	cx = c->x; cy = c->y;
	if (eq(ax,bx)) return (fabs(ax - cx));
        ret=fabs((cy-(-cx*by+cx*ay+ax*by-bx*ay)/(ax-bx))*
			((float) cos(atan((double) ((ay-by)/(ax-bx))))));
	return ret;
}

int colinear(Vertex a,Vertex b,Vertex c) {   
/* Returns >=1 if the three points lay in the same lane, 0 otherwise. 
   it assumes that the three points are distinct. */

	if (distance(a,b,c) < PLUSE )
		return 1;
	if (distance(a,c,b) < PLUSE)
		return 2;
	if (distance(c,b,a) < PLUSE)
		return 3;
	return 0;
}

int turn(Vertex a,Vertex b, Vertex c) {	      
/* Assumes points are distict and c does not lie in the line of a,b.
   Returns 1 or -1 for a rigrt or a left turn respectively. */

	float ax, ay, bx, by, cx, cy;

	ax = a->x; ay = a->y;
	bx = b->x; by = b->y;
	cx = c->x; cy = c->y;
	if (fabs(ax - bx) > PLUSE)  	
		if ((ax*cy - cy*bx + cx*by
			- cx*ay + bx*ay - ax*by) > 0)
			return 1;
		else return -1;
	else if (cx > ax) 
		if (ay > by) return -1;
		else return 1;
	else if (ay > by) return 1;
		else return -1;
}
	
int CCW(Vertex a, Vertex b, Vertex c)        {
/* To the left is -1, to the right 1 */

        float pos, neg;
        pos = (b->x - a->x) * (c->y - a->y);
        neg = (c->x - a->x) * (b->y - a->y);
        if (pos > (neg + PLUSE) ) return(-1);
        if (pos < (neg - PLUSE) ) return(1);
        return(0);
}

int ccw (Vertex a, Vertex b, Vertex c) {	      
/* Implements the CCW primitive (the procedure used). */

	if (samepoint(a,b))
		if (samepoint(a,c))
			return 2;
		else return 2;
	else if (samepoint(a,c) || samepoint(b,c))
		return 2;
	else if (colinear(a,b,c) > 1)
		return order(a,b,c);
	else return turn(a,b,c);
}


float min(float x, float y) {

        if (x < y) return x;
        return y;

}


float max(float x, float y) {

        if (x > y) return x;
        return y;

}

int ecomp(float  x, float  y) {

        float  s;

        s = x - y;
        if ((s < PLUSE) && (s > MINUSE))
                return 0;
        if (s >= PLUSE)
                return 1;
        return -1;
}

float edxposition(EdgeL ed, float yp, int *sst) {
/* Gives the x coordinate of the point in edge ed     
   that has a y coordinate of y.                  */

	int i;
        float  x , y1, y2, x1, x2, dy;

	x1 = ed->e.vu->x;
	x2 = ed->e.vd->x;
	y1 = ed->e.vu->y;
	y2 = ed->e.vd->y;
	if ((ecomp(max(y1, y2),yp) < 0) || (ecomp(min(y1, y2), yp) > 0))
		*sst = 0;
	else 	
		*sst = 1;
	dy = y2 - y1;
	if (ecomp(dy,0.0) == 0) 
		return x1;
	x = x1 + (x2 - x1) * (yp - y1) / dy;
	return x;
}

float edxposition2(EdgeL ed, float yp) {
/* Gives the x coordinate of the point in edge ed   
   that has a y coordinate of y.                  */

	int i;
        float  x , y1, y2, x1, x2, dy;

	x1 = ed->e.vu->x;
	x2 = ed->e.vd->x;
	y1 = ed->e.vu->y;
	y2 = ed->e.vd->y;
	dy = y2 - y1;
	if (ecomp(dy,0.0) == 0) 
		return x1;
	x = x1 + (x2 - x1) * (yp - y1) / dy;
	return x;
}


int sameedges(EdgeL ed1, EdgeL ed2) {

	if ((ed1->e.vu == ed2->e.vu) && (ed1->e.vd == ed2->e.vd))
		return 1;
	return 0;
}

int eet(EdgeL ed, TrapezoidL trap, int *up, int *down) {
/* Returns two sets of values, one for each edge endpoint.
   For up: 0 - in trap,
	   1 - coincides with trap's upper endpoint,
	  -1 - no intersection
	   3 - leaves through upper left parent
	   4 - leaves through upper right parent
   and similarly for down.
   The pointer is where to go next.
*/

	int test;
	EdgeL el;
	float eux, edx, tmxy, tmny, emxy, emny, mn, sv;

	tmxy = trap->t.up->y;
	tmny = trap->t.down->y;
	emxy = ed->e.vu->y;
	emny = ed->e.vd->y;
	if ((ecomp(tmny , emxy) > 0) || (ecomp(emny , tmxy)>0)) {
		*up = *down = -1;
		return 1;
	}
	if ((ecomp(tmxy , emxy)>0) && (ecomp(emny,tmny)>0)) {
		*up = *down = 0;
		/* have to check here if ed is outside trap   */
		eux = edxposition(trap->t.left,emxy,&test);
		edx = ed->e.vu->x;
		if (ecomp(eux,edx) >= 0)
			*up = *down = -1;
		else {
			eux = edxposition(trap->t.right,emxy,&test);
			if (ecomp(eux,edx) <= 0)
				*up = *down = -1;
		}
	/*
		return 1;
	 this is because edges don't intersect */
	}
	if (trap->t.up == ed->e.vu) {
	/* see if it's inside or not */
		mn = max(emny,tmny);
		if ((sv = (edxposition(ed,mn,&test)) > 
			edxposition(trap->t.right,mn,&test)))
			*up = 4;
		else if (sv < edxposition(trap->t.left,mn,&test))
			*up = 3;
		else
			*up = 1;
	}
	else if (tmxy > emxy) 
			*up = 0;
		else  if (trap->t.up != ed->e.vd) {
			if ((edxposition(ed,tmxy,&test)) < trap->t.up->x)
				*up = 3;
			else
				*up = 4;
		}
		else {
			if (trap->upl  != NULL)
				el = trap->upl->t.right;
			else if (trap->upr != NULL)
				el = trap->upr->t.left;
			else 
				el = trap->upm->t.left;
			mn = min(el->e.vu->y, emxy);
			if (edxposition(el,mn,&test) < edxposition(ed,mn,&test))
				*up = 4;
			else 
				*up = 3;
		}
	if (trap->t.down == ed->e.vd) {
		mn = min(emxy, tmxy);
		if ((sv = edxposition(ed,mn,&test)) >
			edxposition(trap->t.right,mn,&test))
			*down = 4;
		else if (sv < edxposition(trap->t.left,mn,&test))
			*down = 3;
		else
			*down = 1;
	}
	else 
		if (tmny < emny)
			*down = 0;
		else  if (trap->t.down != ed->e.vu) {
			if (edxposition(ed, tmny,&test) < trap->t.down->x)
				*down = 3;
			else 
				*down = 4;
		}
		else {
			if (trap->downl != NULL)
				el = trap->downl->t.right;
			else if (trap->downr != NULL)
				el = trap->downr->t.left;
			else 
				el = trap->downm->t.left;
			mn = max(el->e.vd->y, emny);
			if (edxposition(el,mn,&test) < edxposition(ed,mn,&test))
				*down = 4;
			else
				*down = 3;
		}

	return 1;
}
			

	
	
TrapezoidL nexttrap(TrapezoidL trap, EdgeL ed, int up) {
/* Finds the next trapezoid.   
   If it returns trap, move to the next edge. */


	int upv, downv;
	TrapezoidL ret;
	
	eet(ed, trap, &upv, &downv);
	ret = NULL;
	if (up == 1) {
	/* the next edge follows the upper vertex */
		if (upv == 0) 
			ret = trap;
		if (upv == -1) {
			if (downv == 3)
				return trap->downl;
			else if (downv == 4) 
				if ((ret = trap->downr) != NULL)
					return ret;
				else
					return trap->downl;
			fprintf(stderr,"wrong trap\n");
			ret = NULL;
		}
		if (upv == 1)
			if (((trap->t.right->e.vu == trap->t.up) ||
				(trap->t.left->e.vu == trap->t.up)) &&
				(trap->t.left->e.vu != trap->t.right->e.vu))
				ret = trap->upl;
			else 
				if ((ret = trap->upm) == NULL)
					ret = trap->upl;
		if (upv == 3)
			if ((ret = trap->upl) == NULL)
				ret = trap->upm;
		if (upv == 4) 
			if ((ret = trap->upr) == NULL)
				ret = trap->upm;
			if (ret == NULL)
				ret = trap->upl;
	}
	else  {
	/* the next edge follows the lower vertex */
		if (downv == 0) 
			ret = trap;
		if (downv == -1) {
			if (upv == 3)
				return trap->upl;
			else if (upv == 4)
				if ((ret = trap->upr) != NULL)
					return ret;
				else
					return trap->upl;
			fprintf(stderr,"wrong trap\n");
			return NULL;
		}
		if (downv == 1)
			if (((trap->t.right->e.vd == trap->t.down) ||
				(trap->t.left->e.vd == trap->t.down)) &&
				(trap->t.left->e.vd != trap->t.right->e.vd))
				ret = trap->downl;
			else 
				if ((ret = trap->downm) == NULL)
					ret = trap->downl;
		if (downv == 3)
			if ((ret = trap->downl) == NULL)
				ret = trap->downm;
		if (downv == 4) 
			if (( ret = trap->downr) == NULL)
				ret = trap->downm;
			if (ret == NULL)
				ret = trap->downl;
	}
	if (ret == NULL)
		{
		reporttrap(trap, DEFAULT);
		printf("edge info %d\n",ed->e.no);
		printf("up? %d \n",up);
		printf("e2  %f %f %f %f\n",
					ed->e.vu->x,
					ed->e.vu->y,
					ed->e.vd->x,
					ed->e.vd->y);
		printf("-ERROR in next trap\n");
		ret->t.left = NULL;
	}
	return ret;
}
