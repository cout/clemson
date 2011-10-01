#include <stdio.h>
#include <gtk/gtk.h>
#include "../gl/gl.h"
#include "status.h"
#include "widgets.h"
#include "../menus/objectmenu.h"

GdkCursor *cursor = NULL;
int cnum = GDK_ARROW;

#define window (eventbox1->window)

gint on_window_delete_event(GtkWidget *w, gpointer user_data) {
	gtk_main_quit();
	return FALSE;
}

gint on_window_destroy(GtkWidget *w, gpointer user_data) {
	/* If we have reached this point and we allow the destroy event
	 * to occur for the glarea widgets, then the X server may crash.
	 * since this is bad, we exit (more or less) peacefully.
	 */
	exit(0);
	return FALSE;
}

#define CHOBJ(n) {if(n>=0) aw_object[n].object_changed = TRUE;}

void upobj(int n) {
	status_pop();
	if(n == -1) {
		status_push("No object selected.");
	} else {
		char s[128];
		sprintf(s, "Object %d selected.", n + 1);
		status_push(s);
	}
}

gint on_window_key_press_event(GtkWidget *w, GdkEventKey *event) {
	switch(event->keyval) {
		case ']':
		case '=':
		case '+':
			CHOBJ(curr_obj);
			curr_obj++;
			if(curr_obj >= num_objects) curr_obj = -1;
			gl_redraw_all();
			CHOBJ(curr_obj);
			upobj(curr_obj);
			update_color();
			return TRUE;
		case '[':
		case '_':
		case '-':
			CHOBJ(curr_obj);
			curr_obj--;
			if(curr_obj < -1) curr_obj = num_objects - 1;
			gl_redraw_all();
			CHOBJ(curr_obj);
			upobj(curr_obj);
			update_color();
			return TRUE;
	}
	return FALSE;
}

void set_cursor(GdkCursorType t) {
	cnum = t;
	if(cursor) gdk_cursor_destroy(cursor);
	cursor = gdk_cursor_new(t);
	gdk_window_set_cursor(window, cursor);
}

int get_cursor() {
	return cnum;
}
