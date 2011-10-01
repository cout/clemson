#include "stack.h"

// class names should start with uppercase
#define Stack stack

// constructor for the stack class
Stack::Stack(int size) {
 
        // allocate memory for the stack
        nums = new int[size];
 
        // set the  top of the stack to 0 (no elements in the stack)
        top = 0;
 
}
 
// this is dangerous.  there is no bounds checking, which would make a buffer
// overflow extremely easy.  mission-critical applications should do bounds
// checking to avoid smashing the stack.
void Stack::push(int n) {
 
        nums[top++] = n;
 
}
 
// pop an element off the stack
int Stack::pop() {
 
        return(nums[--top]);
 
}
 
// determine if the stack is empty.
// return values: 0 if not empty, otherwise returns nonzero number
int Stack::isEmpty() {
 
        return(!top);
 
}
 
