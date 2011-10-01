#include <GL/gl.h>
#include <gdk/gdk.h>
#include <gdk_imlib.h>
#include "textures.h"

int b2scale(int x) {
	int j;
	for(j = 0; x != 1; x >>= 1) j++;
	j = x << (j+1);
	if(j == 2*x) return x; else return j;
}

int bind_tex(const char* filename, int width, int height, int texnum) {
	/* return bind_tex(filename, width, height, texnum); */
	GdkImlibImage *image, *scaled_image;
	int w, h;

	gdk_imlib_init();
	image = gdk_imlib_load_image((char *)filename);
	w = b2scale(image->rgb_width);
	h = b2scale(image->rgb_height);
	/* g_print("%dx%d --> %dx%d\n",
		image->rgb_width, image->rgb_height, w, h); */
	scaled_image = gdk_imlib_clone_scaled_image(image, w, h);
	if(image == NULL) return 0;

	while(texnum == 0) glGenTextures(1, (GLuint*)&texnum);

	glBindTexture(GL_TEXTURE_2D, texnum);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, scaled_image->rgb_data);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gdk_imlib_kill_image(image);
	
	return texnum;
}

int free_tex(int texnum) {
	glDeleteTextures(1, (GLuint*)&texnum);
	return TEX_OK;
}

