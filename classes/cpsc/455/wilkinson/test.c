/* test.c
 * Tests the Wilkinson parameters for a machine
 * Author: Paul Brannan
 * Last modified: 2/4/98
 */

#include <stdio.h>
#include <math.h>

/* Change this line to test Wilkinson parameters for other types */
#define TEST_TYPE double

int main() {

	TEST_TYPE f, f2;
	int b, t, emin, emax;
	int j;

	/* First, find the base.
	 * ldexp(x,n) scales the number up by b^n, so ldexp(1,1)
	 * should be equal to b.
	 */ 
	b = (int)ldexp(1.0, 1);
	printf("b == %d\n", b);

	/* Now, find the number of b-units in the mantissa.
	 * This can be done by seeing how close we can get to 1 without
	 * actually reaching it, since 1.0 - 1/(b^t) is as high the mantissa
	 * can go.
	 */
	f = 0;
	t = 0;
	while(f != 1.0) f = 1.0 - ldexp(1.0, -(++t));
	printf("t == %d\n", --t);

	/* Next, find the smallest exponent.
	 * This is similar to the previous step, since the smallest
	 * number is (1 / b^-t) * b^emin-1
	 */
	f = 0;
	f2 = 1;
	emin = 0;
	while(f != f2) {
		f2 = f;
		f = ldexp(ldexp(1.0, -t), --emin);
	}
	printf("emin == %d\n", emin);

	/* Finally, find the largest exponent.
	 * A bit tougher, since infinity must be detected; we assume that we
	 * have reached infinity when increasing the number doesn't have any
	 * effect.
	 */
	f = 0;
	f2 = 1;
	emax = 0;
	while(f != f2) {
		f2 = f;
		f = ldexp(1.0 - ldexp(1, -t), ++emax);
	}
		
	printf("emax == %d\n", --emax);

	return 0;
}
