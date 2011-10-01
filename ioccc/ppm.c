/* ppm.c -- load a ppm file as a texture */

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include "ppm.h"

void loadppm(const char *filename, int texnum) {
	FILE *fp;
	char buf[512];
	char *parse, *s;
	int max_color, im_size, im_width, im_height;
	unsigned char *texture_bytes;

	fp = fopen(filename, "rb");
	if (fp == 0) {
#ifdef WIN32
		s = malloc(64000);
		sprintf(s, "The .ppm file %s could not be opened.", filename);
		MessageBox(NULL, s, "Error", MB_OK);
		free(s);
#else
	  fprintf(stderr, "The .ppm file %s cannot be opened.\n", filename);
#endif
	  exit(1);
	}

	/* Ignore the comments */
	fgets(buf, sizeof(buf), fp);
	do {
		fgets(buf, sizeof(buf), fp);
	} while(buf[0] == '#');

	for(parse = s = buf; *parse != ' '; parse++) ;
	*parse = 0;
	im_width = atoi(s);

	parse++;
	for(s = parse; *parse != '\n'; parse++) ;
	*parse = 0;
	im_height = atoi(s);

	fgets(buf, sizeof(buf), fp);
	max_color = atoi(buf);

	im_size = im_width * im_height;

	texture_bytes = (unsigned char *)calloc(3, im_size);
	fread(texture_bytes, 1, 3*im_size, fp);
	fclose(fp);

#ifdef WIN32
	glBindTexture(GL_TEXTURE_2D, texnum);
#else
	glBindTextureEXT(GL_TEXTURE_2D, texnum);
#endif
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, im_width, im_height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, texture_bytes);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	/* why is GL_MODULATE faster than GL_DECAL? */

	free(texture_bytes);
}
