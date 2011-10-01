/*
 *  linux/drivers/block/ide.c	Version 6.19  December 28, 1999
 *
 *  Copyright (C) 1994-1998  Linus Torvalds & authors (see below)
 */

/*
 *  Mostly written by Mark Lord  <mlord@pobox.com>
 *                and Gadi Oxman <gadio@netvision.net.il>
 *
 *  See linux/MAINTAINERS for address of current maintainer.
 *
 * This is the multiple IDE interface driver, as evolved from hd.c.
 * It supports up to MAX_HWIFS IDE interfaces, on one or more IRQs (usually 14 & 15).
 * There can be up to two drives per interface, as per the ATA-2 spec.
 *
 * Primary:    ide0, port 0x1f0; major=3;  hda is minor=0; hdb is minor=64
 * Secondary:  ide1, port 0x170; major=22; hdc is minor=0; hdd is minor=64
 * Tertiary:   ide2, port 0x???; major=33; hde is minor=0; hdf is minor=64
 * Quaternary: ide3, port 0x???; major=34; hdg is minor=0; hdh is minor=64
 * ...
 *
 *  From hd.c:
 *  |
 *  | It traverses the request-list, using interrupts to jump between functions.
 *  | As nearly all functions can be called within interrupts, we may not sleep.
 *  | Special care is recommended.  Have Fun!
 *  |
 *  | modified by Drew Eckhardt to check nr of hd's from the CMOS.
 *  |
 *  | Thanks to Branko Lankester, lankeste@fwi.uva.nl, who found a bug
 *  | in the early extended-partition checks and added DM partitions.
 *  |
 *  | Early work on error handling by Mika Liljeberg (liljeber@cs.Helsinki.FI).
 *  |
 *  | IRQ-unmask, drive-id, multiple-mode, support for ">16 heads",
 *  | and general streamlining by Mark Lord (mlord@pobox.com).
 *
 *  October, 1994 -- Complete line-by-line overhaul for linux 1.1.x, by:
 *
 *	Mark Lord	(mlord@pobox.com)		(IDE Perf.Pkg)
 *	Delman Lee	(delman@mipg.upenn.edu)		("Mr. atdisk2")
 *	Scott Snyder	(snyder@fnald0.fnal.gov)	(ATAPI IDE cd-rom)
 *
 *  This was a rewrite of just about everything from hd.c, though some original
 *  code is still sprinkled about.  Think of it as a major evolution, with
 *  inspiration from lots of linux users, esp.  hamish@zot.apana.org.au
 *
 *  Version 1.0 ALPHA	initial code, primary i/f working okay
 *  Version 1.3 BETA	dual i/f on shared irq tested & working!
 *  Version 1.4 BETA	added auto probing for irq(s)
 *  Version 1.5 BETA	added ALPHA (untested) support for IDE cd-roms,
 *  ...
 * Version 5.50		allow values as small as 20 for idebus=
 * Version 5.51		force non io_32bit in drive_cmd_intr()
 *			change delay_10ms() to delay_50ms() to fix problems
 * Version 5.52		fix incorrect invalidation of removable devices
 *			add "hdx=slow" command line option
 * Version 5.60		start to modularize the driver; the disk and ATAPI
 *			 drivers can be compiled as loadable modules.
 *			move IDE probe code to ide-probe.c
 *			move IDE disk code to ide-disk.c
 *			add support for generic IDE device subdrivers
 *			add m68k code from Geert Uytterhoeven
 *			probe all interfaces by default
 *			add ioctl to (re)probe an interface
 * Version 6.00		use per device request queues
 *			attempt to optimize shared hwgroup performance
 *			add ioctl to manually adjust bandwidth algorithms
 *			add kerneld support for the probe module
 *			fix bug in ide_error()
 *			fix bug in the first ide_get_lock() call for Atari
 *			don't flush leftover data for ATAPI devices
 * Version 6.01		clear hwgroup->active while the hwgroup sleeps
 *			support HDIO_GETGEO for floppies
 * Version 6.02		fix ide_ack_intr() call
 *			check partition table on floppies
 * Version 6.03		handle bad status bit sequencing in ide_wait_stat()
 * Version 6.10		deleted old entries from this list of updates
 *			replaced triton.c with ide-dma.c generic PCI DMA
 *			added support for BIOS-enabled UltraDMA
 *			rename all "promise" things to "pdc4030"
 *			fix EZ-DRIVE handling on small disks
 * Version 6.11		fix probe error in ide_scan_devices()
 *			fix ancient "jiffies" polling bugs
 *			mask all hwgroup interrupts on each irq entry
 * Version 6.12		integrate ioctl and proc interfaces
 *			fix parsing of "idex=" command line parameter
 * Version 6.13		add support for ide4/ide5 courtesy rjones@orchestream.com
 * Version 6.14		fixed IRQ sharing among PCI devices
 * Version 6.15		added SMP awareness to IDE drivers
 * Version 6.16		fixed various bugs; even more SMP friendly
 * Version 6.17		fix for newest EZ-Drive problem
 * Version 6.18		default unpartitioned-disk translation now "BIOS LBA"
 * Version 6.19		Added SMP support; fixed multmode issues.  -ml
 *
 *  Some additional driver compile-time options are in ide.h
 *
 *  To do, in likely order of completion:
 *	- modify kernel to obtain BIOS geometry for drives on 2nd/3rd/4th i/f
*/

#undef REALLY_SLOW_IO		/* most systems can safely undef this */

#define _IDE_C			/* Tell ide.h it's really us */

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
#include <linux/pci.h>
#include <linux/delay.h>

#include <asm/byteorder.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/bitops.h>

#include "ide.h"
#include "ide_modes.h"

#include <linux/t_algo.h>
#include "t_algo.h"

#ifdef CONFIG_KMOD
#include <linux/kmod.h>
#endif /* CONFIG_KMOD */

...

/*
 * Do not even *think* about calling this!
 */
static void init_hwif_data (unsigned int index)
{
	unsigned int unit;
	ide_hwif_t *hwif = &ide_hwifs[index];

	/* bulk initialize hwif & drive info with zeros */
	memset(hwif, 0, sizeof(ide_hwif_t));

	/* fill in any non-zero initial values */
	hwif->index     = index;
	ide_init_hwif_ports(hwif->io_ports, ide_default_io_base(index), &hwif->irq);
	hwif->noprobe	= !hwif->io_ports[IDE_DATA_OFFSET];
#ifdef CONFIG_BLK_DEV_HD
	if (hwif->io_ports[IDE_DATA_OFFSET] == HD_DATA)
		hwif->noprobe = 1; /* may be overridden by ide_setup() */
#endif /* CONFIG_BLK_DEV_HD */
	hwif->major	= ide_hwif_to_major[index];
	hwif->name[0]	= 'i';
	hwif->name[1]	= 'd';
	hwif->name[2]	= 'e';
	hwif->name[3]	= '0' + index;
	for (unit = 0; unit < MAX_DRIVES; ++unit) {
		ide_drive_t *drive = &hwif->drives[unit];

		drive->media			= ide_disk;
		drive->select.all		= (unit<<4)|0xa0;
		drive->hwif			= hwif;
		drive->ctl			= 0x08;
		drive->ready_stat		= READY_STAT;
		drive->bad_wstat		= BAD_W_STAT;
		drive->special.b.recalibrate	= 1;
		drive->special.b.set_geometry	= 1;
		drive->name[0]			= 'h';
		drive->name[1]			= 'd';
		drive->name[2]			= 'a' + (index * MAX_DRIVES) + unit;
/* T_ALGO */
		drive->sched			= 0;
		drive->C_R = drive->C_L		= 0;
/* T_ALGO_END */
	}
}

/*
 * init_ide_data() sets reasonable default values into all fields
 * of all instances of the hwifs and drives, but only on the first call.
 * Subsequent calls have no effect (they don't wipe out anything).
 *
 * This routine is normally called at driver initialization time,
 * but may also be called MUCH earlier during kernel "command-line"
 * parameter processing.  As such, we cannot depend on any other parts
 * of the kernel (such as memory allocation) to be functioning yet.
 *
 * This is too bad, as otherwise we could dynamically allocate the
 * ide_drive_t structs as needed, rather than always consuming memory
 * for the max possible number (MAX_HWIFS * MAX_DRIVES) of them.
 */
#define MAGIC_COOKIE 0x12345678
static void init_ide_data (void)
{
	unsigned int index;
	static unsigned long magic_cookie = MAGIC_COOKIE;

	if (magic_cookie != MAGIC_COOKIE)
		return;		/* already initialized */
	magic_cookie = 0;

	for (index = 0; index < MAX_HWIFS; ++index)
		init_hwif_data(index);

	idebus_parameter = 0;
	system_bus_speed = 0;
}

...

void ide_end_request(byte uptodate, ide_hwgroup_t *hwgroup)
{
	struct request *rq;

/* T_ALGO */
#ifdef T_ALGO
	struct request *tempReq, *nodeReq;
#endif
/* T_ALGO_END */

	unsigned long flags;
	/*
	struct timeval present_time;
        struct timeval diff_timeval;
	*/
	
	spin_lock_irqsave(&io_request_lock, flags);
	rq = hwgroup->rq;

#if 0
	/*******/
	do_gettimeofday(&present_time);
	no_of_reqs_seen++;
        sub_timeval(&diff_timeval,&present_time,&rq->start_of_service);
        sum_of_services+=(diff_timeval.tv_sec*1000000)+diff_timeval.tv_usec;
        sub_timeval(&diff_timeval,&rq->start_of_service,&rq->start_of_wait);
        sum_of_waits+=(diff_timeval.tv_sec*1000000)+diff_timeval.tv_usec;
	/*******/
#endif

	if (!end_that_request_first(rq, uptodate, hwgroup->drive->name)) {
		add_blkdev_randomness(MAJOR(rq->rq_dev));

/* T_ALGO */
#ifdef T_ALGO
	if(rq->next != NULL && rq->next->next != NULL) {
		/* Find the next-to-last request -- we really should
		 * store a pointer so we don't have to do this.
		 */
		for(nodeReq = rq; nodeReq->next->next != NULL; nodeReq =
				nodeReq->next) ;

		/* Figure out which request to process next */
		if(hwgroup->drive->sched == 2) {
			tempReq = get_algorithmT(rq, rq->next,
					nodeReq->next, hwgroup->drive);
		} else if(hwgroup->drive->sched == 1 ||
		          hwgroup->drive->sched == 3) {
			/* greedy, or temporary greedy */
			tempReq = get_greedy(rq, rq->next,
					nodeReq->next);
		} else {
			tempReq = rq->next;
		}

		if(tempReq->next == nodeReq->next) {
		/* We picked the last reqest, so move it to
		 * the front.
	 	*/
			nodeReq->next->next = rq->next;
			rq->next = tempReq->next;
			nodeReq->next = NULL;
		}
	}
#endif
/* T_ALGO_END */

		hwgroup->drive->queue = rq->next;
        	blk_dev[MAJOR(rq->rq_dev)].current_request = NULL;
        	hwgroup->rq = NULL;
		end_that_request_last(rq);
	}
	spin_unlock_irqrestore(&io_request_lock, flags);
}

...

/*
 * get_info_ptr() returns the (ide_drive_t *) for a given device number.
 * It returns NULL if the given device number does not match any present drives.
 */
/* T_ALGO -- we need this function, so now it's no longer static */
ide_drive_t *get_info_ptr (kdev_t i_rdev)
{
	int		major = MAJOR(i_rdev);
	unsigned int	h;

	for (h = 0; h < MAX_HWIFS; ++h) {
		ide_hwif_t  *hwif = &ide_hwifs[h];
		if (hwif->present && major == hwif->major) {
			unsigned unit = DEVICE_NR(i_rdev);
			if (unit < MAX_DRIVES) {
				ide_drive_t *drive = &hwif->drives[unit];
				if (drive->present)
					return drive;
			}
			break;
		}
	}
	return NULL;
}

...

/* T_ALGO */
extern struct t_algo_stats_s t_algo_stats[];
extern unsigned int get_times_index;
/* T_ALGO_END */

static int ide_ioctl (struct inode *inode, struct file *file,
			unsigned int cmd, unsigned long arg)
{
	int err, major, minor;
/* T_ALGO */
	int ii;
/* T_ALGO_END */
	ide_drive_t *drive;
	struct request rq;
	kdev_t dev;
	ide_settings_t *setting;

	if (!inode || !(dev = inode->i_rdev))
		return -EINVAL;
	major = MAJOR(dev); minor = MINOR(dev);
	if ((drive = get_info_ptr(inode->i_rdev)) == NULL)
		return -ENODEV;

	if ((setting = ide_find_setting_by_ioctl(drive, cmd)) != NULL) {
		if (cmd == setting->read_ioctl) {
			err = ide_read_setting(drive, setting);
			return err >= 0 ? put_user(err, (long *) arg) : err;
		} else {
			if ((MINOR(inode->i_rdev) & PARTN_MASK))
				return -EINVAL;
			return ide_write_setting(drive, setting, arg);
		}
	}

	ide_init_drive_cmd (&rq);
	switch (cmd) {

...

		case GET_20000_TIMES:
		/*
			return get_20000_times((struct t_algo_stats_s **)arg);
		*/
		 	if(get_times_index < T_NUM_OF_REQS_MSRD)	
			{
			  copy_to_user( (struct t_algo_stats_s *)arg, &t_algo_stats[get_times_index], sizeof(struct t_algo_stats_s));
			  get_times_index++;
			}
			return 0;

		case START_20000_TIMES:
			return start_20000_times((int)arg); 

		case PRINT_20000_TIMES:
			for( ii = 0; ii < arg; ii++)
			{
			  printk("S_o_W = %d  ", (int)t_algo_stats[ii].start_of_wait.tv_sec);
			  printk("%d\n", (int)t_algo_stats[ii].start_of_wait.tv_usec);
			  printk("S_o_S = %d  ", (int)t_algo_stats[ii].start_of_service.tv_sec);
			  printk("%d\n", (int)t_algo_stats[ii].start_of_service.tv_usec);
			  printk("S_C_T = %d  ", (int)t_algo_stats[ii].service_comp.tv_sec);
			  printk("%d\n", (int)t_algo_stats[ii].service_comp.tv_usec);
			}

		case HDIO_SET_SCHED:
			if(arg == 0 || arg == 1 || arg == 2)
				drive->sched = arg;
			if(arg == 2) {
				/* This wouldn't work if we were to boot
				 * using algorithm T.  I'm not sure how to
				 * deal with this.
				 */
				if(drive && drive->queue)
					algorithmT(drive->queue, drive);
				if(!drive->C_R) drive->C_R =
					kmalloc(DSIZE(T_ALGO_SIZE), GFP_KERNEL);
				if(!drive->C_L) drive->C_L =
					kmalloc(DSIZE(T_ALGO_SIZE), GFP_KERNEL);
			}
			return 0;

		default:
			if (drive->driver != NULL)
				return DRIVER(drive)->ioctl(drive, inode, file, cmd, arg);
			return -EPERM;
	}
}

...

#endif /* MODULE */
