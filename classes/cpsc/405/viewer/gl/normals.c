#include <math/matrix.h>
#include <widgets/status.h>

#include "normals.h"

void calculate_face_normals(Object *o) {
	Vector edge1, edge2, point;
	int f, i, v, v1, v2, v3, verts;
	
	/* Calculate the face normals */
	for(f = 0; f < o->faces.count; f++) {
		/* Update the status bar */
		status_update((float)f / (float)o->faces.count);

		/* Don't draw normals if only two vertices */
		if(o->faces.data[f]->count < 3) continue;
	
		/* Find the center of the face */
		verts = o->faces.data[f]->count;
		v_zero(point);
		for(i = 0; i < verts; i++) {
			v = o->faces.data[f]->data[i];
			vv_add(point, o->verts.data[v]);
		}
		vc_div(point, verts);
		DA_ADD(o->face_centers, point);
	
		/* The direction of the normal is the cross product
		 * of two edges of the face.
		 */
		v1 = o->faces.data[f]->data[0];
		v2 = o->faces.data[f]->data[1];
		v3 = o->faces.data[f]->data[2];
		vv_cpy(edge1, o->verts.data[v1]);
		vv_cpy(edge2, o->verts.data[v2]);
		vv_sub(edge1, o->verts.data[v2]);
		vv_sub(edge2, o->verts.data[v3]);
		vv_cross(edge1, edge2);	/* put the answer in edge1 */
		// edge1[3] = 1.0;
		v_norm(edge1);			/* normalize */
		edge1[3] = 1.0;
		
		/* Add the normal to the array */
		DA_ADD(o->face_normals, edge1);
	}
}

void calculate_vertex_normals(Object *o) {
	Vector point = {0.0, 0.0, 0.0};
	int v, f, i;

	/* Since our vertex numbering system starts at 1, we need a dummy
	 * entry.
	 */
	DA_ADD(o->vertex_normals, point);

	/* For each vertex, we must find all connected vertices */
	for(v = 1; v < o->vert_to_face.count; v++) {
		/* Update the status bar */
		status_update((float)v / (float)o->vert_to_face.count);

		v_zero(point);

		for(i = 0; i < o->vert_to_face.data[v]->count; i++) {
			f = o->vert_to_face.data[v]->data[i];
			vv_add(point, o->face_normals.data[f]);
		}

		v_norm(point);
		
		DA_ADD(o->vertex_normals, point);
	}
}

void calculate_d(Object *o) {
	int f;
	float d;

	for(f = 0; f < o->faces.count; f++) {
		d = -vv_dot(o->face_normals.data[f], o->face_centers.data[f]);
		DA_ADD(o->d, &d);
	}
}

void calculate_normals(Object *o) {
	status_push("Calculating face normals...");
	calculate_face_normals(o);
	status_pop();

	status_push("Calculating vertex normals...");
	calculate_vertex_normals(o);
	status_pop();

	status_push("Calculating plane equations...");
	calculate_d(o);
	status_pop();

	status_clear();
}
