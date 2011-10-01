#include <stdio.h>

#define WS_L 10
#define WS_R 6.371E6

double sf1(double x) {
        return x*x*x / 3 + 2 * x*x*x*x*x / 15 + 17 * x*x*x*x*x*x*x / 135 +
               62 * x*x*x*x*x*x*x*x*x / 2835 - WS_L/(2 * WS_R);
}

double sf2(double x) {
        return (1.0/3.0 + (2.0/15.0 + (17.0/315.0 + 62.0/2835.0 * x*x) *
               x*x) * x*x) * x*x*x - WS_L/(2 * WS_R);
}

int main() {
	double j;

	for(j = 0; j < 1; j += 0.1)
		printf("%.20f %.20f %.20f\n", sf1(j), sf2(j), sf1(j) / sf2(j));

	return 0;
}
