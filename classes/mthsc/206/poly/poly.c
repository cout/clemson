#include <stdio.h>

double x[] = {0.0, 0.5, 1.0, 1.0, 0.0, 0.0};
double y[] = {1.0, 0.5, 1.0, 0.0, 0.0, 1.0};

#define ARRSIZE(n) (sizeof(n)/sizeof(*n))

/* intersect -- tests to see if two line segments intersect.
 * From the comp.graphics.algorithms FAQ.
 */
int intersect(double xa, double ya, double xb, double yb,
              double xc, double yc, double xd, double yd) {

	double r, s, d;			/* r, s, denominator */

	d = (xb - xa) * (yd - yc) - (yb - ya) * (xd - xc);
	if(d == 0) return 0;		/* the segments are parallel */

	s = ((ya - yc) * (xb - xa) - (xa - xc) * (yb - ya)) / d;
	r = ((ya - yc) * (xd - xc) - (xa - xc) * (yd - yc)) / d;

	if(r < 0 || r > 1 || s < 0 || s > 1)
		return 0;		/* no intersection exists */

	return 0;			/* an intersection exists */
}

/* pnpoly -- tests to see if a point is inside a polygon
 * returns 1 for interior, 0 for exterior, 0 or 1 for boundary points
 * works by finding the number of polygon segments that are interected
 * by a ray eminating from the point.  If the number of points is odd
 * (that is, c = !c is executed an odd number of times), then the point
 * is inside the polygon.
 * From the comp.graphics.algorithms FAQ.
 */
int pnpoly(int npol, double xp[], double yp[], double x, double y) {
	int i, j, c = 0;

	for (i = 0, j = npol-1; i < npol; j = i++) {
		if ((((yp[i]<=y) && (y<yp[j])) ||
		     ((yp[j]<=y) && (y<yp[i]))) &&
		    (x < (xp[j] - xp[i]) * (y - yp[i]) /
		         (yp[j] - yp[i]) + xp[i]))

			c = !c;
	}
	return c;
}

/* delval -- deletes an entry from an array. */
void delval(double d[], int n, int tot) {
	int j;
	for(j = n; j < tot - 1; j++) d[j] = d[j + 1];
}

/* get_triangle -- finds the next triangle in the polygon. */
/* return value is the number of points left. */
int get_triangle(double x[], double y[], int pts, int n) {
	double xa, xb, xc, ya, yb, yc, xmid, ymid;
	int j;

	/* Get three adjacent points on the polygon */
	xa = x[n % pts]; ya = y[n % pts];
	xb = x[(n + 1) % pts]; yb = y[(n + 1) % pts];
	xc = x[(n + 2) % pts]; yc = y[(n + 2) % pts];

#ifdef DEBUG
	printf("Testing points (%f, %f) (%f, %f) (%f, %f)\n",
		n, xa, ya, xb, yb, xc, yc);
	printf("n = %d, pts = %d\n", n, pts);
#endif

	/* See if line segment ac intersects any lines on the polygon */
	for(j = 0; j < pts - 1; j++) {
		if(intersect(xa, ya, xc, yc, x[j], y[j], x[j+1], y[j+1])) {
			return pts;
		}
	}

	/* Find the midpoint of line ac */
	xmid = (xa + xc) / 2; ymid = (ya + yc) / 2;

	/* Test to see the midpoint is inside the polygon */
	if(!pnpoly(pts, x, y, xmid, ymid)) {
		return pts;
	}

	/* Print out the coordinates of the triangle.
	 * If we are going to do something with the triangle, we should
	 * do it here.
	 */
	printf("Triangle: (%f, %f) (%f, %f) (%f, %f)\n",
		xa, ya, xb, yb, xc, yc);

	/* Delete point xb, yb from the polygon */
	delval(x, (n + 1) % pts, pts);
	delval(y, (n + 1) % pts, pts);

	return pts - 1;
}

void dopoly(double x[], double y[], int pts) {
	int n = 0;
	double *xcopy, *ycopy;

	/* Make copies of x[] and y[] -- they will be destroyed */
	xcopy = (double *)malloc(sizeof(*x)*pts);
	ycopy = (double *)malloc(sizeof(*y)*pts);
	memcpy(xcopy, x, sizeof(*x)*pts);
	memcpy(ycopy, y, sizeof(*y)*pts);

	pts--;

	/* Iterate through the polygon until only a triangle is left */
	while(pts > 3) {
		pts = get_triangle(xcopy, ycopy, pts, n);
		n = (n + 1) % pts;
	}

	/* Print out the coordinates of the triangle.
	 * If we are going to do something with the triangle, we should
	 * do it here.
	 */
	printf("Triangle: (%f, %f) (%f, %f) (%f, %f)\n",
		x[0], y[0], x[1], y[1], x[2], y[2]);
}

main() {
	dopoly(x, y, ARRSIZE(x));
	printf("Done.\n");
	return 0;
}

