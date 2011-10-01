#ifdef WIN32
#include <windows.h>
#endif

/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
*/

#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include <math.h>
#include <GL/glut.h>
#include <assert.h>
#include <stdlib.h>
#include "pcx.h"
#include "md2.h"
#include "scene.h"
#include "_string.h"

/* The model we are going to load */
#define FILENAME  "scene.def"

/* Useful defines */
#define ZOOM      ((unsigned int)2)
#define ROTATE    ((unsigned int)1)

#define ZOOM_INC  0.25
#define ROTATE_INC   0.0025

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

#ifndef M_PI
#define M_PI 3.1415926535897931
#endif

enum displays {
	D_LEFT,
	D_RIGHT,
	D_MID
};

/* Menu options */
enum menu_options {
	MENU_TEXTURE,
	MENU_LIGHTING,
	MENU_WIREFRAME,
	MENU_AXES,
	MENU_LINEAR,
	MENU_TERRAIN,
	MENU_CRATERS,
	MENU_STARS,
	MENU_GRIDLINES,
        MENU_LOCK,
        MENU_SMOOTH,
        MENU_SNAPSHOT,
/*        MENU_HELMET,*/
	MENU_ANIMATE,
	MENU_ANIM_FORWARD,
	MENU_ANIM_REVERSE,
	MENU_ANIM_PLUS,
	MENU_ANIM_MINUS,
	MENU_ANIM_REWIND,
	MENU_ANIM_STOP,
	MENU_EXIT
};

static char menu_choices[][30] = {
	"* Toggle texture",
	"* Toggle lighting",
	"* Toggle wireframe",
	"* Toggle axes",
	"* Toggle linear filtering",
	"* Toggle terrain",
	"* Toggle craters",
	"* Toggle stars",
	"* Toggle grid lines",
        "* Toggle room lock",
        "* Toggle smooth shading",
        "* Toggle snapshot"
/*        "* Toggle helmet"*/
};

/* initial menu values */
int texture=TRUE, lighting=TRUE, axes=TRUE,
    wireframe=TRUE, filtering=TRUE, terrain=TRUE, 
    lock=TRUE, smooth=TRUE, craters=TRUE, gridlines=TRUE,
    stars=TRUE, snapshot=TRUE, helmet=TRUE;

/* Global variables */
int			glut_window, left_eye, right_eye, action;
float			eye[3] = {0.0, 0.0, 100.0};
float                   eye0[3] = {0.0, 0.0, 100.0};
float			view[3] = {0.0, 0.0, 0.0};
float			up[3] = {0.0, 1.0, 0.0};
float			negview = -1.0, negview0;
float			negup = 1.0, negup0;

float			rot_matrix[9];
float			position[3];

float			global_translate[3] = {0.0, 0.0, 0.0};


int			mousex, mousey, x0, yo;

/* Animation globals */
int			currentFrame = 0;
int			frameDirection = 0;

void set_window_size(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width / (float)height, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}


#ifdef USE_HELMET
/* inserting helmet code */
/* helmet control template */

void main_helmet (void) {
	int pid;
	int status;

	switch (pid = fork()) {
		case -1:  
			printf("OOPS - no fork!\n");
			exit(1);
		case 0:
			return;
		default:
			/* parent */
			while (getchar() != 'q');
			kill(pid, SIGUSR1);  /* tell kid to stop */
			while (wait(&status) != pid);
			exit(0);
	}
}

int fd;

void cleanup() {
	close(fd);
}

void child(int value) {
	printf("child here, reporting for duty, sir.\n");
	birddata(position, rot_matrix);
	if (draw_icos(position, rot_matrix)){
		glutSetWindow(left_eye);
		glutPostRedisplay();
		glutSetWindow(right_eye);
		glutPostRedisplay();
	}
	printf("child returning.\n");
}


draw_icos(float* viewer, float* look) {
	double eyex, eyey, eyez;
	double dirx, diry, dirz;
	double upx, upy, upz;

	eye0[0] = eye[0];
	eye0[1] = eye[1];
	eye0[2] = eye[2];

	eyex = -viewer[1];
	/* compensate for switch to lower hemisphere */
	eyey = -viewer[2] + 3.0;
	eyez = viewer[0];

	dirx = -look[3] + eyex;
	diry = -look[6] + eyey;
	dirz = -look[0] + eyez;

	upx = look[5];
	upy = look[8];
	upz = -look[2];

printf("new_eye[] = %f, %f, %f\n", eyex, eyey, eyez);
printf("new_view[] = %f, %f, %f\n", dirx, diry, dirz);
printf("new_up[] = %f, %f, %f\n", upx, upy, upz);

	if((fabs(fabs(eye0[0])-fabs(eyex)) > ROTATE_INC) ||
		(fabs(fabs(eye0[1])-fabs(eyey)) > ROTATE_INC) ||
		(fabs(fabs(eye0[2])-fabs(eyez)) > ROTATE_INC) ||
		(fabs(fabs(view[0])-fabs(dirx)) > ROTATE_INC) ||
		(fabs(fabs(view[1])-fabs(diry)) > ROTATE_INC) ||
		(fabs(fabs(view[2])-fabs(dirz)) > ROTATE_INC)) {
		eye[0] = eyex;
		eye[1] = eyey;
		eye[2] = eyez;
		view[0] = dirx;
		view[1] = diry;
		view[2] = dirz;
		up[0] = upx;
		up[1] = upy;
		up[2] = upz;
		
		return(1);
	}

	return(0);
}

birdinit() {
	struct termio com_termio;
	char birdcommand;
	char buffit[3]; 
	char bstatus[2];

	fd = open("/dev/ttyd2", O_RDWR);

	/* set baud rate and such junk */
	com_termio.c_iflag = IXOFF;
	com_termio.c_cflag = CS8 | CLOCAL | CREAD;
	com_termio.c_lflag = 0;
	com_termio.c_cc[VMIN] = 0;
	com_termio.c_cc[VTIME] = 20;
	com_termio.c_ospeed = B38400;
	com_termio.c_ispeed = B38400;
	com_termio.c_line = LDISC0;

	ioctl(fd, TCSETA, &com_termio);
 
	/* hack in orientation to lower hemi */
	buffit[0] = 'P';
	buffit[1] = 0x32;
	/* this is calling for 1 bird */
	buffit[2] = 0x01;
	write(fd, buffit, 3);

	/* hack in multi-bird bus control */
	buffit[0] = 'P';
	buffit[1] = 0x32;
	/* this is calling for 1 bird */
	buffit[2] = 0x01;
	write(fd, buffit, 3);

	/*
	hack in group mode... 
	bird id(1, 2, ... ) now follows data record as 1 byte,
	so there are 25 bytes total to read on each call
	*/
	buffit[0] = 'P';
	buffit[1] = 0x23;
	buffit[2] = 0x01;
	write(fd, buffit, 3);

	/* tell the bird we want position and rotation matrix */
	birdcommand = 'Z';
	write(fd, &birdcommand, 1);

	/* tell the bird to get first set ready */
	birdcommand = 'B';
	write(fd, &birdcommand, 1);
	ioctl(fd, TCFLSH, TIOCFLUSH);
	return(0);
}


/*
scale bird output (1.0/32768.0) for matrix to
yield values in range -1,1; position is in feet,
so if bird max is +/- 3ft, should use 3x, and 
if bird max is +/- 6ft, should use 6x
*/
#define POSITION_SCALE (0.00018310542)
#define MATRIX_SCALE   (0.00003051757)

unsigned char buf[24];

birddata(float* pos, float* mat) {
	unsigned char *cbufptr;
	short *bufptr, leftbyte, rightbyte;
	int i;
	char birdcommand;
	static int howmany = 0;

	cbufptr = &buf[howmany];
	howmany += read(fd, cbufptr, 24 - howmany);
	if (howmany != 24) return 0;

	/* translate strange bird output format to integer data ... */
	cbufptr = &buf[0];
	bufptr = (short *)(cbufptr);

	rightbyte = *cbufptr++;
	leftbyte = *cbufptr++;
	*bufptr++ = ((leftbyte << 9) | (rightbyte & 0x7f) << 2);

	bufptr = (short *)(&buf[0]);
	for (i = 0; i < 3; i++) pos[i] = (float)(*bufptr++) * POSITION_SCALE;
	for (i = 0; i < 9; i++) mat[i] = (float)(*bufptr++) * MATRIX_SCALE;

	/* prime the pump for the next read */
	birdcommand = 'B';
	write(fd, &birdcommand, 1);
	ioctl(fd, TCFLSH, TIOCFLUSH);
	return(0);
}
  
/* end helmet code */
#endif

void display(int d) {
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	if(lock) {
		if(eye[0] < -23.0 - 28.0*tan(M_PI/6)) eye[0] = -23.0 - 28.0*tan(M_PI/6);
		if(eye[0] > 33.0) eye[0] = 33.0;
		if(eye[1] < -28.0 - 26.5*tan(M_PI/6)) eye[1] = -28.0 - 26.5*tan(M_PI/6);
		if(eye[1] > 25.0) eye[1] = 25.0;
		if(eye[2] < -25.0) eye[2] = -25.0;
		if(eye[2] > 51.0 + 65.0*tan(M_PI/6)) eye[2] = 51.0 + 65.0*tan(M_PI/6);
	}

        if(!helmet) {
		for(i=0; i < 3; i++)
			view[i] = (2 * negview * eye[i]);
		up[0] = 0;
		up[1] = negup * 1.0;
		up[2] = 0;
	} 

        if(d==D_LEFT) {
		glTranslatef(-2.5, 0.0, 0.0);
	} else if(d==D_RIGHT) {
		glTranslatef(2.5, 0.0, 0.0);
	}

	gluLookAt(eye[0], eye[1], eye[2], view[0], view[1], view[2],
		up[0], up[1], up[2]);
	glTranslatef(global_translate[0], global_translate[1],
			global_translate[2]);

	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

/*		glRotatef(theta, 0.0, 1.0, 0.0);
		glRotatef(phi, 1.0, 0.0, 0.0);*/

		glColor3f(1.0, 1.0, 1.0);

		if(wireframe) {
			glPolygonMode(GL_FRONT, GL_LINE);
			glDisable(GL_CULL_FACE);
		}
		else {
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_CULL_FACE);
		}

		/* Draw coordinate axes */
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		if(axes) {
                        glPushMatrix();
			glTranslatef(-global_translate[0], 
				-global_translate[1],
				-global_translate[2]);
			glBegin(GL_LINES);
				glVertex3f(-100.0, 0.0, 0.0);
				glVertex3f(100.0, 0.0, 0.0);
				glVertex3f(0.0, -100.0, 0.0);
				glVertex3f(0.0, 100.0, 0.0);
				glVertex3f(0.0, 0.0, -100.0);
				glVertex3f(0.0, 0.0, 100.0);
			glEnd();
                        glPopMatrix();
		}
		if(texture) glEnable(GL_TEXTURE_2D);
		if(lighting) glEnable(GL_LIGHTING);
                if(!smooth) {glDisable(GL_SMOOTH); glShadeModel(GL_FLAT); glEnable(GL_FLAT);}
                 else {glDisable(GL_FLAT); glShadeModel(GL_SMOOTH); glEnable(GL_SMOOTH);}
		if(filtering) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			                GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR);
		} else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			                GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_NEAREST);
		}

		draw_scene();

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		if(terrain) draw_terrain();		/* Draw w/o lighting */
		if(stars) draw_stars();

	glPopMatrix();
	glPopAttrib();

	/* And swap buffers to display the image */
	glutSwapBuffers();

/*	if(helmet) child(1);*/
}

void displayright(void) {
	glutSetWindow(right_eye);
	display(D_RIGHT);
	glutSetWindow(left_eye);
	display(D_LEFT);
}

void displayleft(void) {
	glutSetWindow(left_eye);
	display(D_LEFT);
	glutSetWindow(right_eye);
	display(D_RIGHT);
}

void displaymid(void) {
	display(D_MID);
}

void viewchange(void) {
        float newx = eye0[0], newy = eye0[1], newz = eye0[2];
	float r0, r, theta, rho, phi;

	/* Disallow 0,0,0 */
	if(eye0[0] == 0.0) eye0[0] = 0.01;
	if(eye0[1] == 0.0) eye0[1] = 0.01;
	if(eye0[2] == 0.0) eye0[2] = 0.01;

	r0 = sqrt(eye0[0]*eye0[0] + eye0[2]*eye0[2]);
        theta = atan(eye0[2]/eye0[0]);
        if (eye0[0] < 0) theta += M_PI;
        theta += ROTATE_INC * negup * (mousex - x0);
        newx = r0 * cos(theta);
        newz = r0 * sin(theta);

/*printf("new(x,z) = %f, %f\n", newx, newz); */

	r0 = sqrt(newx*newx + newz*newz);
        rho = sqrt(r0*r0 + eye0[1]*eye0[1]);
        phi = atan(eye0[1]/r0);
        phi += ROTATE_INC * negup * (mousey - yo);
        r = rho * cos(phi);
        newy = rho * sin(phi);
	theta = atan(newz/newx);
        if (newx < 0) theta += M_PI;
        newx = r * cos(theta);
        newz = r * sin(theta);

/*
printf("----------------------\ncontinuing rotation:\n");
printf("new eye...\n");
printf("new(x,y,z) = %f, %f, %f\n", newx, newy, newz);
printf("view[] = %f, %f, %f\n",
		(2 * negview * eye[0]),
		(2 * negview * eye[1]),
		(2 * negview * eye[2]));
*/

        eye[0] = newx;
        eye[1] = newy;
        eye[2] = newz;
		if((eye[0] < 0.0 && eye0[0] > 0.0 ||
		    eye[0] > 0.0 && eye0[0] < 0.0) &&
		   (eye[2] < 0.0 && eye0[2] > 0.0 ||
		    eye[2] > 0.0 && eye0[2] < 0.0)) { 
			negup = -negup0;
			negup0 = negup;
			eye0[0] = eye[0];
			eye0[1] = eye[1];
			eye0[2] = eye[2];	
		}
}

void idle(void) {
	if(action & ZOOM) {
		float normeye[3], mageye;

		mageye = sqrt(eye0[0]*eye0[0] + eye0[1]*eye0[1] +
			eye0[2]*eye0[2]);
		normeye[0] = eye0[0] / mageye;
		normeye[1] = eye0[1] / mageye;
		normeye[2] = eye0[2] / mageye;

		eye[0] = eye0[0] - negview * normeye[0] * ZOOM_INC *
			(mousey - yo);
		eye[1] = eye0[1] - negview * normeye[1] * ZOOM_INC *
			(mousey - yo);
		eye[2] = eye0[2] - negview * normeye[2] * ZOOM_INC *
			(mousey - yo);

/*
printf("----------------\ncontinuing zoom:\n");
printf("eye0[] = %f, %f, %f\n", eye0[0], eye0[1], eye0[2]);
printf("normeye[] = %f, %f, %f\n", normeye[0], normeye[1], normeye[2]);
printf("eye[] = %f, %f, %f\n", eye[0], eye[1], eye[2]);
*/

		if((eye[0] < 0.0 && eye0[0] > 0.0 ||
		    eye[0] > 0.0 && eye0[0] < 0.0) &&
		   (eye[1] < 0.0 && eye0[1] > 0.0 ||
		    eye[1] > 0.0 && eye0[1] < 0.0) &&
		   (eye[2] < 0.0 && eye0[2] > 0.0 ||
		    eye[2] > 0.0 && eye0[2] < 0.0)) {
			negview = -negview0;
			negview0 = negview;
			eye0[0] = eye[0];
			eye0[1] = eye[1];
			eye0[2] = eye[2];	
		}

	}
	if(action & ROTATE) {
		viewchange();
	}
	glutPostRedisplay();
}

/*
void idle(void) {
	if(action & ZOOM) {
		z = z0 + ZOOM_INC * (mousey - yo);
		eye[2] = z;
		theta = theta0 + ROTATE_INC * (mousex - x0);
	}
	if(action & ROTATE) {
		theta = theta0 + ROTATE_INC * (mousex - x0);
		phi = phi0 + ROTATE_INC * (mousey - yo);
	}
	glutPostRedisplay();
}
*/

void mouse_button(int button, int state, int x, int y) {
	if(state==GLUT_DOWN) {
		negview0 = negview;
		negup0 = negup;
		x0 = x; yo = y;
                eye0[0] = eye[0]; eye0[1] = eye[1]; eye0[2] = eye[2];

		/*printf("eye0[] = %f, %f, %f\n", eye0[0], eye0[1], eye0[2]);*/
		glutIdleFunc(idle);

		if(!snapshot){
			switch(button) {
			case GLUT_LEFT_BUTTON:
				action |= ZOOM;
				/*printf("starting zoom...\n");*/
				break;
			case GLUT_MIDDLE_BUTTON:
				action |= ROTATE;
				/*printf("\nstarting rotation...\n");*/
				break; 
			}
		}
	} else if(state==GLUT_UP) {
		action &= ~ZOOM;
		action &= ~ROTATE;
		glutIdleFunc(NULL);
	}
}

void passive_mouse_motion(int x, int y) {
  mousex = x; mousey = y;
}

void animate(int value) {
	if(frameDirection) {
		currentFrame += frameDirection;
		glutPostRedisplay();
	}
}

void menu_func(int choice) {
	switch(choice) {
	case MENU_TEXTURE:
		texture = !texture;
		glutChangeToMenuEntry(1, &menu_choices[0][texture?0:2],
			MENU_TEXTURE);
		glutPostRedisplay();
		break;
	case MENU_LIGHTING:
		lighting = !lighting;
		glutChangeToMenuEntry(2, &menu_choices[1][lighting?0:2],
			MENU_LIGHTING);
		glutPostRedisplay();
		break;
	case MENU_WIREFRAME:
		wireframe = !wireframe;
		glutChangeToMenuEntry(3, &menu_choices[2][wireframe?0:2],
			MENU_WIREFRAME);
		glutPostRedisplay();
		break;
	case MENU_AXES:
		axes = !axes;
		glutChangeToMenuEntry(4, &menu_choices[3][axes?0:2], MENU_AXES);
		glutPostRedisplay();
		break;
	case MENU_LINEAR:
		filtering = !filtering;
		glutChangeToMenuEntry(5, &menu_choices[4][filtering?0:2],
			MENU_LINEAR);
		glutPostRedisplay();
		break;
	case MENU_TERRAIN:
		terrain = !terrain;
		glutChangeToMenuEntry(6, &menu_choices[5][terrain?0:2],
			MENU_TERRAIN);
		glutPostRedisplay();
		break;
	case MENU_CRATERS:
		craters = !craters;
		glutChangeToMenuEntry(7, &menu_choices[6][craters?0:2],
			MENU_CRATERS);
		glutPostRedisplay();
		break;
	case MENU_STARS:
		stars = !stars;
		glutChangeToMenuEntry(8, &menu_choices[7][stars?0:2],
			MENU_STARS);
		glutPostRedisplay();
		break;
	case MENU_GRIDLINES:
		gridlines = !gridlines;
		glutChangeToMenuEntry(9, &menu_choices[8][gridlines?0:2],
			MENU_GRIDLINES);
		glutPostRedisplay();
		break;
        case MENU_LOCK:
                lock = !lock;
		glutChangeToMenuEntry(10, &menu_choices[9][lock?0:2],
			MENU_LOCK);
		glutPostRedisplay();
                break;
        case MENU_SMOOTH:
                smooth = !smooth;
		glutChangeToMenuEntry(11, &menu_choices[10][smooth?0:2],
			MENU_SMOOTH);
		glutPostRedisplay();
                break;
        case MENU_SNAPSHOT:
                snapshot = !snapshot;
		glutChangeToMenuEntry(12, &menu_choices[11][snapshot?0:2],
			MENU_SNAPSHOT);
		glutPostRedisplay();
                break;
	case MENU_EXIT:
		exit(0);
		break;
	}
}

void anim_menu(int choice) {
	menu_func(choice);
}

void keyfunc(unsigned char key, int x, int y) {
	switch(tolower(key)) {
	case 't': menu_func(MENU_TEXTURE); break;
	case 'l': menu_func(MENU_LIGHTING); break;
	case 'x': menu_func(MENU_AXES); break;
	case 'w': menu_func(MENU_WIREFRAME); break;
	case 'f': menu_func(MENU_LINEAR); break;
	case 'e': menu_func(MENU_TERRAIN); break;
	case 'a': menu_func(MENU_ANIM_FORWARD); break;
	case 'b': menu_func(MENU_ANIM_REVERSE); break;
	case 'g': menu_func(MENU_GRIDLINES); break;
	case 'c': menu_func(MENU_CRATERS); break;
	case '\\': menu_func(MENU_LOCK); break;
	case 's': menu_func(MENU_SMOOTH); break;
	case '+':
		eye[2] += ZOOM_INC;
		glutPostRedisplay();
		break;
	case '-':
		eye[2] -= ZOOM_INC;
		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
		break;
        case 'k': menu_func(MENU_LOCK); break;
	}
}

void create_menus() {
	int menu, anim, j;

/*	anim = glutCreateMenu(anim_menu);
	glutAddMenuEntry("Play forward (a)", MENU_ANIM_FORWARD);
	glutAddMenuEntry("Play reverse (b)", MENU_ANIM_REVERSE);
	glutAddMenuEntry("Advance frame (>)", MENU_ANIM_PLUS);
	glutAddMenuEntry("Backstep frame (<)", MENU_ANIM_MINUS);
	glutAddMenuEntry("Re(w)ind", MENU_ANIM_REWIND);
	glutAddMenuEntry("(S)top", MENU_ANIM_STOP);*/

	menu = glutCreateMenu(menu_func);
	for(j = 0; j < sizeof(menu_choices)/sizeof(*menu_choices); j++)
		glutAddMenuEntry(menu_choices[j], MENU_TEXTURE + j);
/*	glutAddSubMenu("Animate...", anim);*/
	glutAddMenuEntry("Quit", MENU_EXIT);
	
	glutSetMenu(menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Turn off some menu items */
	menu_func(MENU_WIREFRAME);
        menu_func(MENU_AXES);
	/* menu_func(MENU_GRIDLINES);*/
	menu_func(MENU_SNAPSHOT);
}

void init(void) {
	int md2_retval;

	glEnable(GL_DEPTH_TEST); /* draw things in front only */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
/*	glEnable(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);*/
	/* glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE); */

	/* Only draw one side of the polygons */
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	/* Create a light source */
	do_lighting();

}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |
		GLUT_STENCIL);
	if((argc > 1) && (strcmpi(argv[1], "--helmet") == 0)) {
		/*left eye*/
		glutInitWindowPosition(0,0);
		glutInitWindowSize(640, 480);
		left_eye = glutCreateWindow("Escher World - Left Eye");

		glutSetWindow(left_eye);
		set_window_size(640, 480);
		glutDisplayFunc(displayleft);
		glutKeyboardFunc(keyfunc);
		glutMouseFunc(mouse_button);
		glutPassiveMotionFunc(passive_mouse_motion);
		glutMotionFunc(passive_mouse_motion);
		glutReshapeFunc(set_window_size);
/*		glutTimerFunc(10,child,1);*/

		/* right eye */
		glutInitWindowPosition(641,0);
		glutInitWindowSize(640, 480);
		right_eye = glutCreateWindow("Escher World - Right Eye");

		glutSetWindow(right_eye);
		set_window_size(640, 480);
		glutDisplayFunc(displayright);
		glutKeyboardFunc(keyfunc);
		glutMouseFunc(mouse_button);
		glutPassiveMotionFunc(passive_mouse_motion);
		glutMotionFunc(passive_mouse_motion);
		glutReshapeFunc(set_window_size);

		glutSetWindow(left_eye);
		create_menus();
		init();
		/* Initialize the scene */
		init_scene(FILENAME);

		glutSetWindow(right_eye);
		create_menus();
		create_menus();
		init();
		/* Initialize the scene */
		init_scene(FILENAME);

#ifdef USE_HELMET
/*		main_helmet();
		signal(SIGUSR1, cleanup);
*/		birdinit();
/*		glutTimerFunc(10,child,1);*/
#endif

	} else {
		glutInitWindowPosition(64,64);

		glutInitWindowSize(768, 832);
		glut_window = glutCreateWindow("Escher World");

		glutSetWindow(glut_window);
		set_window_size(768, 832);
		helmet = !helmet;
		glutDisplayFunc(displaymid);
		glutKeyboardFunc(keyfunc);
		glutMouseFunc(mouse_button);
		glutPassiveMotionFunc(passive_mouse_motion);
		glutMotionFunc(passive_mouse_motion);
		glutReshapeFunc(set_window_size);
		create_menus();
		init();
		/* Initialize the scene */
		init_scene(FILENAME);
	}

	glutMainLoop();

	return 0;
}
