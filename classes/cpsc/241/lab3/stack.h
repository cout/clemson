//                           class stack definition
//
//  This class implements a stack of integers.  
//
//  The stack is implemented as a linked list of listNodes.
//
//
#include "listNode.h"

class stack {

  public:  

    // Constructor:
          stack ();     // Construct a stack of integers.

    // stack operations:

     void push (int n); // Push n onto the stack.

     int  pop ();       // Pop the stack (and return the popped item).

     int  isEmpty();    // Return whether the stack is empty.

  private:  

     listNode *front;   // pointer to the front of the list (top of stack)
};
