/* scene.c -- draws a very basic scene */

#include <math.h>
#include <GL/glut.h>

#include "objects.h"
#include "scene.h"
#include "spin.h"
#include "ground.h"
#include "reshape.h"

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

void scene_init() {
	GLfloat light_ambient[] = {1.0, 1.0, 0.8, 1.0};
	GLfloat light_diffuse[] = {1.0, 1.0, 0.8, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = {0.0, 1000.0, 0.0, 0.0};

	/* Set the clear color and initialize shading */
	glEnable(GL_DEPTH_TEST); /* draw things in front only */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	/* Create a light source */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* Enable lighting, and enable the light source */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* Initialize the object drawing module */
	objInit();
	init_ground_list();
}

void scene_redraw(void) {

        really_look_at_it();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* Draw cube 1 */
		glScalef(1.0, 1.0, 1.0);
		glTranslatef(-5.0, 0.0, -10.0);
		glRotatef(spin, 0.0, -1.0, 0.0);
		cube_material();
		objSolidCube();

	/* Draw cube 2 */
		glLoadIdentity();
		glScalef(0.2, 0.2, 0.2);
		glTranslatef(-5.0, 0.5, -10.0);
		glRotatef(spin, -0.5, 1.0, 0.0);
		teapot_material();
		objSolidCube();

	/* Draw ground */
		glLoadIdentity();
	        glScalef(200.0/(GLfloat)GW, 4.0, 200.0/(GLfloat)GH); 
		   /* 2000.0 yeilds a semi-infinite ground */
	        glRotatef(90.0, 1.0, 0.0, 0.0);
		glTranslatef((GLfloat)-GW/2.0, (GLfloat)-GH/2.0, 5.0);
	        draw_ground();

	/* Draw icosahedron */
		glLoadIdentity();
		glTranslatef(0.0, 0.0, -50.0);
		glRotatef(spin*3, 1.0, 1.0, 0.0);
		icosahedron_material();
		objSolidIcosahedron();

	/* Draw tardis */
		glLoadIdentity();
		glTranslatef(0.1, 0.0, -2.0);
		objTardis();

	/* Draw Stars */
		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 1.0);
		objStars();
		glEnable(GL_LIGHTING);

	glPopMatrix();
	/*	glFlush();*/
}
