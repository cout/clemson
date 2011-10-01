/*
 *  linux/drivers/block/ll_rw_blk.c
 *
 * Copyright (C) 1991, 1992 Linus Torvalds
 * Copyright (C) 1994,      Karl Keyte: Added support for disk statistics
 */

/*
 * This handles all read/write requests to block devices
 */
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/config.h>
#include <linux/locks.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <asm/system.h>
#include <asm/io.h>
#include <linux/blk.h>
#include <linux/raid/md.h>
#include <linux/module.h>
#include <linux/time.h>

/* T_ALGO */
#include "t_algo.h"
/* T_ALGO_END */

...

/* T_ALGO */

unsigned int t_algo_index = 0;
unsigned int t_algo_flag = 0;
unsigned int t_algo_start_flag = 0; 
unsigned int get_times_index = 0;
unsigned int num_of_reqs_completed = 0;
/* We want to leave ourselves some breathing room, hence the +1000 */
struct t_algo_stats_s t_algo_stats[T_NUM_OF_REQS_MSRD+1000];

struct request *get_greedy( struct request *first, struct request *second, struct  request *last)
{
  long second_diff, last_diff;

  /* Find the distance to each request */
  second_diff = abs(first->rq_dev - second->rq_dev);
  last_diff = abs(first->rq_dev - last->rq_dev);

  if(second_diff < last_diff)
    return second;
  else
  {
    if (last_diff < second_diff)
      return last;
    else
    {
	/* If the distance to each request is the same, then compare sectors */
  	second_diff = abs(first->sector - second->sector);
  	last_diff = abs(first->sector - last->sector);

  	if(second_diff < last_diff)
      	  return second;
  	else
  	{
      	  return last;
    	}
    }
  }
} 


/* T_ALGO_END */

...

void make_request(int major, int rw, struct buffer_head * bh)
{
	unsigned int sector, count;
	struct request * req;
	int rw_ahead, max_req, max_sectors, max_segments;
	unsigned long flags;

	count = bh->b_size >> 9;
	sector = bh->b_rsector;

	/* Uhhuh.. Nasty dead-lock possible here.. */
	if (buffer_locked(bh))
		return;
	/* Maybe the above fixes it, and maybe it doesn't boot. Life is interesting */
	lock_buffer(bh);
	if (!buffer_lowprio(bh))
		io_events[major]++;

...

/* fill up the request-info, and add it to the queue */
	req->cmd = rw;
	req->errors = 0;
	req->sector = sector;
	req->nr_sectors = count;
	req->nr_segments = 1;
	req->current_nr_sectors = count;
	req->buffer = bh->b_data;
	req->sem = NULL;
/* T_ALGO */
#ifdef T_ALGO
	if(t_algo_flag)
          do_gettimeofday(&(bh->t_algo_stats.start_of_wait));
#endif
/* T_ALGO_END */
	req->bh = bh;
	req->bhtail = bh;
	req->next = NULL;
	add_request(major+blk_dev,req);
	return;

end_io:
	bh->b_end_io(bh, test_bit(BH_Uptodate, &bh->b_state));
}

/* This function can be used to request a number of buffers from a block
   device. Currently the only restriction is that all buffers must belong to
   the same device */

void ll_rw_block(int rw, int nr, struct buffer_head * bh[])
{
	unsigned int major;
	int correct_size;
	struct blk_dev_struct * dev;
	int i;
	ide_drive_t *drive;
	unsigned long flags;

	/* Make sure that the first block contains something reasonable */
	while (!*bh) {
		bh++;
		if (--nr <= 0)
			return;
	}

	dev = NULL;
	if ((major = MAJOR(bh[0]->b_dev)) < MAX_BLKDEV)
		dev = blk_dev + major;
	if (!dev || !dev->request_fn) {
		printk(KERN_ERR
	"ll_rw_block: Trying to read nonexistent block-device %s (%ld)\n",
		kdevname(bh[0]->b_dev), bh[0]->b_blocknr);
		goto sorry;
	}
	/* Determine correct block size for this device.  */
	correct_size = BLOCK_SIZE;
	if (blksize_size[major]) {
		i = blksize_size[major][MINOR(bh[0]->b_dev)];
		if (i)
			correct_size = i;
	}

	/* Verify requested block sizes.  */
	for (i = 0; i < nr; i++) {
		if (bh[i] && bh[i]->b_size != correct_size) {
			printk(KERN_NOTICE "ll_rw_block: device %s: "
			       "only %d-char blocks implemented (%lu)\n",
			       kdevname(bh[0]->b_dev),
			       correct_size, bh[i]->b_size);
			goto sorry;
		}

		/* Md remaps blocks now */
		bh[i]->b_rdev = bh[i]->b_dev;
		bh[i]->b_rsector=bh[i]->b_blocknr*(bh[i]->b_size >> 9);
#ifdef CONFIG_BLK_DEV_MD
		if (major==MD_MAJOR &&
			md_map (bh[i]->b_dev, &bh[i]->b_rdev,
			    &bh[i]->b_rsector, bh[i]->b_size >> 9)) {
		        printk (KERN_ERR
				"Bad md_map in ll_rw_block\n");
		        goto sorry;
		}
#endif
	}

	if ((rw == WRITE || rw == WRITEA) && is_read_only(bh[0]->b_dev)) {
		printk(KERN_NOTICE "Can't write to read-only device %s\n",
		       kdevname(bh[0]->b_dev));
		goto sorry;
	}

	for (i = 0; i < nr; i++) {
		if (bh[i]) {
			set_bit(BH_Req, &bh[i]->b_state);
#ifdef CONFIG_BLK_DEV_MD
			if (MAJOR(bh[i]->b_dev) == MD_MAJOR) {
				md_make_request(bh[i], rw);
				continue;
			}
#endif
			make_request(MAJOR(bh[i]->b_rdev), rw, bh[i]);
/* T_ALGO */
#ifdef T_ALGO
		drive = get_info_ptr(bh[i]->b_rdev);
		if(drive) {
			if(drive->sched == 3) drive->sched = 2;
			if(drive->sched == 2) {
				spin_lock_irqsave(&io_request_lock,flags);
				algorithmT(*get_queue(bh[i]->b_rdev), drive);
				spin_unlock_irqrestore(&io_request_lock,flags);
			}
		}
#endif
/* T_ALGO_END */
		}
	}
	return;

      sorry:
	for (i = 0; i < nr; i++) {
		if (bh[i]) {
			clear_bit(BH_Dirty, &bh[i]->b_state);
			clear_bit(BH_Uptodate, &bh[i]->b_state);
			bh[i]->b_end_io(bh[i], 0);
		}
	}
	return;
}

#ifdef CONFIG_STRAM_SWAP
extern int stram_device_init( void );
#endif

/*
 * First step of what used to be end_request
 *
 * 0 means continue with end_that_request_last,
 * 1 means we are done
 */

int 
end_that_request_first( struct request *req, int uptodate, char *name ) 
{
	struct buffer_head * bh;
	int nsect;

	req->errors = 0;
	if (!uptodate) {
		printk("end_request: I/O error, dev %s (%s), sector %lu\n",
			kdevname(req->rq_dev), name, req->sector);
		if ((bh = req->bh) != NULL) {
			nsect = bh->b_size >> 9;
			req->nr_sectors--;
			req->nr_sectors &= ~(nsect - 1);
			req->sector += nsect;
			req->sector &= ~(nsect - 1);
		}
	}

	if ((bh = req->bh) != NULL) {

/* T_ALGO */
#ifdef T_ALGO
	/* Service completion of ONE buffer header. */
	if(t_algo_flag)
	{
          do_gettimeofday(&(req->bh->t_algo_stats.service_comp));
	  t_algo_stats[num_of_reqs_completed] = req->bh->t_algo_stats;
	  ++num_of_reqs_completed;
	  if(num_of_reqs_completed == T_NUM_OF_REQS_MSRD)
	    t_algo_flag = 0;

   	/* Allow for an initial stabilization period */
   	if(num_of_reqs_completed == (T_NUM_OF_REQS_IGND))
   	{
     	  if(t_algo_start_flag == 0)
     	  {
       		num_of_reqs_completed = 0;
             	do_gettimeofday(&(req->bh->t_algo_stats.service_comp));
	       	t_algo_stats[num_of_reqs_completed] = req->bh->t_algo_stats;
	      	++num_of_reqs_completed;
	       	t_algo_start_flag = 1;
	   }
	 }

	}
#endif
/* T_ALGO_END */

#if defined(PENTIUM_TIMINGS_LAB)
		__cli();
		getTimer();
		sumOfWaits += calcTimer(req->bh->start_of_wait);
		sumOfServices += calcTimer(req->bh->start_of_service);
		__sti();
#endif

		req->bh = bh->b_reqnext;
		bh->b_reqnext = NULL;
		bh->b_end_io(bh, uptodate);
		if ((bh = req->bh) != NULL) {
			req->current_nr_sectors = bh->b_size >> 9;
			if (req->nr_sectors < req->current_nr_sectors) {
				req->nr_sectors = req->current_nr_sectors;
				printk("end_request: buffer-list destroyed\n");
			}
			req->buffer = bh->b_data;
			return 1;
		}
	}
	return 0;
}

...

EXPORT_SYMBOL(end_that_request_last);
