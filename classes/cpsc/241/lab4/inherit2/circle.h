//
// Definition for class circle (version 2 - inheritance).
//
// An instance of circle is a circle figure that can be displayed as for
// instances of class figure.  In this version of the circle class, a
// circle is implemented using inheritance by making circle a derived
// class (or child class) of the figure class.
//
// Author: Joe Turner (for CpSc 241 lab)
//         Clemson University
//         February 1996


#include <figure.h>

class circle : public figure {

public:
    // Constructor:

       circle(double x, double y, double r);
          // Create it at center (x,y) with radius r, and display it.


    // Change the circle.  Erase the old one and display the new one.

    void move(double dx, double dy); 
          // Add dx and dy to the current center (x,y), so that the new center
          //         is at (x+dx, y+dy).

    void expand(double dr);       // Add dr to the current radius.

private:
    double currentX, currentY;  // Current center coordinates.
    double currentR;            // Current radius.
    
    void makeCurrentCircle(); // Private function
};
