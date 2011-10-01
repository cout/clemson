#include <GL/gl.h>
#include <GL/glu.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "selection.h"
#include "gl.h"
#include "../math/matrix.h"
#include "../widgets/window.h"
#include "../menus/objectmenu.h"

#define BUFSIZE 512

int selection_mode = FALSE;

/* From the red book */
void pickobject(int x, int y) {
	GLuint selectbuf[BUFSIZE];
	GLint viewport[4];
	Matrix m;
	int j, hits;

	if(!num_objects) return;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(BUFSIZE, selectbuf);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glGetFloatv(GL_PROJECTION_MATRIX, m);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix(x, (viewport[3]-y), 5.0, 5.0, viewport);
	glMultMatrixf(m);

	do_transformations();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for(j = 0; j < num_objects; j++) {
		glLoadName(j);
		draw_object(&aw_object[j]);
	}
	glFlush();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	hits = glRenderMode(GL_RENDER);
	/*g_print("Hits: %d\n", hits);*/
	if(hits) curr_obj = selectbuf[(hits-1)*4+3];
	update_color();
	upobj(curr_obj);
}

void on_select_object_activate (GtkMenuItem *menuitem, gpointer user_data) {
	set_cursor(GDK_HAND2);
	selection_mode = TRUE;
}
