/* objects.c -- a GLUT-like drawing module */

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "objects.h"
#include "input.h"
#include "ppm.h"
#include "point.h"

static GLuint ground_texture[1];
static GLuint stone_texture[1];

static void init_gridcube(void);
static void init_ground_list(void);
static void init_icosahedron_list(void);
static void init_tardis_list(void);

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
#define X .525731112119133606
#define Z .850650808352039932

static GLfloat vdata[12][3] = {
	{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
	{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
	{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

static GLuint tindices[20][3] = {
	{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
	{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
	{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
};

/* Tardis data */
GLuint tardis_texture[2];

GLuint misc_texture[3];

/* Star data */
static GLfloat stars[NUMSTARS][3];

void objInit() {
	int j;
	GLfloat norm;

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	/* Initialize stars */
	srand(time(NULL));
	for(j = 0; j < NUMSTARS; j++) {
	  stars[j][0] = RANDOM_NUM * 200.0 - 100.0;
	  stars[j][1] = RANDOM_NUM * 200.0 - 100.0;
	  stars[j][2] = RANDOM_NUM * 200.0 - 100.0;
	  norm = sqrt(SQR(stars[j][0]) + SQR(stars[j][1]) + SQR(stars[j][2]));
	  stars[j][0] /= norm;
	  stars[j][1] /= norm;
	  stars[j][2] /= norm;
#ifdef HEMISTARS
	  if (stars[j][1] < 0.0) stars[j][1] = -stars[j][1];
#endif
	}
	glNewList(STARS_LIST, GL_COMPILE);
	  glScalef(STARS_SCALE, STARS_SCALE, STARS_SCALE);
/* 	  glPushMatrix(); */
/* 	  glPushAttrib(GL_CURRENT_BIT); */
	    glDisable(GL_LIGHTING);
	    glBegin(GL_POINTS);
	    for (j = 0; j < NUMSTARS; j++) {
	      /* 	  glColor3f(RANDOM_NUM, RANDOM_NUM, RANDOM_NUM); */
	      /* 	  glColor3f(stars[j][1], stars[j][0], stars[j][2]); */
	      glColor3f(STARS_MIN_R + RANDOM_NUM * (1.0 - STARS_MIN_R), 
			STARS_MIN_G + RANDOM_NUM * (1.0 - STARS_MIN_G), 
			STARS_MIN_B + RANDOM_NUM * (1.0 - STARS_MIN_B));
	      glVertex3f(stars[j][0], stars[j][1], stars[j][2]);
	    }
	    glEnd();
	    glEnable(GL_LIGHTING);
/* 	    glPopAttrib(); */
/* 	  glPopMatrix(); */
	glEndList();

	/* Initialize ground */
	init_ground_list();

	/* Initialize cube */
	init_gridcube();

	/* Initialize icosahedron */
	init_icosahedron_list();

	/* Load tardis texture and initialize tardis call list */
	glGenTextures(2, tardis_texture);
	loadppm("tardis.ppm", tardis_texture[0]);
	loadppm("tartop.ppm", tardis_texture[1]);
	init_tardis_list();

	glGenTextures(2, misc_texture);
	loadppm("litter.ppm", misc_texture[0]);
	loadppm("drwho.ppm", misc_texture[1]);
	loadppm("galaxy.ppm", misc_texture[2]);

	glGenTextures(1, stone_texture);
	loadppm("stone.ppm", stone_texture[0]);
}

void objSolidCube() {
/* 	glutSolidCube(1.0); */
/*   glScalef(0.5, 0.5, 0.5); */
  glCallList(WHOLE_CUBE_LIST);
}

void cross(GLfloat a[], GLfloat b[], Point *p) {
  p->x = a[0]*b[1] - a[1]*b[0];
  p->y = a[2]*b[0] - a[0]*b[2];
  p->z = a[1]*b[2] - a[2]*b[1];
}

void minus(GLfloat a[], GLfloat b[], GLfloat d[]) {
  int i;
  for (i=0; i<3; i++)
    d[i] = a[i] - b[i];
}
 
void init_icosahedron_list(void) {
	int i;
/* 	glutSolidIcosahedron(); */
/* 	Point norm; */
/* 	GLfloat a[3], b[3]; */
/* 	int clockwise[20] = { 1, 1, 1, 1, 1,      */
/* 			      0, 0, 0, 0, 0, */
/* 			      0,0,0,0,0, */
/* 			      1,1,1,1,1}; */
					   
  glNewList(SOLID_ICOSAHEDRON_LIST, GL_COMPILE);
	glBegin(GL_TRIANGLES);		   
	for(i = 0; i < 20; i++) {	   
/* 	  minus(vdata[tindices[i][0]], vdata[tindices[i][1]], a); */
/* 	  minus(vdata[tindices[i][0]], vdata[tindices[i][2]], b); */
/* 	  if (clockwise[i]) */
/* 	    cross(b, a, &norm); */
/* 	  else */
/* 	    cross(a, b, &norm); */
/* 	  glNormal3f(norm.x, norm.y, norm.z); */
	  glVertex3fv(&vdata[tindices[i][0]][0]);
	  glVertex3fv(&vdata[tindices[i][1]][0]);
	  glVertex3fv(&vdata[tindices[i][2]][0]);
	}
	glEnd();
    glEndList();

    glNewList(WIRE_ICOSAHEDRON_LIST, GL_COMPILE);
	for(i = 0; i < 20; i++) {
		glBegin(GL_LINE_LOOP);
		glVertex3fv(&vdata[tindices[i][0]][0]);
		glVertex3fv(&vdata[tindices[i][1]][0]);
		glVertex3fv(&vdata[tindices[i][2]][0]);
		glEnd();
	}
    glEndList();
}

void objSolidIcosahedron() {
	glCallList(SOLID_ICOSAHEDRON_LIST);
}

void objWireIcosahedron() {
	glCallList(WIRE_ICOSAHEDRON_LIST);
}

void objTardisTop() {
    glBegin (GL_QUADS);
		glTexCoord2f(0.0, 1.0);		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(1.0, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f(1.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0);		glVertex3f(0.0, 1.0, 0.0);
    glEnd ();
}

void objTardisWall() {
    glBegin (GL_QUADS);
		glTexCoord2f(0.0, 1.0);		glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0);		glVertex3f(1.0, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0);		glVertex3f(1.0, 2.0, 0.0);
		glTexCoord2f(0.0, 0.0);		glVertex3f(0.0, 2.0, 0.0);
    glEnd ();
}

void objSign() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, misc_texture[0]);
	objTardisTop();
	glDisable(GL_TEXTURE_2D);
}

void objLogo() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, misc_texture[1]);
	objTardisTop();
	glDisable(GL_TEXTURE_2D);
}

void objGalaxy() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, misc_texture[2]);
	objTardisTop();
	glDisable(GL_TEXTURE_2D);
}

void objMountains() {
	int i;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stone_texture[0]);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0, 0.0);		glVertex2f(0.0, 0.0);
		glTexCoord2f(1.0, 0.5);		glVertex2f(1000.0, 50.0);
		glTexCoord2f(0.0, 1.0);		glVertex2f(2000.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void init_tardis_list() {
	glNewList(TARDIS_LIST, GL_COMPILE);

	glEnable (GL_TEXTURE_2D);

	/* Draw sides */
	glBindTexture (GL_TEXTURE_2D, tardis_texture[0]);
	objTardisWall();
	glPushMatrix();
		glTranslatef(1.0, 0.0, -1.0);
		glRotatef(180.0, 0.0, 1.0, 0.0);
		objTardisWall();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0, 0.0, -1.0);
		glRotatef(270.0, 0.0, 1.0, 0.0);
		objTardisWall();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		objTardisWall();
	glPopMatrix();

	/* Draw top */
	glBindTexture(GL_TEXTURE_2D, tardis_texture[1]);
	glTranslatef(0.0, 2.0, -1.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	objTardisTop();

    glDisable (GL_TEXTURE_2D);

	glEndList();
}

void objTardis() {
	glCallList(TARDIS_LIST);
}

void objStars() {
/* 	glBegin(GL_POINTS); */
/* 	for(j = 0; j < NUMSTARS; j++) glVertex3fv(stars[j]); */
/* 	glEnd(); */
 	glTranslatef(eye_x(), eye_y(), eye_z());
	glCallList(STARS_LIST);
}

static void make_dl_for_grid(int dl_num, int w, int h,
			     double **elevation, 
			     double ***color, int singlecolor) {
  /* If elevation is NULL, use zeroes.  If color is null, use random
     colors if singlecolor is zero, or don't specify a color if 
     singlecolor == 1..
     Make sure one or the other of the colors is null.  If both are null,
     random colors are used.
     w corresponds to the first index in the elevation and color arrays, h
     to the second index. */
  int i, j, newi;
  GLfloat f;

  assert(!glIsList(dl_num));

  glNewList(dl_num, GL_COMPILE);
  f =1.0/(double)(h - 1); /* h == w assumed */
  glScalef(f, f, f);
  for (i=0; i<w-1; i++) {
    glBegin(GL_TRIANGLE_STRIP);
    for (j=0; j<h; j++) {
      glNormal3f(0.0, 0.0, 1.0);
      if (color != NULL)
	glColor3f(color[i][j][0], color[i][j][1], color[i][j][2]);
      if (color == NULL && !singlecolor)
	glColor3f(RANDOM_NUM, RANDOM_NUM, RANDOM_NUM);
      glVertex3f((GLfloat) i, (GLfloat)j, 
		 (elevation == NULL) ? 0.0 : (GLfloat)elevation[i][j]);
      newi = i+1;
      if (color != NULL)
	glColor3f(color[newi][j][0], color[newi][j][1], color[newi][j][2]);
      if (color == NULL && !singlecolor)
	glColor3f(RANDOM_NUM, RANDOM_NUM, RANDOM_NUM);
      glVertex3f((GLfloat) newi, (GLfloat)j, 
		 (elevation == NULL) ? 0.0 : (GLfloat)elevation[newi][j]);
    }
    glEnd();
  }
  glEndList();
}


static void init_gridcube(void) {
  make_dl_for_grid(CUBE_FACE_LIST, CUBE_SUBDIV, 
		   CUBE_SUBDIV, NULL, NULL, RANDOM_COLORED_CUBE);
  glNewList(WHOLE_CUBE_LIST, GL_COMPILE);

  /* draw front and back */
  glCallList(CUBE_FACE_LIST);
  glTranslatef(0.0, 0.0, (GLfloat)(CUBE_SUBDIV - 1));
  glCallList(CUBE_FACE_LIST);

  /* draw top and bottom */
  glRotatef(90.0, 0.0, 1.0, 0.0);
  glCallList(CUBE_FACE_LIST);
  glTranslatef(0.0, 0.0, (GLfloat)(CUBE_SUBDIV - 1));
  glCallList(CUBE_FACE_LIST);

  /* draw left and right */
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glCallList(CUBE_FACE_LIST);
  glTranslatef(0.0, 0.0, (GLfloat)(CUBE_SUBDIV - 1)/*, 0.0, 0.0*/);
  glCallList(CUBE_FACE_LIST);

  /* lighting is on by default */
  glEndList();

}

static void init_ground_list(void) {
  double elevation[GW][GH];
  double color[GW][GH][3];
  int i, j, newi;

  for (i=0; i<GW; i++) 
    for (j=0; j<GH; j++) {
      color[i][j][0]=color[i][j][2] = 0.1; 
              /* the red and blue */
      color[i][j][1] = 0.2 + 0.55*((double)rand() / (double)RAND_MAX); 
              /* the green*/
#if HEIGHT_MODE == SINES      
      elevation[i][j] = sin((double)i * (double)j);
#else
      elevation[i][j] = 
	((double)rand() / (double)RAND_MAX) * RANDOM_MAX_HEIGHT;
#endif
    }
  
  /* load the textures */
  glGenTextures(2, ground_texture);  
  loadppm("grass.ppm", ground_texture[0]);

  assert(!glIsList(GLN));

  glNewList(GLN, GL_COMPILE);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, ground_texture[0]);

  for (i=0; i<GW-1; i++) {
    glBegin(GL_TRIANGLE_STRIP);
    for (j=0; j<GH; j++) {
      glTexCoord2i(64*(j%2), 64*(1-(j%2)));
      glVertex3f((GLfloat) i, (GLfloat)j, (GLfloat)elevation[i][j]);
      newi = i+1;
      glTexCoord2i(64*(1-(j%2)), 64*(1-(j%2)));
      glVertex3f((GLfloat) newi, (GLfloat)j, (GLfloat)elevation[newi][j]);
    }
    glEnd();
  }
  glDisable(GL_TEXTURE_2D);
  glEndList();
}

void objGround(void) {
  glCallList(GLN);
}
