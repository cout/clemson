#ifndef SUBTREE_H
#define SUBTREE_H

#include <vector>
#include <list>
#include "graph.h"

class Subtree: public Graph {
public:
	int next_tree();
	void gbuild();
	void setctr(vector<list<int> > e);

	vector<int> f;                  // Lists that maintain the current state
	vector<vector<int> > g;
};

#endif
