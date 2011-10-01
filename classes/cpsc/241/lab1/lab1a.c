
//                          CpSc 241, Lab 1
//
//  This program is intended only to illustrate C++ classes.
//
//  The program reads a sequence of input integers, separated by blanks, tabs,
//  or line boundaries.  The first integer indicates the maximum number of
//  remaining integers to be read.  (There may be fewer than the maximum
//  number remaining in the input file.) The numbers are read from
//  standard input (keyboard) until an end-of-file (^D on the keyboard).
//
//  The input integers (after the first one) are put into a list of integers.
//  The negative numbers are then put into a second list and the nonnegative
//  numbers into a third list.  The numbers in each list are then printed in
//  the input order.
//

#include <iostream.h>  
#include "numlist.h"


void printList(numlist & L) { 
       // Print the integers in list L, 5 per line.

   for (int i=1; i <= L.size(); i++) {
      cout << "   " << L.getnum(i) ;
      if (!(i%5) )  cout << endl; // Start new line after every 5th one.
   }
   if (!((i-1)%5)) cout << endl;  // End last line if needed.
} // end printList
   

int main() {
   int maxSize;       // maximum number of input integers

   // Get the maximum number of input integers;
      cout << endl << "Enter the maximum number of integers to be read: ";
      cout.flush();
      cin >> maxSize;

   // Read the input numbers, put them into a list, and count the negatives.

      numlist nums(maxSize);  // the list for the input integers (declaration)
      int inputNumber;         // an input number
      int negCount=0;          // the number of negative values read

      cout << endl << "Enter the input integers:" << endl;
      while (cin >> inputNumber){
          nums.addnum(inputNumber);  // Put the input number into the list.
          if (inputNumber<0) negCount++;  // Count the negatives.
      }

   // Put the negatives into one list and the nonnegatives into another.

      numlist negNums(negCount);  // the list for negatives
      numlist nonNegNums(nums.size() - negCount);  // list for nonnegatives

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
