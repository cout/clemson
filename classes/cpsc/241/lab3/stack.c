//                           class stack implementation
//
//  This class implements a stack of integers.  
//
//  The stack is implemented as a linked list of listNodes.
//
//

#include <assert.h>
#include "stack.h"

    // Constructor:
       stack::stack () : front(0) {} // Construct a new stack.
         // (Start with an empty list ("front" is null).)

    // stack operations:

       void stack::push (int n){ // Push n onto the stack.
               listNode *newNode = new listNode(n);
               newNode->setNext(front);
               front = newNode;
       }

       int  stack::pop (){ // Pop the stack and return the popped element.
               assert(!isEmpty());   // can't pop an empty stack
	       int temp = front->getData();
               listNode *newFront = front->getNext();
	       delete front;
	       front = newFront;
               return(temp);
       }

       int  stack::isEmpty(){ // Return whether the stack is empty.
	       return(front==0);
       }

