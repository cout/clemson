// Imlib-test
// Paul Brannan <pbranna@clemson.edu
// Please use a tab size of 8 when reading this file
// Please compile this program with a C99 compliant compiler

#include <stdbool.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

// Some useful macros
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef M2A
#define M2A(x,y,w,h) (((x)%(w)) + ((y)%(h))*(w))
#endif

// Global widgets
GtkWidget *loadfile_dlg;
GtkWidget *about_dlg;
GtkWidget *drawingarea;

// Global variables
GdkPixbuf *pixbuf;
bool image_loaded = false;

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------

void init_widgets(GladeXML *xml) {
	loadfile_dlg = glade_xml_get_widget(xml, "loadfile_dlg");
	about_dlg = glade_xml_get_widget(xml, "about_dlg");
	drawingarea = glade_xml_get_widget(xml, "drawingarea");
}

// ------------------------------------------------------------
// Drawing routines
// ------------------------------------------------------------

gboolean on_drawingarea_expose_event(GtkWidget *w, GdkEventExpose *event,
	gpointer data) {

	gdk_window_clear_area(w->window, event->area.x, event->area.y,
		event->area.width, event->area.height);
	gdk_gc_set_clip_rectangle(w->style->fg_gc[w->state], &event->area);

	// Render the pixbuf to our drawing area
	if(image_loaded) {
		gdk_pixbuf_render_to_drawable(pixbuf, w->window,
			w->style->fg_gc[w->state], 0, 0, 0, 0,
			gdk_pixbuf_get_width(pixbuf),
			gdk_pixbuf_get_height(pixbuf),
			GDK_RGB_DITHER_NONE, 0, 0);
	}

	return true;
}

void redraw_pixbuf() {
	GdkRectangle area = {0, 0,
		gdk_pixbuf_get_width(pixbuf),
		gdk_pixbuf_get_height(pixbuf)
	};
	gtk_widget_draw(drawingarea, &area);
}

// ------------------------------------------------------------
// File loading functions
// ------------------------------------------------------------

void on_load_activate(GtkWidget *w, gpointer data) {
	gtk_widget_show(loadfile_dlg);
}

void on_load_ok_clicked(GtkWidget *w, gpointer data) {
	GdkPixbuf *new_pixbuf;
	gchar *filename = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(loadfile_dlg));
	gtk_widget_hide(loadfile_dlg);

	// Tell gdk-pixbuf to load the image
	new_pixbuf = gdk_pixbuf_new_from_file(filename);
	if(!new_pixbuf) g_print("Unable to load image!\n");

	// Add an alpha channel if the image does not already have one
	// We want full opacity (alpha = 1.0 for all pixels)
	gdk_pixbuf_add_alpha(new_pixbuf, 0, 0, 0, 0);

	// If we already have an image loaded, merge the new image witih the
	// current one.
	if(image_loaded) {
		gdk_pixbuf_composite(new_pixbuf, pixbuf, 0, 0,
			gdk_pixbuf_get_width(pixbuf),
			gdk_pixbuf_get_height(pixbuf),
			0.0, 0.0, 1.0, 1.0, GDK_INTERP_BILINEAR,
			127);
	} else {
		pixbuf = new_pixbuf;
	}

	// We now have an image loaded (hopefully)
	image_loaded = true;
	redraw_pixbuf();
}

void on_load_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(loadfile_dlg);
}

// ------------------------------------------------------------
// Image clear functions
// ------------------------------------------------------------

void clear_pixmap() {
	image_loaded = false;
	redraw_pixbuf();
}

void on_clear_activate(GtkWidget *w, gpointer data) {
	clear_pixmap();
}

// ------------------------------------------------------------
// Help functions
// ------------------------------------------------------------
void on_about_activate(GtkMenuItem *w, gpointer data) {
	gtk_widget_show(about_dlg);
}

void on_about_ok_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(about_dlg);
}

// ------------------------------------------------------------
// Exit routines
// ------------------------------------------------------------
gint on_main_window_delete_event(GtkWidget *w, gpointer data) {
	gtk_main_quit();
	return FALSE;
}

gint on_main_window_destroy_event(GtkWidget *w, gpointer data) {
	// glarea widgets don't get destroyed properly, but we don't have
	// any of those.
	gtk_main_quit();
	return FALSE;
}

void on_quit_activate(GtkWidget *w, gpointer data) {
	gtk_main_quit();
}

int main(int argc, char *argv[]) {
	GladeXML *xml;

	gtk_init(&argc, &argv);
	glade_init();
	gdk_rgb_init();

	xml = glade_xml_new("gdkpixbuf-test.glade", NULL);
	if(!xml) {
		g_warning("Error creating the interface");
		exit(1);
	}

	init_widgets(xml);
	glade_xml_signal_autoconnect(xml);
	gtk_main();

	// We will never reach this point
	return 0;
}

