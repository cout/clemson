#include <assert.h>
#include <stdio.h>
#include <GL/glut.h>
#include "box.h"
#include "ew.h"
#include "texgen.h"

int cube_face_list;
int whole_cube_list;

#define CUBE_FACE_LIST cube_face_list
#define WHOLE_CUBE_LIST whole_cube_list
#define CUBE_SUBDIV 2

static void make_dl_for_grid(int dl_num, int w, int h) {
  int i, j, newi;
  GLfloat f;

  glNewList(dl_num, GL_COMPILE);
  f = h - 1;
  for (i=0; i<w-1; i++) {
    glBegin(GL_TRIANGLE_STRIP);
    for (j=0; j<h; j++) {
      glNormal3f(0.0, 0.0, -1.0);
      do_tex_coords(0.0, 0.0, i/f,j/f,0.0, TEX_GRID);
      glVertex3f((GLfloat)i/f, (GLfloat)j/f, 0.0);
      do_tex_coords(0.0, 0.0, i/f,j/f,0.0, TEX_GRID);
      glNormal3f(0.0, 0.0, -1.0);
      glVertex3f((GLfloat)(i+1)/f, (GLfloat)j/f, 0.0);
    }
    glEnd();
  }
  glEndList();
}

static void init_gridcube(void) {
  make_dl_for_grid(CUBE_FACE_LIST, CUBE_SUBDIV, CUBE_SUBDIV);
  glNewList(WHOLE_CUBE_LIST, GL_COMPILE);

  glPushMatrix();

  glTranslatef(-0.5,-0.5,-0.5);

  /* draw front and back */
  glCallList(CUBE_FACE_LIST);
  glTranslatef(1.0, 0.0, 1.0);
  glRotatef(180.0, 0.0, 1.0, 0.0);
  glCallList(CUBE_FACE_LIST);

  /* draw top and bottom */
  glTranslatef(0.0, 0.0, 1.0);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glCallList(CUBE_FACE_LIST);
  glTranslatef(1.0, 0.0, 1.0);
  glRotatef(180.0, 0.0, 1.0, 0.0);
  glCallList(CUBE_FACE_LIST);

  /* draw left and right */
  glTranslatef(0.0, 0.0, 1.0);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glCallList(CUBE_FACE_LIST);
  glTranslatef(1.0, 0.0, 1.0);
  glRotatef(180.0, 0.0, 1.0, 0.0);
  glCallList(CUBE_FACE_LIST);

  glPopMatrix();
  glEndList();
}

static void do_cube() {
	glCallList(WHOLE_CUBE_LIST);
}

void init_box() {
	whole_cube_list = glGenLists(1);
	cube_face_list = glGenLists(1);
	init_gridcube();
}

void draw_box(float l, float w, float h) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_NORMALIZE);
	/* Draw the box using the current color*/
	glScalef(l,w,h);

	/* glDisable(GL_CULL_FACE);*/
	if(!wireframe) {
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	glColor3f(1.0, 1.0, 1.0);
	do_cube();

	/* Draw the frame using black */
	if(!wireframe) {
		glDisable(GL_LIGHTING);
		glColor3f(0.0, 0.0, 0.0);
		glutWireCube(1.01);
	}
	glDisable(GL_NORMALIZE);

	glPopAttrib();
}

