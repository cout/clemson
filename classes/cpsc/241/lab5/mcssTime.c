
//**************************************************************************
//                                                                         *
// Program to find the maximum subsequence sum for a random sequence.      *
//                                                                         *
// Maximum Subsequence Sum Problem:                                        *
//    Given integers a1, a2, ..., an, find the maximum value of            *
// Sum(k=i,j) ak. (For convenience, the maximum subsequnce sum is 0 if all *
// the integers are negative.)                                             *
//                                                                         *
// Example: For input -2, 11, -4, 13, -5, -2, the answer is 20             *
//          (a2 through a4).                                               *
//                                                                         *
// This program requests a maximum sequence size, which it reads from      *
// standard input.  It then generates a random sequence of that size, and  *
// then executes each of 4 algorithms to compute the maximum subsequence   *
// sum on various sizes of sequences beginning with the first integer in   *
// the input sequence.  The execution time for each execution of an        *
// algorithm is printed.  A cutoff time is also requested and read from    *
// standard input.  If an algorithm time exceeds this value then no more   *
// timing is done using that algorithm.                                    *
//                                                                         *
// The sizes of the sequences are 100, 200, ..., 1000, 2000, ..., 10000,   *
// 20000, 30000, ... .  The entire sequence is also used if not included   *
// in one of the previous sizes.                                           *
//                                                                         *
// The 4 algorithms used are those provided in the file mss.c (with header *
// mss.h).                                                                 *
//                                                                         *
// Author: Joe Turner (redesigned from an earlier Modula 2 version)        *
// Date:  January 1995                                                     *
//**************************************************************************

#include <iostream.h>
#include <time.h>
#include <stdlib.h>  // for exit
#include <random.h>  // class random
#include "mss.h"

const int maxNum = 1000; // maximum absolute value of integers in the sequence
const int initSize = 100; // initial sequence size for timing
const int initIncr = 100; // initial increment amount for sequence size
const int maxIncr = 50000; // max increment amount for sizes
const int iterations = 9; // number of iterations using each increment
const int incrFactor = 10; // amount to multiply increment by after iterations

int *   sequence;  // random sequence
int     size;        // size of the random sequence
double  MaxTime;     // cutoff time for algorithm execution

 
//--------------------- function initialize --------------------------------
void initialize() {
   // Get the random sequence size and cutoff time, and generate the sequence.

   random  rand;        // the random number generator;

   // Get the sequence size.
  
      cout << "Enter the maximum size of the sequences for timing: " ;
      cout.flush();
      cin >> size;

      if (cin.fail() || size<2) {
         cout << "Invalid size.  Reenter the maximum size: "; cout.flush();
         cin >> size;
         if (cin.fail() || size<2) {
            cout << "Invalid size.  Program terminated." << endl;
            exit(1);
         }
      }

   // Generate the random sequence.

      sequence = new int[size];

      for (int i=0; i<size; i++)  sequence[i] = rand.between(-maxNum, maxNum);


   // Get the cutoff time for continuation of algorithm execution. 

      cout << "Cutoff (in seconds) for the maximum algorithm execution time: ";
      cout.flush();
      cin >> MaxTime;

      if (cin.fail() || MaxTime<1.0) {
         cout << "Invalid cutoff.  Reenter the cutoff time: "; cout.flush();
         cin >> MaxTime;
         if (cin.fail() || size<1.0) {
            cout << "Invalid cutoff time.  Program terminated." << endl;
            exit(1);
         }
      }

} //----------------------- end initialize -----------------------------------



//------------------------- function printResults --------------------------
void printResults (int size, double time, int sum) {
   // Print the results of an algorithm execution.
   // It is assumed that the column headings have been printed.

   int saveWidth = cout.width();  // Save the current field width for cout.

   cout.width(8);
   cout << size;
   cout.width(1); cout << "      ";
   cout.width(6); cout << time;
   cout.width(11); cout << sum << endl;

   cout.width (saveWidth); // Restore the original width.
} //------------------------ end printResults -----------------------------



//------------------------- function doAlgorithm ---------------------------
void doAlgorithm ( int (*maxss) (const int *, const unsigned int) ) {
   // Use the maximum subsequence sum algorithm provided by function *maxss
   // to compute the max subsequence sum for various sizes of sequences.
   // Print the execution time for each computation (and the sum).
   //
   // The first size is initSize.  An increment is added to the size after
   // each computation.  The first increment is initIncr.  The same increment
   // is used for "iterations" computations, then the increment is multiplied
   // by incrFactor and the process is repeated.  The increment is never
   // allowed to exceed maxIncr.  The process stops when 1) the size becomes
   // greater than the entire available sequence, or 2) the MaxTime is
   // exceeded on a computation.  If the entire available sequence is not
   // done by this process and MaxTime is not exceeded, then the entire 
   // sequence is done also.

   int     sum;           // a max subsequence sum
   double  elapse = 0.0,  // elapsed time for a sum computation
           clocks_per_sec=(double)CLOCKS_PER_SEC;  // to convert time to seconds
   clock_t start;         // start time for a sum calculation

   int     thisSize = initSize; // the current sequence size
   int     incr = initIncr;     // the current increment for sizes
   int     lastSize = 0;        // size of the last sequence completed

   cout << "seq. size   time (sec)    sum" << endl << endl;

   while (thisSize < size  && elapse < MaxTime) { // loop for each increment

      for (int i=0;   i<9 && thisSize<size && elapse<MaxTime; 
                      i++, thisSize += incr) {
         // Do the timing for one sequence size.
            start = clock();
            sum = maxss (sequence, thisSize);
            elapse = (clock() - start) / clocks_per_sec;

            printResults(thisSize, elapse, sum);

         lastSize = thisSize;
      } // end for

      // Increment the increment.
         incr *= incrFactor;
         if (incr>maxIncr)  incr = maxIncr;
   } // end while

   // Do the entire sequence if needed.
      if (lastSize<size  &&  elapse < MaxTime) {
         start = clock();
         sum = maxss (sequence, size);
         elapse = (clock() - start) / clocks_per_sec;

         printResults(size, elapse, sum);
      }
} //--------------------- end doAlgorithm --------------------------------


 
//-------------------------------- main -------------------------------------
int main() {

   cout << "Maximum subsequence sum calculations.  For each algorithm, the execution\n";
   cout << "time and maximum subsequence sum are given for various sizes of sequences\n";
   cout << "from a randomly-generated sequence." << endl << endl;
 
   initialize();    

   cout << endl;
   cout << "Algorithm 1:" << endl << endl;
   doAlgorithm (Max_Subsequence_Sum1);

   cout << endl;
   cout << "Algorithm 2:" << endl << endl;
   doAlgorithm (Max_Subsequence_Sum2);

   cout << endl;
   cout << "Algorithm 3:" << endl << endl;
   doAlgorithm (Max_Subsequence_Sum3);

   cout << endl;
   cout << "Algorithm 4:" << endl << endl;
   doAlgorithm (Max_Subsequence_Sum4);

   cout << endl << "  --------- end ------------ " << endl << endl;

return 0;
} //----------------------- end main -------------------------------------
