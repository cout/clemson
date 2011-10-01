/* COPYRIGHT - Dimitrios Gunopulos, Princeton University, 1994 */
/*  This file contains routines for INDIRECT RED-BLACK TREES.		*/
/*  The routines have been copied and slightly modified from the book	*/
/*  "Introduction to Algorithms" by Cormen, Leiserson, and Rivest.	*/
/*  Some of these routines were implemented by Leonidas Palios.	*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "def.h"
#include "func.h"

#define	RED	0
#define	BLACK	1

int put_edge(EdgeL treeed, EdgeL  newed) {
/* Returns 1 if new edge is on left, -1 if it's on the right. */

	int test, kp;
	float ypupmax, yplomin, tx, nx, tuy, tdy, nuy, ndy;
	
	if (sameedges(treeed,newed)) return 0;

	tuy = treeed->e.vu->y; tdy = treeed->e.vd->y;
	nuy = newed->e.vu->y; ndy = newed->e.vd->y;

	yplomin = max(tdy, ndy);
	ypupmax = min(tuy, nuy);

	if ((( test = ecomp(ypupmax, yplomin)) > 0) && 
			(treeed->e.vu != newed->e.vu)) {
		if (ecomp(ypupmax, tuy) == 0) {
			tx = treeed->e.vu->x;
			nx = edxposition(newed,ypupmax,&test);
		}
		else {
			nx = newed->e.vu->x;
			tx = edxposition(treeed,ypupmax,&test);
		}
		if ((kp = ecomp(nx,tx)) > 0)
			return 1;
		else  if ( kp < 0)
			return -1;
		else  {
			fprintf(stderr,"Indistinguishable edges\n");
			exit(1);
		}
	}
	else if (samepoint(treeed->e.vu, newed->e.vu)) {
		if (!ecomp(yplomin, tdy)) {
			tx = treeed->e.vd->x;
			nx = edxposition(newed,yplomin,&test);
		}
		else {
			nx = newed->e.vd->x;
			tx = edxposition(treeed,yplomin,&test);
		}
		if (( kp = ecomp(nx,tx)) > 0)
			return 1;
		else  if (kp < 0)
			return -1;
		else  {
			fprintf(stderr,"Indistinguishable edges\n");
			return 0;
		}
	}
	else if (test == 0)  {
			return 1;
	}
	else {
		fprintf(stderr,"Error -- insert %d before deletion of %d \n",
			newed->e.no, treeed->e.no);
		exit(1);
	}
	return -1;
}

Treeroot init_rbtree(int n) {

	Treeroot r;
	Treenode t;

	r = (Treeroot) malloc(sizeof(treeroot));
	assert( (r != NULL) ) ;
	t = (Treenode) malloc(n*sizeof(treenode));
	assert( (t != NULL) );
	t->color = BLACK;	/* 0th element of table allocated to sentinel */
	t->keyptr = NULL;
	t->parent = 0;
	t->left = 0;
	t->right = 0;
	r->root = 0;	
	t->next = 0;
	t->prev = 0;
	r->cur = 0;
	r->table = t;
	return(r);
}


void left_rotate(Treeroot tree, int tr, Treenode base)  {
/* rotate left around base[tr]. */
/* ASSUMPTION: the right child of base[tr] is not NULL = sentinel */

	int   help;
	Treenode   t, x, p;

	t = base + tr;
	x = base + (help = t->right);
	if ((t->right = x->left) > 0)		/* update t->right */
		(base + t->right)->parent = tr; /* update reverse pointer */
	if ((x->parent = t->parent) == 0)		/* NULL x->parent */
		tree->root = help;		/* update root of tree */
	else {
		p = base + x->parent;		/* update x->parent */
		if (tr == p->left)		/* left child of its parent */
			p->left = help;
		else				/* right child of its parent */
			p->right = help;
	}
	x->left = tr;
	t->parent = help;
}


void  right_rotate(Treeroot tree, int tr, Treenode base) {
/* Rotate right around base[tr]. 
   Assumption: the left child of base[tr] is not NULL = sentinel */

	int  help; 
	Treenode t, x, p;

	t = base + tr;
	x = base + (help = t->left);
	if ((t->left = x->right) > 0)		/* update t->left */
		(base + t->left)->parent = tr; /* update reverse pointer */
	if ((x->parent = t->parent) == 0)		/* NULL x->parent */
		tree->root = help;		/* update root of tree */
	else  {
		p = base + x->parent;		/* update x->parent */
		if (tr == p->left)		/* left child of its parent */
			p->left = help;
		else				/* right child of its parent */
			p->right = help;
	}
	x->right = tr;
	t->parent = help;
}


Treenode tree_locate(Treeroot tree, EdgeL ed) {
/* Returns NULL if such an edge was not found; else the corresponding */
/* node of the tree */

	int   tr;
	Treenode t, base;

	base = tree->table;
	for (tr = tree->root; tr > 0; )  {
		t = base + tr;
		switch ( put_edge(t->keyptr, ed) ) {
		 	case -1:	
				tr = t->left;
			 	break;
			case  1:
				tr = t->right;
			 	break;
			case  0: 
				return t;
		}
	}
	return NULL;
}

Treenode  tree_simpleinsert(Treeroot tree, EdgeL ed) {
/* Insert the edge ed at the appropriate place in the tree */

	int  k, tr, help, help2, test;
	float x_pos, y_pos, x_comp;
	Treenode  t, x, base;

	base = tree->table;
	for (help = tree->root, help2 = 0; help > 0; ){
		help2 = help;
		x = base + help;
		k = put_edge(x->keyptr,ed);
		if (k == 0)  {
			fprintf(stderr,"Edge %d already in red-black tree\n",
			ed->e.no);
			return(NULL);
		}
		if (k < 0)
			help = x->left;
		else 
			help = x->right;
	}

	tr = ++(tree->cur);
	t = base + tr;
	t->keyptr = ed;
	t->color = RED;
	t->left = 0;
	t->right = 0;
	t->parent = help2;
	if (help2 == 0) {
		tree->root = tr;
		t->prev = t->next = 0;
	}
	else
		if (k < 0) {
			x->left = tr;
			t->prev = x->prev;
			if (x->prev > 0)
				(base + x->prev)->next = tr;
			t->next = help2;
			x->prev = tr;
		    }
		else {
			x->right = tr;
			t->next = x->next;
			if (x->next > 0)
				(base + x->next)->prev = tr;
			t->prev = help2;
			x->next = tr;
		}
	return(t);
}


Treenode  tree_insert(Treeroot tree, EdgeL ed) {
/* insert in a red-black tree */
/* return 0 if insertion was completed, 1 otherwise */

	int  parent;
	Treenode  t, p, pp, y, base, ret;

	if ((t = tree_simpleinsert(tree, ed)) == NULL)
		return(NULL);

	ret = t;
	base = tree->table;
	/* ensure the red-black property */
	while ((parent = t->parent) > 0 && (p = base + parent)->color == RED){
		pp = base + p->parent;
		if (parent == pp->left){
			y = base + pp->right;
			if (y->color == RED){	/* y not NULL */
				p->color = y->color = BLACK;
				pp->color = RED;
				t = pp;
			}
			else {
				if (t == base + p->right){
					left_rotate(tree, parent, base);
					p = t;	/* but pp remains unchanged */
				}
				p->color = BLACK;
				pp->color = RED;
				right_rotate(tree, p->parent, base);
				break;
			}
		}
		else{
			y = base + pp->left;
			if (y->color == RED){	/* y not NULL */
				p->color = y->color = BLACK;
				pp->color = RED;
				t = pp;
			}
			else{
				if (t == base + p->left){
					right_rotate(tree, parent, base);
					p = t;	/* but pp remains unchanged */
				}
				p->color = BLACK;
				pp->color = RED;
				left_rotate(tree, p->parent, base);
				break;
			}
		}
	}
	(base + tree->root)->color = BLACK;
	return(ret);
}


void rb_fix(Treeroot tree, int tr) {

	int  parent, help;
	Treenode  t, p, w, base = tree->table;

	t = base + tr;
	while ((parent = t->parent) > 0 && t->color != RED){
		p = base + parent;
		if (tr == p->left){
				/* w never NULL, as color(t) = BLACK */
			w = base + (help = p->right);
			if (w->color == RED){  /* left(w), right(w) not NULL */
				w->color = BLACK;
				p->color = RED;
				left_rotate(tree, parent, base);
				w = base + (help = p->right);
			}
			if ((base + w->left)->color != RED &&
					(base + w->right)->color != RED){
				w->color = RED;
				tr = parent;
				t = p;
			}
			else{
				if ((base + w->right)->color != RED){
					(base + w->left)->color = BLACK;
					w->color = RED;
					right_rotate(tree, help, base);
					w = base + (help = p->right);
				}
				w->color = p->color;
				p->color = (base + w->right)->color = BLACK;
				left_rotate(tree, parent, base);
				break;	/* note that color(t) = BLACK */
			}
		}
		else{
			w = base + (help = p->left);
			if (w->color == RED){
				w->color = BLACK;
				p->color = RED;
				right_rotate(tree, parent, base);
				w = base + (help = p->left);
			}
			if ((base + w->left)->color != RED &&
					(base + w->right)->color != RED){
				w->color = RED;
				tr = parent;
				t = p;
			}
			else{
				if ((base + w->left)->color != RED){
					(base + w->right)->color = BLACK;
					w->color = RED;
					left_rotate(tree, help, base);
					w = base + (help = p->left);
				}
				w->color = p->color;
				p->color = (base + w->left)->color = BLACK;
				right_rotate(tree, parent, base);
				break;	/* note that color(t) = BLACK */
			}
		}
	}
	t->color = BLACK;
}


void tree_delete(Treeroot tree, EdgeL ed)  {

	int  tr, parent, help;
	Treenode t, p, x, base;

	base = tree->table;
	t = tree_locate(tree, ed);
	if (t == NULL) { 
		fprintf(stderr,"Entry not found in the tree %d.\n",ed->e.no);
		exit(1);
	}

	tr = t - base;

	if (t->left > 0 && t->right > 0) {		/* t has 2 children */
		/* find substitute: treenode with key immediately larger    */
		/* than key in t.  Note that this node has no left child    */

		p = base + t->next;
		t->keyptr = p->keyptr;		/* copy keyptr from p to t */
		t->next = p->next;
		if (t->next > 0)
			(base + t->next)->prev = tr;
		t = p;
	}
	else {
		/* remove t from the list first */
		if (t->prev > 0)
			(base + t->prev)->next = t->next;
		if (t->next > 0)
			(base + t->next)->prev = t->prev;
	}

	/* splice t out */
	if (t->left > 0)	/* not NULL left child */
		help = t->left;
	else
		help = t->right;	/* (help = 0) <==> t has no children */
	tr = t - base;
	x = base + help;
	parent = t->parent;
	p = base + parent;
	x->parent = parent;
	if (parent == 0) {	/* t was root of tree of size <= 2 */
		tree->root = help;
		if (help != 0)
			x->color = BLACK;
	}
	else	/* update t->parent */
		if (tr == p->left)	/* t is left child of its parent */
			p->left = help;
		else
			p->right = help;
	if (t->color != RED)
		rb_fix(tree, help);
	
	help = tree->cur;
	x = base + help;
	if (tr != help) {
		*t = *x;
		p = base + t->parent;
		if (p == base)
			tree->root = tr;
		else {
			if (p->left == help)
				p->left = tr;
			if (p->right == help)
				p->right = tr;
		}
		if (t->left > 0)
			(base + t->left)->parent = tr;
		if (t->right > 0)
			(base + t->right)->parent = tr;
		if (t->prev > 0)
			(base + t->prev)->next = tr;
		if (t->next > 0)
			(base + t->next)->prev = tr;
	}

	(tree->cur)--;
	if (tree->root < 0) printf("root - end of delete\n");
}


void destroy_rbtree(Treeroot tree) {
/* free the space allocated for the tree records and tree root */
 
	(void)free((char *)tree->table);
	(void)free((char *)tree);
}


void	visit(int tr, Treenode base, int ident) {

	int		k;
	Treenode	t = base + tr;

	if ((k = t->left) > 0)		/* t->left != sentinel */
		visit(k, base, ident+1);
	
	for (k=ident; k>0; k--)
		printf("     ");
	printf("(");
/*
	printf("%f %f %f %f %d\n",t->keyptr->e.vu->x, 
				t->keyptr->e.vu->y,
				t->keyptr->e.vd->x,
				t->keyptr->e.vd->y,
*/
	printf("%d\n",t->keyptr->e.no);
	if (t->color == RED)
		printf(",RED)\n");
	else
		printf(",BLACK)\n");

	if ((k = t->right) > 0)		/* t->right != sentinel */
		visit(k, base, ident+1);
}


void	print_tree(Treeroot tree) {
 
	int	k;

	printf("\n");
	if ((k = tree->root) == 0)
		printf("The red-black tree is EMPTY.\n");
	else
		visit(k, tree->table, 0);
}


void	print_table(Treeroot tree) {
 
	int		i;
	Treenode	t;

	printf("\nCur = %d, Root =%d  \n", tree->cur,tree->root );
	for (i=1, t= tree->table + 1; i <= tree->cur; i++, t++){
		printf("t[%d] = (key= <", i);
		if (t->keyptr == NULL)
			printf("NULL");
		else {
			printf("%f %f %f %f %d",t->keyptr->e.vu->x, 
				t->keyptr->e.vu->y,
				t->keyptr->e.vd->x,
				t->keyptr->e.vd->y,
				t->keyptr->e.no);
		}
		if (t->parent == 0)
			printf(">, ROOT, ");
		else
			printf(">, parent=%d, ", t->parent);
		printf("left=%d, right=%d)\n", t->left, t->right);
	}
}
