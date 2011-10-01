//                           class numList implementation
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

#include <assert.h>
#include "numList.h"

    // Constructor:
       numList::numList (){ // Construct a new numList.
                  first = last = 0;
                  count = 0;
       }

    // numList operations:

       void numList::addnum (int n){ // Add n to the numList.
                       listNode * newNode = new listNode(n);
                       if (!last)  
                         // Add to an empty list.
                            first = last = newNode;
                       else { // Add at the end of a nonempty list.
                         last->setNext(newNode);
                         last = newNode;
                       }
                       count++;
       }

       int  numList::getnum (int i){ // Return the ith number from the list.
                       assert (i<=count);
                       listNode * current = first; //for "walking down" the list
                       for (int j=1; j<i; j++)  current = current->getNext();
                       return current->getData();
       }

       int  numList::size(){ // Return the number of integers in the numList.
                       return count;
       }

