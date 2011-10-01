#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <string.h>

/* Matrix math */
/* Note:
 *  - all vectors are of size 4
 *  - all matrices are of size 4x4
 */

typedef float Vector[4];
/*typedef float* pVector;*/
#define pVector float*
typedef float Matrix[16];
/*typedef float* pMatrix;*/
#define pMatrix float*

#ifdef __cplusplus
#define INLINE inline
#else
#define INLINE static
#endif

/* vv_add adds two vectors, a and b, and places the result in a */
INLINE void vv_add(pVector a, const pVector b) {
	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
	a[3] += b[3];
}

/* vv_sub subtracts two vectors, a and b, and places the result in a */
INLINE void vv_sub(pVector a, const pVector b) {
	a[0] -= b[0];
	a[1] -= b[1];
	a[2] -= b[2];
	a[3] -= b[3];
}

/* vv_div divides two vectors, a and b, and places the result in a */
INLINE void vv_div(pVector a, const pVector b) {
	a[0] /= b[0];
	a[1] /= b[1];
	a[2] /= b[2];
	a[3] /= b[3];
}

/* vv_div divides two vectors, a and b, and places the result in a */
INLINE void vv_mul(pVector a, const pVector b) {
	a[0] *= b[0];
	a[1] *= b[1];
	a[2] *= b[2];
	a[3] *= b[3];
}

/* vv_dot finds the dot product of a and b, and returns the result */
INLINE double vv_dot(const pVector a, const pVector b) {
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

/* vv_cpy sets vector dest = vector src */
INLINE void vv_cpy(pVector dest, const pVector src) {
	memcpy(dest, src, sizeof(Vector));
}

/* mm_cpy sets matrix dest = vector src */
INLINE void mm_cpy(pMatrix dest, const pMatrix src) {
	memcpy(dest, src, sizeof(Matrix));
}

/* vv_cross finds the cross product of a and b, and places the result in a */
INLINE void vv_cross(pVector a, const pVector b) {
	Vector result;
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
	vv_cpy(a, result);
}

/* vc_add adds a constant b to vector a */
INLINE void vc_add(pVector a, double b) {
	a[0] += b;
	a[1] += b;
	a[2] += b;
	a[3] += b;
}

/* vc_mul multiples a vector a by a constant b */
INLINE void vc_mul(pVector a, double b) {
	a[0] *= b;
	a[1] *= b;
	a[2] *= b;
	a[3] *= b;
}

/* vc_div divides a vector a by a constant b */
INLINE void vc_div(pVector a, double b) {
	a[0] /= b;
	a[1] /= b;
	a[2] /= b;
	a[3] /= b;
}

/* v_exp places e^a in a */
INLINE void v_exp(pVector a) {
	a[0] = exp(a[0]);
	a[1] = exp(a[1]);
	a[2] = exp(a[2]);
	a[3] = exp(a[3]);
}

/* v_zero sets a to zero */
INLINE void v_zero(pVector a) {
	a[0] = a[1] = a[2] = a[3] = 0;
}

/* v_norm normalizes a vector */
INLINE void v_norm(pVector a) {
	double l = sqrt(vv_dot(a, a));
	if(fabs(l) > 0.0001) vc_div(a, l);
}

/* v_neg negates a vector (opposite direction) */
INLINE void v_neg(pVector a) {
	a[0] = -a[0];
	a[1] = -a[1];
	a[2] = -a[2];
}

/* vm_mul multiplies a vector by a 4x4 matrix, and puts the result in a */
INLINE void vm_mul(pVector a, const pMatrix b) {
	Vector c;
	vv_cpy(c, a);
	a[0] = c[0]*b[0] + c[1]*b[4] + c[2]*b[8] + c[3]*b[12];
	a[1] = c[0]*b[1] + c[1]*b[5] + c[2]*b[9] + c[3]*b[13];
	a[2] = c[0]*b[2] + c[1]*b[6] + c[2]*b[10] + c[3]*b[14];
	a[3] = c[0]*b[3] + c[1]*b[7] + c[2]*b[11] + c[3]*b[15];
}

/* mm_mul multiplies a matrix by a matrix */
INLINE void mm_mul(pMatrix a, const pMatrix b) {
	Matrix t;
	t[0] = a[0]*b[0] + a[4]*b[1] + a[8]*b[2] + a[12]*b[3];
	t[1] = a[1]*b[0] + a[5]*b[1] + a[9]*b[2] + a[13]*b[3];
	t[2] = a[2]*b[0] + a[6]*b[1] + a[10]*b[2] + a[14]*b[3];
	t[2] = a[3]*b[0] + a[7]*b[1] + a[11]*b[2] + a[15]*b[3];
	t[0] = a[0]*b[4] + a[4]*b[5] + a[8]*b[6] + a[12]*b[7];
	t[1] = a[1]*b[4] + a[5]*b[5] + a[9]*b[6] + a[13]*b[7];
	t[2] = a[2]*b[4] + a[6]*b[5] + a[10]*b[6] + a[14]*b[7];
	t[2] = a[3]*b[4] + a[7]*b[5] + a[11]*b[6] + a[15]*b[7];
	t[0] = a[0]*b[8] + a[4]*b[9] + a[8]*b[10] + a[12]*b[11];
	t[1] = a[1]*b[8] + a[5]*b[9] + a[9]*b[10] + a[13]*b[11];
	t[2] = a[2]*b[8] + a[6]*b[9] + a[10]*b[10] + a[14]*b[11];
	t[2] = a[3]*b[8] + a[7]*b[9] + a[11]*b[10] + a[15]*b[11];
	t[0] = a[0]*b[12] + a[4]*b[13] + a[8]*b[14] + a[12]*b[15];
	t[1] = a[1]*b[12] + a[5]*b[13] + a[9]*b[14] + a[13]*b[15];
	t[2] = a[2]*b[12] + a[6]*b[13] + a[10]*b[14] + a[14]*b[15];
	t[2] = a[3]*b[12] + a[7]*b[13] + a[11]*b[14] + a[15]*b[15];
	mm_cpy(a, t);
}

#endif
