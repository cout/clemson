#ifndef TREE_H
#define TREE_H

#include <list>
#include <vector>
#include <map>
#include "biname.h"
#include "edge.h"

class Tree {
public:
	Tree() { n = 0; name = 0; }
	Tree(const vector<Edge> &e) { genTree(e); }
	Tree(const Tree& t) {
		child = t.child;
		heap = t.heap;
		parent = t.parent;
		n = t.n;
		name = t.name;
	}
	Tree& operator=(const Tree& t) {
		child = t.child;
		heap = t.heap;
		parent = t.parent;
		n = t.n;
		name = t.name;
		return *this;
	}
	Tree& operator=(const vector<Edge> &e) {
		genTree(e);
		return *this;
	}

	vector<list<int> > child;	// a child array
	vector<int> heap;		// a heap ordering
	vector<int> parent;		// a parent list
	int n;				// number of nodes in the tree
	Biname name;			// the global minname
	map<Biname, Biname> minname_table;

	void genLocalMinnames(vector<Biname> &r);
	void genGlobalMinname();
	void genTree(const vector<Edge> &e);

	friend bool operator==(Tree& t1, Tree& t2);
	friend ostream& operator<< (ostream& out, Tree& t);
};

#endif
