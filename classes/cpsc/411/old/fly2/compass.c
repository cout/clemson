#include <stdlib.h>
#include <assert.h>
#include <GL/glut.h>
#include "compass.h"
#include "display.h"
#include "input.h"
#include "reshape.h"

int compass_win;
int compass_win_exists = 0;
static int w = 256;
static int h = 256;
extern int disp_win, ae_win;

static void destroy_compass_window(void);
static void create_compass_window(void);
static void compass_reshape(int w, int h);
static void compass_display(void);

static void destroy_compass_window(void) {
  glutSetWindow(compass_win);
/*   glutDisplayFunc(NULL); */
/*   glutKeyboardFunc(NULL); */
/*   glutMouseFunc(NULL); */
/*   glutReshapeFunc(NULL); */
  
  glutDestroyWindow(compass_win);
  compass_win_exists = 0;
  glutSetWindow(disp_win);
}

void toggle_compass_window(void) {
  if (compass_win_exists)
    destroy_compass_window();
  else
    create_compass_window();
}

static void compass_reshape(int neww, int newh) {
  assert(glutGetWindow() == compass_win);
  w = neww;
  h = newh;
  glViewport(0, 0, w, h);
  glutPostRedisplay();
  glutSetWindow(disp_win);
}

static void compass_display(void) {
  assert(glutGetWindow() == compass_win);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glScalef(2.0, 2.0, 2.0);
  glTranslatef(-0.5, -0.5, 0.0);
  drawCompass(getRoll());
  glutSwapBuffers();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glutSetWindow(disp_win);
}

static void create_compass_window(void) {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(0,disp_window_height());
  glutInitWindowSize(w, h);
#ifndef ROLL_WIN_IS_NOT_TOPLEVEL
  compass_win = glutCreateWindow("Roll");
#else
  compass_win = glutCreateSubWindow(disp_win, 0, 0, w, h);
#endif
  glutSetWindow(compass_win);
  
  glutDisplayFunc(compass_display);
  glutKeyboardFunc(keyfunc);
  glutMouseFunc(mouse_button);
  glutReshapeFunc(compass_reshape);
/*   glutVisibilityFunc(compass_vis); */
    /* don't do idle things if they aren't seen */
  compass_win_exists = 1;
  glutSetWindow(disp_win);
}


