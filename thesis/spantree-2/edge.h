#ifndef EDGE_H
#define EDGE_H

#include <vector>

struct Edge {
    // Default construction
    Edge() { }

    // Copy construction
    Edge(const int edge[2]) {
        e[0] = edge[0];
        e[1] = edge[1];
    }
    Edge(const Edge& edge) {
        e[0] = edge[0];
        e[1] = edge[1];
    }

    // Assignment
    Edge& operator=(const Edge& edge) {
        e[0] = edge[0];
        e[1] = edge[1];
        return *this;
    }
    Edge& operator=(const int edge[2]) {
        e[0] = edge[0];
        e[1] = edge[1];
        return *this;
    }

    // Accessors
    const int& operator[](int x) const { return e[x]; }
    int& operator[](int x) { return e[x]; }

    // Comparison
    friend bool operator==(const Edge& e1, const Edge& e2) {
        return e1[0] == e2[0] && e1[1] == e2[1];
    }
    friend bool operator==(const Edge& e1, const int e2[2]) {
        return e1[0] == e2[0] && e1[1] == e2[1];
    }

    int e[2];
};

typedef vector<Edge> Edge_List;

#endif
