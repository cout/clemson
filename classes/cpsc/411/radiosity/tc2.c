#include <stdio.h>

main() {
	FILE *fp;
	float x, y;
	fp = fopen("lampshade.tc", "w");
	for(y = 1.0; y >-0.41; y-=1.0/7.0) {
		for(x = 0.5; x > -0.01; x-=1.0/9.0) {
			fprintf(fp, "%f\t%f\n", y, x);
		}
		for(x = 1.0; x > 0.49; x-=1.0/9.0) {
			fprintf(fp, "%f\t%f\n", y, x);
		}
	}
	fclose(fp);
}
