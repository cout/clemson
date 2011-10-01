#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <assert.h>

#include "scene.h"
#include "gltext.h"
#include "spin.h"
#include "display.h"
#include "input.h"
#include "reshape.h"
#include "freeze.h"
#include "time.h"

#define INIT_WIN_W 640
#define INIT_WIN_H 480

#define INIT_AE_WIN_W 512
#define INIT_AE_WIN_H 512

/* Note: we need the (void) to avoid a compiler warning */

int disp_win, ae_win;
static int frozen = 0;

static void aereshape(int w, int h);
static void reshape(int w, int h);
static void vis(int state);

/* fps stuff */
long oldfpstime = 0;
int update_fps;

static void reshape(int w, int h) {
  disp_really_reshape(w, h);
}

static void display(void) {
	char s[20];

	long newfpstime = get_time();
	long dtfps = newfpstime - oldfpstime;
	static long fps;

	/* Draw the scene */
	scene_redraw();

	glDisable(GL_LIGHTING);

	/* Draw the displays */
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-1.0, -1.0, 0.0);
		glScalef(0.5, 0.5, 1.0);
		drawAEGraph(getAzimuth(), getElevation());
		glTranslatef(1.2, 0.0, 0.0);
		drawCompass(getRoll());
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

	/* Calculate fps */
	if(dtfps > 1000) {
                fps = 1000.0 / (double)dtfps;
		oldfpstime = newfpstime;
        }

	/* Display fps */
		sprintf(s, "%.2f", fps);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.75, 0.9, 0.0);
		glScalef(0.02, 0.02, 0.02);
		draw_text(s);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

	/* Re-enable lighting */
	glEnable(GL_LIGHTING);

	/* Use glFinish() so we don't spin the cube faster than we redraw */
	glFinish();

	glutSwapBuffers();
}

/* use glutUseLayer for compass??? */

static void aedisplay(void) {
  /*
    We cannot put the projection matrix on the stack because that stack
     is only guaranteed to be two deep.  We could check, but the code
     would be more complicated and only a little bit faster since this
     display is very quick anyway.
  */
  assert(glutGetWindow()==ae_win);
  glDisable(GL_LIGHTING);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glTranslatef(-1.0, -1.0, 0.0);
  glScalef(2.0, 2.0, 1.0);
  drawAEGraph(getAzimuth(), getElevation());
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glEnable(GL_LIGHTING);
  glutSwapBuffers();
/*   glutSetWindow(disp_win); */
}

static void aereshape(int w, int h) {
  ae_really_reshape(w, h);
  glViewport(0, 0, w, h);
  glutPostRedisplay();
}

void ae_passive_mouse_motion(int x, int y) {
  if (!frozen) {
    change_look_given_AE_in_pixels(x, y);
    glutPostRedisplay();
    glutSetWindow(disp_win);
    really_look_at_it();
    glutPostRedisplay();
  }
}

int frozenp(void) {
  return frozen;
}

void alternate_freezing(void) {
  glutSetWindow(ae_win);
  if (frozen) {
    /* unfreeze the window */
    glutPassiveMotionFunc(ae_passive_mouse_motion);
  } else {
    glutPassiveMotionFunc(NULL);
  }
  frozen = !frozen;
  glutSetWindow(disp_win);
}

static void vis(int state) {
  /* Don't set an idle function until the display window is visible */
  if (state == GLUT_VISIBLE)
    ok_idle_function(idle);
  else
    glutIdleFunc(NULL);
}

/* Note: if compiling with Win32 be sure to use /subsystem:windows and
 * /entry:mainCRTStartup
 */
int main(int argc, char *argv[]) {
	fsim_mode = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WIN_W,0);
	glutInitWindowSize(INIT_AE_WIN_W, INIT_AE_WIN_H);
	ae_win = glutCreateWindow("Azimuth vs Elevation");
	/* give the callbacks for the azimuth-elevation window */
	glutDisplayFunc(aedisplay);
	glutReshapeFunc(aereshape);
	glutKeyboardFunc(keyfunc);
	glutMouseFunc(mouse_button); /* buttons should work in any window */
	glutPassiveMotionFunc(ae_passive_mouse_motion);

	glutMotionFunc(ae_passive_mouse_motion);
	/* The motion should change azimuth and elevation even if the mouse 
	   button is down.  This way you can zoom and turn.*/
	

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(INIT_WIN_W, INIT_WIN_H);
	disp_win = glutCreateWindow("QuasiPseudoSorta FlightSim");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyfunc);
	glutMouseFunc(mouse_button);
	glutReshapeFunc(reshape);
	glutVisibilityFunc(vis); /* don't do idle things if they aren't seen */

	set_initial_size(INIT_WIN_W, INIT_WIN_H, INIT_AE_WIN_W, INIT_AE_WIN_H);
	init_look();
	spin_init();
	scene_init();
	glutSetWindow(disp_win);
	glutMainLoop();

	return 0; /* we'll never get here, of course */
}

