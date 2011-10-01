#include <stdio.h>

main() {
	for(int j = 32; j < 127; j++) {
		printf("%3d %c   ", j, j);
	}
	return 0;
}
