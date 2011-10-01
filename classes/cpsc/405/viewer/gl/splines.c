#include <math.h>
#include "math/matrix.h"
#include "math/_math.h"
#include "splines.h"
#include "gl.h"
#include "normals.h"

static Matrix bspline_basis = {
	-1.0/6, 3.0/6, -3.0/6, 1.0/6,
	3.0/6, -6.0/6, 0/6, 4.0/6,
	-3.0/6, 3.0/6, 3.0/6, 1.0/6,
	1.0/6, 0, 0, 0
/*	-1, 3, -3, 1,
	3, -6, 3, 0,
	-3, 0, 3, 0,
	1, 4, 1, 0*/
};

/* spline_params finds the four spline parameters given a control point
 * array and an offset.
 */
void spline_params(ctlpt_type *ctlpts, int offset, pVector cx, pVector cy) {
	int j;
	for(j = 0; j < 4; j++) {
		cx[j] = ctlpts->ctrlpoints[offset+j][0];
		cy[j] = ctlpts->ctrlpoints[offset+j][1];
		/*g_print("Control point at: %f, %f\n", cx[j], cy[j]);*/
	}
	vm_mul(cx, bspline_basis);
	vm_mul(cy, bspline_basis);
}


/* draw_1D_spline draws a 1D spline in 2D space.  It assumes that the
 * caller has already called glOrtho with the correct parameters.
 */
void draw_1D_spline(ctlpt_type *ctlpts, int curve_steps) {
	float delta = 1.0/(curve_steps);
	float t = 0;
	int i, j;
	Vector cx, cy;
	float x, y;

	if(ctlpts->numctlpts < 4) return;

	glColor3f(0, 1, 0);

	glBegin(GL_LINE_STRIP);
	for(i = 0; i < ctlpts->numctlpts-3; i++) {
		spline_params(ctlpts, i, cx, cy);
		for(j = 0, t = 0; j <= curve_steps; j++, t += delta) {
			x = ((cx[0]*t + cx[1])*t + cx[2])*t + cx[3];
			y = ((cy[0]*t + cy[1])*t + cy[2])*t + cy[3];
			glVertex2f(x, y);
		}
	}
	glEnd();
}

void draw_1D_spline_GL(ctlpt_type *ctlpts, int curve_steps) {
	int j, total;

	GLfloat cp[MAXPTS*3];
	for(j = 0; j < MAXPTS; j++) {
		cp[j+0] = ctlpts->ctrlpoints[j][0];
		cp[j+1] = ctlpts->ctrlpoints[j][1];
		cp[j+2] = 0.0;
	}
		
	glMap1f(GL_MAP1_VERTEX_3,	/* 3D control points and vertices */
		0.0, 1.0,		/* Range of parameter u */
		3,			/* Stride */
		ctlpts->numctlpts,	/* Order */
		cp);			/* The control points */
	glEnable(GL_MAP1_VERTEX_3);

	glBegin(GL_LINE_STRIP);
	total = curve_steps * ctlpts->numctlpts;
	for(j = 0; j < total; j++)
		/*g_print("Drawing curve element %d\n", j);*/
		glEvalCoord1f((GLfloat)j/total);
	glEnd();

}

void add_to_face(Object *o, Dynarray *a, int i) {
	Dynarray *b;
	if(i >= o->verts.count) i = (i%o->verts.count) + 1;
	/*g_print("%d ", i);*/
	da_add(a, &i);
	while(o->vert_to_face.count <= i) {
		/*g_print("v2f count: %d\n", o->vert_to_face.count);*/
		b = (Dynarray*)g_malloc(sizeof(Dynarray));
		da_init(b, sizeof(int), 16);
		DA_ADD(o->vert_to_face, &b);
	}
	da_add((Dynarray*)o->vert_to_face.data[i], &o->faces.count);
}
	
/* draw_spline_volume draws a 1D spline rotated sweep_steps around the
 * y-axis.  It replaces the currently selected object with whatever
 * object is currently selected.
 */
void draw_spline_volume(ctlpt_type *ctlpts, int curve_steps, int sweep_steps) {
	float delta = 1.0/(curve_steps);
	int i, j, k, verts_per_sweep;
	float x, y, t, angle, sinangle, cosangle;
	Vector v, cx, cy;
	Object *o;
	Dynarray *a;

	if(ctlpts->numctlpts < 4) return;
	if(curve_steps < 1) return;
	if(sweep_steps < 3) return;

	if(curr_obj < 0) {
		if(num_objects >= MAX_OBJECTS) return;
		curr_obj = num_objects;
		num_objects++;
	} else {
		free_object(&aw_object[curr_obj]);
	}
	o = &aw_object[curr_obj];
	init_aw(o);

	/* Find the vertices */
	for(k = 0; k < sweep_steps; k++) {
		for(i = 0; i < ctlpts->numctlpts-3; i++) {
			spline_params(ctlpts, i, cx, cy);
			for(j = 0, t = 0; j <= curve_steps; j++, t += delta) {
				x = ((cx[0]*t + cx[1])*t + cx[2])*t + cx[3];
				y = ((cy[0]*t + cy[1])*t + cy[2])*t + cy[3];
				x /= 400.0; y /= 400.0;

				angle = (2*M_PI/sweep_steps*k);
				sinangle = sin(angle);
				cosangle = cos(angle);

				v[0] = cosangle*x;
				v[1] = y;
				v[2] = sinangle*x;
				/*g_print("Vertex %d at %f %f %f\n",
					o->verts.count, v[0], v[1], v[2]);*/
				DA_ADD(o->verts, v);
			}
		}
	}

	/* Find the faces */
	/*g_print("Beginning face calculation\n");*/
	verts_per_sweep = (ctlpts->numctlpts-3)*(curve_steps+1);
	for(k = 0; k < sweep_steps; k++) {
		for(i = 0; i < verts_per_sweep-1; i++) {
			a = (Dynarray*)g_malloc(sizeof(Dynarray));
			da_init(a, sizeof(int), 16);
			/*g_print("Face %d: ", o->faces.count);*/
			add_to_face(o, a, 1+k*verts_per_sweep+i);
			add_to_face(o, a, 1+k*verts_per_sweep+i+1);
			add_to_face(o, a, 1+(k+1)*verts_per_sweep+i+1);
			add_to_face(o, a, 1+(k+1)*verts_per_sweep+i);
			DA_ADD(o->faces, &a);
			/*g_print("\n");*/
		}
	}

	calculate_normals(o);
	o->invert_normals = TRUE;
	o->object_changed = TRUE;
	gl_redraw_all();
}


