#ifndef OPTIONS_H
#define OPTIONS_H

#include <gtk/gtk.h>

enum PolygonMode {
	WIREFRAME,
	FLAT,
	SMOOTH
};

extern volatile int axes_on;
extern volatile int polygon_mode;
extern volatile int hidden_surface_removal;
extern volatile int opengl_hidden_surface_removal;
extern volatile int vertex_normals_on;
extern volatile int face_normals_on;
extern volatile int lighting_on;

#endif
