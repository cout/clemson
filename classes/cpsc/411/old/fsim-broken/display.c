/* display.c -- draw azimuth, elevation, and roll displays *
 * All functions assume a coordinate system of (0.0,0.0) to (1.0,1.0)
 */

#include <math.h>
#include <GL/glut.h>

#include "display.h"
#include "fsim.h"

static void drawRoundDisplay();
static void drawAxes();
static void drawMeter(float min, float max, float value, float angle);

/* Draws a circle with ticks for use with drawMeter and friends */
static void drawRoundDisplay() {
	int j;
	float tmp, sintmp, costmp;

	/* Draw a circle */
	glBegin(GL_LINE_LOOP);
	for(j = 0; j < 360; j++) {
		tmp = j * 2 * M_PI / 360.0;
		glVertex2f(0.5 + 0.5*cos(tmp), 0.5 + 0.5*sin(tmp));
	}
	glEnd();

	/* Draw tick marks at every 30 degrees */
	glBegin(GL_LINES);
	for(j = 0; j < 360; j += 30) {
		tmp = j * 2 * M_PI / 360.0;
		costmp = cos(tmp); sintmp = sin(tmp);

		glVertex2f(0.5 + 0.5*costmp, 0.5 + 0.5*sintmp);
		glVertex2f(0.5 + 0.45*costmp, 0.5 + 0.45*sintmp);
	}
	glEnd();
}

/* Draws axes along the middle of the window
 * for use with drawAEGraph()
 */
static void drawAxes() {
	int j;

	glBegin(GL_LINES);
		/* Draw coordinate axes */
		glVertex2f(0.5, 0.0);
		glVertex2f(0.5, 1.0);
		glVertex2f(0.0, 0.5);
		glVertex2f(1.0, 0.5);

		/* Draw tick marks */
		for(j = 1; j < 10; j++) {
			glVertex2f(0.5, 0.1 * j);
			glVertex2f(0.55, 0.1 * j);
			glVertex2f(0.1 * j, 0.5);
			glVertex2f(0.1 * j, 0.55);
		}
	glEnd();
}

static void drawMeter(float min, float max, float value, float angle) {
	float tmp = 2*M_PI*((value - min)/max),
		costmp = cos(tmp),
		sintmp = sin(tmp);

	glColor3f(1.0, 1.0, 1.0);
	drawRoundDisplay();

	glBegin(GL_LINES);
		/* Draw north arrow in red */
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5 + 0.3*costmp, 0.5 + 0.3*sintmp);
	
		/* Draw south arrow in grey */
		glColor3f(0.5, 0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5 - 0.3*costmp, 0.5 - 0.3*sintmp);
	glEnd();
}

void drawAltimeter(float min_height, float max_height, float height) {
	drawMeter(min_height, max_height, height, 0.0);
}

void drawCompass(float angle) {
	drawMeter(0.0, 360.0, angle, 90.0);
}

#define ELEV_MIN -90.0
#define ELEV_MAX 90.0
/* Local var elevation must be scaled to within [0.0, 1.0] so that the 
   parameter may take on any values between ELEV_MIN and ELEV_MAX. */
void drawAEGraph(float azimuth, float elevation) {
  elevation = (elevation - ELEV_MIN) / (ELEV_MAX - ELEV_MIN);
        glLineWidth(4.0);
	glBegin(GL_LINES);
		/* Draw azimuth */
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(azimuth/360.0 + 0.5, 0.0);
		glVertex2f(azimuth/360.0 + 0.5, 1.0);

		/* Draw elevation */
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(0.0, elevation);
		glVertex2f(1.0, elevation);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1.0);
	drawAxes();
}
