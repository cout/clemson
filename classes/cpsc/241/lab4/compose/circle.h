//
// Definition for class circle (version 1 - composition).
//
// An instance of circle is a circle figure that can be displayed as for
// instances of class figure.  In this version of the circle class, a
// circle is implemented using composition with the figure class.  That is,
// an instance of figure is a private data object, and the figure instance
// is used for a circle.
//
// Author: Joe Turner (for CpSc 241 lab)
//         Clemson University
//         February 1996

#include <figure.h>

class circle {

public:
    // Constructor:

       circle(double x, double y, double r);
          // Create it at center (x,y) with radius r, and display it.

    // Display control:

    void draw();     // Display the circle.

    void erase();       // Erase the circle.


    // Change the circle.  Erase the old one and display the new one.

    void move(double dx, double dy); 
          // Add dx and dy to the current center (x,y), so that the new center
          //         is at (x+dx, y+dy).

    void expand(double dr);       // Add dr to the current radius.

private:
    double currentX, currentY;  // Current center coordinates.
    double currentR;            // Current radius.
    figure C;                   // The current circle figure.
    
    void makeCurrentCircle(); // Private function
};
