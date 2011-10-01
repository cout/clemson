#include <iostream.h>
#include "ternary.h"

int TTree::search(char *s) {
	register TNode *p=root;

	while(p) {
		if(*s < p->splitchar) p = p->lokid;
		else if (*s == p->splitchar) {
			if (*s++ == 0) return 1;
			p = p->eqkid;
		}
		else p = p->hikid;
	}

	return 0;
}

void TTree::insert(char  *s) {
	register TNode *p, **pp = &root;

	// Search for where the string is to be inserted
	while((p = *pp)) {
		// The compiler should be able to optimize this?
		if (*s == p->splitchar) {
			// If we reach the end of the string then it
			// must already be in the list
			if (*s++ == 0) return;
			pp = &(p->eqkid);
		}
		else if (*s > p->splitchar) pp = &(p->hikid);
		else pp = &(p->lokid);
	}

	// Allocate memory for the root of the subtree
	p = *pp = new TNode;
	p->splitchar = *s;
	p->lokid = p->hikid = 0;
	if(*s++ == 0) return;

	// Completely parse the rest of the subtree
	for(;;) {
		// p = p->eqkid = new TNode;
		if(bufn-- == 0) {
			buf = new TNode[bs];
			freearr[freen++] = (void *) buf;
			bufn = bs - 1;
			bs = bs << 1;
		}
		p = p->eqkid = buf++;

		p->splitchar = *s;
		p->lokid = p->hikid = 0;
		if(*s++ == 0) return;
	}
}

TNode* TTree::rinsert(TNode *p, char  *s) {
	if(p == 0) {
		p = new TNode;
		p->splitchar = *s;
		p->lokid = p->eqkid = p->hikid = 0;
	}
	if (*s < p->splitchar)
		p->lokid = rinsert(p->lokid, s);
	else if (*s == p->splitchar) {
		if (*s != 0)
		p->eqkid = rinsert(p->eqkid, ++s);
	}
	else p->hikid = rinsert(p->hikid, s);
	return p;
}
