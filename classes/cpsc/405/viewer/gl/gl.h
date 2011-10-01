#ifndef VIEWER_GL_H
#define VIEWER_GL_H

#include <GL/gl.h>
#include <gtkgl/gtkglarea.h>
#include "aw.h"
#include "particle.h"

#define MAX_GL_AREAS 10

GtkWidget* create_glarea (gchar *widget_name, gchar *string1, gchar *string2,
                          gint int1, gint int2);

#define MAX_OBJECTS 16
extern Object aw_object[MAX_OBJECTS];
extern int num_objects;
extern int curr_obj;
extern particle_system ps;

void gl_redraw(int n);
void gl_redraw_all();
void gl_reshape(int width, int height);
void gl_init();
void gl_shutdown();
void draw_object(Object *o);
void do_transformations();

#endif
