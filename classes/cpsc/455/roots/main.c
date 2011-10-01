#include <stdio.h>
#include <math.h>
#include "zerofinder.h"

#define WS_L (double)10.0
#define WS_R (double)6.371E6

/* Global variable counter for counting number of iterations */
long counter;

/* Use this test b/c we know the exact answer */
double f_sin(double x) {counter++; return sin(x);}

/* Test from the Forsythe book? */
double f1(double x) {counter++; return (pow(x,3)-2.0*x - 5.0);}

/* Another test */
double f2(double x) {counter++; return cos(x) - x;}

/* And another */
double f3(double x) {counter++; return sin(x) - x;}

/* World on a string functions */
/* stringfunc1 - intuitive method */
double stringfunc1(double x) {return tan(x) - x - WS_L/(2 * WS_R);}
/* stringfunc2 - Taylor expansion */
double stringfunc2(double x) {
	return pow(x, 3) / 3 + 2 * pow(x, 5) / 15 + 17 * pow(x, 7) / 135 + 
	       62 * pow(x, 9) / 2835 - WS_L/(2 * WS_R);
}
/* stringfunc3 - Horner's rule */
double stringfunc3(double x) {
/*	return pow(x, 3) * (1/3 + pow(x, 2) * (2/15 + pow(x, 2) * (17/315 +
	       62 * pow(x, 2) / 2835))) - WS_L/(2 * WS_R);*/
	return (1.0/3.0 + (2.0/15.0 + (17.0/315.0 + 62.0/2835.0 * x*x) *
	       x*x) * x*x) * x*x*x - WS_L/(2 * WS_R);
}

/* test
 * tests a zero finder (z) for a function (f), using message msg.
 * tests the function on the interval [a,b].
 * modified from vzeroin.c, a test for Brent's zeroin function (from netlib)
 */
double test(double a, double b, double (*z)(double a, double b,
     double(*f)(double x), double t), double (*f)(double x), char *msg) {
	double root;
	counter = 0;
	printf("\nFor function %s\nin [%g,%g] root is\t%.9e\n",msg,a,b,
	       (root=zeroin(a,b,f,0.0)) );
	printf("No. of iterations\t\t%ld\n", counter);
	printf("Function value at the root found\t%.4e\n", (*f)(root));
	return root;
}

void test1(double (*z)(double a, double b, double(*f)(double x), double t)) {
	test(2.0, 3.0, z, f1, "x^3 - 2*x - 5");
	printf("Exact root is \t\t2.0945514815\n");

	test(2.0, 3.0, z, f_sin, "sin(x)");
	printf("Exact root is %.9Lf\n", M_PI);

	test(2.0, 3.0, z, f2, "cos(x)-x");
	test(-1.0, 3.0, z, f2, "cos(x)-x");
	test(-1.0, 3.0, z, f3, "sin(x)-x");
}

void testf(double (*z)(double a, double b, double(*g)(double x), double t),
           double (*f)(double x), char *msg) {
        double theta;
        double h;

	puts(msg);
	theta = z(0.0, M_PI_2, f, 0.0);
	printf("theta = %f\n", theta);
	printf("tan theta - theta - l/2r = %.4e\n", f(theta));
	h = WS_R/(cos(theta))-WS_R;
	printf("height = %f\n", h);
}

void test2(double (*z)(double a, double b, double (*f)(double x), double t)) {
	testf(z, stringfunc1, "Method 1: intuitive method");
	testf(z, stringfunc2, "Method 2: Taylor expansion");
	testf(z, stringfunc3, "Method 3: Horner's rule");
}

int main() {
	printf("Part 1: Test functions\n");
	printf("\nzeroin:\n");
	test1(zeroin);
	printf("\nsecant:\n");
	test1(secant);
	printf("\nbisection:\n");
	test1(bisection);

	printf("\nPart 2: World on a string\n");
	printf("\nzeroin:\n");
	test2(zeroin);
	printf("\nsecant:\n");
	test2(secant);
	printf("\nbisection:\n");
	test2(bisection);

	return 0;
}
