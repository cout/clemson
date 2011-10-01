#include <iostream>
#include "spantree.h"

static int ourgraph[][2] = { {0, 1}, {1,2}, {2,3}, {3,4}, {4,5}, {5,0}, {1,4} };

int main() {
	Spantree s;
	vector<Edge> e;			// an edge list
	vector<int> deg;
	vector<vector<int> >::iterator i;
	vector<int>::iterator j;
	int k;

	cout << "Building the graph...";
	for(k = 0; k < 7; k++)
		e.push_back(ourgraph[k]);
	s.graph = e;
	cout << endl;

	cout << "Building g...";
	s.gbuild();
	cout << endl;

	cout << "g:" << endl;
	for(i = s.g.begin(); i != s.g.end(); i++) {
		if(i->begin() == i->end()) cout << "<empty>";
		for(vector<int>::iterator j = i->begin(); j != i->end(); j++) {
			cout << *j << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "Calculating the first tree...";
	s.next_tree();
	cout << endl;

	cout << "g:" << endl;
	for(i = s.g.begin(); i != s.g.end(); i++) {
		if(i->begin() == i->end()) cout << "<empty>";
		for(vector<int>::iterator j = i->begin(); j != i->end(); j++) {
			cout << *j << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "f: ";
	for(j = s.f.begin(); j != s.f.end(); j++)
		cout << *j << ' ';
	cout << endl << endl;
	
	cout << "Calculating the second tree...";
	s.next_tree();
	cout << endl;

	cout << "g:" << endl;
	for(i = s.g.begin(); i != s.g.end(); i++) {
		if(i->begin() == i->end()) cout << "<empty>";
		for(vector<int>::iterator j = i->begin(); j != i->end(); j++) {
			cout << *j << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "f: ";
	for(j = s.f.begin(); j != s.f.end(); j++)
		cout << *j << ' ';
	cout << endl;
	
	return 0;
}
