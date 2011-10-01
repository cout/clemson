#include <stdio.h>
#include <math.h>

#define sign(x) fabs(x) / x

/* Global variables are usually bad, but this program is short */
double j, i;

double cube (double d);
double poly (double d);
double bisection (double xl, double xr, double tolerance, double func());
double secant (double xl, double xr, double tolerance, double func());

int main() {
	double xl = -100.0, xr = 100.0, t = 0.0000001;
	puts("\nTest 1: x^3 = 0.");
	printf("xl = %f; xr = %f; tolerance = %f\n", -100.0, 150.0, t);
	printf("Bisection: x = %f\n", bisection(-100.0, 150.0, t, &cube));
	printf("xl = %f; xr = %f; tolerance = %f\n", -0.001, 0.001, t);
	printf("Secant: x = %f\n", secant(-0.001, 0.001, t, &cube));
	puts("\nTest 2: Matrix test (x^j = 10^i).");
	for(j = 1; j < 10; j++) for(i = 1; i < 10; i++) {
		printf("j = %f; i = %f\n", j, i);
		printf("Bisection: x = %f\n", bisection(xl, xr, t, &poly));
		printf("Secant: x = %f\n", secant(xl, xr, t, &poly));
	}
}

double cube (double x) {return x*x*x;}

double poly (double x) {return pow(x, j) - pow(10, i);}

double bisection (double xl, double xr, double tolerance, double func()) {
	double d = (xl + xr) / 2, f = (*func)(d);
	if (fabs(f) <= tolerance) return d;
	if (sign(f) == sign((*func)(xl)))
		return bisection(d, xr, tolerance, func);
	return bisection(xl, d, tolerance, func);
}

double secant (double xold, double xnew, double tolerance, double func()) {
	double fxold = (*func)(xold), fxnew = (*func)(xnew);
	if (fabs(fxnew) <= tolerance) return xnew;
	return secant(xnew, xnew + fxnew * (xold - xnew) / (fxold - fxnew),
                      tolerance, func);
}
