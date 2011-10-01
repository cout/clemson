// "Smiley2" program
// Paul Brannan
// based on the "Smiley face program by Joe Turner

// Original comments follow:
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
	figure smiley2; // the second smiley face
	figure box;     // the box around the faces
	figure combFig; // the combined figures

	// Build the smiley face.

	smiley.insertArc (2.0, 2.0,   0.25,  0.0, 360.0); // head outline (circle)
	smiley.insertArc (1.92, 2.1,  0.025, 0.0, 360.0); // right eye
	smiley.insertArc (2.08, 2.1,  0.025, 0.0, 360.0); // left eye
	smiley.insertArc (2.0, 2.0,   0.01,  0.0, 360.0); // nose
	smiley.insertArc (2.0, 1.95,  0.1,  -165.0, 150.0); // mouth (smile)

	// Display the face.

	smiley.draw();

	// Build the second smiley face

	smiley2.insertArc (4.0, 2.0, 0.25, 0.0, 360.0); // head outline
	smiley2.insertArc (3.92, 2.1,  0.025, 0.0, 360.0); // right eye
	smiley2.insertArc (4.08, 2.1,  0.025, 0.0, 360.0); // left eye
	smiley2.insertArc (4.0, 2.0,   0.01,  0.0, 360.0); // nose
	smiley2.insertArc (4.0, 1.85,  0.1,  15.0, 150.0); // mouth (frown)

	// Draw the second face

	smiley2.draw();

	// Build the box using penLine

	box.movePenTo(1.0, 1.0);
	box.penLine(5.0,  0.0);
	box.penLine(0.0,  2.0);
	box.penLine(-5.0, 0.0);
	box.penLine(0.0, -2.0);

	// Draw the box

	box.draw();

	crWait();  // Pause

	// Erase the screen

	smiley.erase();
	smiley2.erase();
	box.erase();

	crWait();  // Pause

	combFig = smiley + smiley2 + box;
	combFig.draw();

	crWait();  // Pause until user is ready to quit.

	cout << "--- end of program ---\n\n";

	return 0;
}

