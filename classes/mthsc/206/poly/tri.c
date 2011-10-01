#include <stdio.h>

double x[] = {0.0, 3.0, 0.0};
double y[] = {2.0, 1.0, 0.0};

int left(double x[]) {
	if(x[0] < x[1] && x[0] < x[2]) return 0;
	if(x[1] < x[0] && x[1] < x[2]) return 1;
	return 2;
}

int right(double x[]) {
	if(x[0] > x[1] && x[0] > x[2]) return 0;
	if(x[1] > x[0] && x[1] > x[2]) return 1;
	return 2;
}

int middle(double x[]) {
	if((x[0] > x[1] && x[0] < x[2]) || (x[0] < x[1] && x[0] > x[2]))
		return 0;
	if((x[1] > x[0] && x[1] < x[2]) || (x[1] < x[0] && x[1] > x[2]))
		return 1;
	return 2;
}

void dotri(double x[], double y[]) {
	double m, b, xnew, ynew;
	int lx, rx, mx;

	lx = left(x);
	rx = right(x);
	mx = middle(x);

	printf("Left bound: %f\n", x[lx]);
	printf("Right bound: %f\n", x[rx]);
	
	m = (y[lx] - y[rx]) / (x[lx] - x[rx]);
	b = y[lx] - m*x[lx];

	if(y[mx] > m*x[mx] + b) {
		printf("Top bound: (%f, %f) (%f, %f) (%f, %f)\n",
			x[lx], y[lx], x[mx], y[mx], x[rx], y[rx]);
		printf("Lower bound: (%f, %f) (%f, %f)\n",
			x[lx], y[lx], x[rx], y[rx]);
	} else {
		printf("Top bound: (%f, %f) (%f, %f)\n",
			x[lx], y[lx], x[rx], y[rx]);
		printf("Lower bound: (%f, %f) (%f, %f) (%f, %f)\n",
			x[lx], y[lx], x[mx], y[mx], x[rx], y[rx]);
	}
}

main() {
	dotri(x, y);
	printf("Done.\n");
	return 0;
}

