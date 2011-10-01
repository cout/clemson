#ifndef GRAPHICS_H
#define GRAPHICS_H

enum {
	GRAPHICS_OK,
	GRAPHICS_ERR_OPEN,
	GRAPHICS_ERR_INIT,
	GRAPHICS_ERR_SHUTDOWN,
	GRAPHICS_ERR_UNDEF
} GraphicsErrors;

typedef struct {
	void* framebuf;
	int width, height;
	int draw_color;
	int bg_color;
	int fd;
} GraphicsParams;

int start_graphics();
int end_graphics();

int get_graphics_params(GraphicsParams* p);

int set_draw_color(int c);
int set_bg_color(int c);

int draw_point(int x, int y);
int draw_line(int x1, int y1, int x2, int y2);
int draw_tri(int x1, int y1, int x2, int y2, int x3, int y3);

#endif

