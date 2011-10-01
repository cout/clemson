#include <stdio.h>
#define MAX 100
 
main(int argc, char *argv[]) {
	FILE *fp;
	int array[MAX];
	int count, i, j, high, tmp;

	/* Check for arguments */
	if(argc != 2) {
		fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
		exit(1);
	}

	/* Open input file */
	if((fp = fopen(argv[1], "rt")) == NULL) {
		fprintf(stderr, "Unable to open file %s for reading.\n",
			argv[1]);
		exit(1);
	}

	/* Read file */
	for(count = 0; !feof(fp) && count < MAX;
		fscanf(fp, "%d", &array[count++]));

	/* And close it... */
	fclose(fp);

	/* Print initial array contents */
	printf("Initial array:\n");
	for(j = 0; j <= count; j++) printf("%d ", array[j]);
	printf("\n");

	/* Sort the array */
	for(j = count; j >= 0; j--) {
		high = j;		/* Find highest value in the array */
		for(i = 0; i < j; i++) if(array[i] > array[high]) high = i;
		tmp = array[j];		/* And put it in its place */
		array[j] = array[high];
		array[high] = tmp;
	}

	/* Print final array contents */
	printf("Sorted array:\n");
	for(j = 0; j <= count; j++) printf("%d ", array[j]);
	printf("\n");

	/* Done */
	return 0;
}
