#ifndef _IDE_H
#define _IDE_H
/*
 *  linux/drivers/block/ide.h
 *
 *  Copyright (C) 1994-1998  Linus Torvalds & authors
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/hdreg.h>
#include <linux/blkdev.h>
#include <linux/proc_fs.h>
#include <asm/ide.h>

/* T_ALGO */
#include <linux/t_algo_defs.h>
/* T_ALGO_END */

...

typedef union {
	unsigned all			: 8;	/* all of the bits together */
	struct {
		unsigned set_geometry	: 1;	/* respecify drive geometry */
		unsigned recalibrate	: 1;	/* seek to cyl 0      */
		unsigned set_multmode	: 1;	/* set multmode count */
		unsigned set_tune	: 1;	/* tune interface for drive */
		unsigned reserved	: 4;	/* unused */
		} b;
	} special_t;

typedef struct ide_drive_s {
	struct request		*queue;	/* request queue */
	struct ide_drive_s 	*next;	/* circular list of hwgroup drives */
	unsigned long sleep;		/* sleep until this time */
	unsigned long service_start;	/* time we started last request */
	unsigned long service_time;	/* service time of last request */
	special_t	special;	/* special action flags */
	byte     keep_settings;		/* restore settings after drive reset */
	byte     using_dma;		/* disk is using dma for read/write */
	byte     waiting_for_dma;	/* dma currently in progress */
	byte     unmask;		/* flag: okay to unmask other irqs */
	byte     slow;			/* flag: slow data port */
	byte     bswap;			/* flag: byte swap data */
	byte     dsc_overlap;		/* flag: DSC overlap */
	byte     nice1;			/* flag: give potential excess bandwidth */
	unsigned present	: 1;	/* drive is physically present */
	unsigned noprobe 	: 1;	/* from:  hdx=noprobe */
	unsigned busy		: 1;	/* currently doing revalidate_disk() */
	unsigned removable	: 1;	/* 1 if need to do check_media_change */
	unsigned forced_geom	: 1;	/* 1 if hdx=c,h,s was given at boot */
	unsigned no_unmask	: 1;	/* disallow setting unmask bit */
	unsigned no_io_32bit	: 1;	/* disallow enabling 32bit I/O */
	unsigned nobios		: 1;	/* flag: do not probe bios for drive */
	unsigned revalidate	: 1;	/* request revalidation */
	unsigned atapi_overlap	: 1;	/* flag: ATAPI overlap (not supported) */
	unsigned nice0		: 1;	/* flag: give obvious excess bandwidth */
	unsigned nice2		: 1;	/* flag: give a share in our own bandwidth */
	unsigned doorlocking	: 1;	/* flag: for removable only: door lock/unlock works */
	unsigned autotune	: 2;	/* 1=autotune, 2=noautotune, 0=default */
#if FAKE_FDISK_FOR_EZDRIVE
	unsigned remap_0_to_1	: 1;	/* flag: partitioned with ezdrive */
#endif /* FAKE_FDISK_FOR_EZDRIVE */
	byte		media;		/* disk, cdrom, tape, floppy, ... */
	select_t	select;		/* basic drive/head select reg value */
	byte		ctl;		/* "normal" value for IDE_CONTROL_REG */
	byte		ready_stat;	/* min status value for drive ready */
	byte		mult_count;	/* current multiple sector setting */
	byte 		mult_req;	/* requested multiple sector setting */
	byte 		tune_req;	/* requested drive tuning setting */
	byte		io_32bit;	/* 0=16-bit, 1=32-bit, 2/3=32bit+sync */
	byte		bad_wstat;	/* used for ignoring WRERR_STAT */
	byte		nowerr;		/* used for ignoring WRERR_STAT */
	byte		sect0;		/* offset of first sector for DM6:DDO */
	byte 		usage;		/* current "open()" count for drive */
	byte 		head;		/* "real" number of heads */
	byte		sect;		/* "real" sectors per track */
	byte		bios_head;	/* BIOS/fdisk/LILO number of heads */
	byte		bios_sect;	/* BIOS/fdisk/LILO sectors per track */
	unsigned short	bios_cyl;	/* BIOS/fdisk/LILO number of cyls */
	unsigned short	cyl;		/* "real" number of cyls */
	unsigned int	drive_data;	/* for use by tuneproc/selectproc as needed */
	void		  *hwif;	/* actually (ide_hwif_t *) */
	struct wait_queue *wqueue;	/* used to wait for drive in open() */
	struct hd_driveid *id;		/* drive model identification info */
	struct hd_struct  *part;	/* drive partition table */
	char		name[4];	/* drive name, such as "hda" */
	void 		*driver;	/* (ide_driver_t *) */
	void		*driver_data;	/* extra driver data */
	struct proc_dir_entry *proc;	/* /proc/ide/ directory entry */
	void		*settings;	/* /proc/ide/ drive settings */
	char		driver_req[10];	/* requests specific driver */
/* T_ALGO */
	int 		sched;		/* scheduling type */
	unsigned long	optimal_order[T_ALGO_SIZE];
	unsigned int	optimal_index;
	unsigned long	qarr[T_ALGO_SIZE];
	unsigned long	*C_L, *C_R;
/* T_ALGO_END */
	} ide_drive_t;

...

/* T_ALGO */
ide_drive_t *get_info_ptr (kdev_t i_rdev);
/* T_ALGO_END */
#endif /* _IDE_H */
