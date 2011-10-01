#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <sys/stat.h>

#include <misc/_string.h>
#include <misc/io.h>
#include <misc/dynarray.h>
#include <widgets/status.h>

#include "textures.h"
#include "aw.h"

int init_aw(Object *o) {
	float pa[4] = {0.0, 0.0, 0.0, 0.0};

	/* Initialize dynamic arrays */
	DA_INIT(o->faces, sizeof(Dynarray*), 16384);
	DA_INIT(o->verts, sizeof(Vertex), 16384);
	DA_INIT(o->face_normals, sizeof(Vector), 16384);
	DA_INIT(o->face_centers, sizeof(Vector), 16384);
	DA_INIT(o->vertex_normals, sizeof(Vector), 16384);
	DA_INIT(o->vert_to_face, sizeof(Dynarray*), 16384);
	DA_INIT(o->d, sizeof(float), 16384);

	/* The 0th vertex will be (0,0,0), so that we will be aligned
	 * properly with the numbering system in the object file.
	 */
	DA_ADD(o->verts, pa);

	/* Set the quaternion and the translation vector */
	memset(o->q, 0, sizeof(Quat));
	memset(o->trans, 0, sizeof(Vector));
	o->q[3] = 1.0;

	/* Set the object color (default = white) */
	o->color[0] = 0.8;
	o->color[1] = 0.8;
	o->color[2] = 0.8;
	o->color[3] = 1.0;
	o->specular[0] = 0.2;
	o->specular[1] = 0.2;
	o->specular[2] = 0.2;
	o->specular[3] = 1.0;
	o->shininess = 80.0;

	o->texnum = 0;
	o->invert_normals = FALSE;
	o->object_changed = TRUE;

	return AW_OK;
}

int read_aw(const char *file, Object *o) {
	FILE *fp;
	char op[128];
	float pa[4] = {0.0, 0.0, 0.0, 0.0};
	Vector min, max;
	int first = 1;
	int j, retval = AW_OK, i;
	Dynarray *a, *b;
	struct stat statbuf;
	long filesize;

	/* Get the file's size */
	stat(file, &statbuf);
	filesize = statbuf.st_size;
	
	if((fp = fopen(file, "rt")) == NULL) return AWERR_OPEN;

	/* Initialize values */
	o->displaylist = 0;
	o->object_changed = TRUE;
	
	/* Read the file */
	for(;;) {
		/* Update the status bar */
		status_update((float)ftell(fp) / (float)filesize);

		i = io_getword(op, sizeof(op), fp);
		if(i == IO_EOF) break;

		if(!strcmpi(op, "v")) {
			i = IO_OK;
			for(j = 0; j < 3; j++) {
				if(i == IO_EOL) {
					retval = AWERR_VERT;
					g_warning("Vertex error in file %s",
						file);
					break;
				}
				i = io_getfloat(&pa[j], fp);
				if(first) {
					min[j] = max[j] = pa[j];
				} else {
					if(pa[j] < min[j]) min[j] = pa[j];
					if(pa[j] > max[j]) max[j] = pa[j];
				}
			}
			first = 0;
			while(i == IO_OK) i = io_getword(NULL, INT_MAX, fp);
			DA_ADD(o->verts, pa);
		} else if(!strcmpi(op, "f")) {
			a = (Dynarray*)g_malloc(sizeof(Dynarray));
			da_init(a, sizeof(int), 16);
			for(;;) {
				if(io_getint(&i, fp) != IO_OK) break;
				g_assert(i != 0);
				da_add(a, &i);
				while(o->vert_to_face.count <= i) {
					b = (Dynarray*)g_malloc(
						sizeof(Dynarray));
					da_init(b, sizeof(int), 16);
					DA_ADD(o->vert_to_face, &b);
				}
				da_add((Dynarray*)o->vert_to_face.data[i],
					&o->faces.count);
			}
			DA_ADD(o->faces, &a);
		} else {
			while(i != IO_EOL) i = io_getword(NULL, INT_MAX, fp);
		}
	}

	o->center[0] = (max[0] + min[0]) / 2.0;
	o->center[1] = (max[1] + min[1]) / 2.0;
	o->center[2] = (max[2] + min[2]) / 2.0;

	status_clear();	
	return retval;
}

int save_aw(const char *file, const Object *o) {
	FILE *fp;
	int j, i;

	if((fp = fopen(file, "wt")) == NULL) return AWERR_OPEN;

	for(j = 1; j < o->verts.count; j++) {
		fprintf(fp, "v %f %f %f\n",
			o->verts.data[j][0],
			o->verts.data[j][1],
			o->verts.data[j][2]);
	}
	fprintf(fp, "g faces\n");
	for(j = 0; j < o->faces.count; j++) {
		fprintf(fp, "f ");
		for(i = 0; i < o->faces.data[j]->count; i++) {
			fprintf(fp, "%d ", o->faces.data[j]->data[i]);
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
	return AW_OK;
}

int free_object(Object *o) {
	int j;
	for(j = 0; j < o->faces.count; j++) {
		da_free((Dynarray*)o->faces.data[j]);
		g_free(o->faces.data[j]);
	}
	for(j = 0; j < o->vert_to_face.count; j++) {
		da_free((Dynarray*)o->vert_to_face.data[j]);
		g_free(o->vert_to_face.data[j]);
	}
	DA_FREE(o->faces);
	DA_FREE(o->verts);
	DA_FREE(o->face_normals);
	DA_FREE(o->vertex_normals);
	DA_FREE(o->vert_to_face);
	if(o->texnum) free_tex(o->texnum);
	return AW_OK;
}

