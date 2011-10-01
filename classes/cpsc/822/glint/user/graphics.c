#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>
#include "graphics.h"
#include "uglint3.h"

GraphicsParams gp;
u_glint_device glint_board;

int start_graphics() {
	// Initialize the device
	if((gp.fd = open("/dev/glint3", O_RDWR)) == 0) return GRAPHICS_ERR_OPEN;
	if(ioctl(gp.fd, VMODE, V_800x600_60Hz) != 0) return GRAPHICS_ERR_INIT;
	if(ioctl(gp.fd, BIND_CONTROL, &glint_board.control_base) != 0)
		return GRAPHICS_ERR_INIT;
	FIFOSYNC;

	// Clear the screen to black
	set_draw_color(0, 0, 0);
	draw_rect(0, 0, 800, 600);

	return GRAPHICS_OK;
}

int end_graphics() {
	if(ioctl(gp.fd, VMODE, V_OFF) != 0) return GRAPHICS_ERR_SHUTDOWN;
	return GRAPHICS_OK;
}

int get_graphics_params(GraphicsParams* p) {
	memcpy(p, &gp, sizeof(gp));
	return GRAPHICS_OK;
}

int set_draw_color(int r, int g, int b) {
	gp.draw_color.r = r << 15;
	gp.draw_color.g = g << 15;
	gp.draw_color.b = b << 15;
	return GRAPHICS_OK;
}

int set_bg_color(int r, int g, int b) {
	gp.bg_color.r = r << 15;
	gp.bg_color.g = g << 15;
	gp.bg_color.b = b << 15;
	return GRAPHICS_OK;
}

int draw_point(int x, int y) {
	return GRAPHICS_OK;
}

int draw_line(int x1, int y1, int x2, int y2) {
	return GRAPHICS_OK;
}

int draw_rect(int x1, int y1, int x2, int y2) {
	// Set up constant color
	LOAD_CONTROL(RStart, gp.draw_color.r);
	LOAD_CONTROL(GStart, gp.draw_color.g);
	LOAD_CONTROL(BStart, gp.draw_color.b);
	// LOAD_CONTROL(AStart, 0);
	LOAD_CONTROL(dRdx, 0);
	LOAD_CONTROL(dGdx, 0);
	LOAD_CONTROL(dBdx, 0);
	// LOAD_CONTROL(dAdx, 0);
	LOAD_CONTROL(dRdyDom, 0);
	LOAD_CONTROL(dGdyDom, 0);
	LOAD_CONTROL(dBdyDom, 0);
	// LOAD_CONTROL(dAdyDom, 0);

	LOAD_CONTROL(StartXDom, x1 << 16);
	LOAD_CONTROL(StartXSub, x2 << 16);
	LOAD_CONTROL(dXDom, 0);
	LOAD_CONTROL(dXSub, 0);
	LOAD_CONTROL(Count, y2 - y1);
	LOAD_CONTROL(YStart, y1 << 16);
	LOAD_CONTROL(dY, 1 << 16);
	LOAD_CONTROL(Render, RENDER_TRAPEZOID_SPC);

	return GRAPHICS_OK;
}

// Note that the coordinates MUST be specified in ascending y order!
int draw_tri(int x1, int y1, int x2, int y2, int x3, int y3) {
	draw_color_tri(
		x1, y1, gp.draw_color,
		x2, y2, gp.draw_color,
		x3, y3, gp.draw_color);
	return GRAPHICS_OK;
}

int draw_color_tri(
	int x1, int y1, GraphicsColor c1,
	int x2, int y2, GraphicsColor c2,
	int x3, int y3, GraphicsColor c3) {

	int c;
	int drdy, dgdy, dbdy;
	int drdx, dgdx, dbdx;

	c1.r <<= 15; c2.r <<= 15; c3.r <<= 15;
	c1.g <<= 15; c2.g <<= 15; c3.g <<= 15;
	c1.b <<= 15; c2.b <<= 15; c3.b <<= 15;

	c = abs((x1 - x3)*(y2-y3) - (x2-x3)*(y1-y3));
	drdy = (c3.r-c1.r)/(y3-y1);
	dgdy = (c3.g-c1.g)/(y3-y1);
	dbdy = (c3.b-c1.b)/(y3-y1);

	// Without these casts, we can run into overflow problems
	drdx = ((long long)(c1.r - c3.r)*(y2-y3)/c) -
	       ((long long)(c2.r-c3.r)*(y1-y3)/c);
	dgdx = ((long long)(c1.g - c3.g)*(y2-y3)/c) -
	       ((long long)(c2.g-c3.g)*(y1-y3)/c);
	dbdx = ((long long)(c1.b - c3.b)*(y2-y3)/c) -
	       ((long long)(c2.b-c3.b)*(y1-y3)/c);

	// Set up color
	printf("%d\n%d %d %d\n", c, c1.r, c1.g, c1.b);
	printf("%lld %lld %lld\n%d %d %d\n", drdx, dgdx, dbdx, drdy, dgdy, dbdy);
	LOAD_CONTROL(RStart, c1.r);
	LOAD_CONTROL(GStart, c1.g);
	LOAD_CONTROL(BStart, c1.b);
	LOAD_CONTROL(dRdyDom, drdy);
	LOAD_CONTROL(dGdyDom, dgdy);
	LOAD_CONTROL(dBdyDom, dbdy);
	LOAD_CONTROL(dRdx, drdx);
	LOAD_CONTROL(dGdx, dgdx);
	LOAD_CONTROL(dBdx, dbdx);

	// Set up the top half of the triangle
	LOAD_CONTROL(StartXDom, x1 << 16);
	LOAD_CONTROL(dXDom, ((x3 - x1)<<16) / (y3 - y1));
	LOAD_CONTROL(StartXSub, x1 << 16);
	LOAD_CONTROL(dXSub, ((x2 - x1)<<16) / (y2 - y1));
	LOAD_CONTROL(StartY, y1 << 16);
	LOAD_CONTROL(dY, 1 << 16);
	LOAD_CONTROL(Count, abs(y2 - y1));
	LOAD_CONTROL(Render, RENDER_TRAPEZOID_SPC);

	// Draw the second half of the triangle
	LOAD_CONTROL(StartXSub, x2 << 16);
	LOAD_CONTROL(dXSub, ((x3 - x2)<<16) / (y3 - y2));
	LOAD_CONTROL(ContinueNewSub, abs(y3 - y2));

	return GRAPHICS_OK;
}
