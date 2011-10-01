#ifndef GRAPH3D_H
#define GRAPH3D_H

#include "matrix.h"

#define ColorSet (0x87f0)

enum {
	GRAPH3D_OK,
	GRAPH3D_ERR_OPEN,
	GRAPH3D_ERR_INIT,
	GRAPH3D_ERR_SHUTDOWN,
	GRAPH3D_ERR_BIND_CONTROL,
	GRAPH3D_ERR_UNDEF
} Graph3DErrors;

typedef struct {
        int r;
        int g;
        int b;
} Graph3dColor;

static Graph3dColor gr_red = {255, 0, 0};
static Graph3dColor gr_green = {0, 255, 0};
static Graph3dColor gr_blue = {0, 0, 255};
static Graph3dColor gr_black = {0, 0, 0};
static Graph3dColor gr_white = {255, 255, 255};

int start_graph3d();
int end_graph3d();
int graph3d_load_identity();
int graph3d_translate(float x, float y, float z);
int graph3d_scale(float x, float y, float z);
int graph3d_rotate(float a, float x, float y, float z);
int graph3d_modelview_set(pMatrix a);

int graph3d_set_depth(float d);
int graph3d_set_height(float h);

int graph3d_set_ambient(Graph3dColor c);
int graph3d_set_diffuse(Graph3dColor c);
int graph3d_set_lightpos(pVector v);

int draw_2d_tri(
        int x1, int y1, Graph3dColor c1,
        int x2, int y2, Graph3dColor c2,
        int x3, int y3, Graph3dColor c3);
int draw_3d_tri(
        pVector v1p, Graph3dColor c1p, pVector n1p,
        pVector v2p, Graph3dColor c2p, pVector n2p,
        pVector v3p, Graph3dColor c3p, pVector n3p);
int draw_point(int x, int y, Graph3dColor c);
int graph3d_flush();

#endif

