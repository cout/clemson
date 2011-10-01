#include <gtk/gtk.h>
#include "../options.h"
#include "../gl/gl.h"

volatile int axes_on = TRUE;
volatile int wireframe_on = TRUE;
volatile int hidden_surfaces_removal_on = FALSE;
volatile int face_normals_on = FALSE;
volatile int vertex_normals_on = FALSE;
volatile int hidden_surface_removal = FALSE;
volatile int opengl_hidden_surface_removal = FALSE;
volatile int lighting_on = FALSE;
volatile int polygon_mode = WIREFRAME;

void on_wireframe_activate (GtkMenuItem *menuitem, gpointer user_data) {
	int j;
	polygon_mode = WIREFRAME;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_flat_shading_activate (GtkMenuItem *menuitem, gpointer user_data) {
	int j;
	polygon_mode = FLAT;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_smooth_shading_activate (GtkMenuItem *menuitem, gpointer user_data) {
	int j;
	polygon_mode = SMOOTH;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_lighting_activate (GtkMenuItem *menuitem, gpointer user_data) {
	int j;
	lighting_on = !lighting_on;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_axes_activate (GtkMenuItem *menuitem, gpointer user_data) {
	axes_on = !axes_on;
	gl_redraw_all();
}

void on_face_normals_activate (GtkMenuItem *menuitem, gpointer user_data) {
	int j;
	face_normals_on = !face_normals_on;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_vertex_normals_activate (GtkMenuItem *menuitem, gpointer user_data) {
	int j;
	vertex_normals_on = !vertex_normals_on;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_hidden_surface_removal_activate (GtkMenuItem *menuitem, gpointer
	user_data) {
	int j;
	hidden_surface_removal = !hidden_surface_removal;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_opengl_hidden_surface_removal_activate (GtkMenuItem *menuitem, gpointer
	user_data) {
	opengl_hidden_surface_removal = !opengl_hidden_surface_removal;
	gl_redraw_all();
}
