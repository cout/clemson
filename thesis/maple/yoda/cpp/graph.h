#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <vector>
#include "tree.h"
#include "edge.h"

class Graph {
public:
	vector<Edge> edges;		// list of edges
	vector<list<int> > ctr;		// list of the edges which must be
					// used before edge i is used
	int num_edges;			// number of edges (m)
	int num_nodes;			// number of nodes (n)
};

#endif
