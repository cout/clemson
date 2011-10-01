#include <stdlib.h>
#include <gtk/gtk.h>
#include "../gl/aw.h"
#include "../gl/gl.h"
#include "../gl/normals.h"
#include "../widgets/status.h"
#include "../widgets/window.h"
#include "../gtkmisc.h"

static void load_file(GtkWidget *w, GtkFileSelection *selector) {
	gchar *filename = gtk_file_selection_get_filename(selector);
	gtk_widget_hide(GTK_WIDGET(selector));
	if(num_objects < MAX_OBJECTS) {
		set_cursor(GDK_WATCH);
		status_push("Loading...");
		init_aw(&aw_object[num_objects]);
		g_assert(read_aw(filename, &aw_object[num_objects]) == AW_OK);
		calculate_normals(&aw_object[num_objects]);
		aw_object[num_objects].object_changed = TRUE;
		status_pop();
		set_cursor(GDK_ARROW);
		num_objects++;
	} else {
		quick_message("Too many objects.");
	}
	gl_redraw_all();
}

void on_new1_activate(GtkWidget *w, gpointer data) {
	system("viewer&");
}

void on_load1_activate(GtkWidget *w, gpointer data) {
	create_file_selection(load_file, /*"*.obj"*/"");
}

void on_save1_activate(GtkWidget *w, gpointer data) {
	quick_message("Save function not yet available.");
}

void on_clear1_activate(GtkWidget *w, gpointer data) {
	int j;
	for(j = 0; j < num_objects; j++) {
		free_object(&aw_object[j]);
	}
	num_objects = 0;
	gl_redraw_all();
}

void on_quit1_activate(GtkWidget *w, gpointer data) {
	gtk_main_quit();
}

