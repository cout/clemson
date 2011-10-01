//
// Definition for class rectangle (version 2 - inheritance).
//
// An instance of rectangle is a rectangle figure that can be displayed as for
// instances of class figure.  In this version of the rectangle class, a
// rectangle is implemented using inheritance of the figure class.
//
// Adapted from the circle class of CpSc 241 lab
//         Clemson University
//         February 1996

#include <figure.h>

class rectangle : public figure {

public:
    // Constructor:

       rectangle(double x, double y, double w, double h);
          // Create it with center at (x,y), and with 
          //     width (horizontal) w and height (vertical) h.


    // Change the rectangle.  Erase the old one and display the new one.

    void move(double dx, double dy); 
          // Add dx and dy to the current location (x,y), so that the new 
          //         center is at (x+dx, y+dy).

    void expand(double dw, double dh);  // Add dw to the current width and
                                        //     dh to the current height.

private:
    double currentX, currentY;  // Current center coordinates.
    double currentW, currentH;  // Current width and height.
    
    void constructRectangle(figure &); // Private function
    void makeCurrentRectangle(); // Private function
};
