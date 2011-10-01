#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "zerofinder.h"

#define WS_L (double)10.0
#define WS_R (double)6.371E6

double stringfunc(double x) {
	return (1.0/3.0 + (2.0/15.0 + (17.0/315.0 + 62.0/2835.0 * x*x) *
	       x*x) * x*x) * x*x*x - WS_L/(2 * WS_R);
}

double minfunc(double tolerance) {
	double theta, height;

	theta = zeroin(0.0, M_PI_2, stringfunc, tolerance);
        height = WS_R / cos(theta) - WS_R;

	return height;
}

int main(int argc, char *argv[]) {
	double theta, height;
	double tolerance = 0.0;

	if(argc > 1) {
		if(!strcmp(argv[1], "-h")) {
			printf("Usage: %s [-h|-m] [tolerance]\n", argv[0]);
			return 0;
		}
		else if(!strcmp(argv[1], "-m")) {
			if(argc > 2) tolerance = strtod(argv[2], NULL);
			printf("Finding minimum height...\n");
			tolerance = zeroin(0.0, 0.0335211, minfunc, tolerance);
		}
		else tolerance = strtod(argv[1], NULL);
	}

	printf("World on a string\n");
	printf("tolerance = %f\n", tolerance);

	theta = zeroin(0.0, M_PI_2, stringfunc, tolerance);
	height = WS_R / cos(theta) - WS_R;
	printf("theta = %f\n", theta);
	printf("tan theta - theta - 1/2r = %.4e\n", stringfunc(theta));
	printf("height = %f\n", height);

	return 0;
}
