#ifndef SUBTREE_H
#define SUBTREE_H

#include <vector>
#include <list>
#include "graph.h"

// The Spantree class is an iterator for the graph class.  It generates all
// the spanning trees in a graph in a consistent (but not necessarily
// easily predictable) order.

class Spantree {
public:
    // We can generate a spantree from a graph or from a list of edges.
    // If we generate it from a list of edges, then that list of edges must
    // represent a graph.
	Spantree() { }
	Spantree(const Graph &gr) { graph = gr; }
	Spantree(const vector<Edge> edges) { graph = edges; }
	Spantree(const Spantree& s) {
		f = s.f; g = s.g; graph = s.graph;
	}

    // Here are the iteration functions for spantree.  first_tree generates
    // the first tree, and next_tree generates the next tree.  In both cases,
    // the number of edges in the tree is returned, or 0 if there is no such
    // tree.  The current tree can be retrieved using the f element, which
    // contains a list of all the edges in the tree.
	int next_tree();
	void gbuild();
	int first_tree() { gbuild(); return next_tree(); }

    vector<int> f;                  // Lists that maintain the current state
	vector<vector<int> > g;

	Graph graph;
};

#endif
