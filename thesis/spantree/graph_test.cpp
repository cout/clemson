#include <iostream>
#include "graph.h"

static int ourgraph[][2] = { {0, 1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,0}, {1,4} };

int main() {
	Graph g;
	vector<Edge> e;			// an edge list
	vector<int> deg;
	int j;

	cout << "Building the graph...";
	for(j = 0; j < 7; j++)
		e.push_back(ourgraph[j]);
	g = e;
	g.get_vert_degrees(deg);
	cout << endl;

	cout << "The vertex degrees should be:" << endl;
	cout << "2 3 2 2 3 2" << endl;

	cout << "The vertex degrees are:" << endl;
	for(vector<int>::iterator it = deg.begin(); it != deg.end(); it++)
		cout << *it << ' ';
	cout << endl;

	cout << "The control list ctr follows:" << endl;
	vector<vector<list<int> > >::iterator i;
	for(i = g.ctr.begin(), j = 0; i != g.ctr.end(); i++, j++) {
		cout << j << ": ";
		if(i->begin() == i->end()) cout << "<empty>";
		for(vector<list<int> >::iterator k = i->begin(); k != i->end(); k++) {
			cout << "{";
			for(list<int>::iterator l = k->begin(); l != k->end(); l++) {
				cout << *l << ' ';
			}
			cout << "} ";
		}
		cout << endl;
	}

	return 0;
}
