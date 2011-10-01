/* COPYRIGHT - Dimitrios Gunopulos, Princeton University, 1994 */
/* in this file the initialization functions, timing functions,
   and the main function that partitions a trapezoid to smaller
   ones (findtraps()) are implemented.
*/
    
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <assert.h>
#include "def.h"
#include "func.h"


#define logt  1/log10(8) 
#define log2(x) log10((double) (x))*logt
/*logt */

static vertex cleanvertex = {0, 0, NULL, NULL};
static vertexL cleanvertexL = {{0, 0, NULL, NULL}, NULL};
static svertex cleansvertex = {0.0, 0, NULL};
static edge cleanedge = {NULL, NULL, 0, 0, 0};
static edgeL cleanedgeL = {{NULL, NULL, 0, 0, 0}, NULL, NULL, NULL};
static trapezoidL cleantrapezoidL = {{NULL, NULL, NULL, NULL},
					NULL, NULL, NULL, NULL, NULL, NULL,
					NULL, NULL, NULL, NULL, NULL };
static region cleanregion = { NULL, NULL, NULL, NULL, NULL,
				NULL, NULL, NULL, NULL, NULL};

static trapno = 1;
static edgenum = 0; 
/* both for debugging purposes */

/* timing routines */

static struct rusage r1, r2;

void starttime () {

        getrusage (0, &r1);
}

long endtime () {

        getrusage (0, &r2);
        return ((r2.ru_utime.tv_usec-r1.ru_utime.tv_usec)/1000
                 + (r2.ru_utime.tv_sec-r1.ru_utime.tv_sec)*1000);
}

long totalendtime () {

        getrusage (0, &r2);
        return ((r2.ru_stime.tv_usec-r1.ru_stime.tv_usec)/1000
                 + (r2.ru_stime.tv_sec-r1.ru_stime.tv_sec)*1000);
}


/* a trapezoid like that must be returned every time,
so we can put the pieces together. */

int phaseno(int n) {
/* finds the number of the consecutive phases, given the   */
/* size (no of edges) of the input polygon.		  */

	int f;
	float no;

	for (no = n, f = 1; no >= 1; f++)
		no = log2(no);
	return (f-1);
 
}

int edgesinphase(int p, int n) {
/* p1 keeps the number of edges in current phase, and p2 */
/* keeps the number of edges assigned to phases so far.	 */

	int p1, p2, i;
	float no;

	for (i = 1, no = n, p1 = n, p2 = 0; (i <= p) && (no >= 1.0) ; i++) {
		no = log2(no);
		p1 = ((float) n)/no - p2;
		p2 += p1;
	}
	if (i != p) return -1;
	return p1;
}
		
void edgestophases(int n, int *ar) {
/* p1 keeps the number of edges assigned to phases so far.   */

	int p1, i;
	float no;

	for (i = 0, no = n, p1 = 0; (no >= 1.0); i++) {
		no = log2(no);
		p1 = ((float) n)/no;
		*(ar+i) = p1;
	}
	*(ar + (i-1)) = n;
}
		
Vertex makeVertex() {

	Vertex vt;

	vt = (Vertex) malloc(sizeof(vertex));
	assert( (vt != NULL) );
	*vt = cleanvertex;
	return vt;
}
	

VertexL makeVertexL() {

	VertexL vt;

	vt = (VertexL) malloc(sizeof(vertexL));
	assert( (vt != NULL));
	*vt = cleanvertexL;
	return vt;
}
	
EdgeL makeEdgeL() {

	EdgeL ed;

	ed = (EdgeL) malloc(sizeof(edgeL));
	assert((ed != NULL));
	*ed = cleanedgeL;
	ed->e.no = edgenum;
	edgenum++;
	return ed;
}

TrapezoidL makeTrapezoidL() {
	
	TrapezoidL tr;
	
	tr = (TrapezoidL) malloc(sizeof(trapezoidL));
	assert( (tr != NULL) );
	*tr = cleantrapezoidL;
	tr->t.no = trapno;
	trapno++;
	return tr;
}

Region makeRegion() {

	Region r;
	r = (Region) malloc(sizeof(region));
	assert( (r != NULL) );
	*r = cleanregion;
	return r;
}

int getrandom() {

	int i;
	struct timeval t1;
	struct timezone t2;

	gettimeofday(&t1,&t2);
	i = t1.tv_sec % 32656;
	return i;
}

void makephases(EdgeL list, int n) {
/* assigns at random the edges to the phases of the algorithm.   */

	int i, m, o, p;
	int *ar, *ind;
	EdgeL help;
	
	
	i = phaseno(n);

	srand(getrandom() );
	ar = (int *) malloc(i*sizeof(int));
	assert( (ar != NULL) );
	edgestophases(n,ar);
	list->e.phase = i;
	for (help = list->next, o = n; help && o; o--, help = help->next)  {
		m = (rand() % o);
		for (p = 0, ind = ar; p<i; p++, ind++)
			if (m < *ind)
				break;
		help->e.phase = p+1;
		for (ind = ar + p;p<i;p++, ind++)
			*ind -= 1;
	}
}
	
int readfile(char *str, Vertex vtarr) {
/* Fills the array of the vertices. Each vertex consists of   
   an x and a y coordinate, and two pointers to the edges     
   it is in. Used to read in the polygon if the polygon is    
   stored in a file using the "polygon" format. A maximum     
   of MAXV vertices can be stored.			    */

	Polygon input;
	int fd;
	int i,j,k,n;
	float *v;

	fd = open(str,O_RDONLY,0);
	if (fd >= 0) {
		read(fd,(char *) input, sizeof(polygon));
		close(fd);
	}
	else {
		fprintf(stderr,"Can't open file %s\n", str);
		return -1;
	}
	n = input->nverts;
	v = &(input->verts[0]);
	vtarr = (Vertex) malloc(n*sizeof(vertex));
	assert((vtarr != NULL));
	for (i=0, j=0; i<n; i++,j++) {
		(vtarr+i)->x = *(v+j);
		j++;
		(vtarr+i)->y = *(v+j);
		(vtarr+i)->e1 = NULL;
		(vtarr+i)->e2 = NULL;
	}
	(void)free((char *) input);
	return n;
} 

Vertex readinputvertex(int *N) {
/* This procedures reads the input polygon from standard 
   input. First it receives an integer, which is the number 
   of vertices in the polygon, and then the vertices, first
   x and then y coordinate.
   The coordinates are represented as floats.
*/

	int i, j, n;
	float x_c, y_c;
	Vertex ret, help;

	/* read no of vertices in n */
	scanf("%d", &n);
	*N = n;
	ret = (Vertex) malloc(n*sizeof(vertex));
	assert( (ret != NULL) );
	for (i = 0, help = ret; i<n; i++, help++) {
		scanf("%f %f",&x_c, &y_c);
		*help = cleanvertex;
		help->x = x_c;
		help->y = y_c;
	}
	return ret;
}
		
EdgeL makeedges(int n, Vertex vtarr) {
/* Assumes that the n vertices are stored in an array of n
   'vertex', and creates the equivalent edges. Does not check
   if any two edges thus created intersect each other.	The 
   first ellement of the list of edges, does not correspond
   to an edge, it just holds the length of the list.	*/

	int i, j, k;
	EdgeL ret, help;
	Vertex ind;

	ret = makeEdgeL();
	if (ret == NULL)  exit(2);
	ret->e.no = 0;
	ret->e.phase = n;
	help = ret;
	k = n-1;
	for (i = 0, ind = vtarr; i<k ; i++, ind++) {
		help->next = makeEdgeL();
		if (help->next == NULL)  exit(2);
		help = help->next;
		j = ecomp(ind->y, (ind+1)->y);
		if (j > 0) {
			help->e.vu  = ind;
			help->e.vd  = (ind + 1);
		}
		else if (j < 0) {
			help->e.vd  = ind;
			help->e.vu  = (ind + 1);
		}
		else  {
			fprintf(stderr,"Dublicate y coordinates %f %d\n",
				(ind + 1)->y, i);
			(ind + 1)->y += 2*MINUSE;
			help->e.vu  = ind;
			help->e.vd  = (ind + 1);
		}
		ind->e2 = help;
		(ind + 1)->e1 = help;
	}
	help->next = makeEdgeL();
	if (help->next == NULL)  exit(2);
	help = help->next;
	j = ecomp(ind->y, vtarr->y);
	if (j > 0) {
		help->e.vu  = ind;
		help->e.vd  = vtarr ;
	}
	else if (j < 0) {
		help->e.vd  = ind;
		help->e.vu  = vtarr;
	}
	else  {
		fprintf(stderr,"Dublicate y coordinates\n");
		vtarr->y += 2*MINUSE;
		help->e.vu  = ind;
		help->e.vd  = vtarr;
	}
	ind->e2 = help;
	vtarr->e1 = help;
	return ret;
}
			

int checkcrossings(int n, Vertex vtarr) {
/*  checks if the edges formed by these vertices cross. 	  
    compares everything to everything - n**2 time		*/

	int i, j, k, m, c1, c2, c3, c4;
	Vertex inti, intj;

	m = n-1;
	for (i = 0, inti = vtarr; i<m; i++, inti++) 
		for (j = 0, intj = vtarr; j<i-1; j++, intj++)  {
			c1 = ccw(inti,(inti+1), intj);
			c2 = ccw(inti,(inti+1), (intj+1));
			c3 = ccw(intj,(intj+1), inti);
			c4 = ccw(intj,(intj+1), (inti+1));
			if ((c1*c2 == -1) && (c3*c4 == -1) ||
                		(c1*c2*c3*c4 == 0)) {
				printf("edge 1: %f %f %f %f\n",
				 inti->x, inti->y, (inti+1)->x,(inti+1)->y);
				printf("edge 2: %f %f %f %f\n",
				 intj->x, intj->y, (intj+1)->x,(intj+1)->y);
				printf("the results : %d %d %d %d\n",
					c1, c2, c3, c4);
				return 1;
			}
			if (samepoint(intj,inti) ||
				samepoint((intj+1),inti) ||
				samepoint(intj,(inti+1)) ||
				samepoint((intj+1),(inti+1)) ) {
				printf("same points\n");
				printf("%f %f %f %f %f %f %f %f\n",
				 inti->x, inti->y, (inti+1)->x,(inti+1)->y,
				 intj->x, intj->y, (intj+1)->x,(intj+1)->y);
				return 1;
			}
		}
	for (i = 1, inti = vtarr+1, intj = vtarr+m; i<m-1; i++, inti++) {
		c1 = ccw(inti,(inti+1), intj);
		c2 = ccw(inti,(inti+1), vtarr);
		c3 = ccw(intj,vtarr, inti);
		c4 = ccw(intj,vtarr, (inti+1));
		if ((c1*c2 == -1) && (c3*c4 == -1) ||
               		(c1*c2*c3*c4 == 0)) {
                         printf("edge 1: %f %f %f %f\n",
                          inti->x, inti->y, (inti+1)->x,(inti+1)->y);
                         printf("edge 2: %f %f %f %f\n",
                          intj->x, intj->y, vtarr->x, vtarr->y);
                         printf("the results : %d %d %d %d\n",
                                        c1, c2, c3, c4);
			return 1;
		}
		if (samepoint(inti,vtarr) ||
			samepoint((inti+1),vtarr) ||
			samepoint(inti,intj) ||
			samepoint((inti+1),intj) ) {
                                printf("same points\n");
                                printf("one: %f %f two: %f\n three: %f %f four: %f %f %f\n",
                                 inti->x, inti->y, (inti+1)->x,(inti+1)->y,
                                 intj->x, intj->y, vtarr->x, vtarr->y);
				return 1;
		}
	}
	return 0;
}


int checkycoords(int n, Vertex vtarr) {
/*  finds if the input polygon has vertices with the same y 
    coordinate. If so attempts to "correct" the polygon, changing
    one of them. It uses sorting to find common y-coords.
*/

	int i;
	SVertex vtu;

	vtu = (SVertex) malloc(n*sizeof(svertex));
	assert((vtu != NULL));
	for (i = 0; i < n; i++) {
		*(vtu+i) = cleansvertex;
		(vtu+i)->y = (vtarr+i)->y;
		(vtu+1)->up = i;
	}
	quicksort(n-1,0,vtu);
	for (i = 1; i < n; i++) {
		if (ecomp((vtu+i-1)->y,(vtu+i)->y) == 0) {
			(vtu+i)->y += (2*PLUSE);
			(vtarr + (vtu+i)->up)->y += (2*MINUSE);
		}
	}
	(void)free((char *) vtu);
}
   

SVertex makevtarray (int n, EdgeL list) { 
/* 2 special vertices are created for each edge in the list.  */

	int i,j,k; 
	SVertex ret, rind;
	EdgeL help;

	ret = (SVertex) malloc(n*sizeof(svertex));
	assert( (ret != NULL) );
	for (i = 0, help = list->next, rind = ret;
		(i < n)&&help; i += 2, help = help->next) {
		rind->y = help->e.vu->y;
		rind->up = 1;
		rind->ed = help;
		rind++;
		rind->y = help->e.vd->y;
		rind->up = 0;
		rind->ed = help;
		rind++;
	}

	if (i < n)
		return NULL;
	return ret;
}
	
Region makefirstregion(Region R1, EdgeL list) {
/* creates the first region, that is enclosed in a rectangular
   formed by two artificially created edges. 	*/

	int i, j, k;
	EdgeL help, new;
	TrapezoidL first;
	float x_max, x_min, y_max, y_min, hm;
	Vertex vlimit;
	Edge elimit;

	R1 = makeRegion();
	if (R1 == NULL) exit(2);
	R1->edges = makeEdgeL();
	if ((list == NULL) || (list->next == NULL))
		R1 = NULL;
	else {
		y_max = y_min = list->next->e.vu->y;
		x_max = x_min = list->next->e.vu->x;
		new = makeEdgeL();
		R1->edges = new;
		for (i=0, help = list->next; help != NULL; help = help->next) {
			if (help->e.vu->y > y_max)
				y_max = help->e.vu->y;
			if (help->e.vd->y < y_min)
				y_min = help->e.vd->y;
			hm = max(help->e.vu->x, help->e.vd->x);
			if (hm > x_max)
				x_max = hm;
			hm = min(help->e.vu->x, help->e.vd->x);
			if (hm < x_min)
				x_min = hm;
			if (help->e.phase == 1) {
				new->next = makeEdgeL();
				new = new->next;
				*new = *help;
				new->next = NULL;
				i++;
			}
		}
		R1->edges->e.phase = i;
		R1->leftedge = makeEdgeL();
		R1->rightedge = makeEdgeL();
		R1->topvertex = makeVertex();
		R1->bottomvertex = makeVertex();
		R1->topvertex->y = y_max + 1;
		R1->topvertex->x = x_max + 1;
		R1->topvertex->e1 = R1->rightedge ;
		R1->bottomvertex->y = y_min - 1;
		R1->bottomvertex->x = x_min - 1;
		R1->bottomvertex->e1 = R1->leftedge;
		R1->rightedge->e.vu = R1->topvertex;
		R1->rightedge->e.vd = makeVertex();
		R1->rightedge->e.vd->y = y_min-1;
		R1->rightedge->e.vd->x = x_max+1;
		R1->rightedge->e.vd->e1 = R1->rightedge;
		R1->leftedge->e.vd = R1->bottomvertex;
		R1->leftedge->e.vu = makeVertex();
		R1->leftedge->e.vu->y = y_max+1;
		R1->leftedge->e.vu->x = x_min-1;
		R1->leftedge->e.vu->e1 = R1->leftedge;
		first = makeTrapezoidL();
		first->t.up = R1->topvertex;
		first->t.down = R1->bottomvertex;
		first->t.left = R1->leftedge;
		first->t.right = R1->rightedge;
		R1->topleft = first;
		R1->topright = first;
		R1->bottomleft = NULL;
		R1->bottomright = NULL;
		R1->leftedge->right = first;
		R1->rightedge->left = first;
	}
	return R1;
}

Region makefirstregion1(Region R1, EdgeL list) {
/* this is a modified makefirstregion, that puts all edges in the 
   first (and only) phase. it results to an O(nlogn) algorithm.
   used for debugging. 	*/
/* creates the first region, that is enclosed in a rectangular
   formed by two artificially created edges. 	*/

	int i, j, k;
	EdgeL help, new;
	TrapezoidL first;
	float x_max, x_min, y_max, y_min, hm;
	Vertex vlimit;
	Edge elimit;

	R1 = makeRegion();
	if (R1 == NULL) exit(2);
	R1->edges = makeEdgeL();
	if ((list == NULL) || (list->next == NULL))
		R1 = NULL;
	else {
		y_max = y_min = x_max = x_min = 0;
		new = makeEdgeL();
		R1->edges = new;
		for (i=0, help = list->next; help != NULL; help = help->next) {
			if (help->e.vu->y > y_max)
				y_max = help->e.vu->y;
			if (help->e.vd->y < y_min)
				y_min = help->e.vd->y;
			hm = max(help->e.vu->x, help->e.vd->x);
			if (hm > x_max)
				x_max = hm;
			hm = min(help->e.vu->x, help->e.vd->x);
			if (hm < x_min)
				x_min = hm;

			new->next = makeEdgeL();
			new = new->next;
			new->e = help->e;
			i++;
			new->e.no = i;
		}
		R1->edges->e.phase = i;
		R1->leftedge = makeEdgeL();
		R1->rightedge = makeEdgeL();
		R1->topvertex = makeVertex();
		R1->bottomvertex = makeVertex();
		R1->topvertex->y = y_max + 1;
		R1->topvertex->x = x_max + 1;
		R1->topvertex->e1 = R1->rightedge ;
		R1->bottomvertex->y = y_min - 1;
		R1->bottomvertex->x = x_min - 1;
		R1->bottomvertex->e1 = R1->leftedge;
		R1->rightedge->e.vu = R1->topvertex;
		R1->rightedge->e.vd = makeVertex();
		R1->rightedge->e.vd->y = y_min - 1;
		R1->rightedge->e.vd->x = x_max + 1;
		R1->rightedge->e.vd->e1 = R1->rightedge;
		R1->leftedge->e.vd = R1->bottomvertex;
		R1->leftedge->e.vu = makeVertex();
		R1->leftedge->e.vu->y = y_max + 1;
		R1->leftedge->e.vu->x = x_min - 1;
		R1->leftedge->e.vu->e1 = R1->leftedge;
		first = makeTrapezoidL();
		first->t.up = R1->topvertex;
		first->t.down = R1->bottomvertex;
		first->t.left = R1->leftedge;
		first->t.right = R1->rightedge;
		R1->topleft = first;
		R1->topright = first;
		R1->bottomleft = NULL;
		R1->bottomright = NULL;
		R1->leftedge->right = first;
		R1->rightedge->left = first;
	}
	return R1;
}
				
void quicksort (int r, int l, SVertex vta) {
/* Sorts the array of the special vertices in increasing order of   
   y coordinate.  */

 	float v, vr, vl, vm;
	svertex t;
	SVertex vti, vtj;
	int i,j;

	if (r > l) {
		i = l + (r - l)/2;
		vr = (vta+r)->y;
		vl = (vta+l)->y;
		vm = (vta+i)->y;
		if (vr > vl) {
			if (vl > vm) {
				t = *(vta+l);
				*(vta+l) = *(vta+r);
				*(vta+r) = t;
			}
			else if (vr > vm) {
				t = *(vta+i);
				*(vta+i) = *(vta+r);
				*(vta+r) = t;
			}
		}
		else if (vl > vr) {
			if (vm > vl) {
				t = *(vta+l);
				*(vta+l) = *(vta+r);
				*(vta+r) = t;
			}
			else if (vm > vr) {
				t = *(vta+i);
				*(vta+i) = *(vta+r);
				*(vta+r) = t;
			}
		}
		v = (vta+r)->y;
		i = l-1;
		j = r;
		for (;;) {
			for (vti=vta+i+1, i++; (i<r)&&(vti->y<v); i++, vti++);
			for (vtj=vta+j-1, j--; (j>l)&&(vtj->y)>v; j--, vtj--);
			t = *vti;
			*vti = *vtj;
			*vtj = t;
			if (j <= i) break;
		}
		*vtj = *vti;
		*vti = *(vta+r);
		*(vta+r) = t;
		quicksort(i-1,l,vta);
		quicksort(r,i+1,vta);
	}
	else if (r == (l+1)) 
		if ((vta+l)->y > (vta+r)->y) {
			t = *(vta+l);
			*(vta+r) = *(vta+l);
			*(vta+l) = t;
		}
}

int consec(EdgeL ed1, EdgeL ed2, float y, Region r) {
/* used to distinguish the possible different cases when we have to
   insert/delete two consecutive (in the polygon) edges in the tree. */

	if (ecomp(y,r->topvertex->y) > 0)
		return 0;
	if (ecomp(y,r->bottomvertex->y) < 0)
		return 0;
	if (sameedges(ed1,ed2)) 
		return 0;
	if (samepoint(ed1->e.vu, ed2->e.vu) && !ecomp(ed1->e.vu->y,y)) 
		return 1;      /* insert-insert */
	if (samepoint(ed1->e.vd, ed2->e.vd) && !ecomp(ed1->e.vd->y,y))
		return 2;      /* delete-delete */
	if (samepoint(ed1->e.vu, ed2->e.vd) && !ecomp(ed1->e.vu->y,y))
		return 3;      /* insert-delete */
	if (samepoint(ed1->e.vd, ed2->e.vu) && !ecomp(ed1->e.vd->y,y))
		return 4;      /* delete_insert */
	return 0;
}

void findtraps(Region r) {
/* given a region with its enclosing trapezoid and the edges
   that intersect it, finds the trapezoids these edges divide it. */

	int i, j, k, n;
 	TrapezoidL help, help1, help2, helpr;
	EdgeL pr, nxt, newed, newed2, exe;
	SVertex vta, vta2;
	Treeroot rbt;
	Treenode table;
	Treenode mark, mark2;

	if ((r != NULL) && (r->edges != NULL)) {
		n = r->edges->e.phase;
		k = 2*n;
		r->vertices = makevtarray(k, r->edges);
		quicksort(k-1, 0, r->vertices);
		rbt = init_rbtree(n+1);
		table  = rbt->table;
		for (i = k-1, vta = r->vertices + i; i > -1; i--, vta -= 1) {

/* initially, there are two cases. If we have consecutive edges in the */
/* input, consecutive vertices in vta array must be treated as one     */
	 	    newed = vta->ed;

		    if ((i>0) && (k = consec(newed, (vta-1)->ed, vta->y, r))) {
			i--;
			vta -= 1;
			newed2 = vta->ed;
			switch (k) {
			case 1:	
				mark = tree_insert(rbt, newed);
				if ((mark != NULL) && (mark->prev != 0) ) 
					pr = (table + mark->prev)->keyptr;
				else pr = r->leftedge;
				if ((mark != NULL) && (mark->next != 0))
					nxt = (table + mark->next)->keyptr;
				else nxt = r->rightedge;
				mark2 = tree_insert(rbt, newed2);
				help = makeTrapezoidL();
				help1 = makeTrapezoidL();
				help2 = makeTrapezoidL();
				helpr = pr->right;
				helpr->t.down = newed->e.vu;
				help->t.up = newed->e.vu;
				help1->t.up = newed->e.vu;
				help2->t.up = newed->e.vu;
				helpr->downl = help;
				helpr->downm = help1;
				helpr->downr = help2;
				pr->right = help;
				nxt->left  = help2;
				help->upl = helpr;
				help->upr = helpr;
				help1->upm = helpr;
				help2->upl = helpr;
				help2->upr = helpr;
				help->t.left = pr;
				if ((table+mark2->prev) == mark) {
					help->t.right = newed;
					help1->t.left = newed;
					help1->t.right = newed2;
					help2->t.left = newed2;
					newed->left = help;
					newed->right = help1;
					newed2->left = help1;
					newed2->right = help2;
				}
				else {
					help->t.right = newed2;
					help1->t.left = newed2;
					help1->t.right = newed;
					help2->t.left = newed;
					newed2->left = help;
					newed2->right = help1;
					newed->left = help1;
					newed->right = help2;
				}
				help2->t.right = nxt;
			  	break;
			case 2:
				mark = tree_locate(rbt, newed);
				mark2 = tree_locate(rbt, newed2);
				if ((mark->prev == 0) || (mark2->prev == 0))
					pr = r->leftedge;
				else if ((table+mark2->prev) == mark)
					pr = (table + mark->prev)->keyptr;
				else
					pr = (table + mark2->prev)->keyptr;
				if ((mark->next == 0) || (mark2->next == 0))
					nxt = r->rightedge;
				else if ((table + mark2->next) == mark)
					nxt = (table + mark->next)->keyptr;
				else
					nxt = (table + mark2->next)->keyptr;
				helpr = makeTrapezoidL();
				help = pr->right;
				help2 = nxt->left;
				if (newed->right == help2)
					help1 = newed->left;
				else
					help1 = newed->right;
				help->t.down = newed->e.vd;
				help1->t.down = newed->e.vd;
				help2->t.down = newed->e.vd;
				helpr->t.up = newed->e.vd;
				helpr->t.left = pr;
				helpr->t.right = nxt;
				pr->right = helpr;
				nxt->left = helpr;
				help->downl = helpr;
				help->downr = helpr;
				help1->downl = NULL;
				help1->downr = NULL;
				help1->downm = helpr;
				help2->downl = helpr;
				help2->downr = helpr;
				helpr->upl = help;
				helpr->upm = help1;
				helpr->upr = help2;
				tree_delete(rbt, newed);
				tree_delete(rbt, newed2);
				break;
			case 3: 
				exe = newed;
				newed = newed2;
				newed2 = exe;
			case 4:
				mark = tree_locate(rbt, newed);
				if ((mark != NULL) && (mark->prev != 0))
					pr = (table + mark->prev)->keyptr;
				else
					pr = r->leftedge;
				if ((mark != NULL) && (mark->next != 0))
					nxt = (table + mark->next)->keyptr;
				else
					nxt = r->rightedge;
				help = makeTrapezoidL();
				helpr = makeTrapezoidL();
				help1 = pr->right;
				help2 = nxt->left;
				tree_delete(rbt, newed);
				mark2 = tree_insert(rbt, newed2);
				help1->t.down = newed->e.vd;
				help2->t.down = newed->e.vd;
				help->t.up = newed->e.vd;
				helpr->t.up = newed->e.vd;
				help->t.left = pr;
				help->t.right = newed2;
				helpr->t.left = newed2;
				helpr->t.right = nxt;
				help1->downl = help;
				help1->downr = help;
				help2->downr = helpr;
				help2->downl = helpr;
				help->upl = help1;
				help->upr = help1;
				helpr->upr = help2;
				helpr->upl = help2;
				newed2->left = help;
				newed2->right = helpr;
				pr->right = help;
				nxt->left = helpr;
				break;
			}
		    }
		    else  {
			if (vta->up) {
				mark = tree_insert(rbt,newed);
				if ((mark != NULL) && (mark->prev != 0) ) 
					pr = (table + mark->prev)->keyptr;
				else pr = r->leftedge;
				if ((mark != NULL) && (mark->next != 0))
					nxt = (table + mark->next)->keyptr;
				else nxt = r->rightedge;
/* two rules: if the new edge touches the upper bound, the 
                 trapezoid it falls in splits to two
	      if the new edge starts below the upper bound,
		 the trapezoid it falls in ends there.
   assume that initially only one trapezoid exists.
   for the moment, assume that edges of each group can come
   at any time.
*/
				help = makeTrapezoidL();
				help1 = pr->right;
				if (ecomp(newed->e.vu->y,r->topvertex->y) >= 0){
					help->upnext = help1->upnext;
					help1->upnext = help;
					help->upprev = help1;
					if (help->upnext != NULL)
						help->upnext->upprev = help;
					help->t.right = help1->t.right;
					help->t.left = newed;
					newed->right = help;
					help1->t.right = newed;
					newed->left = help1;
					help->t.up = r->topvertex; /*newed->e.vu; */
					help1->t.up = r->topvertex; /* newed->e.vu; */
					pr->right = help1;
					nxt->left = help;
					if (help->upnext == NULL)
						r->topright = help;
				}
				else {
					helpr = makeTrapezoidL();
					help1->downl = help;
					help1->downr = helpr;
					help1->t.down = newed->e.vu;
					help->t.up = newed->e.vu;
					helpr->t.up = newed->e.vu;
					help->upl = help1;
					help->upr = help1;
					helpr->upl = help1;
					helpr->upr = help1;
					help->t.left = pr;
					pr->right = help;
					help->t.right = newed;
					newed->left = help;
					helpr->t.left = newed;
					newed->right = helpr;
					helpr->t.right = nxt;
					nxt->left = helpr;
					
				}
			}
			else {
				mark = tree_locate(rbt, newed);
				if ((mark != NULL) && (mark->prev != 0) ) {
					pr = (table + mark->prev)->keyptr;
				}
				else pr = r->leftedge;
				if ((mark != NULL) && (mark->next != 0))
					nxt = (table + mark->next)->keyptr;
				else nxt = r->rightedge;
/* the same rules here too. if the new edge touches the lower
   boundary or not	*/
				if (ecomp(newed->e.vd->y,r->bottomvertex->y)>0){
					help1 = pr->right;
					help2 = nxt->left;
					help = makeTrapezoidL();
					help1->downr = help;
					help1->downl = help;
					help1->t.down = newed->e.vd;
					help2->downl = help;
					help2->downr = help;
					help2->t.down = newed->e.vd;
					help->upl = help1;
					help->upr = help2;
					help->t.up = newed->e.vd;
					help->t.left = pr;
					pr->right = help;
					help->t.right = nxt;
					nxt->left = help;
					tree_delete(rbt, newed);
				}
				else {
/* in this case, just put these trapezoids in the botom list of the region */
					help1 = newed->left;
					help2 = newed->right;
					help1->downnext = help2;
					help2->downprev = help1;
					help1->t.down = r->bottomvertex; /*newed->e.vd; */
					help2->t.down = r->bottomvertex; 
					
					if (pr == r->leftedge)
						r->bottomleft = help1;
					if (nxt == r->rightedge) 
						r->bottomright = help2;
				}
			}
		    }
		}
		if (r->bottomleft == NULL)
			r->bottomleft = r->bottomright = r->leftedge->right;
		for (help = r->bottomleft; help != NULL; help = help->downnext) 
			help->t.down = r->bottomvertex;
	}
}


void givetrapno() {

 	printf("TRAPS %d\n", trapno -1);

}
