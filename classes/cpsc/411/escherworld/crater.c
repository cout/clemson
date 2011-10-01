#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ew.h"

#define NUM_CRATERS 250 
#define MAX_RADIUS 20.0
#define MAX_HEIGHT 5.0
#define BINC 180.0*M_PI
#define WINC 5.0/180.0*M_PI
#define RADIUS 400.0
#define SLICES 8 

static float radius[NUM_CRATERS];
static float height[NUM_CRATERS];
static float x[NUM_CRATERS];
static float y[NUM_CRATERS];
static int listnum;

static float frand() {
	return (float)rand() / (float)RAND_MAX;
}

static void draw_outside(float r, float h, float inc) {
	float theta, ct, st;
	inc /= (MAX_RADIUS - r + 1);
	for(theta = 0.0; theta < 2*M_PI; theta += inc) {
		ct = cos(theta);
		st = sin(theta);
		glTexCoord2f(frand(), frand());
		glVertex3f((r-h)*ct, h, (r-h)*st);
		glTexCoord2f(frand(), frand());
		glVertex3f(r*ct, 0.0, r*st);
	}
}

static void draw_inside(float r, float h, float inc) {
	float theta, ct, st;
	inc /= (MAX_RADIUS - r + 1);
	for(theta = 0.0; theta < 2*M_PI; theta += inc) {
		ct = cos(theta);
		st = sin(theta);
		glTexCoord2f(frand(), frand());
		glVertex3f((r-2*h)*ct, 0.0, (r-2*h)*st);
		glTexCoord2f(frand(), frand());
		glVertex3f((r-h)*ct, h, (r-h)*st);
	}
}

static void draw_lines(float r, float h, float inc) {
	float theta, ct, st, cti, sti;
	r += 0.01;
	h += 0.01;
	
	sti = sin(0.0);
	cti = cos(0.0);
	for(theta = 0.0; theta < 2*M_PI; theta += inc) {
		st = sti;
		ct = cti;
		sti = sin(theta + inc);
		cti = cos(theta + inc);

		glVertex3f(r*ct, 0.0, r*st);
		glVertex3f(r*cti, 0.0, r*sti);
		glVertex3f((r-h)*cti, h, (r-h)*sti);
		glVertex3f((r-h)*ct, h, (r-h)*st);

		glVertex3f((r-2*h)*cti, 0.0, (r-2*h)*sti);
		glVertex3f((r-2*h)*ct, 0.0, (r-2*h)*st);
		glVertex3f((r-h)*ct, h, (r-h)*st);
		glVertex3f((r-h)*cti, h, (r-h)*sti);
	}
}

void do_terrain() {
	int j;
	float theta;

	glDisable(GL_TEXTURE);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0.14, 0.165, 0.14);
	glNormal3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_FAN);
 		glVertex3f(0, -0.001, 0);
		for (j = 0; j < SLICES + 1; j++) {
   			theta = (float)j * M_PI / (float)SLICES;
  			glVertex3f(cos(theta) * RADIUS, -0.001, sin(theta) * RADIUS);
                }
 	glEnd(); 
}

void draw_craters() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(!wireframe) glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	if(terrain && craters) glCallList(listnum);
	else if(terrain && !craters) do_terrain();
	glPopAttrib();
}

void make_crater_list() {
	int j;
        float theta;

	listnum = glGenLists(1);
	glNewList(listnum, GL_COMPILE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glColor3f(0.27, 0.3, 0.27);
	for(j = 0; j < NUM_CRATERS; j++) {
		glPushMatrix();
		glTranslatef(x[j], 0.0, y[j]);

		glBegin(GL_TRIANGLE_STRIP);
		if (BINC * sqrt(x[j]*x[j] + y[j]*y[j]) > 2 * M_PI / 5) {
			draw_outside(radius[j], height[j], 2 * M_PI / 5 );
			draw_inside(radius[j], height[j], 2 * M_PI / 5);
 		}
		draw_outside(radius[j], height[j], BINC * sqrt(x[j]*x[j] + y[j]*y[j]));
		draw_inside(radius[j], height[j], BINC * sqrt(x[j]*x[j] + y[j]*y[j]));
		glEnd();
		glPopMatrix();
	}

        do_terrain();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.05, 0.07, 0.05);
	for(j = 0; j < NUM_CRATERS; j++) {
		glPushMatrix();
		glTranslatef(x[j], 0.0, y[j]);

		glBegin(GL_QUADS);
		draw_lines(radius[j], height[j], WINC);
		glEnd();

		glPopMatrix();
	}

	glEndList();
}

void init_craters() {
	int j;
	printf("Initializing %d craters.\n", NUM_CRATERS);
	for(j = 0; j < NUM_CRATERS; j++) {
		do {
			radius[j] = frand() * MAX_RADIUS;
			height[j]  = frand() * MAX_HEIGHT;
			x[j] = frand() * 2 * RADIUS - RADIUS;
			y[j] = frand() * 2 * RADIUS - RADIUS;
			if(radius[j] < height[j]) radius[j] = height[j];
/*			printf("Crater %d: radius = %f, height = %f\n",
				j, radius[j], height[j]);
*/
		} while(sqrt(x[j]*x[j] + y[j]*y[j]) > RADIUS - radius[j] || 
			y[j] < 0.0);
	}
	make_crater_list();
}
