#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <vector>
#include "tree.h"
#include "edge.h"

class Graph {
public:
	Graph() { num_edges = 0; num_nodes = 0; }
	Graph(const vector<Edge> &e) { setctr(e); edges = e; }
	Graph& operator=(const vector<Edge> &e) {
		setctr(e);
		edges = e;
		return *this;
	}
	Graph& operator=(const Graph &g) {
		edges = g.edges;
		ctr = g.ctr;
		num_edges = g.num_edges;
		num_nodes = g.num_nodes;
		return *this;
	}
	void get_vert_degrees(vector<int> &deg);

	vector<Edge> edges;						// list of edges
	vector<vector<list<int> > > ctr;		// list of the edges which must be
											// used before edge i is used
	int num_edges;							// number of edges (m)
	int num_nodes;							// number of nodes (n)

private:
	void setctr(const vector<Edge> &e);
};

#endif
