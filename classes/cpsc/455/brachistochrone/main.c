#include <stdio.h>
#include <math.h>
#include "zerofinder.h"

/* Global vars */
double x=0.01, g=9.81, v0=0;

/* our function */
double func (double h) {
	return 1.0/(((x*x + h*h)*(2*g*h + v0*v0))/(h*(2*g*h + v0*v0) - (x*x + h*h)));
}

int main() {
	double h, y = 0;
	int j;

	for(j = 0; j < 100; j++) {
		h = zeroin(0.0,1.0,func,0.0);
		y += h;
		printf("%e\n", y);
		v0 = v0 + sqrt(2 * g * h);
	}

	return 0;
}
