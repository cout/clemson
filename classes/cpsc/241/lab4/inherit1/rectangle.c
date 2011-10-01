// Implementation for class rectangle (version 2 - inheritance).
//
// Adapted from the circle class for CpSc 241 lab.
//         Clemson University
//         February 1996
//

#include "rectangle.h"

       rectangle::rectangle(double x, double y, double w, double h) :
           currentX(x), currentY(y), currentW(w), currentH(h) {

           makeCurrentRectangle();           
           draw();
       }



    // Change the rectangle.  Erase the old one and display the new one.

    void rectangle::move(double dx, double dy) {
         // Add dx and dy to the current center (x,y), so that the new
         //       center is at (x+dx, y+dy).

         erase();
         currentX += dx; currentY += dy;
         makeCurrentRectangle();
         draw();
    }
         

    void rectangle::expand(double dw, double dh) {
         // Add dw to the current width and dh to the current height.

         erase();
         currentW += dw;  currentH += dh;
         if (currentW < 0.0)
            currentW = 0.0;
         if (currentH < 0.0)
            currentH = 0.0;
         if (currentW > 0.0 && currentH > 0.0){//If there is anything to display
            makeCurrentRectangle();
            draw();
         }
    }
 
    void rectangle::makeCurrentRectangle() {
         // Make figure R represent the current rectangle.

         removeAllComponents();
         movePenTo(currentX - currentW/2.0, currentY - currentH/2.0);
         penLine(currentW, 0.0);    // bottom
         penLine(0.0, currentH);    // right side
         penLine(-currentW, 0.0);   // top
         penLine(0.0, -currentH);   // left side

    }
