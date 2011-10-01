#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>
#include "graph3d.h"
#include "uglint3.h"
#include "matrix.h"

#define WRITE_DMA(r,v) *pbuf++ = maketag(r); *pbuf++ = v

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

static int fd;
static int version;
static int dma_num;
static u_glint_device glint_board;
static dma_start_t dma_data;
static uint32_t *pbuf;

// 3D data globals
static Matrix modelview;
static Vector ambient;
static Vector diffuse;
static Vector lightpos;
static float depth, height;

static Matrix identity_matrix =
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

int flush_dma();
float light_transform(float c, pVector n, pVector L, int j);
int draw_dma_tri(
        int x1, int y1, Graph3dColor c1,
        int x2, int y2, Graph3dColor c2,
        int x3, int y3, Graph3dColor c3);
int draw_dma_rect(
        int x1, int y1, Graph3dColor c1,
        int x2, int y2, Graph3dColor c2);


// -------------------------------------------------------------------------
// Init/shutdown functions
// -------------------------------------------------------------------------

int start_graph3d() {
	int errcode;

	// Initialize the device
	if((fd = open("/dev/glint3", O_RDWR)) == 0) return GRAPH3D_ERR_OPEN;
	if(ioctl(fd, VMODE, V_800x600_60Hz) != 0) return GRAPH3D_ERR_INIT;

	if(ioctl(fd, VERSION, &version) != 0) version = 1;
	printf("version: %d\n", version);

	if(version == 1) {
		if(ioctl(fd, BIND_CONTROL, &glint_board.control_base) != 0)
			return GRAPH3D_ERR_INIT;
		if(ioctl(fd, BIND_DMA, glint_board.dmagob) != 0)
			return GRAPH3D_ERR_INIT;
		pbuf = dma_data.virt_base = glint_board.dmagob[0].virt_base;
		dma_num = 0;
	} else {
		if(ioctl(fd, BIND_DMA, &glint_board.dmagob[0].virt_base) != 0)
			return GRAPH3D_ERR_INIT;
		pbuf = dma_data.virt_base = glint_board.dmagob[0].virt_base;
	}

	// Clear the screen
	draw_dma_rect(0, 0, gr_black, 800, 600, gr_black);
	flush_dma();

	return GRAPH3D_OK;
}

int end_graph3d() {
        if(ioctl(fd, VMODE, V_OFF) != 0) return GRAPH3D_ERR_SHUTDOWN;
	close(fd);
        return GRAPH3D_OK;
}

// -------------------------------------------------------------------------
// Modelview matrix manipulation functions
// -------------------------------------------------------------------------

int graph3d_load_identity() {
	mm_cpy(modelview, identity_matrix);
	return GRAPH3D_OK;
}

int graph3d_translate(float x, float y, float z) {
	modelview[12] += x;
	modelview[13] += y;
	modelview[14] += z;
	return GRAPH3D_OK;
}

int graph3d_scale(float x, float y, float z) {
	modelview[0] *= x;
	modelview[5] *= y;
	modelview[10] *= z;
	return GRAPH3D_OK;
}

int graph3d_rotate(float a, float x, float y, float z) {
	Matrix rotation_matrix;
	float cosa = cos(a);
	float sina = sin(a);
	float norm = sqrt(x*x + y*y + z*z);
	float xp = x / norm;
	float yp = y / norm;
	float zp = z / norm;

	rotation_matrix[0] = xp*xp + cosa*(1.0-xp*xp) + sina*0;
	rotation_matrix[1] = xp*yp + cosa*(-xp*yp) + sina*zp;
	rotation_matrix[2] = xp*zp + cosa*(-xp*zp) + sina*-yp;
	rotation_matrix[3] = 0;
	rotation_matrix[4] = xp*yp + cosa*(-xp*yp) + sina*-zp;
	rotation_matrix[5] = yp*yp + cosa*(1.0-yp*yp) + sina*0;
	rotation_matrix[6] = yp*zp + cosa*(-yp*zp) + sina*xp;
	rotation_matrix[7] = 0;
	rotation_matrix[8] = xp*zp + cosa*(-xp*zp) + sina*yp;
	rotation_matrix[9] = yp*zp + cosa*(-yp*zp) + sina*-xp;
	rotation_matrix[10] = zp*zp + cosa*(1.0-zp*zp) + sina*0;
	rotation_matrix[11] = 0;
	rotation_matrix[12] = 0;
	rotation_matrix[13] = 0;
	rotation_matrix[14] = 0;
	rotation_matrix[15] = 1;

	mm_mul(modelview, rotation_matrix);
	return GRAPH3D_OK;
}

int graph3d_modelview_set(pMatrix a) {
	mm_cpy(modelview, a);
	return GRAPH3D_OK;
}

// -------------------------------------------------------------------------
// Perspective functions
// -------------------------------------------------------------------------

int graph3d_set_depth(float d) {
	depth = d;
	return GRAPH3D_OK;
}

int graph3d_set_height(float h) {
	height = h;
	return GRAPH3D_OK;
}

// -------------------------------------------------------------------------
// Lighting functions
// -------------------------------------------------------------------------

int graph3d_set_ambient(Graph3dColor c) {
	ambient[0] = (float)c.r / 255.0;
	ambient[1] = (float)c.g / 255.0;
	ambient[2] = (float)c.b / 255.0;
	ambient[3] = 1.0;

	return GRAPH3D_OK;
}

int graph3d_set_diffuse(Graph3dColor c) {
	diffuse[0] = (float)c.r / 255.0;
	diffuse[1] = (float)c.g / 255.0;
	diffuse[2] = (float)c.b / 255.0;
	diffuse[3] = 1.0;

	return GRAPH3D_OK;
}

int graph3d_set_lightpos(pVector v) {
	vv_cpy(lightpos, v);
	return GRAPH3D_OK;
}

// We make a LOT of assumptions about the lighting here!
float light_transform(float c, pVector n, pVector L, int j) {
	return c*ambient[j] + MAX(vv_dot(L, n), 0)*c*diffuse[j];
}

// -------------------------------------------------------------------------
// Drawing functions
// -------------------------------------------------------------------------

int draw_2d_tri(
	int x1, int y1, Graph3dColor c1,
	int x2, int y2, Graph3dColor c2,
	int x3, int y3, Graph3dColor c3) {

	// We will wrap the dma_tri func, as it does no error checking.
	if(y3 == y1 || y2 == y1 || y2 == y3)  {
		// What to do in this case?
/*		draw_dma_rect(
			MIN(MIN(x1, x2), x3), y1, c1,
			MAX(MAX(x1, x2), x3), y2, c2);*/
		return GRAPH3D_OK;
	}
	if(y1 < y2) {
		if(y2 < y3) {
			draw_dma_tri(x1,y1,c1,x2,y2,c2,x3,y3,c3);
		} else {
			if(y1 < y3) {
				draw_dma_tri(x1,y1,c1,x3,y3,c3,x2,y2,c2);
			} else {
				draw_dma_tri(x3,y3,c3,x1,y1,c1,x2,y2,c2);
			}
		}
	} else {
		if(y2 < y3) {
			if(y1 < y3) {
				draw_dma_tri(x2,y2,c2,x1,y1,c1,x3,y3,c3);
			} else {
				draw_dma_tri(x2,y2,c2,x3,y3,c3,x1,y1,c1);
			}
		} else {
			draw_dma_tri(x3,y3,c3,x2,y2,c2,x1,y1,c1);
		}
	}
}

int draw_dma_tri(
	int x1, int y1, Graph3dColor c1,
	int x2, int y2, Graph3dColor c2,
	int x3, int y3, Graph3dColor c3) {

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
        WRITE_DMA(RStart, c1.r);
        WRITE_DMA(GStart, c1.g);
        WRITE_DMA(BStart, c1.b);
        WRITE_DMA(dRdyDom, drdy);
        WRITE_DMA(dGdyDom, dgdy);
        WRITE_DMA(dBdyDom, dbdy);
        WRITE_DMA(dRdx, drdx);
        WRITE_DMA(dGdx, dgdx);
        WRITE_DMA(dBdx, dbdx);

        // Set up the top half of the triangle
        WRITE_DMA(StartXDom, x1 << 16);
        WRITE_DMA(dXDom, ((x3 - x1)<<16) / (y3 - y1));
        WRITE_DMA(StartXSub, x1 << 16);
        WRITE_DMA(dXSub, ((x2 - x1)<<16) / (y2 - y1));
        WRITE_DMA(StartY, y1 << 16);
        WRITE_DMA(dY, 1 << 16);
        WRITE_DMA(Count, abs(y2 - y1));
        WRITE_DMA(Render, RENDER_TRAPEZOID_SPC);

        // Draw the second half of the triangle
        WRITE_DMA(StartXSub, x2 << 16);
        WRITE_DMA(dXSub, ((x3 - x2)<<16) / (y3 - y2));
        WRITE_DMA(ContinueNewSub, abs(y3 - y2));

	return GRAPH3D_OK;
}

int flush_dma() {
	dma_data.count = pbuf - dma_data.virt_base;

	if(version == 1) {
		// Start the DMA transfer
		while(EVAL_CONTROL(DMACount) != 0) ;
		LOAD_CONTROL(DMAAddress, glint_board.dmagob[dma_num].phys_base);
		LOAD_CONTROL(DMACount, dma_data.count);

		// Switch to the other DMA buffer
		dma_num = (dma_num == 0)?1:0;
		pbuf = dma_data.virt_base =
			glint_board.dmagob[dma_num].virt_base;
	} else {
		// Start the DMA transfer and get the new virtual base
		ioctl(fd, START_DMA, &dma_data);
		pbuf = dma_data.virt_base;
	}
	return GRAPH3D_OK;
}

int graph3d_flush() {
	// We could do more stuff here later
	return flush_dma();
}

int draw_3d_tri(
	pVector v1p, Graph3dColor c1p, pVector n1p,
	pVector v2p, Graph3dColor c2p, pVector n2p,
	pVector v3p, Graph3dColor c3p, pVector n3p) {

	int j;
	int x1, x2, x3;
	int y1, y2, y3;

	// First, convert the colors to floats
	Vector c1 = {
		(float)c1p.r / 255.0,
		(float)c1p.g / 255.0,
		(float)c1p.b / 255.0,
		1.0
	};
	Vector c2 = {
		(float)c2p.r / 255.0,
		(float)c2p.g / 255.0,
		(float)c2p.b / 255.0,
		1.0
	};
	Vector c3 = {
		(float)c3p.r / 255.0,
		(float)c3p.g / 255.0,
		(float)c3p.b / 255.0,
		1.0
	};

	Vector v1, v2, v3;
	Vector n1, n2, n3;
	Vector L1, L2, L3;

	// Next, apply the modelview matrix to the triangle's vertices
	vv_cpy(v1, v1p); vv_cpy(v2, v2p); vv_cpy(v3, v3p);
	vm_mul(v1, modelview);
	vm_mul(v2, modelview);
	vm_mul(v3, modelview);

	// And also to the normals
	vv_cpy(n1, n1p); vv_cpy(n2, n2p); vv_cpy(n3, n3p);
	vm_mul(n1, modelview);
	vm_mul(n2, modelview);
	vm_mul(n3, modelview);

	// Find the distance for each point from the light
	vv_cpy(L1, lightpos); vv_sub(L1, v1); v_norm(L1);
	vv_cpy(L2, lightpos); vv_sub(L2, v2); v_norm(L2);
	vv_cpy(L3, lightpos); vv_sub(L3, v3); v_norm(L3);

	// Then, apply our lighting equations
	// We assume that the triangle's ambient and diffuse values are the same
	// We also assume that 
	for(j = 0; j < 4; j++) {
		c1[j] = light_transform(c1[j], n1, L1, j);
		c2[j] = light_transform(c2[j], n2, L2, j);
		c2[j] = light_transform(c3[j], n3, L3, j);
	}

	// Convert the color values to values the graphics card will accept
	// c1p et al were passed by value, so we can modify them
	c1p.r = c1[0] * 255;
	c1p.g = c1[1] * 255;
	c1p.b = c1[2] * 255;
	c2p.r = c2[0] * 255;
	c2p.g = c2[1] * 255;
	c2p.b = c2[2] * 255;
	c3p.r = c3[0] * 255;
	c3p.g = c3[1] * 255;
	c3p.b = c3[2] * 255;

	// Lastly, find the coordinates on the screen for each point,
	// and draw the triangle
	x1 = 800 * (depth/height) * (v1[0] / v1[2]);
	y1 = 600 * (depth/height) * (v1[1] / v1[2]);
	x2 = 800 * (depth/height) * (v2[0] / v2[2]);
	y2 = 600 * (depth/height) * (v2[1] / v2[2]);
	x3 = 800 * (depth/height) * (v3[0] / v3[2]);
	y3 = 600 * (depth/height) * (v3[1] / v3[2]);
	draw_2d_tri(x1, y1, c1p, x2, y2, c2p, x3, y3, c3p);

	printf("v1: %f %f %f\n", v1[0], v1[1], v1[2]);
	printf("v2: %f %f %f\n", v2[0], v2[1], v2[2]);
	printf("v2: %f %f %f\n", v3[0], v3[1], v3[2]);
	printf("%d %d; %d %d; %d %d\n", x1, y1, x2, y2, x3, y3);

	return GRAPH3D_OK;
}

int draw_dma_rect(
	int x1, int y1, Graph3dColor c1,
	int x2, int y2, Graph3dColor c2) {

	WRITE_DMA(RStart, c1.r << 15);
        WRITE_DMA(GStart, c1.g << 15);
        WRITE_DMA(BStart, c1.b << 15);
        WRITE_DMA(dRdx, (c1.r + c2.r * (x2 - x1)) << 15);
        WRITE_DMA(dGdx, (c1.g + c2.g * (x2 - x1)) << 15);
        WRITE_DMA(dBdx, (c1.b + c2.b * (x2 - x1)) << 15);
        WRITE_DMA(dRdyDom, (c1.r + c2.r * (y2 - y1)) << 15);
        WRITE_DMA(dGdyDom, (c1.r + c2.r * (y2 - y1)) << 15);
        WRITE_DMA(dBdyDom, (c1.r + c2.r * (y2 - y1)) << 15);

        WRITE_DMA(StartXDom, x1 << 16);
        WRITE_DMA(StartXSub, x2 << 16);
        WRITE_DMA(dXDom, 0);
        WRITE_DMA(dXSub, 0);
        WRITE_DMA(Count, y2 - y1);
        WRITE_DMA(YStart, y1 << 16);
        WRITE_DMA(dY, 1 << 16);
        WRITE_DMA(Render, RENDER_TRAPEZOID_SPC);

        return GRAPH3D_OK;
}

int draw_point(int x, int y, Graph3dColor c) {
    WRITE_DMA(ColorDDAMode, 0);
    WRITE_DMA(ColorSet, c.b << 16 | c.g << 8 | c.r);
    WRITE_DMA(StartXDom, x << 16);
    WRITE_DMA(YStart, y << 16);
    WRITE_DMA(Render, RENDER_POINT);
    WRITE_DMA(ColorDDAMode, 3);
 
    return GRAPH3D_OK;
  /*
  return draw_dma_rect(x, y, c, x, y, c);
  */
}

