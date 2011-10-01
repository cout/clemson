#include <stdio.h>

static float coords0x[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0};
static float coords0y[] = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};
static float coords1x[] = {0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
static float coords1y[] = {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};

main() {
	FILE *fp;
	int i, j;
	float x, y;
	fp = fopen("lampshade.tc", "w");
	for(i = 0; i < 10; i++) {
		for(j = 0; j < 10; j++) {
			if(j%2) {
				fprintf(fp, "%f\t%f\n",
					coords0x[j], coords0y[j]);
			} else {
				fprintf(fp, "%f\t%f\n",
					coords1x[j], coords0y[j]);
			}
		}
	}
	fclose(fp);
}
