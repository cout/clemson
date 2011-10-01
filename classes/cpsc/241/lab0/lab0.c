
//                          CpSc 241 Lab 0
//
//  This program is intended only to illustrate a C++ program.  The only
//  non-C features used in this program are:
//    1) "remainder of line" comments denoted by //
//    2) stream I/O (operators << and >> with predefined stream files cin
//       and cout, and the predefined constant endl
//    3) declarations imbedded in the code
//
//  The program reads a sequence of input integers, separated by blanks, tabs,
//  or line boundaries, into an array of integers and prints the numbers in
//  the order read, one per line.  The numbers are read from
//  standard input (keyboard) until an end-of-file (^D on the keyboard).
//  The positive numbers are then printed, one per line indented 5 spaces.
//

#include <iostream.h>  // header for stream I/O (<<, >>, cin, cout, endl)

const int MAX_NUMS=100;   // maximum number of input values
   
int main() {
   int nums[MAX_NUMS]; // array of input numbers
   int i=0;            // array index

   // Read the input numbers and put them into the array.
      cout << endl << " Enter the input numbers.  They will be printed as ";
      cout << "they are read." << endl;
     
      while (cin >> nums[i]) 
          cout << nums[i++] << endl;

      // (After the loop, i will be the number of input integers, so the
      //  last input integer is in nums[i-1].)

   // Print the positive numbers in reverse input order.

      cout << endl << endl;  // skip two lines
      cout << " The positive input numbers in reverse order are:" << endl;

      for (int j=i-1; j>=0; j--) {
          if (nums[j]>0)  cout << "     " << nums[j] << endl;
      }

   cout << endl << " -- end program -- \n\n" ;

return 0;
}
