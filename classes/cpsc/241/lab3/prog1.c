
//                          CpSc 241, Lab 2 (a)
//
//  This program is intended only to illustrate C++ classes.
//
//  The program reads a sequence of input integers, separated by blanks, tabs,
//  or line boundaries.  The numbers are read from standard input (keyboard)
//  until an end-of-file (^D on the keyboard).  The input integers are put into
//  a list of integers.  The negative numbers are then put into a second list
//  and the nonnegative numbers into a third list.  Each list is then printed.
//

#include <iostream.h>  
#include "numList.h"

void printList(numList & L) { 
       // Print the integers in list L, 5 per line.

   for (int i=1; i <= L.size(); i++) {
      cout << "   " << L.getnum(i) ;
      if (!(i%5) )  cout << endl; // Start new line after every 5th one.
   }
   if (!((i-1)%5)) cout << endl;  // End last line if needed.
} // end printList

   
int main() {

   // Read the input numbers, and put them into a list.

      numList nums;         // the list for the input integers (declaration)
      int inputNumber;      // an input number

      cout << endl << "Enter the input integers:" << endl;
      while (cin >> inputNumber){
          nums.addnum(inputNumber);  // Put the input number into the list.
      }

   // Put the negatives into one list and the nonnegatives into another.

      numList negNums;  // the list for negatives
      numList nonNegNums;  // list for nonnegatives

      for (int i=1; i <= nums.size(); i++){
         inputNumber = nums.getnum(i);
         if (inputNumber < 0)  negNums.addnum(inputNumber);
         else nonNegNums.addnum(inputNumber);
      }

   // Print the lists.

      cout << endl << "The input numbers were:" << endl << endl;
      printList(nums);

      cout << endl << "The negative input numbers were:" << endl << endl;
      printList(negNums);

      cout << endl << "The nonnegative input numbers were:" << endl << endl;
      printList(nonNegNums);


   cout << endl << " -- end program -- " << endl << endl;

return 0;
}
