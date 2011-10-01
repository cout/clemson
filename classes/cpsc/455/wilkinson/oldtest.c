#include <stdio.h>
#include <limits.h>

#define popcount(x, s) pcount((char *)&x, sizeof(s))
int pcount (char *x, int s) {
	int i, j, t = 0;

	for (j = 0; j < s / sizeof(char); j++)
		for (i = 0; i < CHAR_BIT; i++)
			t += (*(x + j) >> i) & 1;

	return t;
}

int main() {
	float f, g;
	int b, t, emin, emax;

	/* Find the base of the system */
	/* If a number is in base b, then squaring a number x==b should
	 * increase the exponent by 1.  If the exponent is already 0, then
	 * the bit population count should increase by 1 if the bits are
	 * stored in a reasonable manner.  This shouldn't happen unless x>=b.
	 */
	b = 1;
	do {
		b++;
		f = b;
		g = b * b;
	} while (popcount(f, float) + 1 != popcount(g, float));
		
	printf("b = %d\n", b); 

	/* Find the number of b-units in the system */
	/* I have no clue how to do this. */

	return 0;
}
