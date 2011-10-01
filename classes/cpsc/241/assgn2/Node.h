// Node class
// This class defines a node of a linked list

#include <iostream.h>

#ifndef __Node__

#define __Node__ __Node__

class Node {
public:
// We want the List class to be able to access private variables directly
friend class List;
	Node(int i = 0, Node * p = 0, Node * n = 0) 
		: number(i), prev(p), next(n) {}
	friend ostream & operator<<(ostream &, const Node &);
	int getValue() {return number;}
private:
	int number;
	Node *prev;
	Node *next;
};

#endif
