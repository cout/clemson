#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "ground.h"

void init_ground_list(void) {
  double elevation[GW][GH];
  double color[GW][GH][3];
  int i, j, newi;
  int dbg=0;

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
  
  assert(!glIsList(GLN));
  glNewList(GLN, GL_COMPILE);
  glDisable(GL_LIGHTING);
  /*  glBegin(GL_TRIANGLE_STRIP);*GL_TRIANGLE_FAN*/


  /* draw top left corner 
  glColor3f(color[0][0][0], color[0][0][1], color[0][0][2]);
  glVertex3f((GLfloat) 0, (GLfloat)0, (GLfloat)elevation[i][j]);
  */
  for (i=0; i<GW-1; i++) {
    glBegin(GL_TRIANGLE_STRIP);
    for (j=0; j<GH; j++) {
      glColor3f(color[i][j][0], color[i][j][1], color[i][j][2]);
      glVertex3f((GLfloat) i, (GLfloat)j, (GLfloat)elevation[i][j]);
      newi = i+1;
      glColor3f(color[newi][j][0], color[newi][j][1], color[newi][j][2]);
      glVertex3f((GLfloat) newi, (GLfloat)j, (GLfloat)elevation[newi][j]);
    }
    glEnd();
  }
/*   fprintf(stderr, "Here %d", dbg++); */
  glEnable(GL_LIGHTING);
  /*  glEnd();*/
/*   fprintf(stderr, "Here %d", dbg++); */
  glEndList();
/*   fprintf(stderr, "Here %d", dbg++); */
}

void draw_ground(void) {
  glCallList(GLN);
}
 
 
   
