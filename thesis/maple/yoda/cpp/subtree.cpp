#include <algorithm>
#include "subtree.h"

// next_tree (nextree) produces the next spanning tree in a graph with
// the given ordered edge set.  It returns the number of edges in the
// next tree, or 0 if there is no next tree.
int Subtree::next_tree() {
	int i, j, k;
	int x, y;
	bool backtrack;

	k = f.size();
	if(k == num_nodes - 1) {
		j = k;
		i = f[k] + 1;
		f.pop_back();
	} else if(k > 0) {
		j = k + 1;
		i = f[k] + 1;
	} else {
		j = 1;
		i = 1;
	}

	// We are not backtracking initially
	backtrack = false;

	while(j < num_nodes) {
		list<int>::iterator it;
		for(it = ctr[i].begin(); it != ctr[i].end(); it++) {
			int s = *it;

			// If there are no common vertices between s and f,
			// then set backtrack
			if(find(f.begin(), f.end(), s) == f.end()) {
				backtrack = true;
				break;
			}

			// If (num edges in spanning tree) - (index of the edge)
			// + 1 is less than (number of nodes in graph) -
			// (stage of building the subtree) then set
			// backtrack
			if((num_edges - i + 1) < (num_nodes - j))
				backtrack = true;

			if(backtrack) {
				j--;
				if(j == 0) return j;
				i = f[j] + 1;
				f.pop_back();
				backtrack = false;
			} else {
				x = g[j-1][edges[i][0]];
				y = g[j-1][edges[i][1]];

				if(x == y) {
					i++;
				} else {
					f.push_back(i);
					for(int v = 0; v < num_nodes; v++) {
						if(g[j-1][v] == y) {
							g[j][v] = x;
						} else {
							g[j][v] = g[j-1][v];
						}
						j++;
						i++;
					}
				}
			}
		}
	}

	return j;
}

// gbuild produces the next nxn component status array for a given edge
// list and edge-index selector.
void Subtree::gbuild() {
	int v, j;
	int x, y;

	g.reserve(num_nodes-1);
	for(int j = 0; j <= num_nodes-1; j++) g[j].reserve(num_nodes);

	for(v = 0; v < num_nodes; v++) g[0][v] = v;
	for(j = 0; j < f.size(); j++) {
		x = g[j-1][edges[f[j]][0]];
		y = g[j-1][edges[f[j]][1]];
		for(v = 0; v < num_nodes; v++) {
			if(g[j-1][v] == y) {
				g[j][v] = x;
			} else {
				g[j][v] = g[j-1][v];
			}
		}
	}
}

// setctr initializes the control array ctr
void Subtree::setctr(vector<Edge> e, vector<int> &deg) {
	vector<int> bond;
	int maxi;

	num_edges = e.size();
	ctr.reserve(num_edges + 1);
	ctr.clear();
	vert.clear();

	// Find the largest element; we assume this to be the number of
	// nodes in the graph
	num_nodes = 0;
	for(vector<Edge>::const_iterator it = e.begin(); it != e.end(); it++) {
		if((*it)[0] > num_nodes) num_nodes = (*it)[0];
		if((*it)[1] > num_nodes) num_nodes = (*it)[1];
	}
	num_nodes++;

	for(int v = 0; v < num_nodes; v++) {
		bond.clear();
		maxi = 0;

		// Find every vertex connected to v and add it to bond
		for(int i = 0; i < num_edges; i++) {
			if(v == e[i][0] || v == e[i][1]) {
				bond.push_back(i);
				maxi++;
			}
		}

		// Add the bond list to the control array at position maxi.
		// The bond list is only added to one position, since we only
		// want to traverse the edges once.
		ctr[maxi].insert(ctr[maxi].end(), bond.begin(), bond.end());

		// Keep track of which vertex this bond list is
		// associated with.
		vert[maxi].push_back(v);

		// The number of elements in bond is the degree of vertex v.
		deg.push_back(bond.size());
	}
}

