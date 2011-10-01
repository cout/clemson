#include <stdio.h>

int dec2bit (int x, int *bitarray, int size) {
        int j;

        for(j = 0; j < size; j++) {
                bitarray[j] = x & 1;
                x >>= 1;
        }

        return x;
}

main() {
	int bitarray[100];
	int i, j;
	
	for(j = 0; j < 100; j++) bitarray[j] = 0;

	for(j = 0; j <= 1000000; j += 5) {
		dec2bit(j, bitarray, 20);
		printf("%d\t", j);
		for(i = 20; i >= 0; i--) printf("%d", bitarray[i]);
		printf("\n");
	}

	return 0;
}

