#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gtkgl/gtkglarea.h>
#include <math.h>

#include <math/matrix.h>
#include <math/_math.h>
#include <gl/gl.h>
#include <options.h>
#include <misc/_time.h>
#include <widgets/buttonctrl.h>
#include <widgets/scale.h>
#include <widgets/status.h>
#include <widgets/window.h>

#include "trackball/trackball.h"
#include "selection.h"
#include "gtkmisc.h"

#define TRANS_INC        0.025
#define ROTATE_INC       0.0025

/* The memory allocated here is never freed */
static GtkWidget* glarea[MAX_GL_AREAS];

Object aw_object[MAX_OBJECTS];
int num_objects = 0;
int curr_obj = -1;
static int last_selected = -1;

static int mouse_down = 0;
static int mouse_xdown, mouse_ydown;
static guint32 last_motion_time = 0;
static guint32 button_down_time = 0;
static gint32 button_event_time = 0;

Matrix modelview;
Matrix projection;

static Vector trans = {0.0, 0.0, -10.0};
static Vector transold;
static Vector view = {0.0, 0.0, -1.0};
static float azimuth = M_PI, elevation = 0.0, roll = 0.0;
static float azimuthold, elevationold, rollold;
static Vector up = {0.0, 1.0, 0.0};

GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light0_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light0_position[] = {0.0, 0.0, 1000.0, 0.0};
GLfloat light0_ambient[] = {0.0, 0.0, 0.0, 1.0};

float q[4] = {0.0, 0.0, 0.0, 1.0};

GtkWidget* create_glarea (gchar *widget_name, gchar *string1, gchar *string2,
                          gint int1, gint int2) {
        int attrlist[] = {
                GDK_GL_RGBA,
                GDK_GL_DOUBLEBUFFER,
		GDK_GL_DEPTH_SIZE, 1,
                GDK_GL_NONE
        };

        /* Create new OpenGL widget. */
        glarea[int1] = GTK_WIDGET(gtk_gl_area_new(attrlist));
        g_assert(glarea[int1] != NULL);

        return glarea[int1];
}

void gl_init() {
	if(gtk_gl_area_make_current(GTK_GL_AREA(glarea[1]))) {
		gl_reshape(glarea[1]->allocation.width,
			glarea[1]->allocation.height);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
	}
	if(gtk_gl_area_make_current(GTK_GL_AREA(glarea[2]))) {
		gl_reshape(glarea[2]->allocation.width,
			glarea[2]->allocation.height);
	}

	/* Initialize the particle system before anything bad happens */
	init_particle_system(&ps);
}

void gl_shutdown() {
	gtk_object_destroy(GTK_OBJECT(glarea[1]));
	gtk_object_destroy(GTK_OBJECT(glarea[2]));
}
	
gint on_glarea1_realize(GtkWidget *w) {
        return TRUE;
}

void draw_object(Object *o) {
	int f, i, v, lighting;
	Vector point, norm;
	Vector view = {0.0, 0.0, 1.0};
	float m[4][4];

	glPushMatrix();

	if(o->invert_normals) {
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
	} else {
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
	}

	/* Place the object */
	glTranslatef(o->center[0], o->center[1], o->center[2]);
	glTranslatef(o->trans[0], o->trans[1], o->trans[2]);
	build_rotmatrix(m, o->q);
	glMultMatrixf(&m[0][0]);
	glTranslatef(-o->center[0], -o->center[1], -o->center[2]);

	/*
	g_print("Object center: %f %f %f\n", o->center[0], o->center[1],
		o->center[2]);
	*/

	/* If the object has not changed, then we can simply use the
	 * display list, otherwise we must recreate it.
	 */
	if(curr_obj >= 0 && o == &aw_object[curr_obj])
		if(last_selected != curr_obj) o->object_changed = TRUE;
	if(last_selected >= 0 && o == &aw_object[last_selected])
		if(last_selected != curr_obj) o->object_changed = TRUE;
	if(!o->object_changed && !hidden_surface_removal) {
		glCallList(o->displaylist);
		glPopMatrix();
		return;
	}

	if(o->displaylist != 0) glDeleteLists(o->displaylist, 1);
	o->displaylist = glGenLists(1);
	glNewList(o->displaylist, GL_COMPILE_AND_EXECUTE);

	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);

	/* g_print("Modelview:"
		"\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		modelview[0], modelview[1], modelview[2], modelview[3],
		modelview[4], modelview[5], modelview[6], modelview[7],
		modelview[8], modelview[9], modelview[10], modelview[11],
		modelview[12], modelview[13], modelview[14], modelview[15]);
	*/

	vm_mul(view, modelview);
	vm_mul(view, projection);
	v_norm(view);
	/* g_print("View vector: %.2f %.2f %.2f\n", view[0], view[1], view[2]);
	 */

	/*
	g_print("Faces: %d\n", o->faces.count);
	g_print("Vertices: %d\n", o->verts.count);
	*/

	/* Draw the object */
	if(o->texnum) {
		float tex_s[] = {0.5, 0.0, 0.5};
		float tex_t[] = {0.0, 0.5, 0.0};
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, o->texnum);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGenfv(GL_S, GL_EYE_PLANE, tex_s);
		glTexGenfv(GL_T, GL_EYE_PLANE, tex_t);
	} else {
		glDisable(GL_TEXTURE_2D);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, o->color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, o->specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, o->shininess);
	glColor3fv(o->color);
	if(curr_obj >= 0 && o == &aw_object[curr_obj]) {
		float color[4];
		color[0] = o->color[0]/0.7;
		color[1] = o->color[1]/0.7;
		color[2] = o->color[2]/0.5 + 0.2;
		color[3] = o->color[3];
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glColor3fv(color);
	}
	/*
	g_print("Color: [%f %f %f]\n", o->color[0], o->color[1], o->color[2]);
	*/
	for(f = 0; f < o->faces.count; f++) {
		/* g_print("Face %d: ", f); */

		/* Determine if the face is hidden */
		if(hidden_surface_removal) {
			vv_cpy(view, o->face_centers.data[f]);
			view[3] = 1.0;
			vm_mul(view, modelview);
			/*vm_mul(view, projection);*/
			vv_cpy(point, view);
			v_neg(view);
			v_norm(view);

			vv_cpy(norm, o->face_normals.data[f]);
			if(o->invert_normals) v_neg(norm);
			vv_add(norm, o->face_centers.data[f]);
			norm[3] = 1.0;
			vm_mul(norm, modelview);
			/*vm_mul(norm, projection);*/
			vv_sub(norm, point);

			/*
			g_print("ctr: [%.2f %.2f %.2f]\n",
				point[0], point[1], point[2]);
			g_print("view: [%.2f %.2f %.2f]\t",
				view[0], view[1], view[2]);
			g_print("norm: [%.2f %.2f %.2f]\t",
				norm[0], norm[1], norm[2]);
			g_print("dot: %f\n", vv_dot(view, norm));
			*/

			if(vv_dot(view, norm) <= 0.0) {
				continue;
			}
		}

		/* g_print("Drawing face %d", f); */

		glBegin(GL_POLYGON);
		if(polygon_mode == FLAT) {
			if(o->invert_normals) {
				glNormal3f(-o->face_normals.data[f][0],
				           -o->face_normals.data[f][1],
				           -o->face_normals.data[f][2]);
			} else {
				glNormal3fv(o->face_normals.data[f]);
			}
		}
		for(i = 0; i < o->faces.data[f]->count; i++) {
			v = o->faces.data[f]->data[i];
			g_assert(v != 0);
			/*
			g_print("%d (%.2f %.2f %.2f) ", v,
				o->verts.data[v][0],
				o->verts.data[v][1],
				o->verts.data[v][2]);
			*/
			if(polygon_mode==SMOOTH || polygon_mode==WIREFRAME) {
				if(o->invert_normals) {
					glNormal3f(
					   -o->vertex_normals.data[v][0],
				           -o->vertex_normals.data[v][1],
				           -o->vertex_normals.data[v][2]);
				} else {
					glNormal3fv(o->vertex_normals.data[v]);
				}
			}
			glVertex3fv(o->verts.data[v]);
		}
		glEnd();
		/* g_print("\n"); */
	}
	glDisable(GL_TEXTURE_2D);

	lighting = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	/* Draw the face normals in red */
	if(face_normals_on) {
		glColor3f(1.0, 0.0, 0.0);
		/* g_print("Drawing face normals\n"); */
		glBegin(GL_LINES);

		for(f = 0; f < o->faces.count; f++) {
			vv_cpy(point, o->face_normals.data[f]);
			if(o->invert_normals) v_neg(point);
			vc_mul(point, face_normal_scale);
			vv_add(point, o->face_centers.data[f]);
			
			/* Draw the normal */
			glVertex3fv(point);
			glVertex3fv(o->face_centers.data[f]);
		}
		glEnd();
	}

	/* Draw the vertex normals in blue */
	if(vertex_normals_on) {
		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);

		/* Stuff goes here */
		for(i = 1; i < o->verts.count; i++) {
			vv_cpy(point, o->vertex_normals.data[i]);
			vc_mul(point, vertex_normal_scale);
			vv_add(point, o->verts.data[i]);
			
			if(o->invert_normals) v_neg(point);
			glVertex3fv(point);
			glVertex3fv(o->verts.data[i]);
		}

		glEnd();
	}

	/* Display the currently selected object in green */
/*	glPushMatrix();
	glTranslatef(o->center[0], o->center[1], o->center[2]);
	glScalef(1.01, 1.01, 1.01);
	glTranslatef(-o->center[0], -o->center[1], -o->center[2]);
	glColor3f(0.0, 1.0, 0.0);
	if(curr_obj >= 0 && o == &aw_object[curr_obj]) {
		for(f = 0; f < o->faces.count; f++) {
			glBegin(GL_LINE_LOOP);
			for(i = 0; i < o->faces.data[f]->count; i++) {
				v = o->faces.data[f]->data[i];
				g_assert(v != 0);
				glVertex3fv(o->verts.data[v]);
			}
			glEnd();
		}
	}
	glPopMatrix();
*/
	if(lighting) glEnable(GL_LIGHTING);

	o->object_changed = 0;
	glEndList();

	glPopMatrix();
}

void do_transformations() {
	float m[4][4];

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* g_print("eye: [%.2f %.2f %.2f]\nview: [%.2f %.2f %.2f]\n",
                -trans[0], -trans[1], -trans[2], view[0], view[1], view[2]); */

        gluLookAt(-trans[0], -trans[1], -trans[2],
                view[0], view[1], view[2],
                up[0], up[1], up[2]);
        glScalef(image_scale, image_scale, image_scale);

        build_rotmatrix(m, q);
        glMultMatrixf(&m[0][0]);
}

/* Draw the model */
gint on_glarea1_draw(GtkWidget *w) {
	int j, object_changed = 0;
	int cur;

	cur = get_cursor();
	set_cursor(GDK_ARROW);

	if(!gtk_gl_area_make_current(GTK_GL_AREA(w))) return FALSE;

	/* Display a message if there is something to draw */
	for(j = 0; j < num_objects; j++)
		object_changed += aw_object[j].object_changed;
	if(object_changed || hidden_surface_removal) {
		set_cursor(GDK_WATCH);
		status_push("Drawing...");
	}

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(polygon_mode == WIREFRAME) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if(opengl_hidden_surface_removal) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	do_transformations();

	glDisable(GL_LIGHTING);
	if(axes_on) {
		/* Axes are yellow */
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(-100.0, 0.0, 0.0);
		glVertex3f(100.0, 0.0, 0.0);
		glVertex3f(0.0, -100.0, 0.0);
		glVertex3f(0.0, 100.0, 0.0);
		glVertex3f(0.0, 0.0, -100.0);
		glVertex3f(0.0, 0.0, 100.0);
		glEnd();
	}

	if(lighting_on || polygon_mode != WIREFRAME) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}

	/* g_print("Drawing %d objects", j); */
	for(j = 0; j < num_objects; j++) draw_object(&aw_object[j]);

	/* Draw the particle system.  We do this last in case we want to use
	 * alpha blending.
	 */
	glDisable(GL_LIGHTING);
	for(j = 0; j < ps.num_particles; j++) {
		particle_type *p;
		p = &ps.particles[j];
		if(!p->is_dead) {
			/* g_print("Drawing particle %d ", j); */
			show_particle(p);
		}
	}

	gtk_gl_area_swapbuffers(GTK_GL_AREA(w));

	if(object_changed || hidden_surface_removal)
		status_pop();

	set_cursor((GdkCursorType)cur);

	last_selected = curr_obj;
	return TRUE;
}

/* Draw a set of axes */
gint on_glarea2_draw(GtkWidget *w) {
	float m[4][4];

	if(!gtk_gl_area_make_current(GTK_GL_AREA(w))) return FALSE;

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.2);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, -500.0);
	build_rotmatrix(m, q);
	glMultMatrixf(&m[0][0]);

	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(100.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 100.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 100.0);
	glEnd();

	gtk_gl_area_swapbuffers(GTK_GL_AREA(w));
	return TRUE;
}

void gl_redraw(int n) {
	GdkRectangle rect = { 0, 0, 0, 0};
	rect.width = glarea[n]->allocation.width;
	rect.height = glarea[n]->allocation.height;
	g_assert(n == 1 || n == 2);
	/* gtk_widget_draw(glarea[n], &rect); */
	switch(n) {
		case 1: on_glarea1_draw(glarea[n]); break;
		case 2: on_glarea2_draw(glarea[n]); break;
	}
}

void gl_redraw_all() {
	gl_redraw(1);
	gl_redraw(2);
}

gint on_glarea1_expose(GtkWidget *w, GdkEventExpose *event) {
	on_glarea1_draw(w);
	return TRUE;
}

gint on_glarea2_expose(GtkWidget *w, GdkEventExpose *event) {
	on_glarea2_draw(w);
	return TRUE;
}

void gl_reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (float)width / (float)height, 0.0001, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);
}

gint on_glarea_configure(GtkWidget *w, GdkEventConfigure *event) {
	int width, height;
	if(!gtk_gl_area_make_current(GTK_GL_AREA(w))) return FALSE;

	width = w->allocation.width;
	height = w->allocation.height;

	gl_reshape(width, height);
	gl_redraw_all();

	return TRUE;
}

gint on_glarea1_button_press(GtkWidget *w, GdkEventButton *event,
	gpointer data) {

	/* Check for selection mode */
	if(selection_mode) {
		pickobject(event->x, event->y);
		gl_redraw_all();
		selection_mode = FALSE;
		set_cursor(GDK_ARROW);
		return TRUE;
	}


	/* Save state */
	mouse_down = event->button;
	mouse_xdown = event->x;
	mouse_ydown = event->y;

	/* Don't handle right mouse button */
	if(mouse_down != 1 && mouse_down != 2) return TRUE;

	button_down_time = get_time();
	button_event_time = event->time;
	last_motion_time = 0;

	switch(buttonctrl_type[mouse_down - 1]) {
		case CAMERA_ROTATE:
			set_cursor(GDK_DIAMOND_CROSS);
			elevationold = elevation;
			azimuthold = azimuth;
			break;
		case CAMERA_ZOOM:
			set_cursor(GDK_TREK);
			memcpy(transold, trans, sizeof(trans));
			break;
		case CAMERA_ROLL:
			set_cursor(GDK_BOX_SPIRAL);
			rollold = roll;
			break;
		case OBJECT_ROTATE:
			set_cursor(GDK_DIAMOND_CROSS);
			if(curr_obj < 0) {
				quick_message("Please select an object "
					"with +/-");
				break;
			}
			break;
		case OBJECT_MOVEXY:
		case OBJECT_MOVEXZ:
			set_cursor(GDK_PLUS);
			if(curr_obj < 0) {
				quick_message("Please select an object "
					"with +/-");
				break;
			}
			memcpy(transold, aw_object[curr_obj].trans,
				sizeof(transold));
			break;
		case SCENE_ROTATE:
			set_cursor(GDK_STAR);
			break;
		default:
			g_assert(FALSE);
	}

	return TRUE;
}

gint on_glarea1_button_release(GtkWidget *w, GdkEventButton *event,
	gpointer data) {

	set_cursor(GDK_ARROW);
	mouse_down = 0;
	return TRUE;
}

static void recalc_view_and_up() {
	float sinA, cosA, sinR, cosR, sinE, cosE;
	Vector view_dir;

	sinA = sin(azimuth);	cosA = cos(azimuth);
	sinE = sin(elevation);	cosE = cos(elevation);
	sinR = sin(roll);	cosR = cos(roll);

	view_dir[0] = -sinA*cosE;
	view_dir[1] = sinE;
	view_dir[2] = cosA*cosE;

	view[0] = view_dir[0] - trans[0];
	view[1] = view_dir[1] - trans[1];
	view[2] = view_dir[2] - trans[2];

	/*up[0] = sinR*cosA + sinA*sinE*cosR;
	up[1] = cosE*cosR;
	up[2] = sinR*sinA - cosA*sinE*cosR;*/
}

gint on_glarea1_motion(GtkWidget *w, GdkEventMotion *event, gpointer data) {
	static int in_motion = 0;
	static int last_abort_time = 0;
	static guint32 event_time;
	float x1, y1, x2, y2;
	int width, height;
	int xd, yd;
	float qnew[4];

	/* It seems it's possible to get more than one motion event at a
	 * time.  Since this causes speed problems, we don't handle motion
	 * events if we are already handling one.
	 * Oddly, it also seems that there is a problem with this code;
	 * sometimes the pointer gets locked.  So we store the last time
	 * an event was thrown away; if it was more than 1/2 a second
	 * ago, we handle the event anyway.
	 */
	if(in_motion) {
		if(get_time() - last_abort_time < 500 || last_abort_time == 0) {
			/* g_print("in motion, aborting, datime = %d\n",
				datime); */
			last_abort_time = get_time();
			return TRUE;
		}
		/* This event should have been thrown away, but it wasn't */
		last_abort_time = get_time();
	}
	in_motion = 1;

	/* We only want to handle left and middle buttons */
	if(mouse_down != 1 && mouse_down != 2) {
		in_motion = 0;
		return TRUE;
	}

	/* More speed increase: if the event occurred before the last time we
	 * updated the screen, then it is an old event, and should be
	 * discarded.
	 */
	event_time = event->time - button_event_time;
	/* g_print("event time: %u last motion time: %u\n",
		event_time, last_motion_time); */
	if(event_time < last_motion_time) {
		/* g_print("motion aborted\n");*/
		in_motion = 0;
		return TRUE;
	}

	width = w->allocation.width;
	height = w->allocation.height;
	x1 = 2.0 * ((float)mouse_xdown / (float)width) - 1.0;
	y1 = 2.0 * ((float)mouse_ydown / (float)height) - 1.0;
	x2 = 2.0 * ((float)event->x / (float)width) - 1.0;
	y2 = 2.0 * ((float)event->y / (float)height) - 1.0;
	xd = event->x - mouse_xdown;
	yd = event->y - mouse_ydown;

	switch(buttonctrl_type[mouse_down - 1]) {
		case CAMERA_ROTATE:
			elevation = elevationold + ROTATE_INC*yd;
			azimuth = azimuthold + ROTATE_INC*xd;
			recalc_view_and_up();
			break;
		case CAMERA_ZOOM:
			trans[2] = transold[2] + yd*TRANS_INC;
			recalc_view_and_up();
			break;
		case CAMERA_ROLL:
			roll = rollold + ROTATE_INC*xd;
			recalc_view_and_up();
			break;
		case OBJECT_ROTATE:
			if(curr_obj < 0) break;
			trackball(qnew, x1, y1, x2, y2);
			add_quats(aw_object[curr_obj].q, qnew,
				aw_object[curr_obj].q);
			mouse_xdown = event->x;
			mouse_ydown = event->y;
			break;
		case OBJECT_MOVEXY:
			if(curr_obj < 0) break;
			aw_object[curr_obj].trans[0] =
				transold[0] + xd*TRANS_INC;
			aw_object[curr_obj].trans[1] =
				transold[1] - yd*TRANS_INC;
			break;
		case OBJECT_MOVEXZ:
			if(curr_obj < 0) break;
			aw_object[curr_obj].trans[0] =
				transold[0] + xd*TRANS_INC;
			aw_object[curr_obj].trans[2] =
				transold[2] + yd*TRANS_INC;
			break;
		case SCENE_ROTATE:
			trackball(qnew, x1, y1, x2, y2);
			add_quats(q, qnew, q);
			mouse_xdown = event->x;
			mouse_ydown = event->y;
			break;
		default:
			g_assert(FALSE);
	}

	gl_redraw_all();
	
	last_motion_time = get_time() - button_down_time;
	in_motion = 0;

	return TRUE;
}
