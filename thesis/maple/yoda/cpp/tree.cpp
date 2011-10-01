#include <list>
#include <vector>
#include <algorithm>
#include "biname.h"
#include "tree.h"

// genLocalMinnames is equivalent to the original LocalMinnameArr function.
// It generates the minname (the lexicographically least binname) for each
// possible node in a tree by creating a list of local minnames.
// Output:
//   l - a list of binames, where l[x] gives the local minname of the branch
//       of the tree rooted at node x.
// NOTE: The root must receive the highest label, namely, n.
void Tree::genLocalMinnames(vector<Biname> &r) {

	list<Biname> s;

	// Allocate space in r for the binames
	r.clear();
	Biname empty_biname;
	for(int j = 0; j < n; j++) r.push_back(empty_biname);

	// Traverse the entire tree using the tree's heap ordering.
	// Note that r is created from the bottom-up, so that minnames of
	// lower nodes are used to create the minnames of the higher nodes.
	for(int i = 0; i < n; i++) {
		int x = heap[i];
		s.clear();
		list<int>::iterator it;
		for(it = child[x].begin(); it != child[x].end(); it++)
			s.push_back(r[*it]);
		r[x].smerg(s);
	}
}

// genGlobalMinname returns the minname for an unrooted tree.  The
// procedure starts with a local best labelling rooted at n.  Then, the
// procedure moves from parent w to child v throughout the tree, modifying
// the existing local labellings to produce the best local labellings
// tmp_minnames for each node as root.  Along the way, a local branch
// labelling tmp_binames is required at the parent of v, by regarding v as
// the root and w as the root of one branch at v.
void Tree::genGlobalMinname() {

	vector<Biname> r;

	// Generate an array of local minnames
	// We start with the root we are given, then work our way to
	// other roots.
	genLocalMinnames(r);
	
	// Here we should check to see if r[n] (the minname of the root)
	// has already been found.  This isn't necessary, however, though
	// it gives a significant speed increase.

	vector<Biname> tmp_minnames;		// t
	vector<Biname> tmp_binames;		// tt
	tmp_minnames.reserve(n);
	tmp_binames.reserve(n);
	tmp_minnames[n-1] = r[n-1];
	Biname best = r[n-1];

	// Try different roots until we find the one with the
	// lexicographically least associated minname.
	int v, w;
	for(int i = n - 2; i >= 0; i--) {
		list<Biname> s;
		list<int>::iterator it;

		v = heap[i];			// v is our new root
		w = parent[v];			// w is the parent of v
		s.clear();
		s.push_back(tmp_binames[w]);

		// Find the minname of the tree from w and below, not
		// counting v.
		for(it = child[w].begin(); it != child[w].end(); it++) {
			if(*it != v) s.push_back(r[*it]);
		}
		tmp_binames[v].smerg(s);

		// We want to include the above biname as part of our
		// new biname.
		s.clear();
		s.push_back(tmp_binames[v]);

		// Find the minname of the tree from v and below.
		for(it = child[v].begin(); it != child[v].end(); it++) {
			s.push_back(r[*it]);
		}
		tmp_minnames[v].smerg(s);

		if(tmp_minnames[v] < best) best = tmp_minnames[v];
	}

	// Insert each minname t[i] into the lookup table
	// (not yet implemented)

	// Set the tree's name to the lexicographically least minname
	name = best;
}

// genTree (childe) generates the child array c, the heap ordering h, and
// the parent list p from an input edge list for a spanning subtree.
void Tree::genTree(const vector<Edge> &e) {
	int itemsleft;

	// Find the largest element; we assume this to be the number of
	// vertices in the tree.
	n = 0;
	for(vector<Edge>::const_iterator it = e.begin(); it != e.end(); it++) {
		if((*it)[0] > n) n = (*it)[0];
		if((*it)[1] > n) n = (*it)[1];
	}
	n++;

	// Allocate memory for our temporary data structures
	bool *undun = new bool[n];
	bool *doitnow = new bool[n];
	bool *doitlater = new bool[n];
	int *pa = new int[n];
	int i, x, y;

	// Initialize our temporary data structures
	undun[0] = false;
	child.clear();
	list<int> empty_list;
	for(i = 0; i < n-1; i++) {
		undun[i] = true;
		doitnow[i] = doitlater[i] = false;
		child.push_back(empty_list);
	}

	child.push_back(empty_list);
	doitnow[n-1] = true;
	doitlater[n-1] = false;

	vector<int> h;
	h.clear();
	h.push_back(n-1);

	itemsleft = n-1;		// is this right?
	while(itemsleft > 0) {
		for(x = 0; x < n; x++) {
			if(!doitnow[x]) continue;
			for(i = 0; i < n-1; i++) {
				if(!undun[i]) continue;
				if(x == e[i][0] || x == e[i][1]) {
					// if the edge is connected
					if(x == e[i][0])
						y = e[i][1];
					else
						y = e[i][0];
					child[x].push_back(y);
					pa[y] = x;
					h.push_back(y);
					undun[i] = false;
					itemsleft--;
					doitlater[y] = true;
				}
			}
		}
		for(x = 0; x < n; x++) {
			doitnow[x] = doitlater[x];
			doitlater[x] = false;
		}
	}

	for(x = 0; x < n-1; x++) {
		parent.push_back(pa[x]);
	}

	// Reverse h and put the result in heap
	heap.clear();
	vector<int>::reverse_iterator it;
	for(it = h.rbegin(); it != h.rend(); it++) {
		heap.push_back(*it);
	}

	// Free memory
	delete[] doitnow;
	delete[] doitlater;
	delete[] undun;
	delete[] pa;
}

bool operator==(Tree& t1, Tree& t2) {
	if(t1.name == 0) t1.genGlobalMinname();
	if(t2.name == 0) t2.genGlobalMinname();
	return(t1.name == t2.name);
}

ostream& operator<< (ostream& out, Tree& t) {
	if(t.name == 0) t.genGlobalMinname();
	out << t.name;
	return out;
}
