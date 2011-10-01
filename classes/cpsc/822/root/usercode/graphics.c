#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include "graphics.h"
#include "uglint3.h"

GraphicsParams gp;

int start_graphics() {
	if((gp.fd = open("/dev/glint3", O_RDWR)) == 0) return GRAPHICS_ERR_OPEN;
	if(ioctl(gp.fd, VMODE, V_800x600_60Hz) != 0) return GRAPHICS_ERR_INIT;
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

int set_draw_color(int c) {
	gp.draw_color = c;
	return GRAPHICS_OK;
}

int set_bg_color(int c) {
	gp.bg_color = c;
	return GRAPHICS_OK;
}

int draw_point(int x, int y) {
	return GRAPHICS_OK;
}

int draw_line(int x1, int y1, int x2, int y2) {
	return GRAPHICS_OK;
}

int draw_tri(int x1, int y1, int x2, int y2, int x3, int y3) {
	return GRAPHICS_OK;
}
