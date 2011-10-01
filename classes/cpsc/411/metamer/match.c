#include <stdio.h>
#include "point.h"
#include "xyz_match_fcts.h"
#include "plambda.h"

#define LOWER_BOUND 380.0
#define UPPER_BOUND 770.0
#define DELTA 10.0
#define NUM_POINTS (77-38+1)

/* do_integration integrates P1P2 */
double do_integration(struct point p1[], struct point p2[]) {
	double total_area = 0.0;
	double base, y1, y2;
	int j;
	for(j = 0; j < NUM_POINTS-1; j++) {
		base = p1[j + 1].x - p1[j].x;
		y1 = p1[j].y * p2[j].y;
		y2 = p1[j + 1].y * p2[j + 1].y;

		total_area = total_area + (base * y1 + base * y2) / 2;
	}
	return total_area;
}

main() {
	double X, Y, Z;


	/* Find X, Y, Z */
	X = do_integration(fx, p);
	Y = do_integration(fy, p);
	Z = do_integration(fz, p);
	printf("X = %f, Y = %f, Z = %f\n", X, Y, Z);

	return 0;
}
