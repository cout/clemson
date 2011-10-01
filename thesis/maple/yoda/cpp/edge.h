#ifndef EDGE_H
#define EDGE_H

struct Edge {
        Edge(int edge[2]) {e[0] = edge[0]; e[1] = edge[1];}
        int e[2];
        const int& operator[](int x) const { return e[x]; }
        int& operator[](int x) { return e[x]; }
};

#endif
