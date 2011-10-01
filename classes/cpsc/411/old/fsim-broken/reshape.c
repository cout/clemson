#include <GL/glut.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <assert.h>
#include "reshape.h"
#include "input.h"

#define FOV_ANGLE 70.0

static int disp_width, disp_height, ae_width, ae_height;
extern int ae_win, disp_win;

void set_initial_size(int dw, int dh, int aew, int aeh) {
  disp_width = dw;
  disp_height = dh;
  ae_width = aew;
  ae_height = aeh;
}

int disp_window_width(void) {
  return disp_width;
}

int disp_window_height(void) {
  return disp_height;
}

int ae_window_width(void) {
  return ae_width;
}
int ae_window_height(void) {
  return ae_height;
}

void disp_really_reshape(int w, int h) {
  /* set new viewing projections */
  assert(glutGetWindow() == disp_win);
  disp_width = w;
  disp_height = h;
  glViewport(0, 0, disp_window_width(), disp_window_height());
  glutPostRedisplay();
}

void really_look_at_it(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(FOV_ANGLE, 
		 (double)disp_window_width()/(double)disp_window_height(),
		 0.1, 1000.0);
  look_at_it();
}

void ae_really_reshape(int w, int h) {
  assert(glutGetWindow() == ae_win);
  ae_width = w;
  ae_height = h;
  glMatrixMode(GL_PROJECTION);
  glViewport(0, 0, w, h);
  glutPostRedisplay();
  glutSetWindow(disp_win);
}
