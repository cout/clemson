#include "List.h"

// Counts the number of elements in a list
int List::size() const {
	int count = 0;
	Node * ptr = front;
	while (ptr != 0) {
		++count;
		ptr = ptr->next;
	}
	return count;
}

// Insert an element at the end of the list
void List::insertAtRear(int n) {
	if(empty()) {
		front = rear = current = new Node(n, 0, 0);
	} else {
		rear->next = new Node(n, rear, 0);
		current = rear = rear->next;
	}
}

// Insert an element at the front of the list
void List::insertAtFront(int n) {
	if(empty()) {
		front = rear = current = new Node(n, 0, 0);
	} else {
		front->prev = new Node(n, 0, front);
		current = front = front->prev;
	}
}

// Clear the list completely
void List::clear() {
	Node *ptr = rear;
	if(empty()) return;
	while((ptr = ptr->prev) != 0) delete ptr->next;
	delete front;
	rear = front = current = 0;
}
