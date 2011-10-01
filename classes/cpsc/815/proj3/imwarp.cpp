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

#define MAX_MESH_X 20
#define MAX_MESH_Y 20
#define MAX_HEIGHT 10240
#define MAX_WIDTH 10240

// Some typedefs
typedef struct {
	float x;
	float y;
} Coord;

// Global widgets
GtkWidget *loadfile_dlg;
GtkWidget *savefile_dlg;
GtkWidget *about_dlg;
GtkWidget *options_dlg;
GtkWidget *drawingarea;
GtkWidget *mesh_rows;
GtkWidget *mesh_cols;
GtkWidget *bilinear_interp;

// Global variables
GdkPixbuf *pixbuf, *orig_pixbuf;
bool image_loaded = false;
int mouse_x, mouse_y, mouse_down_x, mouse_down_y, mouse_down;
int mesh_x = 3, mesh_y = 3;
Coord mesh[MAX_MESH_X+1][MAX_MESH_Y+1];
int sx = 0, sy = 0;
int tx[MAX_MESH_X+1][MAX_HEIGHT];
int ty[MAX_MESH_Y+1][MAX_WIDTH];
bool bilinear = false;

extern "C" {

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------

void init_widgets(GladeXML *xml) {
	loadfile_dlg = glade_xml_get_widget(xml, "loadfile_dlg");
	savefile_dlg = glade_xml_get_widget(xml, "savefile_dlg");
	about_dlg = glade_xml_get_widget(xml, "about_dlg");
	options_dlg = glade_xml_get_widget(xml, "options_dlg");
	drawingarea = glade_xml_get_widget(xml, "drawingarea");
	mesh_rows = glade_xml_get_widget(xml, "mesh_rows");
	mesh_cols = glade_xml_get_widget(xml, "mesh_columns");
	bilinear_interp = glade_xml_get_widget(xml, "bilinear_interp");

	// Glade doesn't support word wrap
	GtkWidget *about_text = glade_xml_get_widget(xml, "about_text");
	gtk_text_set_word_wrap(GTK_TEXT(about_text), true);
}

// ------------------------------------------------------------
// Mesh routines
// ------------------------------------------------------------

void init_mesh() {
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);

	for(int y = 0; y <= mesh_y; y++) {
		for(int x = 0; x <= mesh_x; x++) {
			mesh[x][y].x = (float)width*x/mesh_x;
			mesh[x][y].y = (float)height*y/mesh_y;
		}
	}
}

void calc_line_intercepts(int n, int d, int x1, int y1, int x2, int y2) {

	float m = (float)(y2 - y1) / (float)(x2 - x1);
	float b = y1 - m*x1;

	// g_print("calc_line_intercepts from %d,%d to %d,%d\n", x1, y1, x2, y2);
	// g_print("m = %f, b = %f\n", m, b);

	int xs = MIN(x1, x2);
	int xe = MAX(x1, x2);
	int ys = MIN(y1, y2);
	int ye = MAX(y1, y2);

	if(d == 0) {
		for(int y = ys; y <= ye; y++) {
			if(x2 == x1) {
				tx[n][y] = x1;
			} else {
				tx[n][y] = (int)((y - b) / m);
			}
			// g_print("tx[%d][%d] = %d\n", n, y, tx[n][y]);
		}
	} else {
		for(int x = xs; x <= xe; x++) {
			ty[n][x] = (int)(m*x + b);
		}
	}
}

void calc_intercepts() {
	int x, y;
	for(y = 1; y <= mesh_y; y++) {
		for(x = 1; x <= mesh_x; x++) {
			calc_line_intercepts(y, 1,
				(int)mesh[x-1][y].x, (int)mesh[x-1][y].y,
				(int)mesh[x][y].x, (int)mesh[x][y].y);
			calc_line_intercepts(x, 0,
				(int)mesh[x][y-1].x, (int)mesh[x][y-1].y,
				(int)mesh[x][y].x, (int)mesh[x][y].y);
		}
	}
	/*
	for(x = 1; x < mesh_x; x++) {
		calc_line_intercepts(mesh_x, 0,
			(int)mesh[x][mesh_y-1].x, (int)mesh[x][mesh_y-1].y,
			(int)mesh[x][mesh_y].x, (int)mesh[x][mesh_y].y);
	}
	for(y = 1; y < mesh_y; y++) {
		calc_line_intercepts(mesh_y, 1,
			(int)mesh[mesh_x-1][y].x, (int)mesh[mesh_x-1][y].y,
			(int)mesh[mesh_x][y].x, (int)mesh[mesh_x][y].y);
	}
	*/
}

// ------------------------------------------------------------
// Drawing routines
// ------------------------------------------------------------

gboolean on_drawingarea_expose_event(GtkWidget *w, GdkEventExpose *event,
	gpointer data) {

	GdkWindow *win = w->window;
	GdkGC *gc = drawingarea->style->fg_gc[w->state];

	gdk_window_clear_area(win, event->area.x, event->area.y,
		event->area.width, event->area.height);

	gdk_gc_set_clip_rectangle(gc, &event->area);

	if(!image_loaded) return true;

	// Render the pixbuf to our drawing area
	int width = MIN(event->area.width,
		gdk_pixbuf_get_width(pixbuf) - event->area.x);
	int height = MIN(event->area.height,
		gdk_pixbuf_get_height(pixbuf) - event->area.y);
	if(width < 0 || height < 0) return true;
	gdk_pixbuf_render_to_drawable(pixbuf, win, gc,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		width, height,
		GDK_RGB_DITHER_NONE, 0, 0);

	// Request XOR
	gdk_gc_set_function(gc, GDK_INVERT);

	// Draw the mesh
	int x, y;
	for(y = 1; y < mesh_y; y++) {
		for(x = 1; x < mesh_x; x++) {
			gdk_draw_line(win, gc,
				(int)mesh[x-1][y].x, (int)mesh[x-1][y].y,
				(int)mesh[x][y].x, (int)mesh[x][y].y);
			gdk_draw_line(win, gc,
				(int)mesh[x][y-1].x, (int)mesh[x][y-1].y,
				(int)mesh[x][y].x, (int)mesh[x][y].y);
		}
	}
	for(x = 1; x < mesh_x; x++) {
		gdk_draw_line(win, gc,
			(int)mesh[x][mesh_y-1].x, (int)mesh[x][mesh_y-1].y,
			(int)mesh[x][mesh_y].x, (int)mesh[x][mesh_y].y);
	}
	for(y = 1; y < mesh_y; y++) {
		gdk_draw_line(win, gc,
			(int)mesh[mesh_x-1][y].x, (int)mesh[mesh_x-1][y].y,
			(int)mesh[mesh_x][y].x, (int)mesh[mesh_x][y].y);
	}

	// Return to normal operation
	gdk_gc_set_function(gc, GDK_COPY);

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

	if(image_loaded) {
		gdk_pixbuf_unref(pixbuf);
		gdk_pixbuf_unref(orig_pixbuf);
	}

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
	orig_pixbuf = gdk_pixbuf_copy(pixbuf);

	init_mesh();
	calc_intercepts();
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
	fprintf(fp, "%d %d\n",
		gdk_pixbuf_get_width(pixbuf),
		gdk_pixbuf_get_height(pixbuf));
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
// Warp
// ------------------------------------------------------------

guchar interp(guchar *i, float xp, float yp, int w, int h, int rs, int d) {
	if(bilinear) {
		double dx = xp-(int)xp;
		double dy = yp-(int)yp;
		int x = (int)xp;
		int y = (int)yp;
		guchar p00 = i[M2A(x,y,w,h,rs,4)+d];
		guchar p01 = i[M2A(x+1,y,w,h,rs,4)+d];
		guchar p10 = i[M2A(x,y+1,w,h,rs,4)+d];
		guchar p11 = i[M2A(x+1,y+1,w,h,rs,4)+d];
		return (guchar)(
			(1-dx)*(1-dy)*p00 +
			   dx *(1-dy)*p01 +
			(1-dx)*   dy *p10 +
			   dx *   dy *p11);
	} else {
		return i[M2A((int)xp,(int)yp,w,h,rs,4)+d];
	}
}

void do_warp_x(int nx, int y1, int y2) {
	guchar *src = gdk_pixbuf_get_pixels(orig_pixbuf);
	guchar *dest = gdk_pixbuf_get_pixels(pixbuf);
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);
	int srs = gdk_pixbuf_get_rowstride(orig_pixbuf);
	int drs = gdk_pixbuf_get_rowstride(pixbuf);

	// g_print("do_warp_x from y = %d to %d\n", y1, y2);
	// g_print("width = %d; mesh_x = %d\n", width, mesh_x);

	int x, y;
	for(y = y1; y <= y2; y++) {
		float scale;
		scale = (float)(width/mesh_x) / (float)(tx[nx+1][y] - tx[nx][y]);
		// g_print("x from %d to %d (scale=%f)\n", tx[nx][y], tx[nx+1][y], scale);
		for(x = tx[nx][y]; x < tx[nx+1][y]; x++) {
		        for(int d = 0; d < 3; d++) {
		                guchar pixel;
		                double xp, yp;

		                xp = (width/mesh_x*nx) + scale*(x-tx[nx][y]);
				yp = y;

               			// pixel = src[M2A((int)xp,(int)yp,width,height,srs,4)+d];
				pixel = interp(src, xp, yp, width, height, srs, d);
               			dest[M2A(x,y,width,height,drs,4)+d] = pixel;
			}
		}
	}

}

void do_warp_y(int ny, int x1, int x2) {
	guchar *src = gdk_pixbuf_get_pixels(orig_pixbuf);
	guchar *dest = gdk_pixbuf_get_pixels(pixbuf);
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);
	int srs = gdk_pixbuf_get_rowstride(orig_pixbuf);
	int drs = gdk_pixbuf_get_rowstride(pixbuf);

	// g_print("do_warp_y from x = %d to %d\n", x1, x2);
	// g_print("height = %d; mesh_y = %d\n", height, mesh_y);

	int x, y;
	for(x = x1; x <= x2; x++) {
		float scale;
		scale = (float)(height/mesh_y) / (float)(ty[ny+1][x] - ty[ny][x]);
		// g_print("y from %d to %d (scale=%f)\n", tx[ny][x], tx[ny+1][x], scale);
		for(y = ty[ny][x]; y < ty[ny+1][x]; y++) {
		        for(int d = 0; d < 3; d++) {
		                guchar pixel;
		                double xp, yp;

				xp = x;
		                yp = (height/mesh_y*ny) + scale*(y-ty[ny][x]);

               			// pixel = src[M2A((int)xp,(int)yp,width,height,srs,4)+d];
				pixel = interp(src, xp, yp, width, height, srs, d);
               			dest[M2A(x,y,width,height,drs,4)+d] = pixel;
			}
		}
	}

}

void do_warp() {
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);

	for(int x = 0; x < mesh_x; x++) {
		do_warp_x(x, 0, height);
	}

	GdkPixbuf *tmp_pixbuf = orig_pixbuf;
	orig_pixbuf = pixbuf;
	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, true, 8, width, height);

	for(int y = 0; y < mesh_y; y++) {
		do_warp_y(y, 0, width);
	}

	gdk_pixbuf_unref(orig_pixbuf);
	orig_pixbuf = tmp_pixbuf;
}

void on_warp_activate(GtkMenuItem *item, gpointer data) {
	do_warp();
	redraw_pixbuf();
}

void on_reset_activate(GtkMenuItem *item, gpointer data) {
	init_mesh();
	calc_intercepts();
	gdk_pixbuf_unref(pixbuf);
	pixbuf = gdk_pixbuf_copy(orig_pixbuf);
	redraw_pixbuf();
}

// ------------------------------------------------------------
// Mouse movement
// ------------------------------------------------------------

void draw_mesh_lines(int x, int y) {
	GdkWindow *win = drawingarea->window;
	GdkGC *gc = drawingarea->style->fg_gc[drawingarea->state];

	gdk_draw_line(win, gc,
		(int)mesh[x-1][y].x, (int)mesh[x-1][y].y,
		(int)mesh[x][y].x, (int)mesh[x][sy].y);
	gdk_draw_line(win, gc,
		(int)mesh[x+1][y].x, (int)mesh[x+1][y].y,
		(int)mesh[x][y].x, (int)mesh[x][y].y);
	gdk_draw_line(win, gc,
		(int)mesh[x][y-1].x, (int)mesh[x][y-1].y,
		(int)mesh[x][y].x, (int)mesh[x][y].y);
	gdk_draw_line(win, gc,
		(int)mesh[x][y+1].x, (int)mesh[x][y+1].y,
		(int)mesh[x][y].x, (int)mesh[x][y].y);
}

void on_motion_notify_event(GtkWidget *w, GdkEventMotion *event,
	gpointer data) {

	if(!mouse_down) return;
	if(!image_loaded) return;
	if(!sx || !sy) return;
	GdkGC *gc = drawingarea->style->fg_gc[drawingarea->state];

	GdkRectangle area = {0, 0, G_MAXSHORT, G_MAXSHORT};
	gdk_gc_set_clip_rectangle(gc, &area);

	// Request XOR
	gdk_gc_set_function(gc, GDK_INVERT);

	// Undraw the old lines
	draw_mesh_lines(sx, sy);

	// Update the mesh
	mouse_x = (int)event->x;
	mouse_y = (int)event->y;
	mesh[sx][sy].x = mouse_x;
	mesh[sx][sy].y = mouse_y;

	// Draw the new lines
	draw_mesh_lines(sx, sy);

	// Return to normal operation
	gdk_gc_set_function(gc, GDK_COPY);
}

gboolean on_button_press_event(GtkWidget *w, GdkEventButton *event,
	gpointer data) {

	if(!image_loaded) return true;

	mouse_down = event->button;
	mouse_x = mouse_down_x = (int)event->x;
	mouse_y = mouse_down_y = (int)event->y;

	// This is a hack; we should probably find a better way to do this
	sx = sy = 1;
	float distance = (float)HUGE_VAL, newdist, distx, disty;
	for(int y = 0; y < mesh_y; y++) {
		for(int x = 1; x < mesh_x; x++) {
			distx = mouse_x - mesh[x][y].x;
			disty = mouse_y - mesh[x][y].y;
			newdist = distx*distx + disty*disty;
			if(newdist < distance) {
				distance = newdist;
				sx = x;
				sy = y;
			}
		}
	}
	/*
	if(distance > 5) {
		sx = sy = 0;
		g_print("distance = %f\n", distance);
	}
	*/

	return true;
}

gboolean on_button_release_event(GtkWidget *w,
	GdkEventButton *event, gpointer data) {

	if(!image_loaded) return true;

	mouse_down = 0;
	sx = sy = 0;
	calc_intercepts();

	return true;
}

// ------------------------------------------------------------
// Options
// ------------------------------------------------------------

void on_options_activate(GtkMenuItem *item, gpointer data) {
	gtk_widget_show(options_dlg);
}

void on_options_ok_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(options_dlg);
	mesh_x = gtk_spin_button_get_value_as_int(
		GTK_SPIN_BUTTON(mesh_cols));
	mesh_y = gtk_spin_button_get_value_as_int(
		GTK_SPIN_BUTTON(mesh_rows));
	bilinear = gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(bilinear_interp));
	init_mesh();
	calc_intercepts();
	redraw_pixbuf();
}

void on_options_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(options_dlg);
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
	xml = glade_xml_new("imwarp.glade", (char*)NULL);
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

