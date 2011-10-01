#include <list>
#include <vector>
#include <algorithm>
#include "biname.h"
#include "tree.h"

// genLocalMinnames is equivalent to the original LocalMinnameArr function.
// It generates the minname (the lexicographically least binname) for each
// possible node in a tree by creating a list of local minnames.
// Output:
//   r - a list of binames, where r[x] gives the local minname of the branch
//       of the tree rooted at node x.
// NOTE: The root must receive the highest label, namely, n.
void Tree::genLocalMinnames(vector<Biname> &r) {
    Biname_List s;

    // Allocate space in r for the binames (we want n of them)
    r.clear();
    r.resize(n);

    // Traverse the entire tree using the tree's heap ordering.
    // Note that r is created from the bottom-up, so that minnames of
    // lower nodes are used to create the minnames of the higher nodes.
    for(int i = 0; i < n; i++) {
        int x = heap[i];
        s.clear();
        Child_Array_Elem::iterator it;
        for(it = child[x].begin(); it != child[x].end(); it++)
            s.push_back(r[*it]);
        r[x].smerg(s);
    }
}

// genGlobalMinname returns the minname for an unrooted tree.  The
// procedure starts with a local best labelling rooted at n.  Then, the
// procedure moves from parent w to child v throughout the tree, modifying
// the existing local labellings to produce the best local labellings
// tmp_minnames for each node as root.  Along the way, a local branch
// labelling tmp_binames is required at the parent of v, by regarding v as
// the root and w as the root of one branch at v.
void Tree::genGlobalMinname() {

    vector<Biname> r;
    int i;

    // Generate an array of local minnames
    // We start with the root we are given, then work our way to
    // other roots.
    genLocalMinnames(r);
    
    // Check to see if this biname is already in our table
    Biname best = r[n-1];
    if(minname_table.find(best) != minname_table.end()) {
        name = minname_table[best];
        return;
    }

    // We don't need to explicitly clear the below containers, because the
    // default constructor for Biname uses the value "0".
    vector<Biname> tmp_minnames;        // t
    vector<Biname> tmp_binames;         // tt
    tmp_minnames.resize(n);
    tmp_binames.resize(n);
    tmp_minnames[n-1] = r[n-1];

    // Try different roots until we find the one with the
    // lexicographically least associated minname.
    int v, w;
    for(i = n - 2; i >= 0; i--) {
        Biname_List s;
        Child_Array_Elem::iterator it;

        v = heap[i];            // v is our new root
        w = parent[v];            // w is the parent of v
        s.clear();
        s.push_back(tmp_binames[w]);

        // Find the minname of the tree from w and below, not
        // counting v.
        for(it = child[w].begin(); it != child[w].end(); it++) {
            if(*it != v) s.push_back(r[*it]);
        }
        tmp_binames[v] = 0;
        tmp_binames[v].smerg(s);

        // We want to include the above biname as part of our
        // new biname.
        s.clear();
        s.push_back(tmp_binames[v]);

        // Find the minname of the tree from v and below.
        for(it = child[v].begin(); it != child[v].end(); it++) {
            s.push_back(r[*it]);
        }
        tmp_minnames[v] = 0;
        tmp_minnames[v].smerg(s);

        if(tmp_minnames[v] < best) best = tmp_minnames[v];
    }

    // Insert each minname t[i] into the lookup table
    vector<Biname>::iterator it;
    for(it = tmp_minnames.begin(); it != tmp_minnames.end(); it++) {
        minname_table[*it] = best;
    }

    // Set the tree's name to the lexicographically least minname
    name = best;
}

// genTree (childe) generates the child array c, the heap ordering h, and
// the parent list p from an input edge list for a spanning subtree.
void Tree::genTree(const Edge_List& e) {
    int itemsleft;

    // Find the largest element; we assume this to be the number of
    // vertices in the tree.
    n = 0;
    for(vector<Edge>::const_iterator it = e.begin(); it != e.end(); it++) {
        n = max((*it)[0], n);
        n = max((*it)[1], n);
    }
    n++; // for good measure

    // Create our temporary data structures
    vector<bool> undone(n);
    vector<bool> doitnow(n);
    vector<bool> doitlater(n);
    int i, x, y;

    // Initialize our temporary data structures
    fill(undone.begin(), undone.end(), true);
    fill(doitnow.begin(), doitnow.end(), false);
    fill(doitlater.begin(), doitlater.end(), false);
    doitnow[n-1] = true;
    doitlater[n-1] = false;

    // Prepare our child, heap, and parent structures
    child.clear(); child.resize(n);
    heap.clear(); heap.resize(n);
    parent.clear(); parent.resize(n);

    itemsleft = n-1;
    while(itemsleft > 0) {
        for(x = 0; x < n; x++) {
            if(!doitnow[x]) continue;
            for(i = 0; i < n-1; i++) {
                if(!undone[i]) continue;
                if(x == e[i][0] || x == e[i][1]) {
                    // if the edge is connected
                    if(x == e[i][0])
                        y = e[i][1];
                    else
                        y = e[i][0];
                    child[x].push_back(y);
                    parent[y] = x;
                    heap.push_back(y);
                    undone[i] = false;
                    itemsleft--;
                    doitlater[y] = true;
                }
            }
        }
        for(x = 0; x < n; x++) {
            doitnow[x] = doitlater[x];
            doitlater[x] = false;
        }
    }

    // Reverse our heap
    reverse(heap.begin(), heap.end());
}

// A comparison function.  We first generate the global minname for the two
// trees being compared.  The trees are qual iff their global minnames are
// equal.
bool operator==(Tree& t1, Tree& t2) {
    if(t1.name == 0) t1.genGlobalMinname();
    if(t2.name == 0) t2.genGlobalMinname();
    return(t1.name == t2.name);
}

// An output function for a tree; we simply print the global minname.
ostream& operator<<(ostream& out, Tree& t) {
    if(t.name == 0) t.genGlobalMinname();
    out << t.name;
    return out;
}
