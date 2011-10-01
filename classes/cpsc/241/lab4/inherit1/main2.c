//
//  Main program 2 for C++ inheritance lab.
//
//  This program performs some operations to test the rectangle class.
//  (It also uses the circle class.)
//
//  Author: Joe Turner (for CpSc 241 lab)
//          Clemson University
//  Date:   February 1996

#include "circle.h"
#include "rectangle.h"

int main () {

// Start with a rectangle on the screen.
  rectangle r1(2.0,2.0, 2.0, 1.0);
  Pause(1.0);

// Move the rectangle.
  r1.move(1.0, 0.5);  Pause(1.0);

// Enclose the rectangle with a larger one.
  rectangle r2(3.0, 2.5, 3.0, 2.0);  Pause(1.0);

// Expand the inner rectangle.
  r1.expand(0.5, 0.5);  Pause(1.0);

 // Put a circle in the middle of the rectangles.
  circle c (3.0, 2.5, 0.4);
  crWait();
     
return 0;
}
