//
// CpSc 241 - Main program for lab on using libraries.
//
//    Input: A sequence of "records", each containing 3 components:
//              1) A name (character string) containing no blanks;
//              2) A major code (positive integer);
//              3) An email address (character string with no blanks).
//           The items are separated by the standard stream separators.
//           An end of file marks the end of the input.  The input data
//           is read from standard input.
//
//    Output:
//           1) A list of the records, in the input order.
//           2) A list of the records, alphabetized on the name.
//
//    Processing: The input records are inserted into a binary search tree,
//                where the comparison between records is done on the name in
//                each record.

#include <iostream.h>
#include "datarec.h"  // ADT for data records
#include <bst.h>      // ADT for BSTs
#include "datarec.c"

int main() {

   datarec      InRec;    // Input data record.
   bst <datarec>  DRTree;   // BST for data records.

   // Read, print, and insert the input records into the tree.

     cout << endl << "The input records are:" << endl << endl;
     datarecHeading(cout) << endl << endl;

     while (cin >> InRec) {
       cout << InRec << endl;
       DRTree.add(InRec);
     }
   
   // Print the records in alphabetical order.

     cout << endl << "The records in order are:" << endl << endl;
     datarecHeading(cout) << endl << endl;

    for (DRTree.init(); !DRTree; ++DRTree)
      cout << DRTree() << endl;
    cout << endl;

  return 0;
}
