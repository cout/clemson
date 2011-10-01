#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#include <assert.h>
#include <stdlib.h>
#include "pcx.h"
#include "md2.h"
#include "tankdef.h"

/* The model we are going to load */
#define FILENAME	"tank.def"

/* Useful defines */
#define ZOOM		((unsigned int)1)
#define ROTATE		((unsigned int)2)

#define ZOOM_INC	0.25
#define ROTATE_INC	0.25

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

/* Menu options */
enum menu_options {
	MENU_TEXTURE,
	MENU_LIGHTING,
	MENU_WIREFRAME,
	MENU_LINEAR,
	MENU_AXES,
	MENU_WEAPON,
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
	"* Toggle linear filtering",
	"* Toggle axes",
	"* Toggle weapon display"
};

int texture=TRUE, lighting=TRUE, axes=TRUE, weapon=TRUE,
    wireframe=FALSE, filtering=TRUE;

/* Global variables */
int			glut_window, action;
float			eye[3] = {0.0, 0.0, 100.0};
float			view[3] = {0.0, 0.0, 0.0};

float			theta = 0.0, theta0 = 0.0;
float			phi = 0.0, phi0 = 0.0;
float			z = 100.0, z0 = 100.0;

md2_model_t		model;
md2_model_t		weapon_model;
tank_t			tank = {"", "", "", "", {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
int			mousex, mousey, x0, y0;

/* Animation globals */
int			currentFrame = 0;
int			frameDirection = 0;

void set_window_size(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)width / (float)height, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -eye[2]);
	glRotatef(phi, 1.0, 0.0, 0.0);
	glRotatef(theta, 0.0, 1.0, 0.0);

	glPushMatrix();
	
		/* Draw coordinate axes */
		glColor3f(1.0, 1.0, 1.0);
		glDisable(GL_LIGHTING);
		if(axes) {
			glBegin(GL_LINES);
				glVertex3f(-100.0, 0.0, 0.0);
				glVertex3f(100.0, 0.0, 0.0);
				glVertex3f(0.0, -100.0, 0.0);
				glVertex3f(0.0, 100.0, 0.0);
				glVertex3f(0.0, 0.0, -100.0);
				glVertex3f(0.0, 0.0, 100.0);
			glEnd();
		}
		if(lighting) glEnable(GL_LIGHTING);

		/* Draw the model */
		glRotatef(90.0, -1.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 0.0, -1.0);

		currentFrame %= model.numframes;
		glCallList(model.framestart + currentFrame);

		if(weapon) glCallList(weapon_model.framestart);
	glPopMatrix();

	/* And swap buffers to display the image */
	glutSwapBuffers();
}

void idle(void) {
	if(action & ZOOM) {
		z = z0 + ZOOM_INC * (mousey - y0);
		eye[2] = z;
		theta = theta0 + ROTATE_INC * (mousex - x0);
	} else if(action & ROTATE) {
		theta = theta0 + ROTATE_INC * (mousex - x0);
		phi = phi0 + ROTATE_INC * (mousey - y0);
	}
	glutPostRedisplay();
}

void mouse_button(int button, int state, int x, int y) {
	if(state==GLUT_DOWN) {
		x0 = x; y0 = y; theta0 = theta; phi0 = phi; z0 = z;
		glutIdleFunc(idle);
		switch(button) {
		case GLUT_LEFT_BUTTON: action |= ZOOM; break;
		case GLUT_MIDDLE_BUTTON: action |= ROTATE; break;
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
		if(texture) glEnable(GL_TEXTURE_2D);
		else glDisable(GL_TEXTURE_2D);
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
		glutChangeToMenuEntry(3, &menu_choices[2][wireframe?2:0],
			MENU_WIREFRAME);
		if(wireframe) {
			glPolygonMode(GL_FRONT, GL_LINE);
			glDisable(GL_CULL_FACE);
		}
		else {
			glPolygonMode(GL_FRONT, GL_FILL);
			glEnable(GL_CULL_FACE);
		}
		glutPostRedisplay();
		break;
	case MENU_LINEAR:
		filtering = !filtering;
		glutChangeToMenuEntry(4, &menu_choices[3][filtering?0:2],
			MENU_LINEAR);
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
		glutPostRedisplay();
		break;
	case MENU_AXES:
		axes = !axes;
		glutChangeToMenuEntry(5, &menu_choices[4][axes?0:2], MENU_AXES);
		glutPostRedisplay();
		break;
	case MENU_WEAPON:
		weapon = !weapon;
		glutChangeToMenuEntry(6, &menu_choices[5][weapon?0:2],
			MENU_WEAPON);
		glutPostRedisplay();
		break;
	case MENU_ANIM_FORWARD: frameDirection = 1; break;
	case MENU_ANIM_REVERSE: frameDirection = -1; break;
	case MENU_ANIM_PLUS: currentFrame++; glutPostRedisplay(); break;
	case MENU_ANIM_MINUS: currentFrame--; glutPostRedisplay(); break;
	case MENU_ANIM_REWIND: currentFrame = 0;
	case MENU_ANIM_STOP: frameDirection = 0; glutPostRedisplay(); break;
	case MENU_EXIT: exit(0); break;
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
	case 'w': menu_func(MENU_WEAPON); break;
	case 'r': menu_func(MENU_WIREFRAME); break;
	case 'f': menu_func(MENU_LINEAR); break;
	case 'a': menu_func(MENU_ANIM_FORWARD); break;
	case 'b': menu_func(MENU_ANIM_REVERSE); break;
	case 's': menu_func(MENU_ANIM_STOP); break;
	case '<':
	case ',': menu_func(MENU_ANIM_MINUS); break;
	case '>':
	case '.': menu_func(MENU_ANIM_PLUS); break;
	case 'e': menu_func(MENU_ANIM_REWIND); break;
	case '+':
		z += ZOOM_INC;
		eye[2] = z;
		glutPostRedisplay();
		break;
	case '-':
		z -= ZOOM_INC;
		eye[2] = z;
		glutPostRedisplay();
		break;
	case '8':
		phi += ROTATE_INC;
		glutPostRedisplay();
		break;
	case '2':
		phi -= ROTATE_INC;
		glutPostRedisplay();
		break;
	case '4':
		theta -= ROTATE_INC;
		glutPostRedisplay();
		break;
	case '6':
		theta += ROTATE_INC;
		glutPostRedisplay();
		break;
	case 'q':
		exit(0);
		break;
	}
}

void create_menus() {
	int menu, anim, j;

	anim = glutCreateMenu(anim_menu);
	glutAddMenuEntry("Play forward (a)", MENU_ANIM_FORWARD);
	glutAddMenuEntry("Play reverse (b)", MENU_ANIM_REVERSE);
	glutAddMenuEntry("Advance frame (>)", MENU_ANIM_PLUS);
	glutAddMenuEntry("Backstep frame (<)", MENU_ANIM_MINUS);
	glutAddMenuEntry("Re(w)ind", MENU_ANIM_REWIND);
	glutAddMenuEntry("(S)top", MENU_ANIM_STOP);

	menu = glutCreateMenu(menu_func);
	for(j = 0; j < sizeof(menu_choices)/sizeof(*menu_choices); j++)
		glutAddMenuEntry(menu_choices[j], MENU_TEXTURE + j);
	glutAddSubMenu("Animate...", anim);
	glutAddMenuEntry("Quit", MENU_EXIT);
	
	glutSetMenu(menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(void) {
	int md2_retval;

	GLfloat light0_diffuse[] = {2.0, 2.0, 2.0, 1.0};
	GLfloat light0_specular[] = {2.0, 2.0, 2.0, 1.0};
	GLfloat light0_position[] = {0.0, 0.0, 50.0, 0.0};
	GLfloat light0_ambient[] = {0.0, 0.0, 0.0, 1.0};
	
	glEnable(GL_DEPTH_TEST); /* draw things in front only */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	/* glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE); */

	/* Only draw one side of the polygons */
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	/* Create a light source */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	/* Enable lighting, and enable the light source */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

	/* Parse the md2 file and put the frames into a display list */
	read_tankdef(FILENAME, &tank);
	md2_retval = parse_md2(tank.model, tank.skin, &model);
	assert(md2_retval != MD2ERR_OPEN);
	assert(md2_retval != MD2ERR_VERSION);
	assert(md2_retval != MD2ERR_MAGIC);
	assert(md2_retval != MD2ERR_RANGE);
	assert(md2_retval != MD2ERR_VERTEX);
	assert(md2_retval != MD2ERR_NORMALS);
	assert(md2_retval != MD2ERR_DISPLAYLIST);
	assert(md2_retval == MD2_OK);
	if(*tank.weapon != 0) md2_retval = parse_md2(tank.weapon, "", &weapon_model);
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0,0);

	glutInitWindowSize(512, 512);
	glut_window = glutCreateWindow("MD2 Test");

	glutSetWindow(glut_window);
	set_window_size(512, 512);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyfunc);
	glutMouseFunc(mouse_button);
	glutPassiveMotionFunc(passive_mouse_motion);
	glutMotionFunc(passive_mouse_motion);
	glutReshapeFunc(set_window_size);
	glutTimerFunc(100, animate, 1);

	init();
	create_menus();
	glutMainLoop();

	return 0;
}
