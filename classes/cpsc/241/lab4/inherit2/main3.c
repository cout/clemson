//
//  Main program 3 for C++ inheritance lab.
//
//  This program illustrates that the circle and rectangle classes, if
//  derived from figure, inherit the operations of figure and can be used
//  in conjunction with figure.
//
//  Author: Joe Turner (for CpSc 241 lab)
//          Clemson University
//  Date:   February 1996

#include "circle.h"
#include "rectangle.h"

int main () {

   figure f;
   rectangle r1(3.0, 2.5, 3.0, 2.0);
   rectangle r2(3.0, 2.5, 2.5, 1.5);
   circle c1(3.0, 2.5, 0.4);

   f = r1;   // A rectangle is a figure, so it can be assigned to a figure.
   f += r2;  // A rectangle is a figure, so it can be added to a figure.
   f += c1;

   Pause(1.5);
   r1.erase(); r2.erase(); c1.erase();  Pause(1.0);

   f.draw();
   Pause(1.5);

   circle c2(1.0, 1.0, 0.25);  Pause(1.0);
   c2.erase();
   c2.insertLine(0.5,1.5, 1.0, -1.0);  Pause(1.0);

   c2.draw();
   crWait();

return 0;
}
