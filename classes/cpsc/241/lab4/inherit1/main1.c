//
//  Main program 1 for C++ inheritance lab.
//
//  This program performs some operations to test the circle class.
//
//  Author: Joe Turner (for CpSc 241 lab)
//          Clemson University
//  Date:   February 1996

#include "circle.h"

int main () {

// Instantiate and display a circle with center at (1,1) and radius .5.
  circle c1(1.0, 1.0, 0.5);  
  Pause(1.0);

// Instantiate and display a circle with center at (2,3) and radius .25.
  circle c2(2.0, 3.0, 0.25);
  Pause(1.0);

// Move the first circle and expand the second.
  c1.move(0.5, -0.5);  c2.expand(0.2);
  Pause(1.0);

// Erase the two circles.
  c1.erase(); c2.erase();

// Create an array of pointers to circles.
  circle * C[10];
  int i;        // index for loops

// Build an array of concentric circles for cascading, and display the circles.
  double r;
  for (i=0, r=0.1; i<10; i++, r+=0.1) {
     C[i] = new circle(2.0, 2.0, r); 
  }
  
// Cascade the expanding circles.
  int start=0;    // successive beginning indices for cascades
  for (int j=0; j<20; j++) {  // Do it 20 times.
     for (int k=0, i=start; k<10; k++, i = (++i)%10) {//Treat array as circular.
        C[i]->expand(0.05);  Pause(0.02);
     }
     start = (start+9)%10;
     C[start]->expand(-1.0);   // Start the largest one over as the smallest.
     for (k=0, i=start; k<10; k++, i= (++i)%10) {
        C[i]->expand(0.05);  Pause(0.02);
     }
  }
     
  

return 0;
}
