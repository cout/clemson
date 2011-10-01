#include <glib.h>
#include <math.h>
#include "dynarray.h"
#include "math/matrix.h"
#include "gl/aw.h"

#define sign(x) ((x<0)?-1:1)

/* ray_intersect_plane finds whether a ray p1p2 intersects a plane.
 * If the ray does intersect the plane, the point of intersection is stored
 * in the vector specified by p.
 */
int ray_intersect_plane(pVector p1, pVector p2, pVector face_normal, 
	pVector face_center, float d, pVector p) {

	Vector v1, v2;
	double t, tmp;

	/* First check to ensure that the points are on opposite sides of
	 * the face.
	 */
	vv_cpy(v1, face_center);
	vv_sub(v1, p1);
	vv_cpy(v2, face_center);
	vv_sub(v2, p2);

	if(sign(vv_dot(v1, face_normal)) == sign(vv_dot(v2, face_normal)))
		return 0;
	
	/* Next, find the point of intersection of the ray and the plane */
	vv_cpy(p, p2);			/* p = p2 - p1 */
	vv_sub(p, p1);

	tmp = vv_dot(p, face_normal);
	if(tmp == 0) return 0;

	/* t = -(N dot p1 + D) / (N dot (p2 - p1)) */
	t = -(vv_dot(face_normal, p1) + d) / vv_dot(face_normal, p);

	if(t <= 0 || t >= 1) return 0;

	vc_mul(p, t);
	vv_add(p, p1);
	/*v_neg(p);*/		/* I don't know why I put this here.  I know
				 * I had a reason at one point, but I honestly
				 * can't remember what it was.
				 */

	return 1;
}

/* point_inside face checks to see if a point lying on the plane with a
 * polygon is indeed inside that polygon.  Returns 1 for true, 0 for false.
 */
int point_inside_face(pVector p, Face* face, VertList* verts) {
	int j;
	float theta = 0.0;
	Vector v1, v2;

	/* This is slow! */
	for(j = 0; j < face->count; j++) {
		vv_cpy(v1, p);
		vv_sub(v1, verts->data[face->data[j]]);
		v_norm(v1);
		/*g_print("[%f %f %f], ", v1[0], v1[1], v1[2]);*/
		vv_cpy(v2, p);
		vv_sub(v2, verts->data[face->data[(j+1)%(face->count)]]);
		v_norm(v2);
		/*g_print("[%f %f %f], ", v2[0], v2[1], v2[2]);*/
		theta += acos(vv_dot(v1, v2));
		/*g_print("angle: %f\n", acos(vv_dot(v1, v2)));*/
	}

	/*g_print("theta = %f\n", theta);*/

	if(fabs(theta - 2*M_PI) < 0.1) return 1;
	return 0;
}
