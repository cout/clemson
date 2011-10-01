// Imwave
// Paul Brannan <pbranna@clemson.edu
// Please use a tab size of 4 when reading this file

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <zlib.h>

using namespace std;

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
#define SCALE(a,b,c,d,n) ((n-a)*(d-c)/(b-a)+c)
#define EPSILON 0.0001

// Some wavelet stuphs
struct Wavelet {
	vector<float> h, g, ht, gt;
	float maxh, maxg, minh, ming;
	int size;
};

struct Haar: public Wavelet {
	Haar() {
		h.push_back(1.0/sqrt(2));
		h.push_back(1.0/sqrt(2));
		g.push_back(1.0/sqrt(2));
		g.push_back(-1.0/sqrt(2));
		ht.push_back(1.0/sqrt(2));
		ht.push_back(1.0/sqrt(2));
		gt.push_back(1.0/sqrt(2));
		gt.push_back(-1.0/sqrt(2));
		minh = 0.0;
		maxh = 2.0/sqrt(2);
		ming = -1.0/sqrt(2);
		maxg = 1.0/sqrt(2);
		size = 2;
	}
} haar;

struct Daub4: public Wavelet {
	Daub4() {
		h.push_back((1+sqrt(3))/(4*sqrt(2)));
		h.push_back((3+sqrt(3))/(4*sqrt(2)));
		h.push_back((3-sqrt(3))/(4*sqrt(2)));
		h.push_back((1-sqrt(3))/(4*sqrt(2)));
		g.push_back(h[3]);
		g.push_back(-h[2]);
		g.push_back(h[1]);
		g.push_back(-h[0]);
		ht.push_back(h[2]);
		ht.push_back(h[1]);
		ht.push_back(h[0]);
		ht.push_back(h[3]);
		gt.push_back(h[3]);
		gt.push_back(-h[0]);
		gt.push_back(h[1]);
		gt.push_back(-h[2]);
		minh = (1-sqrt(3))/(4*sqrt(2));
		maxh = (7+sqrt(3))/(4*sqrt(2));
		ming = (-3-sqrt(3))/(4*sqrt(2));
		maxg = (3+sqrt(3))/(4*sqrt(2));
		size = 4;
	}
} daub4;

Wavelet *wavelet = &haar;

// Global widgets
GtkWidget *loadfile_dlg;
GtkWidget *savefile_dlg;
GtkWidget *about_dlg;
GtkWidget *drawingarea;
GtkWidget *options_dlg;
GtkWidget *levels_sb;
GtkWidget *threshold_sb;

// Global variables
GdkPixbuf *pixbuf, *orig_pixbuf;
double *darr, *orig_darr;
bool image_loaded = false;
bool bilinear = false;
int levels = 1;
int threshold = 255;
bool compressed_io = false;

extern "C" {

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------

void init_widgets(GladeXML *xml) {
	loadfile_dlg = glade_xml_get_widget(xml, "loadfile_dlg");
	savefile_dlg = glade_xml_get_widget(xml, "savefile_dlg");
	about_dlg = glade_xml_get_widget(xml, "about_dlg");
	drawingarea = glade_xml_get_widget(xml, "drawingarea");
	options_dlg = glade_xml_get_widget(xml, "options_dlg");
	levels_sb = glade_xml_get_widget(xml, "levels_sb");
	threshold_sb = glade_xml_get_widget(xml, "threshold_sb");

	// Glade doesn't support word wrap
	GtkWidget *about_text = glade_xml_get_widget(xml, "about_text");
	gtk_text_set_word_wrap(GTK_TEXT(about_text), true);
}

// ------------------------------------------------------------
// Double array functions
// ------------------------------------------------------------

void pixbuf2darr(GdkPixbuf *p, double *d) {
}

void darr2pixbuf(double *d, GdkPixbuf *p) {
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
// Miscellaneous functions
// ------------------------------------------------------------

void gtk_sync_display () {
	while (gtk_events_pending ()) gtk_main_iteration ();
	gdk_flush ();
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


void decimate(int x1, int y1, int x2, int y2) {
	guchar *buf = gdk_pixbuf_get_pixels(pixbuf);
	int w = gdk_pixbuf_get_width(pixbuf);
	int h = gdk_pixbuf_get_height(pixbuf);
	int rs = gdk_pixbuf_get_rowstride(pixbuf);

	guchar pixel;
	for(int y = y1; y < y2; y++) {
		for(int x = x1; x < x2; x++) {
			for(int d = 0; d < 3; d++) {
				pixel = buf[M2A(x,y,w,h,rs,4)+d];
				if(pixel <= (255-threshold)) pixel = 0;
				buf[M2A(x,y,w,h,rs,4)+d] = pixel;
			}
		}
	}
	
}

// ------------------------------------------------------------
// Wavelet functions
// ------------------------------------------------------------

float do_filter(const vector<float> &f, const vector<float> &p) {
	float t = 0.0;
	for(unsigned int j = 0; j < f.size(); j++) t += f[j]*p[j];
	return t;
}

void do_transform_dir(int x1, int y1, int x2, int y2, int dir) {
	guchar *src = gdk_pixbuf_get_pixels(orig_pixbuf);
	guchar *dest = gdk_pixbuf_get_pixels(pixbuf);
	int w = gdk_pixbuf_get_width(pixbuf);
	int h = gdk_pixbuf_get_height(pixbuf);
	int srs = gdk_pixbuf_get_rowstride(orig_pixbuf);
	int drs = gdk_pixbuf_get_rowstride(pixbuf);

	vector<float> pixel;
	float low, high;
	int dpos, hpos;

	for(int j = 0; j < wavelet->size; j++) pixel.push_back (0.0);


	for(int y = y1; y < y2; y+=dir?2:1) {
		for(int x = x1; x < x2; x+=dir?1:2) {
			hpos = dir?((y2-y1)/2+y1):((x2-x1)/2+x1);
			for(int d = 0; d < 4; d++) {
				for(int j = 0; j < wavelet->size; j++) {
					pixel[j] = src[M2A(x+(dir?0:j),y+(dir?j:0),w,h,srs,4)+d]
						/ 255.0;
				}

				low = do_filter(wavelet->h, pixel);
				high = do_filter(wavelet->g, pixel);
				dpos = dir?((y-y1)/2+y1):((x-x1)/2+x1);
				dest[M2A(dir?x:dpos,dir?dpos:y,w,h,drs,4)+d] =
					(unsigned char)
					SCALE(wavelet->minh, wavelet->maxh, 0, 255, low);
				dest[M2A(dir?x:(dpos+hpos),dir?(dpos+hpos):y, w,h,drs,4)+d] =
					(unsigned char)
					SCALE(wavelet->ming, wavelet->maxg, 0, 255, high);
			}
		}
	}
}

void do_transform() {
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);

	GdkPixbuf *tmp_pixbuf = gdk_pixbuf_copy(orig_pixbuf);
	GdkPixbuf *tpb2;

	tpb2 = orig_pixbuf; orig_pixbuf = pixbuf; pixbuf = tpb2;
	for(int j = 0, n = 1; j < levels; j++) {
		tpb2 = orig_pixbuf; orig_pixbuf = pixbuf; pixbuf = tpb2;
		do_transform_dir(0, 0, width/n, height/n, 1); // y dir
		tpb2 = orig_pixbuf; orig_pixbuf = pixbuf; pixbuf = tpb2;
		do_transform_dir(0, 0, width/n, height/n, 0); // x dir

		n <<= 1;
		decimate(width/n, 0, 2*width/n, height/n);
		decimate(width/n, height/n, 2*width/n, 2*height/n);
		decimate(0, height/n, 2*width/n, 2*height/n);
	}

	gdk_pixbuf_unref(orig_pixbuf);
	orig_pixbuf = tmp_pixbuf;
}

/*
float do_unfilter(const vector<float> &f, float low, float high, int j) {
	return (low+high)*f[j];
}
*/

// This isn't right.  Oh well, it works for Haar.
float do_unfilter(const vector<float> &f, float low, float high, int j) {
	return low*f[0]+high*f[1];
}

void do_untransform_dir(int x1, int y1, int x2, int y2, int dir) {
	guchar *src = gdk_pixbuf_get_pixels(orig_pixbuf);
	guchar *dest = gdk_pixbuf_get_pixels(pixbuf);
	int w = gdk_pixbuf_get_width(pixbuf);
	int h = gdk_pixbuf_get_height(pixbuf);
	int srs = gdk_pixbuf_get_rowstride(orig_pixbuf);
	int drs = gdk_pixbuf_get_rowstride(pixbuf);

	float low, high;
	int hpos;
	guchar pixel;

	// Clear the area we are going to reconstruct the image into
	for(int y = y1; y < y2; y++) {
		for(int x = x1; x < x2; x++) {
			for(int d = 0; d < 3; d++) {
				dest[M2A(x,y,w,h,drs,4)+d] = 0;
			}
			dest[M2A(x,y,w,h,drs,4)+3] = 255;
		}
	}

	// Perform the actual reconstruction
	int ymax = dir?((y2-y1)/2+y1):y2;
	int xmax = dir?x2:((x2-x1)/2+x1);
	for(int y = y1; y < ymax; y++) {
		for(int x = x1; x < xmax; x++) {
			hpos = dir?((y2-y1)/2+y1):((x2-x1)/2+x1);
			for(int d = 0; d < 4; d++) {
// g_print("x = %d, y = %d, dir = %d\n", x, y, dir);
				low = src[M2A(x,y,w,h,srs,4)+d];
				high = src[M2A(x+(dir?0:hpos),y+(dir?hpos:0),w,h,srs,4)+d];
// g_print("low = %f, high = %f\n", low, high);
				low = SCALE(-1.0, 255.0, wavelet->minh, wavelet->maxh, low);
				high = SCALE(-1.0, 255.0, wavelet->ming, wavelet->maxg, high);
// g_print("low = %f, high = %f\n", low, high);
				// Okay, this isn't correct, but it will work for Haar
				pixel = (int)(255.0*do_unfilter(wavelet->ht, low, high,0));
				dest[M2A(dir?x:(x*2+0), dir?(y*2+0):y,w,h,drs,4)+d] =
					pixel;
// g_print("dest[%d,%d] = %d\n", dir?x:(x*2+0), dir?(y*2+0):y, pixel);
				pixel = (int)(255.0*do_unfilter(wavelet->gt, low, high,0));
				dest[M2A(dir?x:(x*2+1), dir?(y*2+1):y,w,h,drs,4)+d] =
					pixel;
// g_print("dest[%d,%d] = %d\n", dir?x:(x*2+1), dir?(y*2+1):y, pixel);
			}
		}
		// redraw_pixbuf();
		// gtk_sync_display();
	}
}

void do_untransform() {
	int width = gdk_pixbuf_get_width(pixbuf);
	int height = gdk_pixbuf_get_height(pixbuf);

	GdkPixbuf *tmp_pixbuf = gdk_pixbuf_copy(orig_pixbuf);
	GdkPixbuf *tpb2;

	for(int j = 0, n = 1<<(levels-1); j < levels; j++) {
		tpb2 = orig_pixbuf; orig_pixbuf = pixbuf; pixbuf = tpb2;
		do_untransform_dir(0, 0, width/n, height/n, 0); // x dir
		tpb2 = orig_pixbuf; orig_pixbuf = pixbuf; pixbuf = tpb2;
		do_untransform_dir(0, 0, width/n, height/n, 1); // y dir

		n >>= 1;
	}

	gdk_pixbuf_unref(orig_pixbuf);
	orig_pixbuf = tmp_pixbuf;
}

// ------------------------------------------------------------
// Wavelet UI
// ------------------------------------------------------------

void on_transform_activate(GtkMenuItem *m, gpointer data) {
	do_transform();
	redraw_pixbuf();
}

void on_untransform_activate(GtkMenuItem *m, gpointer data) {
	do_untransform();
	redraw_pixbuf();
}

void on_reset_activate(GtkMenuItem *m, gpointer data) {
	pixbuf = gdk_pixbuf_copy(orig_pixbuf);
	redraw_pixbuf();
}

void on_options_activate(GtkMenuItem *m, gpointer data) {
	gtk_widget_show(options_dlg);
}

void on_options_ok_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(options_dlg);
	levels = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(levels_sb));
	threshold = gtk_spin_button_get_value_as_int(
		GTK_SPIN_BUTTON(threshold_sb));
}

void on_options_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(options_dlg);
}

// ------------------------------------------------------------
// File loading functions
// ------------------------------------------------------------

void on_load_activate(GtkWidget *w, gpointer data) {
	compressed_io = false;
	gtk_widget_show(loadfile_dlg);
}

void on_load_compressed_activate(GtkWidget *w, gpointer data) {
	compressed_io = true;
	gtk_widget_show(loadfile_dlg);
}

static void free_compressed_pixels(guchar *pixels, gpointer data) {
	delete[] pixels;
}

// FIX ME!  We need to handle loading compressed data.
void on_load_ok_clicked(GtkWidget *w, gpointer data) {
	gchar *filename = gtk_file_selection_get_filename(
		GTK_FILE_SELECTION(loadfile_dlg));
	gtk_widget_hide(loadfile_dlg);

	if(image_loaded) {
		gdk_pixbuf_unref(pixbuf);
		gdk_pixbuf_unref(orig_pixbuf);
	}

	if(compressed_io) {
		// Load a wavelet-compressed image
		FILE *fp = fopen(filename, "rb");
		if(!fp) goto no_image;

		int width, height;
		char s[1024];

		Bytef *dest;
		Bytef *src;
		uLongf destlen;
		uLong srclen;

		fgets(s, sizeof(s), fp);
		if(strcmp(s, "W6\n")) {
			fclose(fp);
			g_print("Not a compressed wavelet image\n");
			goto no_image;
		}
		fgets(s, sizeof(s), fp);
		sscanf(s, "%d %d", &width, &height);
		fgets(s, sizeof(s), fp);
		sscanf(s, "%ld %d", &srclen, &levels);
		
		destlen = width*height*4+1;
		src = new Bytef[srclen];
		dest = new Bytef[destlen];
		fread(src, srclen, 1, fp);

		g_print("First character: %d\n", src[0]);
		if(uncompress(dest, &destlen, src, srclen) != Z_OK) {
			g_print("Unable to decompress data\n");
			goto no_image;
		}
		pixbuf = gdk_pixbuf_new_from_data((guchar*)dest,
			GDK_COLORSPACE_RGB, true, 8, width, height, width,
			free_compressed_pixels, NULL);
		delete[] src;
	} else {
		// Tell gdk-pixbuf to load the image
		pixbuf = gdk_pixbuf_new_from_file(filename);
	}
	if(!pixbuf) goto no_image;

	// Add an alpha channel if the image does not already have one
	// We want full opacity (alpha = 1.0 for all pixels)
	GdkPixbuf *tmp_pixbuf;
	tmp_pixbuf = gdk_pixbuf_add_alpha(pixbuf, 0, 0, 0, 0);
	gdk_pixbuf_unref(pixbuf);
	pixbuf = tmp_pixbuf;
	if(!gdk_pixbuf_get_has_alpha(pixbuf)) g_print("um... no alpha?\n");

	// We now have an image loaded (hopefully)
	image_loaded = true;
	orig_pixbuf = gdk_pixbuf_copy(pixbuf);

	if(compressed_io) {
		do_untransform();
		gdk_pixbuf_unref(orig_pixbuf);
		orig_pixbuf = gdk_pixbuf_copy(pixbuf);
	}

	redraw_pixbuf();
	return;

no_image:
	g_print("Unable to load image!\n");
	image_loaded = false;
}

void on_load_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(loadfile_dlg);
}

// ------------------------------------------------------------
// File saving functions
// ------------------------------------------------------------

void on_save_activate(GtkWidget *w, gpointer data) {
	if(!image_loaded) return;
	compressed_io = false;
	gtk_widget_show(savefile_dlg);
}

void on_save_compressed_activate(GtkWidget *w, gpointer data) {
	if(!image_loaded) return;
	compressed_io = true;
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

	if(compressed_io)
		fprintf(fp, "W6\n");
	else
		fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n",
		gdk_pixbuf_get_width(pixbuf),
		gdk_pixbuf_get_height(pixbuf));

	guchar *src = gdk_pixbuf_get_pixels(pixbuf);
	int srs = gdk_pixbuf_get_rowstride(pixbuf);

	if(compressed_io) {
		uLongf sourcelen = gdk_pixbuf_get_size(pixbuf);
		uLongf destlen = 11*sourcelen/10+12;
		guchar *compressed_data = new guchar[destlen];
		do_transform();
		compress(compressed_data, &destlen, src, sourcelen);
		fprintf(fp, "%ld %d\n", destlen, levels);
		fwrite(compressed_data, destlen, 1, fp);
		delete[] compressed_data;
		g_print("Compression ratio: %f%%\n", 100.0*destlen/sourcelen);
	} else {
		fprintf(fp, "255\n");
		g_print("Writing %d bytes\n", width*height*3);
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				for(int d = 0; d < 3; d++) {
					guchar pixel;
					pixel = src[M2A(
						x,y,width,height,srs,4)+d];
					fputc(pixel, fp);
				}
			}
		}
	}

	fclose(fp);
}

void on_save_cancel_clicked(GtkWidget *w, gpointer data) {
	gtk_widget_hide(savefile_dlg);
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
	xml = glade_xml_new("imwave.glade", (char*)NULL);
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

