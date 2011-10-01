//                           class stack definition
//
//  This class implements a stack of integers.  
//
//  The stack is implemented as an array of integers.
//
//

class stack {

  public:  // (The following functions are accessible outside the class.)

    // Constructor:
       stack (int size);  // Construct a stack for up to size integers.

    // stack operations:

     void push (int n); // Push n onto the stack.

     int  pop (); // Pop the stack (and return the popped item).

     int  isEmpty();         // Return whether the stack is empty.

  private:  // (The following are not accessible except by the above functions.)

     int * nums;   // an integer array containing the numbers in the stack
     int top;      // the index for the next available array element
                   //   (This is one past the top stack element.)
};
