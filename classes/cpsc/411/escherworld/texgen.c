#include <math.h>
#include <GL/glut.h>
#include "texgen.h"
#include "ew.h"
#include "pcx.h"

#define stripeImageWidth 8

static GLuint contour_tex, grid_tex;
GLubyte stripeImage[4*stripeImageWidth];
static GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};
static GLfloat slanted[] = {1.0, 1.0, 1.0, 0.0};

/* From the red book */
void makeStripeImage() {
	int j;
	for(j = 0; j < stripeImageWidth; j++) {
		stripeImage[4*j] = (GLubyte) ((j!=8) ? 255 : 0); /* R */
		stripeImage[4*j+1] = (GLubyte) ((j!=8)?255:0); /* G */
		stripeImage[4*j+2] = (GLubyte) ((j!=8)?255:0); /* B */
		stripeImage[4*j+3] = (GLubyte) 255; /* A */
	}
}

#define _3dgridsize 8
static GLuint _3dgrid_tex;
GLubyte _3dgrid[3*_3dgridsize*_3dgridsize*_3dgridsize];

void make3dgrid() {
	int x,y,z;
	for(x = 0; x < _3dgridsize; x++) {
		for(y = 0; y < _3dgridsize; y++) {
			for(z = 0; z < _3dgridsize; z++) {
				_3dgrid[x+_3dgridsize*(y+z*_3dgridsize)+0] =
				_3dgrid[x+_3dgridsize*(y+z*_3dgridsize)+1] =
				_3dgrid[x+_3dgridsize*(y+z*_3dgridsize)+2] =
					(x==0||y==0||z==0)?0:255;
			}
		}
	}
}

void do_tex_coords(float s, float t, float x, float y, float z, int tt) {
        switch(tt) {
        case TEX_STD:
                glTexCoord2f(s, t);
                break;
        case TEX_GRID:
		if(gridlines) {
	                s = z*8.0;
        	        t = y;
                	glTexCoord2f(s,t);
		}
                break;
        case TEX_DBLGRID:
                /* Not right, yet */
                if(gridlines) glTexCoord2f(s, t);
                break;
        case TEX_NONE:
	default:
		break;
        }
}

void start_tex(int tt) {
	if(!texture) tt = TEX_NONE;
	if(!gridlines && (tt == TEX_CONTOUR || tt == TEX_MAP2 ||
		tt == TEX_GRID3 || tt == TEX_GRID)) tt = TEX_NONE;

	switch(tt) {
	case TEX_CONTOUR:
		/* From the red book */
		glBindTexture(GL_TEXTURE_1D, contour_tex);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, xequalzero);
		glEnable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_1D);
		break;
	case TEX_MAP2:
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_3D_EXT);
		glBindTexture(GL_TEXTURE_2D, grid_tex);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		break;
	case TEX_GRID3:
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_3D_EXT);
		glBindTexture(GL_TEXTURE_3D_EXT, _3dgrid_tex);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
		break;
	case TEX_NONE:
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D_EXT);
		break;
	case TEX_GRID:
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_3D_EXT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, grid_tex);
		break;
	default:
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_3D_EXT);
		glEnable(GL_TEXTURE_2D);
	}
}

void init_texgen() {
	/* From the red book */
	makeStripeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &contour_tex);
	glBindTexture(GL_TEXTURE_1D, contour_tex);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, stripeImageWidth, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, stripeImage);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	make3dgrid();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_3dgrid_tex);
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_RGB, _3dgridsize, _3dgridsize,
		_3dgridsize, 0, GL_RGB, GL_UNSIGNED_BYTE, _3dgrid);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        grid_tex = bind_pcx("grid.pcx", 256, 256, 0);
        printf("Allocated texture %d for grid.\n", grid_tex);
}
