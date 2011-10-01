//   "Smiley face" program to illustrate use of the figure class.
//
//   Author: Joe Turner
//           Clemson University
//   Date:  August 1995
//
//   A "smiley" is constructed with center at (2,2) with a radius of .25,
//   and the face is displayed in the graphics window.

#include <figure.h>
#include <iostream.h>

int main() {

   figure smiley;  // the smiley face

   //Build the smiley face.

      smiley.insertArc (2.0, 2.0,   0.25,  0.0, 360.0); // head outline (circle)
      smiley.insertArc (1.92, 2.1,  0.025, 0.0, 360.0); // right eye
      smiley.insertArc (2.08, 2.1,  0.025, 0.0, 360.0); // left eye
      smiley.insertArc (2.0, 2.0,   0.01,  0.0, 360.0); // nose
      smiley.insertArc (2.0, 1.95,  0.1,  -165.0, 150.0); // mouth (smile)

   // Display the face.

      smiley.draw();

   crWait();  // Pause until user is ready to quit.

   cout << "--- end of program ---\n\n";

   return 0;
}

