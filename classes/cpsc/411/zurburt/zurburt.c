/* "Color Dance" -- based on zurburt example
 * Paul Brannan - September 10, 1999
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_MI 5
#define MI_W 160
#define MI_H 40
#define MI_BSZ 1

#define WIN_HEIGHT 400
#define WIN_WIDTH 500

char *choice[] =
{"Red", "Green", "Blue", "Yellow", "zurburt"};

enum CHOICES {
	CH_RED,
	CH_GREEN,
	CH_BLUE,
	CH_YELLOW,
	CH_ZURBURT
};

int zcolors[CH_ZURBURT];

Display *dpy;
Window rootw, win, menu, mi[NUM_MI];
GC win_gc, menu_gc, mi_gc[NUM_MI];

void flip_video();
int take_action();
void write_menu_label();
void z_fill_window(int);
void init_zcolors(Colormap);
int z_get_color(Colormap, const char *);

int main(int argc, char *argv[]) {
	XSizeHints whisper;
	XEvent report;
	XGCValues values;
	XFontStruct *font;
	XFontStruct *font2;
	unsigned long bp, wp, valuemask;
	int screen, width, height, bsz;
	int i;
	int fancy;
	Colormap cmap;

/*
 * open(NULL) means get display name from environment  variable DISPLAY,
 * set by xterm  
 */
	if ((dpy = XOpenDisplay(NULL)) == NULL) {
		fprintf(stderr, "can't open\n");
		exit(1);
	}
	screen = DefaultScreen(dpy);
	/* XMatchVisualInfo(dpy, screen, depth, class, &vinfo); */
	width = WIN_WIDTH;
	height = WIN_HEIGHT;
	bp = BlackPixel(dpy, screen);
	wp = WhitePixel(dpy, screen);
	rootw = RootWindow(dpy, screen);

/* Initialize the colors */
	cmap = DefaultColormap(dpy, screen);
	init_zcolors(cmap);
/*
 * create output window with black border and white background; upper left 
 * corner will be at (400,100); border size at this level  is overriden by 
 * window manager, so just make it 0 
 */
	bsz = 0;
	win = XCreateSimpleWindow(dpy, rootw, 400, 100, width, height, bsz, bp, wp);
	menu = XCreateSimpleWindow(dpy, rootw, 200, 50, MI_W, MI_H * NUM_MI, bsz, bp, wp);

/*
 * individual menu items will overlay menu window 
 */
	bsz = MI_BSZ;
	for (i = 0; i < NUM_MI; i++) {
		mi[i] = XCreateSimpleWindow(dpy, menu, 0, MI_H * i, MI_W - bsz, MI_H - bsz, bsz, bp, wp);
	}

/*
 * whisper some hints to the window manager; suggest that it use the
 * indicated  position, width, and height for window placement  
 */

	whisper.flags = PSize | PPosition;
	XSetWMNormalHints(dpy, win, &whisper);
	XSetWMNormalHints(dpy, menu, &whisper);

/*
 * indicate which events are important; NOTE: buttonpress in menu (window) 
 * will determine which mi (subwindow); so, don't recognize buttonpress in 
 * subwindow 
 */
	XSelectInput(dpy, win, ExposureMask | KeyPressMask | StructureNotifyMask);
	XSelectInput(dpy, menu, ButtonPressMask | ButtonReleaseMask | KeyPressMask
		     | StructureNotifyMask);
	for (i = 0; i < NUM_MI; i++)
		XSelectInput(dpy, mi[i], ExposureMask);

/*
 * set up graphics contexts and get the fonts ... but if you ask for one
 * that isn't there ... BLAMO! 
 */
	valuemask = 0;
	font = XLoadQueryFont(dpy, "9x15");
	for (i = 0; i < NUM_MI; i++) {
		mi_gc[i] = XCreateGC(dpy, mi[i], valuemask, &values);
		XSetFont(dpy, mi_gc[i], font->fid);
		XSetForeground(dpy, mi_gc[i], bp);
	}
	font2 = XLoadQueryFont(dpy, "10x20");
	win_gc = XCreateGC(dpy, win, valuemask, &values);
	XSetFont(dpy, win_gc, font2->fid);
	XSetForeground(dpy, win_gc, bp);

	XMapWindow(dpy, win);
	XMapWindow(dpy, menu);
	for (i = 0; i < NUM_MI; i++)
		XMapWindow(dpy, mi[i]);

/*
 * wait on expose from main window and expose from menu items; if you try
 * to draw before this, you go down in flames 
 */
	for (i = 0; i <= NUM_MI; i++) {
		XNextEvent(dpy, &report);
		while (report.type != Expose) {
			fprintf(stderr, "do wah\n");
			XNextEvent(dpy, &report);
		}
	}

	for (i = 0; i < NUM_MI; i++)
		write_menu_label(i);

	fancy = 0;
	while (1) {
		XNextEvent(dpy, &report);
		switch (report.type) {
		case Expose:
			for (i = 0; i < NUM_MI; i++)
				write_menu_label(i);
			break;

		case KeyPress:
			XUnloadFont(dpy, font->fid);
			XUnloadFont(dpy, font2->fid);
			XFreeGC(dpy, win_gc);
			for (i = 0; i < NUM_MI; i++)
				XFreeGC(dpy, mi_gc[i]);
			XCloseDisplay(dpy);
			exit(0);

		case ButtonPress:
			for (i = 0; i < NUM_MI; i++) {
				if (report.xbutton.subwindow == mi[i])
					break;
			}
			flip_video(i, wp, bp);
			/*
			 * wait for ham-handed user to let go of the button 
			 */
			XNextEvent(dpy, &report);
			while (report.type != ButtonRelease) {
				XNextEvent(dpy, &report);
			}
			flip_video(i, bp, wp);
			fancy = take_action(i);
			break;

		default:
			break;
		}
		if(fancy) {
			while(XEventsQueued(dpy, QueuedAfterReading) == 0) {
				XSetForeground(dpy, win_gc,
					zcolors[rand() % CH_ZURBURT]);
				XFillRectangle(dpy, win, win_gc,
					rand() % WIN_WIDTH, rand() % WIN_HEIGHT,
					100, 100);
			}
		}
				
	}
}

void flip_video(int item, unsigned long fore, unsigned long back) {
	XSetForeground(dpy, mi_gc[item], fore);
	XSetWindowBackground(dpy, mi[item], back);
	XClearWindow(dpy, mi[item]);
	write_menu_label(item);
}

void write_menu_label(int i) {
	XDrawString(dpy, mi[i], mi_gc[i], 2 * MI_BSZ, MI_H / 2, choice[i], strlen(choice[i]));
}

/*
 * trivial action 
 */
int take_action(int i) {
	switch(i) {
	case CH_RED:
	case CH_GREEN:
	case CH_BLUE:
	case CH_YELLOW:
		z_fill_window(i);
		break;
	case CH_ZURBURT:
		return 1;
		break;
	}
	return 0;
}

void z_fill_window(int color) {
	XSetWindowBackground(dpy, win, zcolors[color]);
	XClearWindow(dpy, win);
}

void init_zcolors(Colormap cmap) {
	zcolors[CH_RED] = z_get_color(cmap, "red");
	zcolors[CH_GREEN] = z_get_color(cmap, "green");
	zcolors[CH_BLUE] = z_get_color(cmap, "blue");
	zcolors[CH_YELLOW] = z_get_color(cmap, "yellow");
}

int z_get_color(Colormap cmap, const char *s) {
	XColor color;
	if(!XAllocNamedColor(dpy, cmap, s, &color, &color))
		fprintf(stderr, "Unable to get color entry for %s", s);
	return color.pixel;
}
