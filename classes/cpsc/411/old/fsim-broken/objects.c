/* objects.c -- a GLUT-like drawing module */

#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "objects.h"

/* Cube data (from the book) */
static GLubyte frontIndices[] = {4, 5, 6, 7};
static GLubyte rightIndices[]  = {1, 2, 6, 5};
static GLubyte bottomIndices[] = {0, 1, 5, 4};
static GLubyte backIndices[] = {0, 3, 2, 1};
static GLubyte leftIndices[] = {0, 4, 7, 3};
static GLubyte topIndices[] = {2, 3, 7, 6};

static GLfloat vertices[][3] = {
	{-0.5, -0.5, -0.5},
	{0.5, -0.5, -0.5},
	{0.5, 0.5, -0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, 0.5, 0.5},
	{-0.5, 0.5, 0.5}
};

/* Icosahedron data (from the book) */

/* Tardis data */

/* Star data */
#define NUMSTARS 100
GLfloat stars[NUMSTARS][3];

void objInit() {
	int j;

	/* Initialize OpenGL */
/*	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);*/
	
	/* Initialize stars */
	srand(time(NULL));
	for(j = 0; j < NUMSTARS; j++) {
		stars[j][0] = ((double)rand() / (double)RAND_MAX) * 200.0 
		                   - 100.0;
		stars[j][1] = ((double)rand() / (double)RAND_MAX) * 200.0 
		                   - 100.0;
		stars[j][2] = ((double)rand() / (double)RAND_MAX) * 200.0 
		                   - 100.0;
	}
}

void objSolidCube() {
/*	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, frontIndices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, rightIndices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, bottomIndices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, backIndices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, leftIndices);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, topIndices);*/
	glutSolidCube(1.0);
}

void objSolidIcosahedron() {
	glutSolidIcosahedron();
}

void objTardis() {
}

void objStars() {
	int j;

	glBegin(GL_POINTS);
	for(j = 0; j < NUMSTARS; j++) glVertex3fv(stars[j]);
	glEnd();
}
