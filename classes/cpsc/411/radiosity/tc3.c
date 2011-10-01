#include <stdio.h>

main() {
	FILE *fp;
	int i, j;
	float x, y;
	fp = fopen("lampshade.tc", "w");
		for(j = 0; j < 10; j++) {
	for(i = 0; i < 10; i++) {
			if((i%2)&&(j==2)) {
				fprintf(fp, "0.0\t0.0\n1.0\t0.0\n0.0\t1.0\n1.0\t1.0\n");
			} else if((!(i%2))&&(j==7)) {
				fprintf(fp, "0.0\t0.0\n1.0\t0.0\n0.0\t1.0\n1.0\t1.0\n");
			} else if((i%2)&&(j<2 || j>6)){
				fprintf(fp, "0.0\t0.0\n");
			} else if((!(i%2))&&(j<7)){
				fprintf(fp, "0.0\t0.0\n");
			}
		}
	}
	fclose(fp);
}
