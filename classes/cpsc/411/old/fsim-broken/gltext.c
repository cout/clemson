/* gltext.c -- a text drawing module
 * this should probably make use of call lists
 */

#include <GL/glut.h>

#include "gltext.h"

#define vect_sizeof(v) (sizeof(v) / (3 * sizeof(GLfloat)))

static GLfloat number0[][3] = {
	{0.0,	2.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	0.0,	0.0},
	{0.0,	0.0,	0.0},
	{0.0,	2.0,	0.0}
};

GLfloat number1[][3] = {
	{1.0,	2.0,	0.0},
	{1.0,	0.0,	0.0}
};

GLfloat number2[][3] = {
	{0.0,	2.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	1.0,	0.0},
	{0.0,	1.0,	0.0},
	{0.0,	0.0,	0.0},
	{1.0,	0.0,	0.0}
};

GLfloat number3[][3] = {
	{0.0,	2.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	1.0,	0.0},
	{0.0,	1.0,	0.0},
	{1.0,	1.0,	0.0},
	{1.0,	0.0,	0.0},
	{0.0,	0.0,	0.0}
};

GLfloat number4[][3] = {
	{0.0,	2.0,	0.0},
	{0.0,	1.0,	0.0},
	{1.0,	1.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	0.0,	0.0}
};

	
GLfloat number5[][3] = {
	{1.0,	2.0,	0.0},
	{0.0,	2.0,	0.0},
	{0.0,	1.0,	0.0},
	{1.0,	1.0,	0.0},
	{1.0,	0.0,	0.0},
	{0.0,	0.0,	0.0}
};

GLfloat number6[][3] = {
	{1.0,	2.0,	0.0},
	{0.0,	2.0,	0.0},
	{0.0,	1.0,	0.0},
	{1.0,	1.0,	0.0},
	{1.0,	0.0,	0.0},
	{0.0,	0.0,	0.0},
	{0.0,	1.0,	0.0}
};

GLfloat number7[][3] = {
	{0.0,	2.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	0.0,	0.0}
};

GLfloat number8[][3] = {
	{1.0,	2.0,	0.0},
	{0.0,	2.0,	0.0},
	{0.0,	1.0,	0.0},
	{1.0,	1.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	0.0,	0.0},
	{0.0,	0.0,	0.0},
	{0.0,	1.0,	0.0}
};

GLfloat number9[][3] = {
	{1.0,	2.0,	0.0},
	{0.0,	2.0,	0.0},
	{0.0,	1.0,	0.0},
	{1.0,	1.0,	0.0},
	{1.0,	2.0,	0.0},
	{1.0,	0.0,	0.0}
};

GLfloat period[][3] = {
	{0.8,	0.0,	0.0},
	{1.0,	0.0,	0.0},
	{1.0,	0.2,	0.0},
	{0.8,	0.2,	0.0},
	{0.9,	0.0,	0.0}
};

void draw_points(GLfloat points[][3], int n) {
	int j;

	glBegin(GL_LINE_STRIP);
	for(j = 0; j < n; j++) glVertex3fv(points[j]);
	glEnd();
};

void draw0() {
	draw_points(number0, 5);
}

void draw_a_char(char s) {
	switch(s) {
	case '0': draw_points(number0, vect_sizeof(number0)); break;
	case '1': draw_points(number1, vect_sizeof(number1)); break;
	case '2': draw_points(number2, vect_sizeof(number2)); break;
	case '3': draw_points(number3, vect_sizeof(number3)); break;
	case '4': draw_points(number4, vect_sizeof(number4)); break;
	case '5': draw_points(number5, vect_sizeof(number5)); break;
	case '6': draw_points(number6, vect_sizeof(number6)); break;
	case '7': draw_points(number7, vect_sizeof(number7)); break;
	case '8': draw_points(number8, vect_sizeof(number8)); break;
	case '9': draw_points(number9, vect_sizeof(number9)); break;
	case '.': draw_points(period, vect_sizeof(period)); break;
	}
}

void draw_char(char s) {
	draw_a_char(s);
}

void draw_text(const char *s) {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	for(; *s != 0; s++) {
		glTranslatef(1.4, 0.0, 0.0);
		draw_a_char(*s);
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
