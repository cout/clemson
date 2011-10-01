//                           class numlist implementation
//
//  This class implements a list of integers.  Integers added to the list
//  are inserted at the rear of the list.  Integers in the list are
//  retrieved by specifying their position in the list.
//
//  The list is implemented as an array of integers.
//
//  Note:  this is not a useful class.  It is intended only to illustrate
//  some of the basics about classes in C++.
//

#include "numlist.h"

    // Constructor:
       numlist::numlist (int size){ // Construct a new numlist.
                  list = new int[size];
                  count = 0;
       }

    // numlist operations:

       void numlist::addnum (int n){ // Add n to the numlist.
                       list[count++] = n;
       }

       int  numlist::getnum (int i){ // Return the ith number from the list.
                       return list[i-1];
       }

       int  numlist::size(){ // Return the number of integers in the numlist.
                       return count;
       }

