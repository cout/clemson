#ifndef AW_H
#define AW_H

#include "../misc/dynarray.h"
#include "../math/matrix.h"

typedef float Vertex[4];
typedef float Quat[4];

typedef struct {
	DYNARRAY(DYNARRAY(int)*) faces;
	DYNARRAY(Vertex) verts;
	DYNARRAY(Vector) face_normals;
	DYNARRAY(Vector) vertex_normals;
	DYNARRAY(Vector) face_centers;
	int displaylist;
	int object_changed;
	Quat q;
	Vector trans;
	Vector center;
	int invert_normals;
	float color[4];
	float specular[4];
	float shininess;
	int texnum;
} Object;
 
enum aw_return_values {
	AW_OK,
	AWERR_OPEN,
	AWERR_VERT,
	AWERR_FACE,
	AWERR_MALLOC,
	AWERR_FREE
};

int init_aw(Object *o);
int read_aw(const char *file, Object *o);
int free_object(Object *o);

#endif
