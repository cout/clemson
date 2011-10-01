#include <algorithm>
#include "graph.h"

// setctr initializes the control array ctr
void Graph::setctr(const vector<Edge> &e) {
    Bond_List bond;
    int maxi;
    Bond_List emptylist;
    Ctr_Element emptyvect;

    num_edges = e.size();
    ctr.clear();
    emptyvect.push_back(emptylist);
    for(int j = 0; j < num_edges + 1; j++)
        ctr.push_back(emptyvect);

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
                maxi = i + 1;
            }
        }

        // Add the bond list to the control array at position maxi.
        // The bond list is only added to one position, since we only
        // want to traverse the edges once.
        ctr[maxi].push_back(bond);
    }
}

void Graph::get_vert_degrees(vector<int> &deg) {
    int degree;

    deg.clear();
    for(int v = 0; v < num_nodes; v++) {
        degree = 0;
        for(int i = 0; i < num_edges; i++) {
            if(v == edges[i][0] || v == edges[i][1]) {
                degree++;
            }
        }
        deg.push_back(degree);
    }
}
