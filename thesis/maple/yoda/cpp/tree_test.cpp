#include "tree.h"

static int ourtree[5][2] = { {0,1}, {1,2}, {2,3}, {3,4}, {4,5} };

main() {
	Tree t;				// a tree
	vector<Edge> e;			// an edge list
	int j;

	cout << "Building the tree...";
	for(j = 0; j < 5; j++)
		e.push_back(ourtree[j]);
	t.genTree(e);
	cout << endl;

	cout << "Edge list:" << endl;
	for(j = 0; j < 5; j++)
		cout << '[' << e[j][0] << ' ' << e[j][1] << ']';
	cout << endl;

	cout << "Parent list" << endl;
	for(j = 0; j < 5; j++)
		cout << t.parent[j] << ' ';
	cout << endl;

	cout << "Heap ordering" << endl;
	for(j = 0; j < 5; j++)
		cout << t.heap[j] << ' ';
	cout << endl;

	cout << "The minname for this tree should be 3f:" << endl;
	cout << hex << t << endl;

	return 0;
}
