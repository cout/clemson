#include <stdio.h>
#include "matrix.h"

void matrix_print(pMatrix m) {
	int j;
	for(j = 0; j < 16; j++) {
		printf("%f\t", m[j]);
		if(!((j + 1) % 4)) printf("\n");
	}
}

void vect_print(pVector v) {
	int j;
	for(j = 0; j < 4; j++) {
		printf("%f\t", v[j]);
	}
	printf("\n");
}

int main() {
Matrix m = {
	1, 2, 3, 4,
	5, 6, 7, 8,
	4, 3, 2, 1,
	8, 7, 6, 5
};

Vector v = {
	3, 5, 7, 9
};

	vect_print(v);
	vm_mul(v, m);
	vect_print(v);
	return 0;
}
