// Implementation for class rectangle (version 1 - composition).
//
// This implementation of class rectangle uses a private figure object to 
// represent a rectangle.
//
// Adapted from the circle class for CpSc 241 lab.
//         Clemson University
//         February 1996
//

#include "rectangle.h"

       rectangle::rectangle(double x, double y, double w, double h) :
           currentX(x), currentY(y), currentW(w), currentH(h) {

           makeCurrentRectangle();           
           R.draw();
       }


    // Display control:

    void rectangle::draw() {
      R.draw(); 
    }

    void rectangle::erase() {
      R.erase(); 
    }


    // Change the rectangle.  Erase the old one and display the new one.

    void rectangle::move(double dx, double dy) {
         // Add dx and dy to the current center (x,y), so that the new
         //       center is at (x+dx, y+dy).

         R.erase();
         currentX += dx; currentY += dy;
         makeCurrentRectangle();
         R.draw();
    }
         

    void rectangle::expand(double dw, double dh) {
         // Add dw to the current width and dh to the current height.

         R.erase();
         currentW += dw;  currentH += dh;
         if (currentW < 0.0)
            currentW = 0.0;
         if (currentH < 0.0)
            currentH = 0.0;
         if (currentW > 0.0 && currentH > 0.0){//If there is anything to display
            makeCurrentRectangle();
            R.draw();
         }
    }
 
    void rectangle::constructRectangle(figure & f) { 
         // Insert the current rectangle into figure f.
         // Note: This could be combined with makeCurrentRectangle and
         // eliminated (instead of just having makeCurrentRectangel use it),
         // but it is included here for the practice in using it.

         // Start at the lower left corner.
            f.movePenTo(currentX - currentW/2.0, currentY - currentH/2.0);

         f.penLine(currentW, 0.0);    // bottom
         f.penLine(0.0, currentH);    // right side
         f.penLine(-currentW, 0.0);   // top
         f.penLine(0.0, -currentH);   // left side
    }

    void rectangle::makeCurrentRectangle() {
         // Make figure R represent the current rectangle.

         R.removeAllComponents();
         constructRectangle(R);
    }
