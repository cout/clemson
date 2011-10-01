//
// Traveling Salesperson Problem - Exhaustive Search "Solution"
//

#include <iostream.h>
#include <time.h>

// Algorithm: Initialize a vector ("path") to the sequence 1, 2, ..., n,
//   where n is the number of cities.  Read a matrix ("distance") of
//   distances between cities.  Generate all permutations of the sequence,
//   keeping the current distance corresponding to the (partial) sequence
//   being generated.  As each circuit is completed, update the best
//   distance and path if appropriate.
//
// Inputs:  (all from standard input, without prompt)
//          (Each item below begins on a new line. Extra data on a line
//            is ignored.)
//   1. Tracing indicator (0=>no, other=>yes)
//   2. Pruning indicator (0=>no, other=>yes)
//   3. Number of cities
//   4. Distance matrix for cities: 
//        Each line contains the distances for one row of the matrix.  Any
//        remaining data on a line is ignored after n items are read, where
//        n is the number of cities (input item 4).  
//   Input is not checked for validity.  The maximum length of an input line
//   is 100.
//
// Options:
//
//   Tracing: Each completed path is printed, including the last city in
//            a path when pruning occurs.
//   Pruning: With this option, the generation of additional permutations of
//            the tail of a sequence terminates when the length of a partial
//            path exceeds the best length previously found.

#define MAXCITIES 20  // maximum number of cities
#define CLOCKS_PER_SEC  1000000L   // for use with clock()        

int NumCities,  // the number of cities for this execution
    Prune,      // pruning indicator (0=>no)
    Trace,      // tracing indicator
    CurrentLength, // current length of (partial) path
    BestLength;    // length of the best path so far
// For timing:
    double  elapse, clocks_per_sec=(double)CLOCKS_PER_SEC;
    clock_t start;

char LineBuf[100];  // buffer for flushing the remainder of input lines

int path[MAXCITIES+1],  // the current path being generated
    BestPath[MAXCITIES+1], // the best path found so far
    distance[MAXCITIES+1][MAXCITIES+1];  // distances between cities


void Initialize(){
  // Program initialization, including the input data.

  // Get the input data.

     cin >> Trace;  cin.getline(LineBuf,100);
     cin >> Prune;  cin.getline(LineBuf,100);
     cin >> NumCities;  cin.getline(LineBuf,100);

     // Get the distance matrix.

        // Set the distances corresponding to city 0 to 0.  (This avoids
        // handling special cases.)
        for (int i=0; i<=NumCities; i++) {
          distance[0][i] = 0;  distance[0][i] = 0;
        }

        for (i=1; i<=NumCities; i++){
          for (int j=1; j<=NumCities;  j++) // Get row i.
            cin >> distance[i][j];
          cin.getline(LineBuf, 100); // Flush the rest of the line.
        }

  // Initialize the path array to 0, 1, 2, 3, ..., NumCities.  The initial
  // 0 is to avoid a special case for the first entry.
     for (i=0; i<=NumCities; i++)
        path[i] = i;
  BestLength = -1;  CurrentLength = 0;
} // *** end Initialize. ***


void PrintData(){
  // Print the data for the path finding.
  int dist;

  cout << endl << "Traveling Salesperson Problem --" << endl <<endl;
  cout << "Number of cities: " << NumCities << endl << endl;
  cout << "Distance matrix: " << endl;
  for (int i=1; i<=NumCities; i++){
    cout << "    ";
    for (int j=1; j<=NumCities; j++){
      dist = distance[i][j];
      if (dist<100) cout << " ";
      if (dist<10)  cout << " ";
      cout << dist << " ";
    }
    cout << endl;
  }
  cout << endl;

  if (Prune) cout << "Pruning is used." << endl;
  else cout << "Pruning is not used." << endl;

} // *** end PrintData ***


void PrintTrace (int position){
  // Print current trace info: best length, current length, path[1..position].

  cout << "Best: " << BestLength << ", Current: " << CurrentLength <<
          ", Path: " ;
  for (int i=1; i<=position; i++)
     cout << path[i] << " ";
  cout << endl;
} // *** end PrintTrace ***


void FindBest (int position) {
  // Find the best path that can be obtained by trying all permutations
  // of the tail of the path, beginning at position "position".

  int SaveCurrent, // value of CurrentLength at entry
      temp;        // for interchanging cities in the path
      

  SaveCurrent = CurrentLength;

  // Add the distance from the previous city to this city to the current length.
  CurrentLength = CurrentLength + distance[path[position-1]][path[position]];

  if (position == NumCities) {  // path is complete
     
     // Add the distance from the current city to the start city.
     CurrentLength = CurrentLength + distance[path[position]][path[1]];

     if (BestLength<0 || BestLength>CurrentLength){ // This one is best so far.
       BestLength = CurrentLength;
       for (int i=1; i<=NumCities; i++) BestPath[i] = path[i];
     }

     if (Trace) PrintTrace(position);
   }
   else {  // path is not complete

     if (Prune && CurrentLength>=BestLength && BestLength>0) {
        // No need to proceed: this one is already no good.
       if (Trace) PrintTrace(position);
     }
     else { // need to generate the rest of the path

       FindBest(position+1); // Find permutations of the tail.
     }

     // Interchange position with successive tail positions and permute.
     for (int i=position+1; i<=NumCities; i++){
         temp = path[position];
         path[position] = path[i];
         path[i] = temp;
         CurrentLength = SaveCurrent +
                           distance[path[position-1]][path[position]];
         if (Prune && CurrentLength>=BestLength && BestLength>0) {
            // No need to proceed: this one is already no good.
           if (Trace) PrintTrace(position);
         }
         else { // need to generate the rest of the path
    
           FindBest(position+1); // Find permutations of the tail.
         }
     }
     // Restore the base path.
       temp = path[position];
       for (i=position; i<NumCities; i++) path[i] = path[i+1];
       path[NumCities] = temp;
   }
   CurrentLength = SaveCurrent; // Leave it like we found it.
} // *** end FindBest ***


int main(){

   Initialize();
   PrintData();

   start = clock();
   FindBest(1);
   elapse = (clock() - start) / clocks_per_sec;

   // Print results.

     cout << endl;
     cout << "Best path: " ;
     for (int i=1; i<=NumCities; i++)
        cout << BestPath[i] << " ";
     cout << endl;
     cout << "Length: " << BestLength << endl << endl;

     cout << "Execution time: " << elapse << endl << endl;
}
