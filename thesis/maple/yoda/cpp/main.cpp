#include <time.h>
#include <iostream>
#include "graph.h"

void spantree(Graph& g) {
	Vector<int> edges;
	clock_t start_time = clock();

	int jcount = g.nextree();
	while(jcount > 0) {
		// Get the edges in the spanning tree
		edges.clear();
		for(
