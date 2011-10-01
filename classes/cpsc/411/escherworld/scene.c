#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "scene.h"
#include "crater.h"
#include "_string.h"
#include "box.h"
#include "npr_draw.h"
#include "ew.h"

GLuint crater_tex;
GLuint stars1_tex;
GLuint stars2_tex;
GLuint stars3_tex;

#define RADIUS 420.0  	/* should be slightly larger than RADIUS in crater.c */
#define SLICES 8	/* should be same as SLICES in crater.c */

#define NUM_STARS 10000
static float stars[NUM_STARS][3];

static scene_t scene;
static item_t null_item = {
	/* name, modelfile, skinfile */
	"", "", "",
	/* rotate 1, 2, 3 */
	{0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0},
	/* translate, scale, model */
	{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0, 0, 0},
	/* item_type, params, draw_outline */
	ITEM_NONE, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 0,
	/* tex_type, tex_s, tex_t */
	TEX_NONE, {0.1, 0.1, 0.0}, {0.0, 0.1, 0.1}
};

static char* find_space1(char *s) {
	for(; *s != ' ' && *s != '\t' && *s != 0; s++);
	return s;
}

static char* find_space2(char *s) {
	for(; (*s == ' ' || *s == '\t') && *s != 0; s++);
	return s;
}

static void strip_newline(char *s) {
	for(; *s != 0; s++)
		if(*s == '\r' || *s == '\n') *s = 0;
}

int read_scenedef(const char *filename, scene_t *scene) {
	FILE *fp;
	char s[2048], *t;
	int j, retval;
	item_t *i = &scene->item[0];

	if((fp = fopen(filename, "rt")) == NULL)
		return SDERR_OPEN;

	scene->num_items = 2;
	memcpy(i, &null_item, sizeof(item_t));
	i++;
	memcpy(i, &null_item, sizeof(item_t));

	retval = SD_OK;

	for(;;) {
		fgets(s, sizeof(s), fp);
		if(feof(fp)) break;

		if(s[0] == '#' || s[0] == '\r' || s[0] == '\n') continue;

		strip_newline(s);
		t = find_space1(s);
		if(*t != 0) {
			*t = 0;
			t = find_space2(t+1);
		}
		
		if(!strcmpi(s, "name")) {
			strncpy(i->name, t, sizeof(i->name));
			i->name[sizeof(i->name)-1] = 0;
			printf("Name = %s\n", i->name);
		} else if(!strcmpi(s, "model")) {
			strncpy(i->modelfile, t, sizeof(i->modelfile));
			i->modelfile[sizeof(i->modelfile)-1] = 0;
			i->item_type = ITEM_MD2;
			printf("Model = %s\n", i->modelfile);
		} else if(!strcmpi(s, "skin")) {
			strncpy(i->skinfile, t, sizeof(i->skinfile));
			i->skinfile[sizeof(i->skinfile)-1] = 0;
			printf("Model = %s\n", i->skinfile);
			if(i->tex_type == TEX_NONE) i->tex_type = TEX_STD;
		} else if(!strcmpi(s, "terrain")) {
			i->item_type = ITEM_TERRAIN;
			sscanf(t, "%f %f", &i->params[0], &i->params[1]);
			printf("Terrain: start angle %f, end angle %f\n",
				i->params[0], i->params[1]); 
		} else if(!strcmpi(s, "box")) {
			i->item_type = ITEM_BOX;
			sscanf(t, "%f %f %f", &i->params[0], &i->params[1],
				&i->params[2]);
			printf("Box: %fx%fx%f\n", i->params[0], i->params[1],
				i->params[2]);
		} else if(!strcmpi(s, "global_translate")) {
			sscanf(t, "%f %f %f", &global_translate[0],
				&global_translate[1],
				&global_translate[2]);
			printf("Translating all objects by (%f,%f,%f)\n",
				global_translate[0], 
				global_translate[1],
				global_translate[2]);
		} else if(!strcmpi(s, "translate")) {
			sscanf(t, "%f %f %f", &i->translate[0],
				&i->translate[1], &i->translate[2]);
			printf("Translate by %f %f %f\n",
				i->translate[0], i->translate[1],
				i->translate[2]);
		} else if(!strcmpi(s, "rotate") || !strcmpi(s, "rotate1")) {
			sscanf(t, "%f %f %f %f", &i->rotate1[0],
				&i->rotate1[1], &i->rotate1[2],
				&i->rotate1[3]);
			printf("Rotation 1:  %f degrees about (%f, %f, %f)\n",
				i->rotate1[0], i->rotate1[1], i->rotate1[2],
				i->rotate1[3]);
		} else if(!strcmpi(s, "rotate2")) {
			sscanf(t, "%f %f %f %f", &i->rotate2[0],
				&i->rotate2[1], &i->rotate2[2], &i->rotate2[3]);
			printf("Rotation 2: %f degrees about (%f, %f, %f)\n",
				i->rotate2[0], i->rotate2[1], i->rotate2[2],
				i->rotate2[3]);
		} else if(!strcmpi(s, "rotate3")) {
			sscanf(t, "%f %f %f %f", &i->rotate3[0],
				&i->rotate3[1], &i->rotate3[2], &i->rotate3[3]);
			printf("Rotation 3: %f degrees about (%f, %f, %f)\n",
				i->rotate3[0], i->rotate3[1], i->rotate3[2],
				i->rotate3[3]);
		} else if(!strcmpi(s, "scale")) {
			sscanf(t, "%f %f %f", &i->scale[0], &i->scale[1],
				&i->scale[2]);
			printf("Scaling by (%f,%f,%f)\n",
				i->scale[0], i->scale[1], i->scale[2]);
		} else if(!strcmpi(s, "tex_s")) {
			sscanf(t, "%f %f %f", &i->tex_s[0], &i->tex_s[1],
				&i->tex_s[2]);
			printf("Using texture generation s parameters "
				"(%f,%f,%f)\n",
				i->tex_s[0], i->tex_s[1], i->tex_s[2]);
		} else if(!strcmpi(s, "tex_t")) {
			sscanf(t, "%f %f %f", &i->tex_t[0], &i->tex_t[1],
				&i->tex_t[2]);
			printf("Using texture generation s parameters "
				"(%f,%f,%f)\n",
				i->tex_t[0], i->tex_t[1], i->tex_t[2]);
		} else if(!strcmpi(s, "draw_outline")) {
			printf("Drawing outline for this object.\n");
			i->draw_outline = 1;
		} else if(!strcmpi(s, "tex_type")) {
			if(!strcmpi(t, "std") || !strcmpi(t, "standard")) {
				i->tex_type = TEX_STD;
				printf("Using standard texture coordinates.\n");
			} else if(!strcmpi(t, "grid")) {
				i->tex_type = TEX_GRID;
				printf("Using grid-type textures.\n");
			} else if(!strcmpi(t, "dblgid") ||
				!strcmpi(t, "dbl grid") ||
				!strcmpi(t, "double grid")) {
				i->tex_type = TEX_DBLGRID;
				printf("Using double-grid texture.\n");
			} else if(!strcmpi(t, "contour")) {
				i->tex_type = TEX_CONTOUR;
				printf("Using red book contour texture.\n");
			} else if(!strcmpi(t, "map 2") ||
				!strcmpi(t, "map2")) {
				i->tex_type = TEX_MAP2;
				printf("Using 2D auto texture-generation "
					"for grid.\n");
			} else if(!strcmpi(t, "grid 3") ||
				!strcmpi(t, "grid3")) {
				i->tex_type = TEX_GRID3;
				printf("Using 3D grid texture.\n");	
			} else {
				printf("Unknown texture type %s\n", t);
			}
		} else if(!strcmpi(s, "newitem")) {
			scene->num_items++;
			printf("--------------------------------------\n");
			i++;
			memcpy(i, &null_item, sizeof(item_t));
			if(strcmpi(t, "")) {
				strncpy(i->name, t, sizeof(i->name));
				i->name[sizeof(i->name)-1] = 0;
				printf("Name = %s\n", i->name);
			}
		} else {
			printf("Bad input: %s %s\n", s, t);
			retval = SDERR_BADINPUT;
		}
	}
	printf("--------------------------------------\n");
	printf("Total number of items read: %d\n", scene->num_items);
	printf("\n");

	fclose(fp);
	return retval;
}

/* test to see if the md2 specified in item j has been loaded */
static int is_loaded(int j) {
	int i;
	for(i = 0; i < j; i++) {
		if(scene.item[i].item_type == ITEM_MD2 &&
		!strcmpi(scene.item[j].modelfile, scene.item[i].modelfile) &&
		!strcmpi(scene.item[j].skinfile, scene.item[i].skinfile) &&
		scene.item[j].tex_type == scene.item[i].tex_type) {
			return i;
		}
	}
	return 0;
}

void make_models() {
	int md2_retval;
	int i,j;

	for(j = 0; j < scene.num_items; j++) {
		if(scene.item[j].item_type == ITEM_MD2) {
			start_tex(scene.item[j].tex_type);
			/* See if this model has already been loaded */
			if(i = is_loaded(j)) {
				scene.item[j].model = scene.item[i].model;
				continue;
			}

			md2_retval = parse_md2(scene.item[j].modelfile,
			                       scene.item[j].skinfile,
			                       &scene.item[j].model,
			                       scene.item[j].tex_type);
			printf("Loading MD2: %s with skin: %s\n",
				scene.item[j].modelfile,
				(*scene.item[j].skinfile!=0)?
				scene.item[j].skinfile:"no skin");
		        assert(md2_retval != MD2ERR_OPEN);
		        assert(md2_retval != MD2ERR_VERSION);
		        assert(md2_retval != MD2ERR_MAGIC);
		        assert(md2_retval != MD2ERR_RANGE);
		        assert(md2_retval != MD2ERR_VERTEX);
		        assert(md2_retval != MD2ERR_NORMALS);
		        assert(md2_retval != MD2ERR_DISPLAYLIST);
		        assert(md2_retval == MD2_OK);
		}
	}
}

static float frand() {
        return (float)rand() / (float)RAND_MAX;
}

void init_scene(char *filename) {
	int j;

	read_scenedef(filename, &scene);
	make_models();
	init_craters();
	init_box();

        /* Initialize textures */
        printf("Initilizing 1-D contour texture.\n");
	init_texgen();
        crater_tex = bind_pcx("be.pcx", 256, 256, 0);
        printf("Allocated texture %d for craters skin: %s.\n",
                crater_tex, "be.pcx");
	stars1_tex = bind_pcx("stars1.pcx", 1024, 1024, 0);
	printf("Allocated texture %d for stars1 skin: %s.\n",
		stars1_tex, "stars1.pcx");
	stars2_tex = bind_pcx("stars2.pcx", 1024, 1024, 0);
	printf("Allocated texture %d for stars2 skin: %s.\n",
		stars2_tex, "stars2.pcx");
	stars3_tex = bind_pcx("stars3.pcx", 1024, 1024, 0);
	printf("Allocated texture %d for stars3 skin: %s.\n",
		stars3_tex, "stars3.pcx");

	printf("Initializing star points.\n");	
	for(j = 0; j < NUM_STARS; j++) {
		float x, y, z, norm;
		x = frand() * 400.0 - 200.0;
		y = frand() * 400.0 - 200.0;
		z = frand() * 400.0 - 200.0;
		norm = sqrt(x*x+y*y+z*z);
		x /= norm;
		y /= norm;
		z /= norm;
		stars[j][0] = x;
		stars[j][1] = y;
		stars[j][2] = z;
	}
}

void gen_tex(int j) {
	GLfloat op[4] = {1.0, 1.0, 1.0, 0.0};
	
	switch(scene.item[j].tex_type) {
	case TEX_MAP2:
		op[0] = 0.01 / scene.item[j].scale[0];
		op[1] = 0.01 / scene.item[j].scale[1];
		op[2] = 0.01 / scene.item[j].scale[2];
		if(scene.item[j].item_type == ITEM_BOX) {
			op[0] /= scene.item[j].params[2];
			op[1] /= scene.item[j].params[0];
			op[2] /= scene.item[j].params[1];
		}

		glTexGenfv(GL_S, GL_OBJECT_PLANE, op);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, op);
		glTexGenfv(GL_S, GL_EYE_PLANE, scene.item[j].tex_s);
		glTexGenfv(GL_T, GL_EYE_PLANE, scene.item[j].tex_t);
		break;
	default:
		break;
	}
}

void do_matrix1(int j) {
	glTranslatef(scene.item[j].translate[2],
		scene.item[j].translate[0],
		scene.item[j].translate[1]);
	glRotatef(scene.item[j].rotate1[0],
		scene.item[j].rotate1[3],
		scene.item[j].rotate1[1],
		scene.item[j].rotate1[2]);
	glRotatef(scene.item[j].rotate2[0],
		scene.item[j].rotate2[3],
		scene.item[j].rotate2[1],
		scene.item[j].rotate2[2]);
	glRotatef(scene.item[j].rotate3[0],
		scene.item[j].rotate3[3],
		scene.item[j].rotate3[1],
		scene.item[j].rotate3[2]);
	gen_tex(j);
	glScalef(scene.item[j].scale[0],
		scene.item[j].scale[1],
		scene.item[j].scale[2]);
}

void do_matrix2(int j) {
	glTranslatef(scene.item[j].translate[0],
		scene.item[j].translate[1],
		scene.item[j].translate[2]);
	glRotatef(scene.item[j].rotate1[0],
		scene.item[j].rotate1[1],
		scene.item[j].rotate1[2],
		scene.item[j].rotate1[3]);
	glRotatef(scene.item[j].rotate2[0],
		scene.item[j].rotate2[1],
		scene.item[j].rotate2[2],
		scene.item[j].rotate2[3]);
	glRotatef(scene.item[j].rotate3[0],
		scene.item[j].rotate3[1],
		scene.item[j].rotate3[2],
		scene.item[j].rotate3[3]);
	gen_tex(j);
	glScalef(scene.item[j].scale[0],
		scene.item[j].scale[1],
		scene.item[j].scale[2]);
}

void draw_scene() {
	int j;

	/* We don't want to overflow */
	/* currentFrame %= scene.item[0].model.numframes; */
	/* We have animation disabled, so currentFrame = 0; */

	for(j = 1; j < scene.num_items; j++) {
			
		switch(scene.item[j].item_type) {
		case ITEM_MD2:
			glPushMatrix();
       			glRotatef(90.0, -1.0, 0.0, 0.0);
		        glRotatef(90.0, 0.0, 0.0, -1.0);
			do_matrix1(j);

			start_tex(scene.item[j].tex_type);

			/* Draw the model */
			if(scene.item[j].draw_outline || wireframe) {
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glColor3f(1.0, 1.0, 1.0);	
				glCallList(scene.item[j].model.framestart +
					currentFrame);
				glColor3f(0.0, 0.0, 0.0);
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glScalef(1.01, 1.01, 1.01);
				glCallList(scene.item[j].model.framestart +
					currentFrame);
				glPopAttrib();
			} else {
				npr_draw_model(scene.item[j].model,
					currentFrame);
			}
			glPopMatrix();
			break;
		case ITEM_BOX:
			glPushMatrix();
			do_matrix2(j);
			start_tex(scene.item[j].tex_type);
			draw_box(scene.item[j].params[0],
				scene.item[j].params[2],
				scene.item[j].params[1]);
			glPopMatrix();
			break;
		default:
			break;
		}
	}	
}

void draw_terrain() {
	int i, j;
	float theta;

	glDisable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_2D, crater_tex);

	for(j = 0; j < scene.num_items; j++) {
		switch(scene.item[j].item_type) {
		case ITEM_TERRAIN:
			/* Carve out pie in stencil buffer */
			glPushMatrix();
			do_matrix2(j);
			glEnable(GL_STENCIL_TEST);
			glClear(GL_STENCIL_BUFFER_BIT);	
			glStencilFunc(GL_ALWAYS, 0x1, 0x1);
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_KEEP);
			glDrawBuffer(GL_NONE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_CULL_FACE);
			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0, 5.0, 0);
				glVertex3f(cos(-M_PI / 8) * RADIUS, 5.0, sin(-M_PI / 8) * RADIUS); 
				theta =  0.0;
				for (i = 0; theta < scene.item[j].params[1]; i++) {
					theta = (float)i * M_PI / (float)SLICES;
					glVertex3f(cos(theta) * RADIUS, 5.0, sin(theta) * RADIUS);
				}
			glEnd();
			glStencilFunc(GL_GEQUAL, 0x1, 0x1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			
			glDrawBuffer(GL_BACK);			
			if(scene.item[j].tex_type != GL_NONE)
				glEnable(GL_TEXTURE_2D);
			else
				glDisable(GL_TEXTURE_2D);
			draw_craters();
			glPopMatrix();

			glDisable(GL_STENCIL_TEST);
			break;
		default:
			break;
		}
	}
}

void do_quad() {
    glBegin (GL_QUADS);
                glTexCoord2f(0.0, 1.0);         glVertex3f(0.0, 0.0, 0.0);
                glTexCoord2f(1.0, 1.0);         glVertex3f(1.0, 0.0, 0.0);
                glTexCoord2f(1.0, 0.0);         glVertex3f(1.0, 1.0, 0.0);
                glTexCoord2f(0.0, 0.0);         glVertex3f(0.0, 1.0, 0.0);
    glEnd ();
}

void draw_stars() {
	float op1[] = {1.0, 0.0, 0.0, 0.0};
	float op2[] = {0.0, 1.0, 0.0, 0.0};
	float rho, inc, r, x, y, z, phi, theta;
	float norm;
	int j;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

/*	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK_FACE);*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if(texture) {
		glEnable(GL_TEXTURE_2D);
	} else {
		glColor3f(0.0, 0.0, 0.0);
	}

/*	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGenfv(GL_S, GL_EYE_PLANE, op1);
	glTexGenfv(GL_T, GL_EYE_PLANE, op2);*/
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glBindTexture(GL_TEXTURE_2D, stars3_tex);
	glPushMatrix();
		glTranslatef(-225.0, -115.0, -400.0);
		glScalef(500.0, 350.0, 1.0);
		do_quad();	
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, stars2_tex);
	glPushMatrix();
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glTranslatef(-225.0, -115.0, -400.0);
		glScalef(-500.0, -350.0, 1.0);
		do_quad();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, stars1_tex);
	glPushMatrix();
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glTranslatef(-175.0, 120.0, -200.0);
		glScalef(500.0, 350.0, 1.0);
		do_quad();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, stars1_tex);
	glPushMatrix();
		glTranslatef(-175.0, -200.0, 400.0);
		glScalef(500.0, 350.0, 1.0);
		do_quad();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, stars2_tex);
	glPushMatrix();
		glRotatef(0.0, 0.0, 1.0, 0.0);
		glTranslatef(33.0, -50.0, -14.5);
		glScalef(200.0, 50.0, 200.0);
		do_quad();
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
		glScalef(400.0, 400.0, 400.0);
		glBegin(GL_POINTS);
		for(j = 0; j < NUM_STARS; j++) glVertex3fv(stars[j]);
		glEnd();
	glPopMatrix();

/*	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glColor3f(1.0, 1.0, 1.0);

	rho = 400.0;
	inc = 5.0 / (M_PI/2.0);
	for(theta = 0; theta < M_PI/2.0; theta += inc) {
		glBegin(GL_TRIANGLE_STRIP);
		for(phi = 0; phi < M_PI/2.0; phi += inc) {
			r = rho*cos(phi);
			y = rho*sin(phi);
			x = r*cos(theta);
			z = r*sin(theta);
			glVertex3f(x, y, z);
			x = r*cos(theta+inc);
			z = r*sin(theta+inc);
			glVertex3f(x, y, z);
		}
		glEnd();
	}*/

	glPopAttrib();
}
