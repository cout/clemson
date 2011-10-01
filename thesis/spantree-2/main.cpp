#include <time.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>
#include "spantree.h"

// The TreeInfo structure allows us to keep track of per-tree information.
struct TreeInfo {
    TreeInfo() {
        occurances = 0;
    }

    int occurances;
    set<int> required_edges;
    vector<Edge> edges;
};

void spantree(Graph& g, int min) {
    clock_t start_time = clock();
    Spantree s(g);
    Tree t;
    vector<Edge> edges;
    map<Biname, TreeInfo> subtrees;

    int jcount = s.first_tree();
    int tcount = 0;

    while(jcount > 0) {

        // Get the edges in the spanning tree
        edges.clear();
        for(int k = 0; k < g.num_nodes-1; k++)
            edges.push_back(g.edges[s.f[k]]);

        // Calculate the child array for this tree
        t = edges;

        // Determine if this subtree has been found already.
        t.genGlobalMinname();
        map<Biname, TreeInfo>::iterator i;
        if((i = subtrees.find(t.name)) != subtrees.end()) {
            // If this subtree has already been found
            TreeInfo *ti = &i->second;
            ti->occurances++;
            set<int> re;
            for(unsigned int x = 0; x < s.f.size(); x++) re.insert(s.f[x]);
            set<int>::iterator rit;
            for(rit = ti->required_edges.begin();
                rit != ti->required_edges.end(); rit++) {
                if(re.find(*rit) == re.end()) ti->required_edges.erase(rit);
            }
        } else {
            // If this is the first occurrance of this subtree
            TreeInfo ti;
            ti.occurances = 1;
            ti.edges = edges;
            ti.required_edges.insert(s.f.begin(), s.f.end());
            subtrees[t.name] = ti;
        }

        // Give an intermediate report
        // cout << "Elapsed time: " << (float)(clock() - start_time) /
        //         CLOCKS_PER_SEC << endl;
        cout << "Subtree found: ";
        for(vector<int>::iterator it = s.f.begin(); it != s.f.end(); it++)
            cout << *it + min << ' ';
        cout << endl;

        // Find the next spanning tree.  next_tree returns 0 if there are
        // no spanning trees left, which will end the loop.
        tcount++;
        jcount = s.next_tree();
    }

    clock_t end_time = clock();
    cout << endl;
    cout << "Total time: "
         << (float)(end_time - start_time) / CLOCKS_PER_SEC
         << " seconds."
         << endl;

    cout << "Number of trees found: " << tcount << endl;
    cout << "Number of NON-ISOMORPHIC trees found: " << subtrees.size() << endl;

    map<Biname, TreeInfo>::iterator i;
    vector<Edge>::iterator e;
    for(i = subtrees.begin(); i != subtrees.end(); i++) {
        const Biname *b = &i->first;
        TreeInfo *ti = &i->second;
        cout << "Tree " << hex << *b << " occured " << dec << ti->occurances
             << " times (edges: ";

        for(e = ti->edges.begin(); e!= ti->edges.end(); e++) {
            cout << "[" << dec << (*e)[0] + min << " " << (*e)[1] + min << "] ";
        }
        cout << ")" << endl;
    }
    cout << endl;

    unsigned int x;
    set<Biname> treq;
    vector<int> nreq;
    for(x = 0; x < g.edges.size(); x++) {
        treq.clear();
        for(i = subtrees.begin(); i != subtrees.end(); i++) {
            set<int> &re = (&i->second)->required_edges;
            if(re.find(x) != re.end() && treq.find(i->first) == treq.end()) {
                treq.insert(i->first);
            }
        }
        if(treq.size() != 0) {
            cout << "Edge [" << dec << g.edges[x][0] + min << ' '
                 << g.edges[x][1] + min << "] is required by trees ";

            set<Biname>::iterator bit = treq.begin();
            cout << hex << *bit;
            bit++;
            while(bit != treq.end()) {
                cout << ", " << hex << *bit;
                bit++;
            }
            cout << endl;
        } else {
            nreq.push_back(x);
        }
    }

    cout << "Edges not required: ";
    for(unsigned int i = 0; i < nreq.size(); i++) {
        x = nreq[i];
        cout << "[" << dec << g.edges[x][0] + min << ' '
             << g.edges[x][1] + min << "] ";
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    Graph g;
    vector<Edge> edges;

    // Get the name of the input file
    if(argc < 2) { 
        cout << "Usage: " << endl
             << "   " << argv[0] << " inputfile" << endl
             << "or " << argv[0] << " inputfile > outputfile" << endl;
        exit(1);
    }

    // Read the input file
    ifstream in(argv[1]);
    if(!in) {
        cout << "Unable to open file " << argv[1] << " for reading." << endl;
        exit(2);
    }
    int min;
    while(!in.eof()) {
        Edge e;
        in >> e[0]; if(e[0] < min) min = e[0];
        in >> e[1]; if(e[1] < min) min = e[1];
        if(find(edges.begin(), edges.end(), e) == edges.end())
            edges.push_back(e);
    }
    for(int j = 0; j < edges.size(); j++) {
        edges[j][0] -= min;
        edges[j][1] -= min;
    }

    // Build g
    g = edges;

    // Generate the spanning trees
    spantree(g, min);

    return 0;
}
