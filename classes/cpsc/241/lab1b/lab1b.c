// lab1b.c
// Paul Brannan
// based on lab1a.c

#include <iostream.h>  
#include "stack.h"

// all class names should begin with capital letters
#define Stack stack

void printAndDestroyStack(stack& S);

int main() {
   int maxSize;

   // Get the maximum number of input integers;
      cout << endl << "Enter the maximum number of integers to be read: ";
      cout.flush();
      cin >> maxSize;

   // Read the input numbers, put them into a list, separate negatives.

      Stack nums(maxSize);	// the list for the input integers (declaration)
      Stack negNums(maxSize);
      Stack nonNegNums(maxSize);
      int inputNumber;		// an input number

      cout << endl << "Enter the input integers:" << endl;
      while (cin >> inputNumber){

          nums.push(inputNumber);	// Put the input number into the stack.
          if (inputNumber < 0) negNums.push(inputNumber);
          else nonNegNums.push(inputNumber);
      }

   // Print the lists.

      cout << endl << "The input numbers were:" << endl << endl;
      printAndDestroyStack(nums);

      cout << endl << "The negative input numbers were:" << endl << endl;
      printAndDestroyStack(negNums);

      cout << endl << "The nonnegative input numbers were:" << endl << endl;
      printAndDestroyStack(nonNegNums);


   cout << endl << " -- end program -- " << endl << endl;

return 0;
}

void printAndDestroyStack(Stack& S) {

	int count=0;

	while(!S.isEmpty()) {

		count++;
		cout << "   " << S.pop();
		if ((count % 5) == 0) cout << endl;

	}

	if((count % 5) != 0) cout << endl;
}
