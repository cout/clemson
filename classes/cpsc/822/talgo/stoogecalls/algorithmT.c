#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/major.h>
#include <linux/errno.h>
#include <linux/genhd.h>
#include <linux/malloc.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "../drivers/block/ide.h"
#include "../drivers/block/ide_modes.h"

/* Misc other macros */
#define MIN(a,b) ((a<b)?(a):(b))

/* returns the next request according to algorithmT. CHECK THE RETURN VALUE!
 * it will return NULL if something screws up.  Please catch.
 */
struct request* get_algorithmT(struct request* first, struct request* second, 
	struct request* last, ide_drive_t* drive){
	
	unsigned int minor = 0;
	unsigned long second_abs = 0;
	unsigned long last_abs = 0;
	
	/* find out the absolute block numbers of the second and last 
	 * entries in the queue */
	minor = MINOR(second->rq_dev);
	second_abs = second->sector + 
		drive->part[minor&PARTN_MASK].start_sect + drive->sect0;
	minor = MINOR(last->rq_dev);
	last_abs = last->sector + 
		drive->part[minor&PARTN_MASK].start_sect + drive->sect0;

	/* pick the queue entry that matches the next block in the optimal 
	 * order determined by algorithmT
	 */
	if(drive->optimal_order[drive->optimal_index] == second_abs){
		drive->optimal_index++;
		return(second);
	}
	else if(drive->optimal_order[drive->optimal_index] = last_abs){
		drive->optimal_index++;
		return(last);
	}
	else{
		printk("AlgorithmT ERROR!!!\n");
		printk("Optimal order is apparently not in queue!");
		return(NULL);
	}
}

/* produces an optimal ordering or requests based on the request queue
 * given to it.  Also takes ide_drive_t structure as an argument so that 
 * absolute block numbering can be computed.  Returns number of requests 
 * in this list, or -1 on error.
 */
int algorithmT(struct request* request_q, ide_drive_t *drive)	{

	int i = 0; /* indexing variables */
	int j = 0; 
	int start_j = 0; /* crazy diagonal iteration control */
	unsigned long tmp1 = 0; /* storage for temporary computations */
	unsigned long tmp2 = 0;
	unsigned long tmp3 = 0;
	int count = 0; /* another iteration counter for path traversal */
	int queue_size = 0; 
	unsigned long cur_head_location;
	struct request* tmp_req = NULL;
	unsigned int minor = 0;
	unsigned long *queue = drive->qarr; /* no error checking here! */
	unsigned long *C_L = drive->C_L;
	unsigned long *C_R = drive->C_R;

	/* need to figure out size of queue to start off with */
	/* note that I am not counting the first one- I think it is the current
 	 * head position
 	 */
	for(tmp_req = request_q->next; tmp_req; tmp_req = tmp_req->next){
		queue_size++;
	}

	if(queue_size < 1){
		/* printk("Warning: algorithmT called for empty queue!\n"); */
		/* rebuild_table_flag = 0; */
		drive->optimal_index = 0;
		return(0);
	}

	/* let's check to see if this is the first time this has been called.
	 * If so, initialize the tables to a default size */
#if 0
	if(max_table_size == 0){
		/*
		optimal_order = (unsigned long*)kmalloc((sizeof(unsigned
			long)*DEFAULT_TABLE_SIZE), GFP_KERNEL);
		*/
		C_L = (unsigned long*)kmalloc(
			DSIZE(DEFAULT_TABLE_SIZE), GFP_KERNEL);
		C_R = (unsigned long*)kmalloc(
			DSIZE(DEFAULT_TABLE_SIZE), GFP_KERNEL);
		max_table_size = DEFAULT_TABLE_SIZE;
	}
#endif

	if(queue_size == 1){
		/* shortcut- if only one item then I know the order :) */
		drive->optimal_order[0] = request_q->next->sector + 
		drive->part[minor&PARTN_MASK].start_sect + drive->sect0;
		/* rebuild_table_flag = 0; */
		drive->optimal_index = 0;
		return(1);
	}

	/* see if we have room to store the tables and final order */
	if(T_ALGO_SIZE < queue_size) {
#if 0
		kfree(optimal_order);
		optimal_order = (unsigned long*)kmalloc((sizeof(unsigned long)
				* queue_size), GFP_KERNEL);
		kfree(C_L);
		kfree(C_R);

		max_table_size = queue_size*16/10;
		C_L = (unsigned long*)kmalloc(DSIZE(max_table_size),
					GFP_KERNEL);
		C_R = (unsigned long*)kmalloc(DSIZE(max_table_size),
					GFP_KERNEL);

		if(!C_L || !C_R) {
			printk("AlgorithmT: Unable to allocate memory, "
					"switching to greedy\n");
			drive->sched = 0;
		}
#endif
		printk("Buffer not large enough, temporarily switching to "
				"greedy (%d).\n", queue_size);
		drive->sched = 3;
		return -1;
	}
		
	/* this is where we will store the block numbers from the queue */
	/*
	queue = (unsigned long*)kmalloc(sizeof(unsigned long)*queue_size,
		GFP_KERNEL);
		*/

	/* current head location stuff */
	minor = MINOR(request_q->rq_dev);
	cur_head_location = request_q->sector + 
		drive->part[minor&PARTN_MASK].start_sect + drive->sect0;

	/* now let's grab all of the starting block numbers from the queue */
	i = 0;
	for(tmp_req = request_q->next; tmp_req; tmp_req = tmp_req->next){
		minor = MINOR(tmp_req->rq_dev);
		queue[i] = tmp_req->sector + drive->part[minor&PARTN_MASK].start_sect + drive->sect0;
		i++;
	}

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
		
	/* Ok, let's start filling the table... */
	for(start_j=2; start_j < queue_size; start_j++) {
		for(j=start_j,i=0; j < queue_size; j++,i++) {

			/* work done at each iteration */

			/* left table */
			tmp1 = start_j; /* cheating here to get lr(i+1,j) */
			tmp1 *= abs(queue[i] - queue[i+1]); /* d() */
			tmp1 += C_L[DLOOK(i+1,j)];

			tmp3 = start_j; /* cheating here to get lr(i+1,j) */
			tmp3 *= abs(queue[i] - queue[j]); /* d() */
			tmp2 = tmp3 + C_R[DLOOK(i+1,j)];

			C_L[DLOOK(i,j)] = MIN(tmp1, tmp2);

			/* right table */
			tmp1 = start_j; /* cheating here to get lr(i+1,j) */
			tmp1 *= abs(queue[j] - queue[j-1]); /* d() */
			tmp1 += C_R[DLOOK(i,j-1)];

			tmp2 = tmp3 + C_L[DLOOK(i,j-1)];

			C_R[DLOOK(i,j)] = MIN(tmp1, tmp2);
		}
	}
	
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
			drive->optimal_order[queue_size-count] = queue[i];
			cur_head_location = queue[i];
			i++;
		} else{
			drive->optimal_order[queue_size-count] = queue[j];
			cur_head_location = queue[j];
			j--;
		}
	}
	
	/* rebuild_table_flag = 0; */
	drive->optimal_index = 0;
	/* kfree(queue); */
	return(queue_size);
}


void dump_table(unsigned long *table, int n){

	int i, j;

	for(j=0; j<n; j++){
		for(i=0; i<n; i++){
			if(DISVALID(i,j))
				printk("%d\t", table[DLOOK(i,j)]);
			else
				printk("0\t");
		}
		printk("\n");
	}
	printk("\n");

	return;

}
