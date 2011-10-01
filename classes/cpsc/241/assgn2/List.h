// List class
// Defines a linked list

#include "Node.h"

#ifndef __List__

#define __List__ __List__

class List {
public:
	List() 			{ rear = front = current = 0; }
	~List()			{ clear(); }
	void insertAtFront(int n);
	void insertAtRear(int n);
	bool empty() const	{ return front == 0; }
	int size() const;
	Node * getFirst() const	{ current = front; return current; }
	Node * getLast() const	{ current = rear; return current; }
	Node * getNext() const	{ current = current->next; return current; }
	Node * getPrev() const 	{ current = current->prev; return current; }
	void clear();
private:
	Node * front;
	Node * rear;
	mutable Node * current;
};

#endif
