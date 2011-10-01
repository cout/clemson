#include <gtk/gtk.h>
#include "scale.h"
#include "../gl/gl.h"

float image_scale = 1.0;
float face_normal_scale = 1.0;
float vertex_normal_scale = 1.0;

void on_image_scale_value_changed(GtkWidget *w, GdkEventButton *event) {
	int j;
	image_scale = GTK_RANGE(w)->adjustment->value;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_face_normal_scale_value_changed(GtkWidget *w, GdkEventButton *event) {
	int j;
	face_normal_scale = GTK_RANGE(w)->adjustment->value;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

void on_vertex_normal_scale_value_changed(GtkWidget *w, GdkEventButton *event) {
	int j;
	vertex_normal_scale = GTK_RANGE(w)->adjustment->value;
	for(j = 0; j < num_objects; j++) aw_object[j].object_changed = TRUE;
	gl_redraw_all();
}

