/* Matrix math */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

/* vv_add adds two vectors, a and b, and places the result in a */
void vv_add(pVector a, const pVector b) {
	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
	a[3] += b[3];
}

/* vv_sub subtracts two vectors, a and b, and places the result in a */
void vv_sub(pVector a, const pVector b) {
	a[0] -= b[0];
	a[1] -= b[1];
	a[2] -= b[2];
	a[3] -= b[3];
}

/* vv_div divides two vectors, a and b, and places the result in a */
void vv_div(pVector a, const pVector b) {
	a[0] /= b[0];
	a[1] /= b[1];
	a[2] /= b[2];
	a[3] /= b[3];
}

/* vv_div divides two vectors, a and b, and places the result in a */
void vv_mul(pVector a, const pVector b) {
	a[0] *= b[0];
	a[1] *= b[1];
	a[2] *= b[2];
	a[3] *= b[3];
}

/* vv_dot finds the dot product of a and b, and returns the result */
double vv_dot(const pVector a, const pVector b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/* vv_cross finds the cross product of a and b, and places the result in a */
void vv_cross(pVector a, const pVector b) {
	Vector result;
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
	vv_cpy(a, result);
}

/* vv_cpy sets vector dest = vector src */
void vv_cpy(pVector dest, const pVector src) {
	memcpy(dest, src, sizeof(Vector));
}

/* vc_add adds a constant b to vector a */
void vc_add(pVector a, double b) {
	a[0] += b;
	a[1] += b;
	a[2] += b;
	a[3] += b;
}

/* vc_mul multiples a vector a by a constant b */
void vc_mul(pVector a, double b) {
	a[0] *= b;
	a[1] *= b;
	a[2] *= b;
	a[3] *= b;
}

/* vc_div divides a vector a by a constant b */
void vc_div(pVector a, double b) {
	a[0] /= b;
	a[1] /= b;
	a[2] /= b;
	a[3] /= b;
}

/* v_exp places e^a in a */
void v_exp(pVector a) {
	a[0] = exp(a[0]);
	a[1] = exp(a[1]);
	a[2] = exp(a[2]);
	a[3] = exp(a[3]);
}

/* v_zero sets a to zero */
void v_zero(pVector a) {
	a[0] = a[1] = a[2] = a[3] = 0;
}

/* v_norm normalizes a vector */
void v_norm(pVector a) {
	double l = sqrt(vv_dot(a, a));
	if(fabs(l) > 0.0001) vc_div(a, l);
}

/* v_neg negates a vector (opposite direction) */
void v_neg(pVector a) {
	a[0] = -a[0];
	a[1] = -a[1];
	a[2] = -a[2];
}

/* vm_mul multiplies a vector by a 4x4 matrix, and puts the result in a */
void vm_mul(pVector a, const pMatrix b) {
	Vector c;
	vv_cpy(c, a);
	a[0] = c[0]*b[0] + c[1]*b[4] + c[2]*b[8] + c[3]*b[12];
	a[1] = c[0]*b[1] + c[1]*b[5] + c[2]*b[9] + c[3]*b[13];
	a[2] = c[0]*b[2] + c[1]*b[6] + c[2]*b[10] + c[3]*b[14];
	a[3] = c[0]*b[3] + c[1]*b[7] + c[2]*b[11] + c[3]*b[15];
}
