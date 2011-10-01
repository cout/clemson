#include <stdio.h>
#define POW2(n) (1<<n)
#define HIGH32(n) ((n&0xff000000)>>24)
#define MID32(n) ((n&0xf0)>>4)

main() {
	int j;
	scanf("%d", &j);
	printf("POW2(%d) == %d\n", j, POW2(j));
	printf("HIGH32(%d) == %d\n", j, HIGH32(j));
	printf("MID32(%d) == %d\n", j, MID32(j));
	return 0;
}
