//                           class numlist definition
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

class numlist {

  public:  // (The following functions are accessible outside the class.)

    // Constructor:
       numlist (int size);  // Construct a numlist for up to size integers.

    // numlist operations:

     void addnum (int n); // Add the number n to the (end of the) numlist.

     int  getnum (int i); // Return the ith number in the list.

     int  size();         // Return the number of integers in the list.

  private:  // (The following are not accessible except by the above functions.)

     int * list;   // an integer array containing the numbers in the list
     int  count;   // the number of integers (currently) in the list
};
