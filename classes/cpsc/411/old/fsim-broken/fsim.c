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

#define INIT_WIN_W 640
#define INIT_WIN_H 480

#define INIT_AE_WIN_W 512
#define INIT_AE_WIN_H 512

/* Note: we need the (void) to avoid a compiler warning */

int disp_win;
static int frozen = 0;

static void aereshape(int w, int h);
static void reshape(int w, int h);
static void vis(int state);


static void reshape(int w, int h) {
  disp_really_reshape(w, h);
}

static void display(void) {
	char s[20];

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

int frozenp(void) {
  return frozen;
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(INIT_WIN_W, INIT_WIN_H);
	disp_win = glutCreateWindow("QuasiPseudoSorta FlightSim");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyfunc);
	glutMouseFunc(mouse_button);
	glutMotionFunc(track_motion);
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

