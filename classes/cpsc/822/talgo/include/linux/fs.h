#ifndef _LINUX_FS_H
#define _LINUX_FS_H

/*
 * This file has definitions for some important file table
 * structures etc.
 */

#include <linux/config.h>
#include <linux/linkage.h>
#include <linux/limits.h>
#include <linux/wait.h>
#include <linux/types.h>
#include <linux/vfs.h>
#include <linux/net.h>
#include <linux/kdev_t.h>
#include <linux/ioctl.h>
#include <linux/list.h>
#include <linux/dcache.h>
#include <linux/stat.h>

#include <asm/atomic.h>
#include <linux/bitops.h>
#include <asm/cache.h>
#include <linux/stddef.h>	/* just in case the #define NULL previously in here was needed */

#include <linux/time.h>
struct poll_table_struct;

/* T_ALGO */
#include <linux/t_algo.h>
/* T_ALGO_END */

...

struct buffer_head {

      /* T_ALGO  */
        unsigned long request_num;
	struct t_algo_stats_s t_algo_stats;

      /* T_ALGO_END  */
#if defined(PENTIUM_TIMINGS_LAB)
	struct pentium_timing_s start_of_wait, start_of_service;
	int pid;
#endif

	...

	/*
	 * I/O completion
	 */
	void (*b_end_io)(struct buffer_head *bh, int uptodate);
	void *b_dev_id;
};

...

#endif
