#include <ctype.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "spin.h"
#include "reshape.h"
#include "freeze.h"
#include "input.h"
#include "fsim.h"
#include "gettime.h"
#include "compass.h"

#define SQR(x) ((x)*(x))

#define ZOOM_IN  ((unsigned int)1)
#define ZOOM_OUT ((unsigned int)2)
#define ROLL     ((unsigned int)4)
#define SPIN     ((unsigned int)8)


static int AEGraph_is_in_disp_win = 1;
static int compass_is_in_disp_win = 1;
static float increment_speed = 1.0;
static int lastx = 0;
static int lasty = 0;
int mousex = 0, mousey = 0;
static Point eye, view, up;
static Real azimuth, elevation, roll;
static int tracking = 0;
int reality_mode = 0;
static unsigned int action;
extern int disp_win, ae_win;
extern int compass_win;
extern int compass_win_exists;

static void recalculate_light1(void);
static void alternate_spinning(void);
static void recalc_view_and_up_from_AER(void);
static void recalc_AE(int win_x, int win_y);
static void roll_clockwise(void);
static void zoom(void);

double eye_x(void) {
  return eye.x;
}

double eye_y(void) {
  return eye.y;
}

double eye_z(void) {
  return eye.z;
}

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
  recalc_view_and_up_from_AER();
}

int AEGraph_in_disp_win(void) {
  return AEGraph_is_in_disp_win;
}

int compass_in_disp_win(void) {
  return compass_is_in_disp_win;
}

static void recalc_AE(int win_x, int win_y) {
  /* from window coords */
  azimuth = M_PI * (-1.0 + 2.0*(Real)win_x/(Real)ae_window_width());
  while (azimuth > M_PI) azimuth -= 2.0*M_PI;
  while (azimuth < -M_PI) azimuth += 2.0*M_PI;
  elevation = (M_PI_2)*(1.0 - 2.0*(Real)win_y/(Real)ae_window_height());
  while (elevation > M_PI_2) elevation -= M_PI;
  while (elevation < -M_PI_2) elevation += M_PI;
}

static void recalc_view_and_up_from_AER(void) {
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

  /*  recalculate_light1();*/
/*glPushMatrix();
  gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);*/
}  

void look_at_it(void) {
  /*  fprintf(stderr, "up is %f %f %f; view is %f %f %f\n", up.x, up.y, up.z, view.x, view.y, view.z);*/

/*   printf("eye is %f %f %f\n", eye.x, eye.y, eye.z); */
/*   printf("eye is %f %f %f\n", eye_x(), eye_y(), eye_z()); */
  gluLookAt(eye.x, eye.y, eye.z, view.x, view.y, view.z, up.x, up.y, up.z);
}

void init_look(void) {
  /* call to set the initial view */
  eye.x = 0.0;
  eye.y = 0.0;
  eye.z = -25.0;
/*   view.x = view.y = view.z = 0.0; */
/*   up.x = 0.0; up.y = 1.0; up.z = 0.0; */
  roll = 0.0;
  azimuth = 0.0;
  elevation = 0.0;
  recalc_view_and_up_from_AER();
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
  if(reality_mode) {
    static long oldtime = 0;
    long newtime = get_time();
    long dt = newtime - oldtime;
    oldtime = newtime;

    elevation += (double)dt*MULTIPLIER*ELEV_RATE*
		(int)((mousey - ae_window_width()/2)/5);
    while(elevation > M_PI_2) {
		elevation -= M_PI;
		azimuth -= 2*M_PI;
	}
    while(elevation < -M_PI_2) {
		elevation += M_PI;
		azimuth += 2*M_PI;
	}

    azimuth += (double)dt*MULTIPLIER*AZI_RATE*
		(int)((mousex - ae_window_width()/2)/5);
    while(azimuth > M_PI) azimuth -= 2.0*M_PI;
    while(azimuth < -M_PI) azimuth += 2.0*M_PI;

    if(action & ROLL) {
      roll += (double)dt*MULTIPLIER*ROLL_RATE*
		(int)((mousex - ae_window_width()/2)/5);
      while (roll > 2.0*M_PI) roll = roll - 4.0*M_PI;
    
		if (compass_win_exists) {
			glutSetWindow(compass_win);
			glutPostRedisplay();
			glutSetWindow(disp_win);
		}
    }

    glutSetWindow(ae_win);
    glutPostRedisplay();
    glutSetWindow(disp_win);
    recalc_view_and_up_from_AER();
    really_look_at_it();
  } else {
    if (action & ROLL) {
      roll_clockwise();
      if (compass_win_exists) {
	glutSetWindow(compass_win);
	glutPostRedisplay();
	glutSetWindow(disp_win);
      }
    }
  }
  if (action & ZOOM_IN)
    zoom();
  else if (action & ZOOM_OUT)
    zoom();
  if (action & SPIN)
    do_spin();
  glutSetWindow(disp_win);
  glutPostRedisplay();
}

static void roll_clockwise(void) {
  roll += ROLL_INCREMENT * increment_speed;
  if (roll > 2.0*M_PI) /* while? */
    roll = roll - 2.0*M_PI;
  recalc_view_and_up_from_AER();
}

void mouse_motion(int x, int y) {
  int dx = x - lastx;
  int dy = y - lasty;
  if (tracking) {
    switch (action) {
      /*    case ROLL:
      glutIdleFunc(roll_clockwise);
      tracking = 0;
      return;*/
      /*      roll += ROLL_INCREMENT;
	      recalc_view_from_AER();*/
      /* placing it here means the mouse must be moved while the middle button is pressed */
/*       break; */
    case ZOOM_IN:
      break;
    case ZOOM_OUT:
      break;
    default:
      assert(0);
    }
    glutSetWindow(disp_win);
    glutPostRedisplay();
  }
}

static void recalculate_light1(void) {
  GLfloat eye_position[4], spot_direction[3];

  eye_position[0] = eye.x;
  eye_position[1] = eye.y;
  eye_position[2] = eye.z;
  eye_position[3] = 1.0; /* not at infinity */
  spot_direction[0] = view.x;
  spot_direction[1] = view.y;
  spot_direction[2] = view.z;
  
/*   printf("direction is %f %f %f\n", spot_direction[0], spot_direction[1],  */
/* 	 spot_direction[2]);  */
/*   glLightfv(GL_LIGHT1, GL_POSITION, eye_position); */
/*   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction); */
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
  /*  recalculate_light1();*/
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
  case 'A':
    AEGraph_is_in_disp_win = !AEGraph_is_in_disp_win;
    glutSetWindow(disp_win);
    glutPostRedisplay();
    break;
  case 'B':
    compass_is_in_disp_win = !compass_is_in_disp_win;
    glutSetWindow(compass_win);
    glutPostRedisplay();
    glutSetWindow(disp_win);
    break;
  case 'C':
    toggle_compass_window(); /* create it if it doesn't exist,
				destroy it if it does exist. */
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
    break;
  case 'G':
    if(reality_mode == 0) {
      reality_mode = 1;
      glutIdleFunc(idle);
    } else {
      reality_mode = 0;
      if(action == 0) glutIdleFunc(NULL);
    }
    break;
  default:
    break;
  }
  glutSetWindow(disp_win);
}
