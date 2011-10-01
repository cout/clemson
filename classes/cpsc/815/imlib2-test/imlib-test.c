// Imlib-test
// Paul Brannan <pbranna@clemson.edu
// Please use a tab size of 8 when reading this file
// Please compile this program with a C99 compliant compiler

#include <stdbool.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk_imlib.h>

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
GtkWidget *pixmap;

// Global variables
GdkImlibImage *image;
bool image_loaded = false;

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------

void init_widgets(GladeXML *xml) {
	loadfile_dlg = glade_xml_get_widget(xml, "loadfile_dlg");
	about_dlg = glade_xml_get_widget(xml, "about_dlg");
	pixmap = glade_xml_get_widget(xml, "pixmap");
}

// ------------------------------------------------------------
// Image manipulation functions
// ------------------------------------------------------------
void merge_images(GdkImlibImage *dest, GdkImlibImage *src) {
	int x, y;
	int w, h;
	float a1, a2;

	w = MIN(dest->rgb_width, src->rgb_width);
	h = MIN(dest->rgb_height, src->rgb_height);
	g_print("Merging images... (w = %d, h = %d)\n", w, h);
	if(dest->alpha_data) g_print("Destination has alpha\n");
	if(src->alpha_data) g_print("Source has alpha\n");

	for(y = 0; y < h; y++) {
		for(x = 0; x < w; x++) {

			// Find the alpha values
			if(dest->alpha_data) a1 = (float)dest->alpha_data[M2A(x,y,
				dest->rgb_width, dest->rgb_height)] / 255.0;
			else a1 = 1.0;
			if(src->alpha_data) a2 = (float)src->alpha_data[M2A(x,y,
				src->rgb_width, src->rgb_height)] / 255.0;
			else a2 = 1.0;

			// Find the new pixel value
			dest->rgb_data[M2A(x,y,dest->rgb_width,
				dest->rgb_height)] = 
				/*(int)(0.5*(float)(
				a1 * (float)dest->rgb_data[M2A(x,y,dest->rgb_width,
					dest->rgb_height)] +
				a2 * (float)src->rgb_data[M2A(x,y,src->rgb_width,
					src->rgb_height)]));*/
				0;
		}
	}

}

// ------------------------------------------------------------
// File loading functions
// ------------------------------------------------------------

void on_load_activate(GtkWidget *w, gpointer data) {
	gtk_widget_show(loadfile_dlg);
}

void on_load_ok_clicked(GtkWidget *w, gpointer data) {
	GdkPixmap *gdk_pixmap;
	GdkImlibImage *im;
	gchar *filename = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(loadfile_dlg));

	// Tell imlib to load the image
	im = gdk_imlib_load_image(filename);
	if(!im) g_print("unable to load image\n");

	// If there is already an image loaded, merge it with the current
	// image
	if(image_loaded)
		merge_images(image, im);
	else
		image = im;

	// Render the image to an internal Imlib pixmap
	gdk_imlib_render(image, image->rgb_width, image->rgb_height);

	// Copy imlib's pixmap to another pixmap, so we can draw it
	// Since this is the only function we every modify the pixmap from
	// (and we notify GTK of that), we can use move_image.  Otherwise,
	// we would have to use copy_image.
	gdk_pixmap = gdk_imlib_move_image(image);
	if(!gdk_pixmap) g_print("unable to copy to pixmap\n");

	// Now give the pixmap back to GTK for viewing
	gtk_widget_set_usize(pixmap, image->rgb_width, image->rgb_height);
	gtk_pixmap_set(GTK_PIXMAP(pixmap), gdk_pixmap, NULL);
	gtk_widget_show(pixmap);
	gtk_widget_hide(loadfile_dlg);

	// We now have an image loaded (hopefully)
	image_loaded = true;
}

void on_load_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(loadfile_dlg);
}

// ------------------------------------------------------------
// Image clear functions
// ------------------------------------------------------------

void clear_pixmap() {
	gtk_widget_hide(pixmap);
	image_loaded = false;
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

	xml = glade_xml_new("imlib-test.glade", NULL);
	if(!xml) {
		g_warning("Error creating the interface");
		exit(1);
	}

	gdk_imlib_init();
	gtk_widget_push_visual(gdk_imlib_get_visual());
	gtk_widget_push_colormap(gdk_imlib_get_colormap());

	init_widgets(xml);
	glade_xml_signal_autoconnect(xml);
	gtk_main();

	// We will never reach this point
	return 0;
}

