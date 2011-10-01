#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcx.h"

#ifndef byte
#define byte unsigned char
#endif

/* We don't want to have problems with alignment in the structs */
#pragma pack(1)

typedef struct {
	byte				r;
	byte				g;
	byte				b;
} rgb;

typedef struct {
	byte				manufacturer;
	byte				version;
	byte				encoding;
	byte				bpp;				/* Bits per pixel */

	/* We separate low and high bytes for the little-endian platforms */
	byte				xminl, xminh;
	byte				yminl, yminh;
	byte				xmaxl, xmaxh;
	byte				ymaxl, ymaxh;

	byte				hresh, hresl;
	byte				vresh, vresl;

	rgb					colormap[16];
	byte				reserved;
	byte				numplanes;
	byte				bpll, bplh;			/* Bytes per line */
	byte				palette_infol, palette_infoh;

	byte				filler[58];
} pcx_header;

int load_pcx(pcx_byte **buffer, const char *filename, int* width, int* height) {
	FILE				*fp;
	static pcx_header	image;
	int					image_width, image_height;
	int					i, j;
	byte				*image_buffer;
	static rgb			palette[256];
	pcx_byte *buf;

	if((fp = fopen(filename, "rb")) == NULL) 
		return PCXERR_OPEN;

	/* Get the header and do some sanity checking */
	fread(&image, 128, 1, fp);
	if(image.manufacturer != 1 && image.manufacturer != 10) return PCXERR_MAGIC;
	if(image.encoding != 1) return PCXERR_MAGIC;
	if(image.numplanes != 1) return PCXERR_PLANES;
	if(image.bpp != 8) return PCXERR_COLORS;

	image_width = (image.xmaxh*256 + image.xmaxl) - (image.xminh*256 + image.xminl) + 1;
	image_height = (image.ymaxh*256 + image.ymaxl) - (image.yminh*256 + image.yminl) + 1;
	*width = image_width;
	*height = image_height;
	buf = *buffer = malloc(3 *image_width * image_height);

	image_buffer = (byte *)malloc(image_width * image_height + 256);
	if(image_buffer == NULL) {
		fclose(fp);
		return PCXERR_MALLOC;
	}
	i = 0;
	
	/* Decode the pcx image */
	while(i < (image_width * image_height)) {
		/* Char-at-a-time is probably bad for speed, but it works */
		int c = fgetc(fp);

		if(c >= 0xc0) {
			int repeat = c & ~0xc0;
			c = fgetc(fp);
			for(j = 0; j < repeat; j++) {
				assert(i < image_width * image_height + 256);
				image_buffer[i++] = c;
			}
		} else {
			assert(i < image_width * image_height);
			image_buffer[i++] = c;
		}
	}

	/* Retrieve the palette from the image */
	j = fseek(fp, -768, SEEK_END);
	assert(j == 0);
	j = fread(palette, 768, 1, fp);
	assert(j == 1);

	/* Now rescale the image and apply the palette */
	/* Note that 24-bit images are not yet supported */
	/* This will produce weird results if we scale the textures up */
	for (j = 0; j < image_height; j++) {
		for (i = 0; i < image_width; i++) {
			rgb pal;
			assert(3*(j * image_width + i) + 2 < 3 * image_width * image_height);
			assert(j * image_width + i < image_width * image_height);
			assert(image_buffer[j * image_width + i] < 256);

			memcpy(&pal, &palette[image_buffer[j * image_width + i]], 3);
			if(PCX_TEXRES == GL_RGB) {
				buf[3*(j * image_width + i) + 0] =
					pal.r;
				buf[3*(j * image_width + i) + 1] =
					pal.g;
				buf[3*(j * image_width + i) + 2] =
					pal.b;
			} else if(PCX_TEXRES == GL_RGB16) {
				buf[2*(j * image_width + i)] =
					(pal.r >> 2) << 10 |
					(pal.g >> 2) << 5 |
					(pal.b >> 3);
			}
		}
	}

	fclose(fp);
	free(image_buffer);
	return PCX_OK;
}

int bind_pcx(const char *filename, int image_width, int image_height, int texnum) {
	int file_width, file_height;
	byte *texture;
	int retcode;

	if(texnum == 0) glGenTextures(1, (GLuint*)&texnum);

	if((retcode = load_pcx(&texture, filename, &file_width, &file_height)) == PCX_OK) {

/*		gluScaleImage(PCX_TEXRES, file_width, file_height,
		              PCX_TEXRES, texture, image_width,
		              image_height, PCX_TEXRES, texture);*/
		
		/* Put the texture in memory */
#ifdef WIN32
		glBindTexture(GL_TEXTURE_2D, texnum);
#else
		glBindTextureEXT(GL_TEXTURE_2D, texnum);
#endif

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, image_width,
		             image_height, 0, PCX_TEXRES,
		             GL_UNSIGNED_BYTE, texture);
		glTexParameterf(GL_TEXTURE_2D,
		                GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,
		                GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		          GL_MODULATE);
		/* why is GL_MODULATE faster than GL_DECAL? */
		glEnable(GL_TEXTURE_2D);

	} else {
		switch(retcode) {
		case PCXERR_OPEN:
			printf("PCX Error: could not open file.\n");
			break;
		case PCXERR_MAGIC:
			printf("PCX Error: could not find signature.\n");
			break;
		case PCXERR_PLANES:
			printf("PCX Error: wrong number of planes.\n");
			break;
		case PCXERR_COLORS:
			printf("PCX Error: wrong number of colors.\n");
			break;
		case PCXERR_MALLOC:
			printf("PCX Error: could not allocate memory.\n");	
			break;
		default:
			printf("PCX Error: undefined error.\n");
			break;
		}
		texnum = 0;
	}

	free(texture);
	return texnum;
}
