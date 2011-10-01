#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <assert.h>
#include <math.h>
#include "md2.h"
#include "pcx.h"
#include "fixnums.h"
#include "texgen.h"

#define NUMVERTEXNORMALS 162
static float avertexnormals[NUMVERTEXNORMALS][3] = {
#include "anorms.h"
};

#define MD2_MAGIC	844121161
#define MD2_VERSION	8

#define MAX_TRIANGLES	4096 
#define MAX_VERTICES	2048 
#define MAX_TEXCOORDS	2048 
#define MAX_FRAMES		512 
#define MAX_SKINS		32 

#ifndef byte
#define byte unsigned char
#endif

/* We don't want to have problems with alignment in the structs */
#pragma pack(1)

typedef struct { 
   int magic;
   int version; 
   int skinWidth; 
   int skinHeight; 
   int frameSize; 
   unsigned int numSkins; 
   unsigned int numVertices; 
   unsigned int numTexCoords; 
   unsigned int numTriangles; 
   unsigned int numGlCommands; 
   unsigned int numFrames; 
   int offsetSkins; 
   int offsetTexCoords; 
   int offsetTriangles; 
   int offsetFrames; 
   int offsetGlCommands; 
   int offsetEnd; 
} model_t;

typedef struct {
   byte vertex[3];
   byte lightNormalIndex;
} triangleVertex_t;

typedef struct {
   float scale[3];
   float translate[3];
   char name[16];
   triangleVertex_t vertices[1];
} frame_t;

typedef struct {
   short vertexIndices[3];
   short textureIndices[3];
} triangle_t;

typedef struct {
   short s, t;
} textureCoordinate_t;

typedef struct {
   float s, t;
   unsigned int vertexIndex;
} glCommandVertex_t;

int parse_md2(const char *file, const char *skin, md2_model_t *md2, int tt) {
	FILE *fp;
	int retcode;

	/* Open the file */
	if((fp = fopen(file, "rb")) == NULL)
		return MD2ERR_OPEN;
	
	retcode = parse_md2_fp(fp, skin, md2, tt);
	fclose(fp);
	return retcode;
}

int parse_md2_fp(FILE *fp, const char *skin, md2_model_t *md2, int tt) {
	static model_t				model;
	static glCommandVertex_t	glCommandVertex[MAX_VERTICES];
	static frame_t				frames[MAX_FRAMES];
	static char					skin_filename[64];
	int							texnum;
	int							framesize, i, j, n;
	int							retcode = MD2_OK;

	/* Get the header */
	fread(&model, sizeof(model_t), 1, fp);

	/* Fix the endianness of the header */
	model.magic = fixint(model.magic);
	model.version = fixint(model.version);
	model.skinWidth = fixint(model.skinWidth);
	model.skinHeight = fixint(model.skinHeight);
	model.frameSize = fixint(model.frameSize);
	model.numSkins = fixint(model.numSkins);
	model.numVertices = fixint(model.numVertices);
	model.numTexCoords = fixint(model.numTexCoords);
	model.numTriangles = fixint(model.numTriangles);
	model.numGlCommands = fixint(model.numGlCommands);
	model.numFrames = fixint(model.numFrames);
	model.offsetSkins = fixint(model.offsetSkins);
	model.offsetTexCoords = fixint(model.offsetTexCoords);
	model.offsetTriangles = fixint(model.offsetTriangles);
	model.offsetFrames = fixint(model.offsetFrames);
	model.offsetGlCommands = fixint(model.offsetGlCommands);
	model.offsetEnd = fixint(model.offsetEnd);

	/* Do some sanity checking */
	if(model.magic != 0x32504449) return MD2ERR_MAGIC;
	if(model.version != MD2_VERSION)
		return MD2ERR_VERSION;
	if(model.numSkins > MAX_SKINS ||
	   model.numVertices > MAX_VERTICES ||
	   model.numTexCoords > MAX_TEXCOORDS ||
	   model.numTriangles > MAX_TRIANGLES ||
	   model.numFrames > MAX_FRAMES) {
		printf("Range Error!  Skins: %d, Vertices: %d,"
			"TexCoords: %d, Triangles: %d, GlCommands: %d"
			"Frames: %d\n",
			model.numSkins, model.numVertices,
			model.numTexCoords, model.numTriangles,
			model.numGlCommands, model.numFrames);
		return MD2ERR_RANGE;
	}

	/* Read the first skin */
	/* I'm not yet sure how to use multiple skins */
	if(*skin == 0 && *skin_filename != 0) {
		fseek(fp, model.offsetSkins, SEEK_SET);
		fread(skin_filename, sizeof(skin_filename), 1, fp);
		/* Load the texture into memory, if necessary */
		texnum = bind_pcx(skin_filename, model.skinWidth, model.skinHeight, 0);
		md2->skin = texnum;
	} else if (*skin != 0) {
		texnum = bind_pcx(skin, model.skinWidth, model.skinHeight, 0);
		md2->skin = texnum;
	}

	if((md2->framestart = glGenLists(model.numFrames)) == 0)
		return MD2ERR_DISPLAYLIST;
	md2->numframes = model.numFrames;

	/* Read the first frame */
	/* We should read this into an array of frames, optimally */
	framesize = model.frameSize * model.numFrames;
	fseek(fp, model.offsetFrames, SEEK_SET);
	fread(&frames[0], framesize, 1, fp);

	/* Draw the first frame */
	i = 0;
	fseek(fp, model.offsetGlCommands, SEEK_SET);

	for(i = 0; i < md2->numframes; i++) {
		for(j = 0; j < 3; j++) {
			frames[i].scale[j] = fixfloat(frames[i].scale[j]);
			frames[i].translate[j] = fixfloat(frames[i].translate[j]);
		}

		/* Now create the display list */
		glNewList(md2->framestart + i, GL_COMPILE);

		/* Start by binding the texture */
		if(tt == TEX_STD) glBindTexture(GL_TEXTURE_2D, texnum);

		do {
			int loc;
			glCommandVertex_t *glvertex;
			triangleVertex_t *tvertex;
			float vertex[3];
			
			/* Read the number of GL Commands we are to process */
			fread(&n, sizeof(int), 1, fp);
			n = fixint(n);
			
			if(n > 0) {
				glBegin(GL_TRIANGLE_STRIP);
			} else {
				glBegin(GL_TRIANGLE_FAN);
				n = -n;
			}
			if(n > MAX_VERTICES) {
				n = MAX_VERTICES;
				retcode = MD2ERR_RANGE;
				printf("Range Error: Reading too many GL commands\n");
			}
			
			loc=ftell(fp);
			fread(glCommandVertex, n * sizeof(glCommandVertex_t), 1, fp);

			for(j = 0; j < n; j++) {
				glvertex = &glCommandVertex[j];
				glvertex->s = fixfloat(glvertex->s);
				glvertex->t = fixfloat(glvertex->t);
				glvertex->vertexIndex = fixint(glvertex->vertexIndex);

				if(glvertex->vertexIndex >= model.numVertices) {
					retcode = MD2ERR_VERTEX;
					continue;
				}
				
				tvertex = &frames[i].vertices[glvertex->
					vertexIndex];
				if(tvertex->lightNormalIndex >= NUMVERTEXNORMALS) {
					retcode = MD2ERR_NORMALS;
					continue;
				}
				
				glNormal3fv(avertexnormals[tvertex->lightNormalIndex]);

				/* Do the texture coords differently */
				do_tex_coords(glvertex->s, glvertex->t,
					tvertex->vertex[0] * frames[i].scale[0],
					tvertex->vertex[1] * frames[i].scale[1],
					tvertex->vertex[2] * frames[i].scale[2],
					tt);
				
				vertex[0] = tvertex->vertex[0] * frames[i].scale[0]
				            + frames[i].translate[0];
				vertex[1] = tvertex->vertex[1] * frames[i].scale[1]
				            + frames[i].translate[1];
				vertex[2] = tvertex->vertex[2] * frames[i].scale[2]
				            + frames[i].translate[2];
				glVertex3fv(vertex);
			}
			
			glEnd();
		} while(n != 0);

		glEndList();
	}

	return retcode;
}
