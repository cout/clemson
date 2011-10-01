/*
 *  linux/drivers/block/ide-disk.c	Version 1.08  Dec   10, 1998
 *
 *  Copyright (C) 1994-1998  Linus Torvalds & authors (see below)
 */

/*
 *  Mostly written by Mark Lord <mlord@pobox.com>
 *                and  Gadi Oxman <gadio@netvision.net.il>
 *
 *  See linux/MAINTAINERS for address of current maintainer.
 *
 * This is the IDE/ATA disk driver, as evolved from hd.c and ide.c.
 *
 * Version 1.00		move disk only code from ide.c to ide-disk.c
 *			support optional byte-swapping of all data
 * Version 1.01		fix previous byte-swapping code
 * Version 1.02		remove ", LBA" from drive identification msgs
 * Version 1.03		fix display of id->buf_size for big-endian
 * Version 1.04		add /proc configurable settings and S.M.A.R.T support
 * Version 1.05		add capacity support for ATA3 >= 8GB
 * Version 1.06		get boot-up messages to show full cyl count
 * Version 1.07		disable door-locking if it fails
 * Version 1.08		fixed CHS/LBA translations for ATA4 > 8GB,
 *			process of adding new ATA4 compliance.
 *			fixed problems in allowing fdisk to see
 *			the entire disk.
 */

#define IDEDISK_VERSION	"1.08"

#undef REALLY_SLOW_IO		/* most systems can safely undef this */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/major.h>
#include <linux/errno.h>
#include <linux/genhd.h>
#include <linux/malloc.h>
#include <linux/delay.h>

#include <asm/byteorder.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "ide.h"

/* T_ALGO */
#include <linux/t_algo.h>
#ifdef T_ALGO
extern struct t_algo_stats_s  t_algo_stats[];
extern unsigned int t_algo_flag;
#endif
/*  T_ALGO_END */

...

/*
 * do_rw_disk() issues READ and WRITE commands to a disk,
 * using LBA if supported, or CHS otherwise, to address sectors.
 * It also takes care of issuing special DRIVE_CMDs.
 */
static ide_startstop_t do_rw_disk (ide_drive_t *drive, struct request *rq, unsigned long block)
{
#if defined(PENTIUM_TIMINGS_LAB) || defined(T_ALGO)
struct buffer_head *temp_bh;
#endif

/* T_ALGO */
#ifdef T_ALGO
struct timeval tv;
	/* We get an initial time for all requests at once, so that
	 * we won't interfere with timing.
	 */
	if(t_algo_flag) {
		do_gettimeofday(&tv);
		for(temp_bh = rq->bh; temp_bh != NULL; temp_bh =
			temp_bh->b_reqnext) {
			temp_bh->t_algo_stats.start_of_service = tv;
		}
	}
#endif
/* T_ALGO_END */

	...

}

...

#endif /* MODULE */
