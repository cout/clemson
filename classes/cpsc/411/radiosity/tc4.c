#include <stdio.h>
#include <stdlib.h>

main() {
	FILE *fp;
	int i, j;
	float x, y;
	fp = fopen("lampshade.tc", "w");
	for(i = 0; i < 10; i++) {
		for(j = 0; j < 10; j++) {
			fprintf(fp, "%f\t%f\n",
				random()%2?0.0:1.0,
				random()%2?0.0:1.0
			);
		}
	}
	fclose(fp);
}
