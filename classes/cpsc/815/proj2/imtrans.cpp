// Imtrans
// Paul Brannan <pbranna@clemson.edu
// Please use a tab size of 8 when reading this file

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Some useful macros
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef M2A
#define M2A(x,y,w,h,r,d) (((x)%(w))*(d) + ((y)%(h))*(r))
#endif
#ifndef M_PI
#define M_PI 3.1415926535793238462643383
#endif

// Global widgets
GtkWidget *loadfile_dlg;
GtkWidget *savefile_dlg;
GtkWidget *about_dlg;
GtkWidget *drawingarea;

GtkWidget *translate_dlg;
GtkWidget *translate_x;
GtkWidget *translate_y;

GtkWidget *scale_dlg;
GtkWidget *scale_x;
GtkWidget *scale_y;

GtkWidget *rotate_dlg;
GtkWidget *rotate_value;

// Global variables
GdkPixbuf *pixbuf, *orig_pixbuf;
bool image_loaded = false;
gint mouse_down_x, mouse_down_y, mouse_down;
gint mouse_x, mouse_y;
bool bilinear = false;

// This is how we'll keep track of what we are doing
enum {
	IMTRANS_OP_NONE,
	IMTRANS_OP_TRANSLATE,
	IMTRANS_OP_SCALE,
	IMTRANS_OP_ROTATE
};
int imtrans_op = IMTRANS_OP_NONE;

extern "C" {

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------

void init_widgets(GladeXML *xml) {
	loadfile_dlg = glade_xml_get_widget(xml, "loadfile_dlg");
	savefile_dlg = glade_xml_get_widget(xml, "savefile_dlg");
	about_dlg = glade_xml_get_widget(xml, "about_dlg");
	drawingarea = glade_xml_get_widget(xml, "drawingarea");

	translate_dlg = glade_xml_get_widget(xml, "translate_dlg");
	translate_x = glade_xml_get_widget(xml, "translate_x");
	translate_y = glade_xml_get_widget(xml, "translate_y");

	scale_dlg = glade_xml_get_widget(xml, "scale_dlg");
	scale_x = glade_xml_get_widget(xml, "scale_x");
	scale_y = glade_xml_get_widget(xml, "scale_y");

	rotate_dlg = glade_xml_get_widget(xml, "rotate_dlg");
	rotate_value = glade_xml_get_widget(xml, "rotate_value");

	// Glade doesn't support word wrap
	GtkWidget *about_text = glade_xml_get_widget(xml, "about_text");
	gtk_text_set_word_wrap(GTK_TEXT(about_text), true);
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
		int width = MIN(event->area.width,
			gdk_pixbuf_get_width(pixbuf) - event->area.x);
		int height = MIN(event->area.height,
			gdk_pixbuf_get_height(pixbuf) - event->area.y);
		if(width < 0 || height < 0) return true;
		gdk_pixbuf_render_to_drawable(pixbuf, w->window,
			w->style->fg_gc[w->state],
			event->area.x, event->area.y,
			event->area.x, event->area.y,
			width, height,
			GDK_RGB_DITHER_NONE, 0, 0);
	}

	return true;
}

void redraw_pixbuf() {
	GdkRectangle area = {0, 0,
		drawingarea->allocation.width,
		drawingarea->allocation.height
	};
	gtk_widget_draw(drawingarea, &area);
}

// ------------------------------------------------------------
// Miscellaneous pixbuf operations
// ------------------------------------------------------------

gint gdk_pixbuf_get_size(const GdkPixbuf *p) {
	int height = gdk_pixbuf_get_height(p);
	int rowstride = gdk_pixbuf_get_rowstride(p);
	return rowstride*height;
}

void gdk_pixbuf_clear(const GdkPixbuf *p) {
	guchar *pixels = gdk_pixbuf_get_pixels(p);
	memset(pixels, 0, gdk_pixbuf_get_size(p));
}

// ------------------------------------------------------------
// File loading functions
// ------------------------------------------------------------

void on_load_activate(GtkWidget *w, gpointer data) {
	gtk_widget_show(loadfile_dlg);
}

void on_load_ok_clicked(GtkWidget *w, gpointer data) {
	gchar *filename = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(loadfile_dlg));
	gtk_widget_hide(loadfile_dlg);

	if(image_loaded) gdk_pixbuf_unref(pixbuf);
	if(imtrans_op != IMTRANS_OP_NONE) gdk_pixbuf_unref(orig_pixbuf);

	// Tell gdk-pixbuf to load the image
	pixbuf = gdk_pixbuf_new_from_file(filename);
	if(!pixbuf) {
		g_print("Unable to load image!\n");
		image_loaded = false;
	} else {
		// Add an alpha channel if the image does not already have one
		// We want full opacity (alpha = 1.0 for all pixels)
		GdkPixbuf *tmp_pixbuf;
		tmp_pixbuf = gdk_pixbuf_add_alpha(pixbuf, 0, 0, 0, 0);
		gdk_pixbuf_unref(pixbuf);
		pixbuf = tmp_pixbuf;
		if(!gdk_pixbuf_get_has_alpha(pixbuf)) g_print("um... no alpha?\n");

		// We now have an image loaded (hopefully)
		image_loaded = true;
	}

	imtrans_op = IMTRANS_OP_NONE;
	gtk_widget_hide(translate_dlg);
	gtk_widget_hide(rotate_dlg);
	gtk_widget_hide(scale_dlg);

	redraw_pixbuf();
}

void on_load_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(loadfile_dlg);
}

// ------------------------------------------------------------
// File saving functions
// ------------------------------------------------------------

void on_save_activate(GtkWidget *w, gpointer data) {
	if(!image_loaded) return;
	gtk_widget_show(savefile_dlg);
}

void on_save_ok_clicked(GtkWidget *w, gpointer data) {
	gchar *filename = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(savefile_dlg));
	gtk_widget_hide(savefile_dlg);

	FILE *fp;
	if((fp = fopen(filename, "wb")) == NULL) {
		g_print("Error writing file %s\n", filename);
		return;
	}

	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);

	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "255\n");
	g_print("Writing %d bytes\n", width*height*3);

	guchar *src = gdk_pixbuf_get_pixels(pixbuf);
	int srs = gdk_pixbuf_get_rowstride(pixbuf);

	for(int y = 0; y < height; y++) {
	for(int x = 0; x < width; x++) {
	for(int d = 0; d < 3; d++) {
		guchar pixel;
		pixel = src[M2A(x,y,width,height,srs,4)+d];
		fputc(pixel, fp);
	}
	}
	}

	fclose(fp);
}

void on_save_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(savefile_dlg);
}

// ------------------------------------------------------------
// Translation
// ------------------------------------------------------------

void on_translate_activate(GtkMenuItem *w, gpointer data) {
	if(imtrans_op == IMTRANS_OP_TRANSLATE) return;
	if(!image_loaded) return;

	// Make a copy of the pixbuf
	if(imtrans_op != IMTRANS_OP_NONE) gdk_pixbuf_unref(orig_pixbuf);
	orig_pixbuf = gdk_pixbuf_copy(pixbuf);

	// Set the parameters in the dialog
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);
	GTK_RANGE(translate_x)->adjustment->lower = -width;
	GTK_RANGE(translate_x)->adjustment->upper = width;
	GTK_RANGE(translate_y)->adjustment->lower = -height;
	GTK_RANGE(translate_y)->adjustment->upper = height;

	// Display the dialog
	gtk_widget_show(translate_dlg);
	gtk_widget_hide(rotate_dlg);
	gtk_widget_hide(scale_dlg);
	imtrans_op = IMTRANS_OP_TRANSLATE;
}

void do_translate() {
	gdk_pixbuf_clear(pixbuf);

	// Find the parameters of the translation
	int tr_x = (int)(GTK_RANGE(translate_x)->adjustment->value);
	int tr_y = (int)(GTK_RANGE(translate_y)->adjustment->value);
	int src_x = (tr_x>0) ? 0 : -tr_x;
	int src_y = (tr_y>0) ? 0 : -tr_y;
	int dest_x = (tr_x>0) ? tr_x : 0;
	int dest_y = (tr_y>0) ? tr_y : 0;

	// Find the width and height of the translation
	int im_width = gdk_pixbuf_get_width(orig_pixbuf);
	int im_height = gdk_pixbuf_get_height(orig_pixbuf);
	int width = im_width - abs(tr_x);
	int height = im_height - abs(tr_y);

	// Perform the translation
	gdk_pixbuf_copy_area(orig_pixbuf, src_x, src_y, width, height,
		pixbuf, dest_x, dest_y);
}

void on_translate_ok_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(translate_dlg);
	imtrans_op = IMTRANS_OP_NONE;
	do_translate();
	gdk_pixbuf_unref(orig_pixbuf);
	redraw_pixbuf();
}

void on_translate_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(translate_dlg);
	imtrans_op = IMTRANS_OP_NONE;
	gdk_pixbuf_unref(pixbuf);
	pixbuf = orig_pixbuf;
	redraw_pixbuf();
}

void on_translate_apply_clicked(GtkWidget *w, gpointer data) {
	do_translate();
	redraw_pixbuf();
}

// ------------------------------------------------------------
// Scaling
// ------------------------------------------------------------

void on_scale_activate(GtkMenuItem *w, gpointer data) {
	if(imtrans_op == IMTRANS_OP_SCALE) return;
	if(!image_loaded) return;

	// Make a copy of the pixbuf
	if(imtrans_op != IMTRANS_OP_NONE) gdk_pixbuf_unref(orig_pixbuf);
	orig_pixbuf = gdk_pixbuf_copy(pixbuf);

	// Set the parameters of the dialog
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(scale_x), width);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(scale_y), height);

	// Display the dialog
	gtk_widget_show(scale_dlg);
	gtk_widget_hide(rotate_dlg);
	gtk_widget_hide(translate_dlg);
	imtrans_op = IMTRANS_OP_SCALE;
}

void do_scale() {
	// Find the width and height of the scale
	int width = gtk_spin_button_get_value_as_int(
			GTK_SPIN_BUTTON(scale_x));
	int height = gtk_spin_button_get_value_as_int(
			GTK_SPIN_BUTTON(scale_y));
	int orig_width = gdk_pixbuf_get_width(orig_pixbuf);
	int orig_height = gdk_pixbuf_get_height(orig_pixbuf);

	// Find the scale factors
	double scale_x = (double)width / (double)orig_width;
	double scale_y = (double)height / (double)orig_height;

	// Create a new pixbuf with the proper dimensions
	gdk_pixbuf_unref(pixbuf);
	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8,
		width, height);
	/*
	gdk_pixbuf_scale(orig_pixbuf, pixbuf, 0, 0, width, height,
		0, 0, scale_x, scale_y, GDK_INTERP_BILINEAR);
	*/

	guchar *src = gdk_pixbuf_get_pixels(orig_pixbuf);
	guchar *dest = gdk_pixbuf_get_pixels(pixbuf);

	int srs = gdk_pixbuf_get_rowstride(orig_pixbuf); // source rowstride
	int drs = gdk_pixbuf_get_rowstride(pixbuf);

	for(int y = 0; y < height; y++) {
	for(int x = 0; x < width; x++) {
	for(int d = 0; d < 3; d++) {
		guchar pixel;
		double xp, yp;

		xp = (double)x/scale_x;
		yp = (double)y/scale_y; 

		if(bilinear) {
/*
			double p = 0;
			for(int j = 0; j < 2; j++)
			for(int i = 0; i < 2; i++)
				p += 0.25*(1.0 -
					(xp-(int)(xp+0.5*i))*(xp-(int)(xp+0.5*i)) -
					(yp-(int)(yp+0.5*j))*(yp-(int)(yp+0.5*j)))*
					src[M2A((int)(xp+0.5*i),(int)(yp+0.5*j),
						orig_width,orig_height,srs,4)+d];
			pixel = (int)p;
*/
			double dx = xp-(int)xp;
			double dy = yp-(int)yp;
			guchar p00 = src[M2A((int)xp,(int)yp,orig_width,orig_height,srs,4)+d];
			guchar p01 = src[M2A((int)(xp+1),(int)yp,orig_width,orig_height,srs,4)+d];
			guchar p10 = src[M2A((int)xp,(int)(yp+1),orig_width,orig_height,srs,4)+d];
			guchar p11 = src[M2A((int)(xp+1),(int)(yp+1),orig_width,orig_height,srs,4)+d];
			pixel = (int)((1-dx)*(1-dy)*p00 + dx*(1-dy)*p01 + (1-dx)*dy*p10 + dx*dy*p11);
		} else {
			pixel = src[M2A((int)xp,(int)yp,
				orig_width,orig_height,srs,4)+d];
		}
		dest[M2A(x,y,width,height,drs,4)+d] = pixel;
	}
	}
	}
}

void on_scale_ok_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(scale_dlg);
	imtrans_op = IMTRANS_OP_NONE;
	do_scale();
	gdk_pixbuf_unref(orig_pixbuf);
	redraw_pixbuf();
}

void on_scale_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(scale_dlg);
	imtrans_op = IMTRANS_OP_NONE;
	gdk_pixbuf_unref(pixbuf);
	pixbuf = orig_pixbuf;
	redraw_pixbuf();
}

void on_scale_apply_clicked(GtkWidget *w, gpointer data) {
	do_scale();
	redraw_pixbuf();
}

// ------------------------------------------------------------
// Rotation
// ------------------------------------------------------------

void on_rotate_activate(GtkMenuItem *w, gpointer data) {
	if(imtrans_op == IMTRANS_OP_ROTATE) return;
	if(!image_loaded) return;

	// Make a copy of the pixbuf
	if(imtrans_op != IMTRANS_OP_NONE) gdk_pixbuf_unref(orig_pixbuf);
	orig_pixbuf = gdk_pixbuf_copy(pixbuf);

	// Display the dialog
	gtk_widget_show(rotate_dlg);
	gtk_widget_hide(translate_dlg);
	gtk_widget_hide(scale_dlg);
	imtrans_op = IMTRANS_OP_ROTATE;
}

void do_rotate() {
	double angle = GTK_RANGE(rotate_value)->adjustment->value * M_PI / 180.0;
	double cosa = cos(angle);
	double sina = sin(angle);
	int x,y,d,srs,drs;
	double xp,yp,xt,yt;
	int width, height;
	guchar *src, *dest, pixel;

	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	src = gdk_pixbuf_get_pixels(orig_pixbuf);
	dest = gdk_pixbuf_get_pixels(pixbuf);
	srs = gdk_pixbuf_get_rowstride(orig_pixbuf);
	drs = gdk_pixbuf_get_rowstride(pixbuf);
	xt = -(width/2*(cosa-1)+height/2*sina);
	yt = -(height/2*(cosa-1)-width/2*sina);
	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			for(d = 0; d < 3; d++) {
				xp = cosa*x + sina*y + xt;
				yp = cosa*y - sina*x + yt;

				if(xp < 0 || xp >= width || yp < 0 || yp >= height) {
					dest[M2A(x,y,width,height,drs,4)+d] = 0;
					continue;
				}
				if(bilinear) {
					double dx = xp-(int)xp;
					double dy = yp-(int)yp;
					guchar p00 = src[M2A((int)xp,(int)yp,width,height,srs,4)+d];
					guchar p01 = src[M2A((int)(xp+1),(int)yp,width,height,srs,4)+d];
					guchar p10 = src[M2A((int)xp,(int)(yp+1),width,height,srs,4)+d];
					guchar p11 = src[M2A((int)(xp+1),(int)(yp+1),width,height,srs,4)+d];
					pixel = (int)((1-dx)*(1-dy)*p00 + dx*(1-dy)*p01 + (1-dx)*dy*p10 + dx*dy*p11);
				} else {
					pixel = src[M2A((int)xp,(int)yp,width,height,srs,4)+d];
				}
				dest[M2A(x,y,width,height,drs,4)+d] = pixel;
			}
		}
	}
			
}


void on_rotate_ok_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(rotate_dlg);
	imtrans_op = IMTRANS_OP_NONE;
	do_rotate();
	gdk_pixbuf_unref(orig_pixbuf);
	redraw_pixbuf();
}

void on_rotate_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(rotate_dlg);
	imtrans_op = IMTRANS_OP_NONE;
	gdk_pixbuf_unref(pixbuf);
	pixbuf = orig_pixbuf;
	redraw_pixbuf();
}

void on_rotate_apply_clicked(GtkWidget *w, gpointer data) {
	do_rotate();
	redraw_pixbuf();
}

// ------------------------------------------------------------
// Mouse movement
// ------------------------------------------------------------

void on_motion_notify_event(GtkWidget *w, GdkEventMotion *event,
	gpointer data) {

	GdkWindow *win = drawingarea->window;
	GdkGC *gc = drawingarea->style->fg_gc[drawingarea->state];

	if(!mouse_down) return;

	// Request XOR
	gdk_gc_set_function(gc, GDK_INVERT);

	gdk_draw_line(win, gc, mouse_down_x, mouse_down_y, mouse_x, mouse_y);

	mouse_x = (int)event->x;
	mouse_y = (int)event->y;

	gdk_draw_line(win, gc, mouse_down_x, mouse_down_y, mouse_x, mouse_y);

	// Return to normal operation
	gdk_gc_set_function(drawingarea->style->fg_gc[w->state], GDK_COPY);

	switch(imtrans_op) {
		case IMTRANS_OP_NONE: break;
		case IMTRANS_OP_TRANSLATE:
			GTK_RANGE(translate_x)->adjustment->value = mouse_x - mouse_down_x;
			GTK_RANGE(translate_y)->adjustment->value = mouse_y - mouse_down_y;
			break;
		case IMTRANS_OP_SCALE:
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(scale_x),
				abs(mouse_x - mouse_down_x));
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(scale_y),
				abs(mouse_y - mouse_down_y));
			break;
		case IMTRANS_OP_ROTATE:
			if(mouse_x == mouse_down_x)
				GTK_RANGE(rotate_value)->adjustment->value =
					(mouse_y-mouse_down_y<0)?270:90;
			else
				GTK_RANGE(rotate_value)->adjustment->value =
					180.0 / M_PI * atan(
					((double)mouse_y-(double)mouse_down_y)/
					((double)mouse_x-(double)mouse_down_x))+
					((mouse_down_x>mouse_x)?180.0:0.0)+
					((mouse_down_y>mouse_y &&
					  mouse_down_x<mouse_x)?360.0:0.0);
			gtk_widget_hide(rotate_value);
			gtk_widget_show(rotate_value);
			break;
	}	
}

gboolean on_button_press_event(GtkWidget *w, GdkEventButton *event,
	gpointer data) {

	mouse_down = event->button;
	mouse_x = mouse_down_x = (int)event->x;
	mouse_y = mouse_down_y = (int)event->y;

	return true;
}

gboolean on_button_release_event(GtkWidget *w,
	GdkEventButton *event, gpointer data) {

	mouse_down = 0;
	switch(imtrans_op) {
		case IMTRANS_OP_NONE: break;
		case IMTRANS_OP_TRANSLATE: do_translate(); break;
		case IMTRANS_OP_SCALE: do_scale(); break;
		case IMTRANS_OP_ROTATE: do_rotate(); break;
	}
	redraw_pixbuf();
	return true;
}

// ------------------------------------------------------------
// Options
// ------------------------------------------------------------
void on_bilinear_activate(GtkMenuItem *m, gpointer data) {
	bilinear = !bilinear;
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

} // extern "C"

int main(int argc, char *argv[]) {
	GladeXML *xml;

	gtk_init(&argc, &argv);
	glade_init();
	gdk_rgb_init();

	gtk_widget_set_default_visual(gdk_rgb_get_visual());
	gtk_widget_set_default_colormap(gdk_rgb_get_cmap());
	xml = glade_xml_new("imtrans.glade", (char*)NULL);
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

