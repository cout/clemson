#include <gtk/gtk.h>

gint on_right_vbox_remove(GtkContainer *c, GtkWidget *w, gpointer data) {
	gtk_container_check_resize(c);
	return TRUE;
}

