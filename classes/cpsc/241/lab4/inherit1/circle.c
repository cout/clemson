// Implementation for class circle (version 2 - inheritance).
//
// This implementation of class circle uses inheritance with class circle
// being a derived class of class figure.
// 
// Author: Joe Turner (for CpSc 241 lab)
//         Clemson University
//         February 1996 
// 

#include "circle.h"

       circle::circle(double x, double y, double r) : 
           currentX(x), currentY(y), currentR(r) {

           makeCurrentCircle();
           draw();
       }



    // Change the circle.  Erase the old one and display the new one.

    void circle::move(double dx, double dy) {
         // Add dx and dy to the current center (x,y), so that the new
         //       center is at (x+dx, y+dy).

         erase();
         currentX += dx; currentY += dy;
         makeCurrentCircle();
         draw();
    }
         

    void circle::expand(double dr) {      // Add dr to the current radius.

         erase();
         currentR += dr;
         if (currentR<=0.0)
            currentR = 0.0;
         else {
            makeCurrentCircle();
            draw();
         }
    }

    void circle::makeCurrentCircle() {
         // Make the figure represent the current circle.
         
           removeAllComponents();
           insertArc(currentX, currentY, currentR, 0.0, 360.0);
         
    }
