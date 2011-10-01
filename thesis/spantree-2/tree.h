#ifndef TREE_H
#define TREE_H

#include <list>
#include <vector>
#include <map>
#include "biname.h"
#include "edge.h"

typedef list<int> Child_Array_Elem;
typedef vector<Child_Array_Elem> Child_Array;
typedef vector<int> Heap_Ordering;
typedef vector<int> Parent_List;
typedef map<Biname, Biname> Minname_Table;

class Tree {
public:
    Tree() { n = 0; name = 0; }
    Tree(const Edge_List &e) { genTree(e); }
    Tree(const Tree& t) {
        child = t.child;
        heap = t.heap;
        parent = t.parent;
        n = t.n;
        name = t.name;
    }
    Tree& operator=(const Tree& t) {
        child = t.child;
        heap = t.heap;
        parent = t.parent;
        n = t.n;
        name = t.name;
        return *this;
    }
    Tree& operator=(const Edge_List &e) {
        genTree(e);
        return *this;
    }

    Child_Array child;         // a child array
    Heap_Ordering heap;        // a heap ordering
    Parent_List parent;        // a parent list
    int n;                     // number of nodes in the tree
    Biname name;               // the global minname
    Minname_Table minname_table;

    void genLocalMinnames(vector<Biname> &r);
    void genGlobalMinname();
    void genTree(const vector<Edge> &e);

    friend bool operator==(Tree& t1, Tree& t2);
    friend ostream& operator<<(ostream& out, Tree& t);
};

#endif
