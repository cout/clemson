#ifndef MATRIX_H
#define MATRIX_H

/* Matrix math */
/* Note:
 *  - all vectors are of size 3
 *  - all matrices are of size 4x4
 */

typedef float Vector[3];
typedef float* pVector;
typedef float Matrix[16];
typedef float* pMatrix;

void vv_add(pVector a, const pVector b);
void vv_sub(pVector a, const pVector b);
void vv_mul(pVector a, const pVector b);
void vv_div(pVector a, const pVector b);
void vv_cpy(pVector dest, const pVector src);
double vv_dot(const pVector a, const pVector b);
void vv_cross(pVector a, const pVector b);

void vc_add(pVector a, double b);
void vc_mul(pVector a, double b);
void vc_div(pVector a, double b);

void v_exp(pVector a);
void v_zero(pVector a);
void v_norm(pVector a);
void v_neg(pVector a);

void vm_mul(pVector a, const pMatrix b);

#endif
