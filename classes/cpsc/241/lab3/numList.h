//                           class numList definition
//
//  This class implements a list of integers.  Integers added to the list
//  are inserted at the rear of the list.  Integers in the list are
//  retrieved by specifying their position in the list.
//  
//  The list is implemented as a linked list of listNodes.
//
//  Note:  This is intended only to illustrate some of the basics about
//  classes in C++.
//
#include "listNode.h"

class numList {

  public:  

    // Constructor:
          numList ();  

    // numList operations:

     void addnum (int n); // Add the number n to the (end of the) numList.

     int  getnum (int i); // Return the ith number in the list.

     int  size();         // Return the number of integers in the list.

  private:  

     listNode * first;  // pointer to the first node in the list
     listNode * last;   // pointer to the last node in the list
     int  count;        // the number of integers (currently) in the list
};
