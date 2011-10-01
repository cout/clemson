#include <ctype.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "spin.h"
#include "fsim.h"
#include "reshape.h"
#include "freeze.h"
#include "input.h"
#include "time.h"

#define SQR(x) ((x)*(x))

#define ROLL_INCREMENT 0.10
#define ZOOM_IN_INCREMENT 1.0
#define ZOOM_OUT_INCREMENT 1.0

#define ELEV_RATE 0.045
#define AZI_RATE 0.045
#define ROLL_RATE 0.045

#define ZOOM_IN  ((unsigned int)1)
#define ZOOM_OUT ((unsigned int)2)
#define ROLL     ((unsigned int)4)
#define SPIN     ((unsigned int)8)

static float increment_speed = 1.0;
static int lastx = 0, mousex;
static int lasty = 0, mousey;
static Point eye, view, up;
static Real azimuth, elevation, roll;
static int tracking = 0;
static unsigned int action;
extern disp_win, ae_win;
int fsim_mode=0;

long oldtime;

static void alternate_spinning(void);
static void recalc_view_from_AER(void);
static void recalc_AE(int win_x, int win_y);
static void roll_clockwise(void);
static void zoom(void);

double getRoll(void) {
  /* returns the roll in degrees */
  return (roll/M_PI*180.0);
}

double getElevation(void) {
  /* returns the elevation in degrees */
  return (elevation/M_PI*180.0);
}

double getAzimuth(void) {
  /* returns the azimuth in degrees */
  return (azimuth/M_PI*180.0);
}

void change_look_given_AE_in_pixels(int x, int y) {
  recalc_AE(x, y);
  recalc_view_from_AER();
}

static void recalc_AE(int win_x, int win_y) {
  /* from window coords */
  azimuth = M_PI * (-1.0 + 2.0*(Real)win_x/(Real)ae_window_width());
  elevation = (M_PI_2)*(1.0 - 2.0*(Real)win_y/(Real)ae_window_height());
}

static void recalc_view_from_AER(void) {
  Real sinA, cosA, sinR, cosR, sinE, cosE;
  Point view_dir;
  
  /*  recalc_AE();*/
  sinA = sin(azimuth); cosA = cos(azimuth);
  sinE = sin(elevation); cosE = cos(elevation);
  sinR = sin(roll); cosR = cos(roll);

  view_dir.x = -sinA*cosE;
  view_dir.y = sinE;
  view_dir.z = cosA*cosE;

  view.x = view_dir.x + eye.x;
  view.y = view_dir.y + eye.y;
  view.z = view_dir.z + eye.z;

  up.x = sinR*cosA + sinA*sinE*cosR;
  up.y = cosE*cosR;
  up.z = sinR*sinA - cosA*sinE*cosR;

/*glPushMatrix();
  gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);*/
}  

void look_at_it(void) {
  /*  fprintf(stderr, "up is %f %f %f; view is %f %f %f\n", up.x, up.y, up.z, view.x, view.y, view.z);*/

  gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
}

void init_look(void) {
  /* call to set the initial view */
  eye.x = 0.0;
  eye.y = 0.0;
  eye.z = -25.0;
  view.x = view.y = view.z = 0.0;
  up.x = 0.0; up.y = 1.0; up.z = 0.0;
  roll = 0.0;
  azimuth = 0.0;
  elevation = 0.0;
  recalc_view_from_AER();
  action = SPIN;
}

static void alternate_spinning(void) {
  if (action == 0)
    glutIdleFunc(idle);
  if (action & SPIN)
    action &= ~SPIN;
  else
    action |= SPIN;
  if (action == 0)
    glutIdleFunc(NULL);
}

void idle(void) {
  long newtime = get_time();
  long dt = newtime - oldtime;
  oldtime = newtime;

  if(fsim_mode) {
	elevation += ELEV_RATE*dt*trunc(5*(mousey - disp_window_height()));
	azimuth += AZI_RATE*dt*trunc(5*(mousex - disp_window_width()));
	if(action & ROLL)
		roll += ROLL_RATE*dt*trunc(5*(mousex - disp_window_width()));
  } else {
	  if (action & ROLL)
	    roll_clockwise();
	  if (action & ZOOM_IN)
	    zoom();
	  else if (action & ZOOM_OUT)
	    zoom();
  }
  if (action & SPIN)
    do_spin();
  glutPostRedisplay();
}

static void roll_clockwise(void) {
  roll += ROLL_INCREMENT * increment_speed;
  if (roll > 2.0*M_PI)
    roll = roll - 2.0*M_PI;
  recalc_view_from_AER();
}

static void zoom(void) {
  Point view_dir;
  double norm;

  view_dir.x = view.x - eye.x;
  view_dir.y = view.y - eye.y;
  view_dir.z = view.z - eye.z;
  norm = sqrt(SQR(view_dir.x) + SQR(view_dir.y) + SQR(view_dir.z));
  view_dir.x /= norm;
  view_dir.y /= norm;
  view_dir.z /= norm;

  if (action & ZOOM_IN) {
    eye.x += ZOOM_IN_INCREMENT * increment_speed * view_dir.x;
    eye.y += ZOOM_IN_INCREMENT * increment_speed * view_dir.y;
    eye.z += ZOOM_IN_INCREMENT * increment_speed * view_dir.z;
  } else {
    assert(action & ZOOM_OUT);
    eye.x -= ZOOM_OUT_INCREMENT * increment_speed * view_dir.x;
    eye.y -= ZOOM_OUT_INCREMENT * increment_speed * view_dir.y;
    eye.z -= ZOOM_OUT_INCREMENT * increment_speed * view_dir.z;
  }
  view.x = eye.x + view_dir.x;
  view.y = eye.y + view_dir.y;
  view.z = eye.z + view_dir.z;
  glutPostRedisplay();
}
  
void mouse_button(int button, int state, int x, int y) {
  glutSetWindow(disp_win);
  if (state == GLUT_UP) {
    if (button==GLUT_LEFT_BUTTON) {
      action &= ~ZOOM_IN;
    } else if (button==GLUT_RIGHT_BUTTON) {
      action &= ~ZOOM_OUT;
    } else {
      assert(button==GLUT_MIDDLE_BUTTON);
      action &= ~ROLL;
    }
    if (action == 0)
      glutIdleFunc(NULL);
  } else {
    if (action == 0)
      glutIdleFunc(idle);
    assert(state == GLUT_DOWN);
    lastx=x;
    lasty=y;
    if (button==GLUT_LEFT_BUTTON) {
      action |= ZOOM_IN;
    } else if (button==GLUT_RIGHT_BUTTON) {
      action |= ZOOM_OUT;
    } else {
      assert(button==GLUT_MIDDLE_BUTTON);
      action |= ROLL;
    }
  }
}

void ok_idle_function(void (*func)(void)) {
  /* This function is called when it's OK to start the idle function again. */
  /* Generally, that's after an expose event. */
  if (action != 0) {
    glutIdleFunc(idle);
  }
}

void keyfunc(unsigned char key, int x, int y) {
  key = toupper(key);
  switch (key) {
  case '+':
    increment_speed *= 1.5;
    break;
  case '-':
    increment_speed *= 2.0/3.0;
    break;
  case 'Q': 
    exit(0);
    break;
  case 'S':
    alternate_spinning();
    break;
  case 'F':
    /* 
       Freeze the azimuth and elevation until the next time the 
       f key is pressed.
    */
    alternate_freezing();
    /* if we just unfroze, go ahead and recalc the view so the user
       doesn't have to move the mouse to get a redraw from the current
       mouse position */
    if (!frozenp()) {
      ae_passive_mouse_motion(x, y);
      glutSetWindow(ae_win);
      glutPostRedisplay();
      glutSetWindow(disp_win);
    }
    break;
  case 'R':
    /*reset view*/
    init_look();
    increment_speed = 1.0;
  default:
    break;
  }
  glutSetWindow(disp_win);
}

/* Track the current mouse position.
 * (for use with fsim_mode = 1)
 */
void track_motion(int x, int y) {
	mousex = x;
	mousey = y;
}
