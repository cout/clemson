/**********************************************
 * CPSC 822 Team 1
 * project 3
 * User level algorithm T example
 **********************************************/

/* notes:
 * This stuff doesn't have any optimizations yet...
 * here are some possibilities:
 * 1) The math stuff (keeping track of repeated values if often enought
 * to be significant)
 * 2) managing table size over iterations.  This code dynamically
 * allocates room for the table each time it runs- we should hang 
 * onto the existing space unless the queue gets larger than the 
 * current max size, THEN reallocate.  Otherwise keep reusing the 
 * existing table space.
 * 4) is there a way to compute the path as we go rather than having to
 * run through computations after the table is built?
 */

/* code needs to be cleaned- I stretched out everything for clarity */
/* also note: no error checking in here. */

#include <stdio.h>
#include <stdlib.h>

/* Diagonal lookup macros */
#define DLOOK(i,j) ((j)*((j)+1)/2+(i))
#define DSIZE(i) ((i)*((i)+1)/2+(i)+1)*sizeof(int)
#define DISVALID(i,j) ((i)<=(j))

/* Misc other macros */
#define MIN(a,b) ((a<b)?(a):(b))

void dump_table(int *table, int n);

static int default_queue[] = {60, 61, 80, 32, 41};
static int default_queue_size = 5;
static int default_cur_head_location = 50;

int main(int argc, char **argv)	{

	int *C_L = NULL;  /* tables */
	int *C_R = NULL;
	int *optimal_order = NULL; /* final order */
	int i = 0; /* indexing variables */
	int j = 0; 
	int start_j = 0; /* crazy diagonal iteration control */
	int tmp1 = 0; /* storage for temporary computations */
	int tmp2 = 0;
	int tmp3 = 0;
	int count = 0; /* another iteration counter for path traversal */
	int queue_size, *queue, cur_head_location;

	/* Set the default values */
	if(argc == 1) {
		queue_size = default_queue_size;
		queue = malloc(sizeof(int)*queue_size);
		memcpy(queue, default_queue, sizeof(int)*queue_size);
		cur_head_location = default_cur_head_location;
	} else {
		queue_size = argc-2;
		queue = malloc(sizeof(int)*queue_size);
		for(j = 0; j < queue_size; j++)
			queue[j] = atoi(argv[j+2]);
		cur_head_location = atoi(argv[1]);
	}

	/* make room to store the new queue order */
	optimal_order = (int*)malloc(queue_size*sizeof(int));

	/* make room for the tables */
	C_L = (int*)malloc(DSIZE(queue_size));
	C_R = (int*)malloc(DSIZE(queue_size));

	/* set the diagonals to zero */
	for(i=0; i<queue_size; i++){
		C_L[DLOOK(i,i)] = 0;
		C_R[DLOOK(i,i)] = 0;
	}

	/* The first iteration is the same for both tables */
	for(i=0; i < queue_size; i++) {
		C_L[DLOOK(i,i+1)] = C_R[DLOOK(i,i+1)] =
			abs(queue[i] - queue[i+1]); /* d() */
	}
		
	printf("C_L:\n");
	dump_table(C_L, queue_size);
	printf("C_R:\n");
	dump_table(C_R, queue_size);

	/* Ok, let's start filling the table... */
	for(start_j=2; start_j < queue_size; start_j++) {
		for(j=start_j,i=0; j < queue_size; j++,i++) {

			/* work done at each iteration */
			/* all this stuff can be condensed.  I stretched it out for
			 * clarity until we know this works */

			/* left table */
			tmp1 = start_j; /* cheating here to get lr(i+1,j) */
			tmp1 *= abs(queue[i] - queue[i+1]); /* d() */
			tmp1 += C_L[DLOOK(i+1,j)];

			tmp3 = start_j; /* cheating here to get lr(i+1,j) */
			tmp3 *= abs(queue[i] - queue[j]); /* d() */
			tmp2 = tmp3 + C_R[DLOOK(i+1,j)];

			printf("%d %d | ", tmp1, tmp2);
			C_L[DLOOK(i,j)] = MIN(tmp1, tmp2);

			/* right table */
			tmp1 = start_j; /* cheating here to get lr(i+1,j) */
			tmp1 *= abs(queue[j] - queue[j-1]); /* d() */
			tmp1 += C_R[DLOOK(i,j-1)];

			tmp2 = tmp3 + C_L[DLOOK(i,j-1)];

			printf("%d %d\n", tmp1, tmp2);
			C_R[DLOOK(i,j)] = MIN(tmp1, tmp2);
		}
	}

	/* show the resulting tables */
	printf("C_L:\n");
	dump_table(C_L, queue_size);
	printf("C_R:\n");
	dump_table(C_R, queue_size);
	
	/* now we need to compute the optimum path throught the queue based
	 * on these tables 
	 */
	/* notice now that things are swapped up a bit:
	 * i now indexes the leftmost item in the queue
	 * j now indexes the rightmost item in the queue
	 */
	i = 0;
	j = queue_size-1;
	for(count = queue_size; count > 0; count--){
		tmp1 = count; /* cheating again for lr */
		tmp1 *= abs(cur_head_location - queue[i]); /* d() */
		tmp1 += C_L[DLOOK(queue_size-count,queue_size-1)];

		tmp2 = count; /* cheating again for lr */
		tmp2 *= abs(cur_head_location - queue[j]); /* d() */
		tmp2 += C_R[DLOOK(queue_size-count,queue_size-1)];

		/* choose the minimum cost */
		if(tmp1 < tmp2){
			optimal_order[queue_size-count] = queue[i];
			cur_head_location = queue[i];
			i++;
		} else{
			optimal_order[queue_size-count] = queue[j];
			cur_head_location = queue[j];
			j--;
		}
	}
	
	/* print out the optimal ordering */
	printf("Optimal order:\n");
	for(i=0; i<queue_size; i++){
		printf("%d\t", optimal_order[i]);
	}
	printf("\n");

	/* free up the tables */
	free(C_L);
	free(C_R);


	return(0);
}


void dump_table(int *table, int n){

	int i, j;

	for(j=0; j<n; j++){
		for(i=0; i<n; i++){
			if(DISVALID(i,j))
				printf("%d\t", table[DLOOK(i,j)]);
			else
				printf("0\t");
		}
		printf("\n");
	}
	printf("\n");

	return;

}
