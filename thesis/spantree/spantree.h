#ifndef SUBTREE_H
#define SUBTREE_H

#include <vector>
#include <list>
#include "graph.h"

class Spantree {
public:
	Spantree() { }
	Spantree(const Graph &gr) { graph = gr; }
	Spantree(const vector<Edge> edges) { graph = edges; }
	Spantree(const Spantree& s) {
		f = s.f; g = s.g; graph = s.graph;
	}
	int next_tree();
	void gbuild();
	int first_tree() { gbuild(); return next_tree(); }

	vector<int> f;                  // Lists that maintain the current state
	vector<vector<int> > g;

	Graph graph;
};

#endif
