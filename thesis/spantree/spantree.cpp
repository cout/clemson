#include <algorithm>
#include <assert.h>
#include "spantree.h"

// next_tree (nextree) produces the next spanning tree in a graph with
// the given ordered edge set.  It returns the number of edges in the
// next tree, or 0 if there is no next tree.
// This code would probably be much cleaner if it were written recursively.
int Spantree::next_tree() {
	int i, j, k;
	int x, y;
	bool backtrack;

	k = f.size();
	if(k == graph.num_nodes - 1) {
		j = k;
		i = f[k-1] + 1;
		f.pop_back();
	} else if(k > 0) {
		j = k + 1;
		i = f[k-1] + 1;
	} else {
		j = 1;
		i = 0;
	}

	// We are not backtracking initially
	backtrack = false;

	// j represents the number of edges in our tree plus one.  A spanning
	// tree must have exactly edges=nodes+1.  Thus, we stop when this
	// condition is met.
	while(j < graph.num_nodes) {

		// This code is a bit difficult to understand at a first glance.  We
		// traverse the ctr structure, and if we don't find an intersection
		// between at least one element of each list in the ctr structure and
		// f, then we will set backtrack.
		// Remember that ctr[i] represents which edges must be visited before
		// edge i can be visited.  We want to visit edge i.  We cannot do so
		// if we have not taken one of the paths described in ctr[i].
		// This is only an optimization; the algorithm will run perfectly
		// without this check.
		vector<list<int> >::iterator it;
		for(it = graph.ctr[i].begin(); it != graph.ctr[i].end(); it++) {
			list<int>::iterator it2;
			if(it->begin() == it->end()) continue;
			backtrack = true;
			for(it2 = it->begin(); it2 != it->end(); it2++) {
				int s = *it2;
				if(find(f.begin(), f.end(), s) != f.end()) {
					backtrack = false;
				}
			}
			if(backtrack) break;
		}

		// If (num edges in spanning tree) - (index of the edge)
		// + 1 is less than (number of nodes in graph) -
		// (stage of building the subtree) then set
		// backtrack
		// (i.e. check to see if there are enough edges left over
		// to build a tree)
		if((graph.num_edges - i - 0) < (graph.num_nodes - j))
			backtrack = true;

		// We must backtrack
		if(backtrack) {
			j--;
			if(j == 0) return j;
			i = f[j-1] + 1;
			while(f.size() != (unsigned)(j-1)) f.pop_back();
			backtrack = false;

		// Determine which edge to visit next
		} else {
			x = g[j-1][graph.edges[i][0]];
			y = g[j-1][graph.edges[i][1]];

			// graph.edges[i] represents the vertices of edge i.  g[j-1]
			// represents the component list for the current state (that is,
			// g is a stack of component lists, and every time we add an
			// edge, we add a new component list onto the stack.  When we
			// backtrack, we decrement j, which "pops" a component list off
			// the stack, so we can go back to a previous state).
			// x and y therefore represent the component numberings for
			// the two vertices of the currently selected edge (which is
			// edge i).  If two edges have the same component numberings, then
			// they are both parts of the same component, and so adding this
			// edge will generate a cycle (which cannot occur in a tree).
			// Thus, to avoid creating a cycle, we increment i to pick a
			// different edge.
			if(x == y) {
				i++;
			} else {
				// Mark this edge as visited by adding it to f.
				f.push_back(i);

				// Update the g structure.  We are using g as if it were a
				// stack.  Initially, g[0][v] is equal to v.  As we add an
				// edge, we update the component numberings so we know which
				// vertices are connected to each other.  What the component
				// numbering is we really don't care; all that matters is that
				// the two vertices of the newly added edge receive the same
				// numbering.
				for(int v = 0; v < graph.num_nodes; v++) {
					if(g[j-1][v] == y) {
						g[j][v] = x;
					} else {
						g[j][v] = g[j-1][v];
					}
				}
				j++;
				i++;
			}
		}

	}

	return j;
}

// gbuild produces the next nxn component status array for a given edge
// list and edge-index selector.
void Spantree::gbuild() {
	int v, j;
	int x, y;

	vector<int> emptyvect;
	for(int j = 0; j < graph.num_nodes; j++) {
		g.push_back(emptyvect);
		g[j].reserve(graph.num_nodes);
		for(v = 0; v < graph.num_nodes; v++) g[j].push_back(-1);
		g[0][j] = j;
	}

	/* 
	for(j = 1; (unsigned)j < f.size(); j++) {
		x = g[j-1][graph.edges[f[j]][0]];
		y = g[j-1][graph.edges[f[j]][1]];
		for(v = 0; v < graph.num_nodes; v++) {
			if(g[j-1][v] == y) {
				g[j][v] = x;
			} else {
				g[j][v] = g[j-1][v];
			}
		}
	}
	*/
}

