// Implementation for class circle (version 1 - composition).
//
// This implementation of class circle uses a private figure object to 
// represent a circle.
//
// Author: Joe Turner (for CpSc 241 lab)
//         Clemson University
//         February 1996
//

#include "circle.h"

       circle::circle(double x, double y, double r) : 
           currentX(x), currentY(y), currentR(r) {

           makeCurrentCircle();
           C.draw();
       }


    // Display control:

    void circle::draw() {
       C.draw();
    }

    void circle::erase() {
       C.erase();
    }


    // Change the circle.  Erase the old one and display the new one.

    void circle::move(double dx, double dy) {
         // Add dx and dy to the current center (x,y), so that the new
         //       center is at (x+dx, y+dy).

         C.erase();
         currentX += dx; currentY += dy;
         makeCurrentCircle();
         C.draw();
    }
         

    void circle::expand(double dr) {      // Add dr to the current radius.

         C.erase();
         currentR += dr;
         if (currentR <= 0.0)
            currentR = 0.0;
         else {
            makeCurrentCircle();
            C.draw();
         }
    }

    void circle::makeCurrentCircle() {
         // Make figure C represent the current circle.

         C.removeAllComponents();
         C.insertArc(currentX, currentY, currentR, 0.0, 360.0);
    }
