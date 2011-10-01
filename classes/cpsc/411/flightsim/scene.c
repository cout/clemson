/* scene.c -- draws a very basic scene */

#include <math.h>
#include <GL/glut.h>

#include "objects.h"
#include "scene.h"
#include "spin.h"
#include "reshape.h"
#include "input.h"

void tardis_material() {
	GLfloat mat_ambient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void cube_material() {
	GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_diffuse[] = {0.2, 0.5, 0.8, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void teapot_material() {
	GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_diffuse[] = {0.5, 0.5, 0.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void icosahedron_material() {
	GLfloat mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat mat_diffuse[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

#define LOCAL_VIEWER GL_FALSE /* GL_TRUE is slow*/
#define AMBIENCE
#define LIGHT0_VERTICAL_POSITION 10.0
#define LIGHT1_VERTICAL_POSITION 100.0 
/* doesn't matter, this light is a coal miner's light */

void scene_init() {
	GLfloat light0_ambient[] = 
#ifdef AMBIENCE
  {1.0, 1.0, 0.8, 1.0};
#else
  {0.0, 0.0, 0.0, 1.0};
#endif
	GLfloat light0_diffuse[] = {1.0, 1.0, 0.8, 1.0};
	GLfloat light0_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light0_position[] = {0.0, LIGHT0_VERTICAL_POSITION, 0.0, 0.0};

/* 	GLfloat light1_ambient[] =  */
/* #ifdef AMBIENCE */
/*   {1.0, 1.0, 0.8, 1.0}; */
/* #else */
/*   {0.0, 0.0, 0.0, 1.0}; */
/* #endif */
/* 	GLfloat light1_diffuse[] = {1.0, 1.0, 0.8, 1.0}; */
/* 	GLfloat light1_specular[] = {1.0, 1.0, 1.0, 1.0}; */
/* 	GLfloat light1_position[4] = {0.0, LIGHT1_VERTICAL_POSITION, 0.0, 0.0}; */

	/* Set the clear color and initialize shading */
	glEnable(GL_DEPTH_TEST); /* draw things in front only */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	/* Create a light source */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	/* Create a light source */
/* 	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient); */
/* 	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse); */
/* 	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular); */
/* 	glLightfv(GL_LIGHT1, GL_POSITION, light1_position); */
/* 	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0); */
/* 	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0); */
/* 	glLight */

	/* Enable lighting, and enable the light source */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
/* 	glEnable(GL_LIGHT1); */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, LOCAL_VIEWER);

	/* Initialize the object drawing module */
	objInit();
}

void scene_redraw(void) {

        really_look_at_it();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* Draw cube 1 */
/* 		glScalef(1.0, 1.0, 1.0); */
		glTranslatef(-5.0, 0.0, -10.0);
		glRotatef(spin, 0.0, -1.0, 0.0);
 		glDisable(GL_LIGHTING);
/*   		cube_material();   */
		glColor3f(0.0, 0.0, 1.0);
		objSolidCube();
 		glEnable(GL_LIGHTING); 

	/* Draw cube 2 */
 		glLoadIdentity();/*scale tran rot*/
		glTranslatef(-5.0, 1.0, -10.0);
 		glRotatef(spin, -0.0, 1.0, 0.0); 
/* 		glRotatef(325.0, -1.0, -1.0, 0.0); */
 		glScalef(0.2, 0.2, 0.2); 
 		glDisable(GL_LIGHTING);
/*  		teapot_material();  */
		glColor3f(1.0, 1.0, 0.5);
		objSolidCube();
 		glEnable(GL_LIGHTING); 

	/* Draw ground */
		glLoadIdentity();
 	        glScalef(2000.0/(GLfloat)GW, 1.0, 2000.0/(GLfloat)GH);
/*  glScalef(HILLINESS*2000.0/(GLfloat)GW,  */
/* 			 40.0*HILLINESS,  */
/* 			 HILLINESS*2000.0/(GLfloat)GH); */
		   /* 2000.0 yeilds a semi-infinite ground */
 	        glRotatef(90.0, 1.0, 0.0, 0.0);
 		glTranslatef((GLfloat)-GW/4.0, 
			     (GLfloat)-GH/4.0, 
			     1.0);
 	        objGround();

	/* Draw icosahedron */
		glLoadIdentity();
		glLineWidth(1.5);
		glTranslatef(0.0, 1.5, -50.0);
		glRotatef(spin*3, 1.0, 1.0, 0.0);
		icosahedron_material();
		objSolidIcosahedron();
		glDisable(GL_LIGHTING);
 		glColor3f((spin/360.0), 0.0, 0.0); 
		objWireIcosahedron();
		glEnable(GL_LIGHTING);
		glRotatef(spin, 0.0, 0.0, 1.0);
		glScalef(2.0, 2.0, 2.0);
		objWireIcosahedron();
		glLineWidth(1.0);

	/* Draw icosahedron */
/* 		glLoadIdentity(); */
/* 		glTranslatef(0.0, 0.0, -50.0); */
/* 		glRotatef(spin*3, 1.0, 1.0, 0.0); */
/* 		icosahedron_material(); */
/* 		objSolidIcosahedron(); */

	/* Draw tardis */
		glLoadIdentity();
		glTranslatef(0.1, 0.0, -2.0);
		tardis_material();
		objTardis();

	/* Draw sign */
		glLoadIdentity();
		glTranslatef(3.0, 0.0, -3.0);
		glRotatef(210.0, 0.0, 1.0, 0.0);
		glScalef(1.0, 0.4, 1.0);
		objSign();

	/* Draw logo */
		glLoadIdentity();
		glTranslatef(-4000.0, 250.0, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glScalef(2000.0, 2000.0, 0.0);
		objLogo();

	/* Draw galaxy */
/*		glLoadIdentity();
		glTranslatef(4000.0, 500.0, 4000.0);
		glRotatef(45.0, 0.0, 1.0, 0.0);
		glScalef(2000.0, 2000.0, 0.0);
		objGalaxy();*/

	/* Draw mountains */
		glLoadIdentity();
		glTranslatef(-500.0, 0.0, 1000.0);
		objMountains();

	/* Draw Stars */
		glLoadIdentity();
/* 		glTranslatef(0.1, 0.0, -2.0); */
/*   		printf("eye is at %f %f %f\n", eye_x(), eye_y(), eye_z());   */

		objStars();

	glPopMatrix();
	/*	glFlush();*/
}
