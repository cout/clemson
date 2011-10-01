#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <vector>
#include "tree.h"
#include "edge.h"

typedef vector<Edge> Edge_List;
typedef list<int> Bond_List;
typedef vector<Bond_List> Ctr_Element;
typedef vector<Ctr_Element> Ctr_Array;

class Graph {
public:
    Graph() : num_edges(0), num_nodes(0) { }
    Graph(const vector<Edge> &e) : edges(e) { setctr(e); }
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

    Edge_List edges;        // list of edges
    Ctr_Array ctr;          // list of the edges which must be used before
                            //   edge i is used
    int num_edges;          // number of edges (m)
    int num_nodes;          // number of nodes (n)

private:
    void setctr(const vector<Edge> &e);
};

#endif
