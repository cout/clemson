/*
 * raid1.c : Multiple Devices driver for Linux
 * Copyright (C) 1996, 1997, 1998 Ingo Molnar, Miguel de Icaza, Gadi Oxman
 *
 * RAID-1 management functions.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example /usr/src/linux/COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/malloc.h>
#include <linux/raid/raid1.h>
#include <asm/atomic.h>

#define MAJOR_NR MD_MAJOR
#define MD_DRIVER
#define MD_PERSONALITY

#define MAX_LINEAR_SECTORS 128

#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))

static mdk_personality_t raid1_personality;
struct buffer_head *raid1_retry_list = NULL;

static void * raid1_kmalloc (int size)
{
	void * ptr;
	/*
	 * now we are rather fault tolerant than nice, but
	 * there are a couple of places in the RAID code where we
	 * simply can not afford to fail an allocation because
	 * there is no failure return path (eg. make_request())
	 */
	while (!(ptr = kmalloc (sizeof (raid1_conf_t), GFP_KERNEL)))
		printk ("raid1: out of memory, retrying...\n");

	memset(ptr, 0, size);
	return ptr;
}

static int __raid1_map (mddev_t *mddev, kdev_t *rdev,
		        unsigned long *rsector, unsigned long size)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);
	int i, disks = MD_SB_DISKS;

	/*
	 * Later we do read balancing on the read side 
	 * now we use the first available disk.
	 */

	for (i = 0; i < disks; i++) {
		if (conf->mirrors[i].operational) {
			*rdev = conf->mirrors[i].dev;
			return (0);
		}
	}

	printk (KERN_ERR "raid1_map(): huh, no more operational devices?\n");
	return (-1);
}

static int raid1_map (mddev_t *mddev, kdev_t dev, kdev_t *rdev,
		      unsigned long *rsector, unsigned long size)
{
	return 0;
}

static void raid1_reschedule_retry (struct buffer_head *bh)
{
	struct raid1_bh * r1_bh = (struct raid1_bh *)(bh->b_dev_id);
	mddev_t *mddev = r1_bh->mddev;
	raid1_conf_t *conf = mddev_to_conf(mddev);

	r1_bh->next_retry = raid1_retry_list;
	raid1_retry_list = bh;
	md_wakeup_thread(conf->thread);
}

/*
 * raid1_end_bh_io() is called when we have finished servicing a mirrored
 * operation and are ready to return a success/failure code to the buffer
 * cache layer.
 */
static void raid1_end_bh_io (struct raid1_bh *r1_bh, int uptodate)
{
	struct buffer_head *bh = r1_bh->master_bh;

	bh->b_end_io(bh, uptodate);
	kfree(r1_bh);
}

void raid1_end_request (struct buffer_head *bh, int uptodate)
{
	struct raid1_bh * r1_bh = (struct raid1_bh *)(bh->b_dev_id);
	unsigned long flags;

	save_flags(flags);
	cli();

	/*
	 * this branch is our 'one mirror IO has finished' event handler:
	 */
	if (!uptodate)
		md_error (bh->b_dev, bh->b_rdev);
	else {
		/*
		 * Set BH_Uptodate in our master buffer_head, so that
		 * we will return a good error code for to the higher
		 * levels even if IO on some other mirrored buffer fails.
		 *
		 * The 'master' represents the complex operation to 
		 * user-side. So if something waits for IO, then it will
		 * wait for the 'master' buffer_head.
		 */
		set_bit (BH_Uptodate, &r1_bh->state);
	}

	/*
	 * We split up the read and write side, imho they are 
	 * conceptually different.
	 */

	if ( (r1_bh->cmd == READ) || (r1_bh->cmd == READA) ) {
		/*
		 * we have only one buffer_head on the read side
		 */
		if (uptodate) {
			raid1_end_bh_io(r1_bh, uptodate);
			restore_flags(flags);
			return;
		}
		/*
		 * oops, read error:
		 */
		printk(KERN_ERR "raid1: %s: rescheduling block %lu\n", 
			 partition_name(bh->b_dev), bh->b_blocknr);
		raid1_reschedule_retry(bh);
		restore_flags(flags);
		return;
	}

	/*
	 * WRITE:
	 *
	 * Let's see if all mirrored write operations have finished 
	 * already.
	 */

	if (atomic_dec_and_test(&r1_bh->remaining)) {
		int i, disks = MD_SB_DISKS;

		for ( i = 0; i < disks; i++)
			if (r1_bh->mirror_bh[i])
				kfree(r1_bh->mirror_bh[i]);

		raid1_end_bh_io(r1_bh, test_bit(BH_Uptodate, &r1_bh->state));
	}
	restore_flags(flags);
}

/*
 * This routine checks if the undelying device is an md device
 * and in that case it maps the blocks before putting the
 * request on the queue
 */
static void map_and_make_request (int rw, struct buffer_head *bh)
{
	if (MAJOR (bh->b_rdev) == MD_MAJOR)
		md_map (bh->b_rdev, &bh->b_rdev,
				&bh->b_rsector, bh->b_size >> 9);
	clear_bit(BH_Lock, &bh->b_state);
	make_request (MAJOR (bh->b_rdev), rw, bh);
}
	
static int raid1_make_request (mddev_t *mddev, int rw,
						 struct buffer_head * bh)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);
	struct buffer_head *mirror_bh[MD_SB_DISKS], *bh_req;
	struct raid1_bh * r1_bh;
	int disks = MD_SB_DISKS;
	int i, sum_bhs = 0, switch_disks = 0, sectors, lowprio = 0;
	struct mirror_info *mirror;

	r1_bh = raid1_kmalloc (sizeof (struct raid1_bh));

/*
 * make_request() can abort the operation when READA or WRITEA are being
 * used and no empty request is available.
 *
 * Currently, just replace the command with READ/WRITE.
 */
	if (rw == READA) rw = READ;
	if (rw == WRITEA) rw = WRITE;

	if (rw == WRITE) {
		/*
		 * Too early ?
		 */
		mark_buffer_clean(bh);
		/*
		 * not too early. we _first_ clean the bh, then we start
		 * the IO, then when the IO has finished, we unlock the
		 * bh and mark it uptodate. This way we do not miss the
		 * case when the bh got dirty again during the IO.
		 */
	}

	/*
	 * special flag for 'lowprio' reconstruction requests ...
	 */
	if (buffer_lowprio(bh))
		lowprio = 1;

/*
 * i think the read and write branch should be separated completely,
 * since we want to do read balancing on the read side for example.
 * Comments? :) --mingo
 */

	r1_bh->master_bh=bh;
	r1_bh->mddev=mddev;
	r1_bh->cmd = rw;

	if (rw==READ) {
		int last_used = conf->last_used;

		/*
		 * read balancing logic:
		 */
		mirror = conf->mirrors + last_used;
		bh->b_rdev = mirror->dev;
		sectors = bh->b_size >> 9;

		if (bh->b_blocknr * sectors == conf->next_sect) {
			conf->sect_count += sectors;
			if (conf->sect_count >= mirror->sect_limit)
				switch_disks = 1;
		} else
			switch_disks = 1;
		conf->next_sect = (bh->b_blocknr + 1) * sectors;
		/*
		 * Do not switch disks if full resync is in progress ...
		 */
		if (switch_disks && !conf->resync_mirrors) {
			conf->sect_count = 0;
			last_used = conf->last_used = mirror->next;
			/*
			 * Do not switch to write-only disks ...
			 * reconstruction is in progress
			 */
			while (conf->mirrors[last_used].write_only)
				conf->last_used = conf->mirrors[last_used].next;
		}
		bh_req = &r1_bh->bh_req;
		memcpy(bh_req, bh, sizeof(*bh));
		bh_req->b_end_io = raid1_end_request;
		bh_req->b_dev_id = r1_bh;
		map_and_make_request (rw, bh_req);
		return 0;
	}

	/*
	 * WRITE:
	 */

	for (i = 0; i < disks; i++) {

		if (!conf->mirrors[i].operational) {
			/*
			 * the r1_bh->mirror_bh[i] pointer remains NULL
			 */
			mirror_bh[i] = NULL;
			continue;
		}

 		/*
 		 * special case for reconstruction ...
 		 */
 		if (lowprio && (i == conf->last_used)) {
 			mirror_bh[i] = NULL;
 			continue;
 		}
 
  	/*
  	 * We should use a private pool (size depending on NR_REQUEST),
  	 * to avoid writes filling up the memory with bhs
  	 *
 	 * Such pools are much faster than kmalloc anyways (so we waste
 	 * almost nothing by not using the master bh when writing and
 	 * win alot of cleanness) but for now we are cool enough. --mingo
 	 *
  	 * It's safe to sleep here, buffer heads cannot be used in a shared
 	 * manner in the write branch. Look how we lock the buffer at the
 	 * beginning of this function to grok the difference ;)
  	 */
 		mirror_bh[i] = raid1_kmalloc(sizeof(struct buffer_head));
  	/*
  	 * prepare mirrored bh (fields ordered for max mem throughput):
  	 */
 		mirror_bh[i]->b_blocknr    = bh->b_blocknr;
 		mirror_bh[i]->b_dev        = bh->b_dev;
 		mirror_bh[i]->b_rdev	   = conf->mirrors[i].dev;
 		mirror_bh[i]->b_rsector    = bh->b_rsector;
 		mirror_bh[i]->b_state      = (1<<BH_Req) | (1<<BH_Dirty);
 		if (lowprio)
 			mirror_bh[i]->b_state |= (1<<BH_LowPrio);
 
 		mirror_bh[i]->b_count      = 1;
 		mirror_bh[i]->b_size       = bh->b_size;
 		mirror_bh[i]->b_data       = bh->b_data;
 		mirror_bh[i]->b_list       = BUF_LOCKED;
 		mirror_bh[i]->b_end_io     = raid1_end_request;
 		mirror_bh[i]->b_dev_id     = r1_bh;
  
  		r1_bh->mirror_bh[i] = mirror_bh[i];
  		sum_bhs++;
	}

	md_atomic_set(&r1_bh->remaining, sum_bhs);

	/*
	 * We have to be a bit careful about the semaphore above, thats
	 * why we start the requests separately. Since kmalloc() could
	 * fail, sleep and make_request() can sleep too, this is the
	 * safer solution. Imagine, end_request decreasing the semaphore
	 * before we could have set it up ... We could play tricks with
	 * the semaphore (presetting it and correcting at the end if
	 * sum_bhs is not 'n' but we have to do end_request by hand if
	 * all requests finish until we had a chance to set up the
	 * semaphore correctly ... lots of races).
	 */
	for (i = 0; i < disks; i++)
		if (mirror_bh[i])
			map_and_make_request(rw, mirror_bh[i]);

	return (0);
}
			   
static int raid1_status (char *page, mddev_t *mddev)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);
	int sz = 0, i;
	
	sz += sprintf (page+sz, " [%d/%d] [", conf->raid_disks,
						 conf->working_disks);
	for (i = 0; i < conf->raid_disks; i++)
		sz += sprintf (page+sz, "%s",
			conf->mirrors[i].operational ? "U" : "_");
	sz += sprintf (page+sz, "]");
	return sz;
}

static void unlink_disk (raid1_conf_t *conf, int target)
{
	int disks = MD_SB_DISKS;
	int i;

	for (i = 0; i < disks; i++)
		if (conf->mirrors[i].next == target)
			conf->mirrors[i].next = conf->mirrors[target].next;
}

#define LAST_DISK KERN_ALERT \
"raid1: only one disk left and IO error.\n"

#define NO_SPARE_DISK KERN_ALERT \
"raid1: no spare disk left, degrading mirror level by one.\n"

#define DISK_FAILED KERN_ALERT \
"raid1: Disk failure on %s, disabling device. \n" \
"       Operation continuing on %d devices\n"

#define START_SYNCING KERN_ALERT \
"raid1: start syncing spare disk.\n"

#define ALREADY_SYNCING KERN_INFO \
"raid1: syncing already in progress.\n"

static void mark_disk_bad (mddev_t *mddev, int failed)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);
	struct mirror_info *mirror = conf->mirrors+failed;
	mdp_super_t *sb = mddev->sb;

	mirror->operational = 0;
	unlink_disk(conf, failed);
	mark_disk_faulty(sb->disks+mirror->number);
	mark_disk_nonsync(sb->disks+mirror->number);
	mark_disk_inactive(sb->disks+mirror->number);
	sb->active_disks--;
	sb->working_disks--;
	sb->failed_disks++;
	mddev->sb_dirty = 1;
	md_wakeup_thread(conf->thread);
	conf->working_disks--;
	printk (DISK_FAILED, partition_name (mirror->dev),
				 conf->working_disks);
}

static int raid1_error (mddev_t *mddev, kdev_t dev)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);
	struct mirror_info * mirrors = conf->mirrors;
	int disks = MD_SB_DISKS;
	int i;

	if (conf->working_disks == 1) {
		/*
		 * Uh oh, we can do nothing if this is our last disk, but
		 * first check if this is a queued request for a device
		 * which has just failed.
		 */
		for (i = 0; i < disks; i++) {
			if (mirrors[i].dev==dev && !mirrors[i].operational)
				return 0;
		}
		printk (LAST_DISK);
	} else {
		/*
		 * Mark disk as unusable
		 */
		for (i = 0; i < disks; i++) {
			if (mirrors[i].dev==dev && mirrors[i].operational) {
				mark_disk_bad (mddev, i);
				break;
			}
		}
	}
	return 0;
}

#undef LAST_DISK
#undef NO_SPARE_DISK
#undef DISK_FAILED
#undef START_SYNCING

/*
 * Insert the spare disk into the drive-ring
 */
static void link_disk(raid1_conf_t *conf, struct mirror_info *mirror)
{
	int j, next;
	int disks = MD_SB_DISKS;
	struct mirror_info *p = conf->mirrors;

	for (j = 0; j < disks; j++, p++)
		if (p->operational && !p->write_only) {
			next = p->next;
			p->next = mirror->raid_disk;
			mirror->next = next;
			return;
		}

	printk("raid1: bug: no read-operational devices\n");
}

static void print_raid1_conf (raid1_conf_t *conf)
{
	int i;
	struct mirror_info *tmp;

	printk("RAID1 conf printout:\n");
	if (!conf) {
		printk("(conf==NULL)\n");
		return;
	}
	printk(" --- wd:%d rd:%d nd:%d\n", conf->working_disks,
			 conf->raid_disks, conf->nr_disks);

	for (i = 0; i < MD_SB_DISKS; i++) {
		tmp = conf->mirrors + i;
		printk(" disk %d, s:%d, o:%d, n:%d rd:%d us:%d dev:%s\n",
			i, tmp->spare,tmp->operational,
			tmp->number,tmp->raid_disk,tmp->used_slot,
			partition_name(tmp->dev));
	}
}

static int raid1_diskop(mddev_t *mddev, mdp_disk_t **d, int state)
{
	int err = 0;
	int i, failed_disk=-1, spare_disk=-1, removed_disk=-1, added_disk=-1;
	raid1_conf_t *conf = mddev->private;
	struct mirror_info *tmp, *sdisk, *fdisk, *rdisk, *adisk;
	unsigned long flags;
	mdp_super_t *sb = mddev->sb;
	mdp_disk_t *failed_desc, *spare_desc, *added_desc;

	save_flags(flags);
	cli();

	print_raid1_conf(conf);
	/*
	 * find the disk ...
	 */
	switch (state) {

	case DISKOP_SPARE_ACTIVE:

		/*
		 * Find the failed disk within the RAID1 configuration ...
		 * (this can only be in the first conf->working_disks part)
		 */
		for (i = 0; i < conf->raid_disks; i++) {
			tmp = conf->mirrors + i;
			if ((!tmp->operational && !tmp->spare) ||
					!tmp->used_slot) {
				failed_disk = i;
				break;
			}
		}
		/*
		 * When we activate a spare disk we _must_ have a disk in
		 * the lower (active) part of the array to replace. 
		 */
		if ((failed_disk == -1) || (failed_disk >= conf->raid_disks)) {
			MD_BUG();
			err = 1;
			goto abort;
		}
		/* fall through */

	case DISKOP_SPARE_WRITE:
	case DISKOP_SPARE_INACTIVE:

		/*
		 * Find the spare disk ... (can only be in the 'high'
		 * area of the array)
		 */
		for (i = conf->raid_disks; i < MD_SB_DISKS; i++) {
			tmp = conf->mirrors + i;
			if (tmp->spare && tmp->number == (*d)->number) {
				spare_disk = i;
				break;
			}
		}
		if (spare_disk == -1) {
			MD_BUG();
			err = 1;
			goto abort;
		}
		break;

	case DISKOP_HOT_REMOVE_DISK:

		for (i = 0; i < MD_SB_DISKS; i++) {
			tmp = conf->mirrors + i;
			if (tmp->used_slot && (tmp->number == (*d)->number)) {
				if (tmp->operational) {
					err = -EBUSY;
					goto abort;
				}
				removed_disk = i;
				break;
			}
		}
		if (removed_disk == -1) {
			MD_BUG();
			err = 1;
			goto abort;
		}
		break;

	case DISKOP_HOT_ADD_DISK:

		for (i = conf->raid_disks; i < MD_SB_DISKS; i++) {
			tmp = conf->mirrors + i;
			if (!tmp->used_slot) {
				added_disk = i;
				break;
			}
		}
		if (added_disk == -1) {
			MD_BUG();
			err = 1;
			goto abort;
		}
		break;
	}

	switch (state) {
	/*
	 * Switch the spare disk to write-only mode:
	 */
	case DISKOP_SPARE_WRITE:
		sdisk = conf->mirrors + spare_disk;
		sdisk->operational = 1;
		sdisk->write_only = 1;
		break;
	/*
	 * Deactivate a spare disk:
	 */
	case DISKOP_SPARE_INACTIVE:
		sdisk = conf->mirrors + spare_disk;
		sdisk->operational = 0;
		sdisk->write_only = 0;
		break;
	/*
	 * Activate (mark read-write) the (now sync) spare disk,
	 * which means we switch it's 'raid position' (->raid_disk)
	 * with the failed disk. (only the first 'conf->nr_disks'
	 * slots are used for 'real' disks and we must preserve this
	 * property)
	 */
	case DISKOP_SPARE_ACTIVE:

		sdisk = conf->mirrors + spare_disk;
		fdisk = conf->mirrors + failed_disk;

		spare_desc = &sb->disks[sdisk->number];
		failed_desc = &sb->disks[fdisk->number];

		if (spare_desc != *d) {
			MD_BUG();
			err = 1;
			goto abort;
		}

		if (spare_desc->raid_disk != sdisk->raid_disk) {
			MD_BUG();
			err = 1;
			goto abort;
		}
			
		if (sdisk->raid_disk != spare_disk) {
			MD_BUG();
			err = 1;
			goto abort;
		}

		if (failed_desc->raid_disk != fdisk->raid_disk) {
			MD_BUG();
			err = 1;
			goto abort;
		}

		if (fdisk->raid_disk != failed_disk) {
			MD_BUG();
			err = 1;
			goto abort;
		}

		/*
		 * do the switch finally
		 */
		xchg_values(*spare_desc, *failed_desc);
		xchg_values(*fdisk, *sdisk);

		/*
		 * (careful, 'failed' and 'spare' are switched from now on)
		 *
		 * we want to preserve linear numbering and we want to
		 * give the proper raid_disk number to the now activated
		 * disk. (this means we switch back these values)
		 */
	
		xchg_values(spare_desc->raid_disk, failed_desc->raid_disk);
		xchg_values(sdisk->raid_disk, fdisk->raid_disk);
		xchg_values(spare_desc->number, failed_desc->number);
		xchg_values(sdisk->number, fdisk->number);

		*d = failed_desc;

		if (sdisk->dev == MKDEV(0,0))
			sdisk->used_slot = 0;
		/*
		 * this really activates the spare.
		 */
		fdisk->spare = 0;
		fdisk->write_only = 0;
		link_disk(conf, fdisk);

		/*
		 * if we activate a spare, we definitely replace a
		 * non-operational disk slot in the 'low' area of
		 * the disk array.
		 */

		conf->working_disks++;

		break;

	case DISKOP_HOT_REMOVE_DISK:
		rdisk = conf->mirrors + removed_disk;

		if (rdisk->spare && (removed_disk < conf->raid_disks)) {
			MD_BUG();	
			err = 1;
			goto abort;
		}
		rdisk->dev = MKDEV(0,0);
		rdisk->used_slot = 0;
		conf->nr_disks--;
		break;

	case DISKOP_HOT_ADD_DISK:
		adisk = conf->mirrors + added_disk;
		added_desc = *d;

		if (added_disk != added_desc->number) {
			MD_BUG();	
			err = 1;
			goto abort;
		}

		adisk->number = added_desc->number;
		adisk->raid_disk = added_desc->raid_disk;
		adisk->dev = MKDEV(added_desc->major,added_desc->minor);

		adisk->operational = 0;
		adisk->write_only = 0;
		adisk->spare = 1;
		adisk->used_slot = 1;
		conf->nr_disks++;

		break;

	default:
		MD_BUG();	
		err = 1;
		goto abort;
	}
abort:
	restore_flags(flags);
	print_raid1_conf(conf);
	return err;
}


#define IO_ERROR KERN_ALERT \
"raid1: %s: unrecoverable I/O read error for block %lu\n"

#define REDIRECT_SECTOR KERN_ERR \
"raid1: %s: redirecting sector %lu to another mirror\n"

/*
 * This is a kernel thread which:
 *
 *	1.	Retries failed read operations on working mirrors.
 *	2.	Updates the raid superblock when problems encounter.
 */
static void raid1d (void *data)
{
	struct buffer_head *bh;
	kdev_t dev;
	unsigned long flags;
	struct raid1_bh *r1_bh;
	mddev_t *mddev;

	while (raid1_retry_list) {
		save_flags(flags);
		cli();
		bh = raid1_retry_list;
		r1_bh = (struct raid1_bh *)(bh->b_dev_id);
		raid1_retry_list = r1_bh->next_retry;
		restore_flags(flags);

		mddev = kdev_to_mddev(bh->b_dev);
		if (mddev->sb_dirty) {
			printk(KERN_INFO "dirty sb detected, updating.\n");
			mddev->sb_dirty = 0;
			md_update_sb(mddev);
		}
		dev = bh->b_rdev;
		__raid1_map (mddev, &bh->b_rdev, &bh->b_rsector,
							 bh->b_size >> 9);
		if (bh->b_rdev == dev) {
			printk (IO_ERROR, partition_name(bh->b_dev), bh->b_blocknr);
			raid1_end_bh_io(r1_bh, 0);
		} else {
			printk (REDIRECT_SECTOR,
				partition_name(bh->b_dev), bh->b_blocknr);
			map_and_make_request (r1_bh->cmd, bh);
		}
	}
}
#undef IO_ERROR
#undef REDIRECT_SECTOR

/*
 * Private kernel thread to reconstruct mirrors after an unclean
 * shutdown.
 */
static void raid1syncd (void *data)
{
        raid1_conf_t *conf = data;
        mddev_t *mddev = conf->mddev;

        if (!conf->resync_mirrors)
                return;
        if (conf->resync_mirrors == 2)
                return;
	down(&mddev->recovery_sem);
        if (md_do_sync(mddev, NULL)) {
		up(&mddev->recovery_sem);
		return;
	}
	/*
	 * Only if everything went Ok.
	 */
        conf->resync_mirrors = 0;
	up(&mddev->recovery_sem);
}


/*
 * This will catch the scenario in which one of the mirrors was
 * mounted as a normal device rather than as a part of a raid set.
 *
 * check_consistency is very personality-dependent, eg. RAID5 cannot
 * do this check, it uses another method.
 */
static int __check_consistency (mddev_t *mddev, int row)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);
	int disks = MD_SB_DISKS;
	kdev_t dev;
	struct buffer_head *bh = NULL;
	int i, rc = 0;
	char *buffer = NULL;

	for (i = 0; i < disks; i++) {
		printk("(checking disk %d)\n",i);
		if (!conf->mirrors[i].operational)
			continue;
		printk("(really checking disk %d)\n",i);
		dev = conf->mirrors[i].dev;
		set_blocksize(dev, 4096);
		if ((bh = bread(dev, row / 4, 4096)) == NULL)
			break;
		if (!buffer) {
			buffer = (char *) __get_free_page(GFP_KERNEL);
			if (!buffer)
				break;
			memcpy(buffer, bh->b_data, 4096);
		} else if (memcmp(buffer, bh->b_data, 4096)) {
			rc = 1;
			break;
		}
		bforget(bh);
		fsync_dev(dev);
		invalidate_buffers(dev);
		bh = NULL;
	}
	if (buffer)
		free_page((unsigned long) buffer);
	if (bh) {
		dev = bh->b_dev;
		bforget(bh);
		fsync_dev(dev);
		invalidate_buffers(dev);
	}
	return rc;
}

static int check_consistency (mddev_t *mddev)
{
	if (__check_consistency(mddev, 0))
/*
 * we do not do this currently, as it's perfectly possible to
 * have an inconsistent array when it's freshly created. Only
 * newly written data has to be consistent.
 */
		return 0;

	return 0;
}

#define INVALID_LEVEL KERN_WARNING \
"raid1: md%d: raid level not set to mirroring (%d)\n"

#define NO_SB KERN_ERR \
"raid1: disabled mirror %s (couldn't access raid superblock)\n"

#define ERRORS KERN_ERR \
"raid1: disabled mirror %s (errors detected)\n"

#define NOT_IN_SYNC KERN_ERR \
"raid1: disabled mirror %s (not in sync)\n"

#define INCONSISTENT KERN_ERR \
"raid1: disabled mirror %s (inconsistent descriptor)\n"

#define ALREADY_RUNNING KERN_ERR \
"raid1: disabled mirror %s (mirror %d already operational)\n"

#define OPERATIONAL KERN_INFO \
"raid1: device %s operational as mirror %d\n"

#define MEM_ERROR KERN_ERR \
"raid1: couldn't allocate memory for md%d\n"

#define SPARE KERN_INFO \
"raid1: spare disk %s\n"

#define NONE_OPERATIONAL KERN_ERR \
"raid1: no operational mirrors for md%d\n"

#define RUNNING_CKRAID KERN_ERR \
"raid1: detected mirror differences -- running resync\n"

#define ARRAY_IS_ACTIVE KERN_INFO \
"raid1: raid set md%d active with %d out of %d mirrors\n"

#define THREAD_ERROR KERN_ERR \
"raid1: couldn't allocate thread for md%d\n"

#define START_RESYNC KERN_WARNING \
"raid1: raid set md%d not clean; reconstructing mirrors\n"

static int raid1_run (mddev_t *mddev)
{
	raid1_conf_t *conf;
	int i, j, disk_idx;
	struct mirror_info *disk;
	mdp_super_t *sb = mddev->sb;
	mdp_disk_t *descriptor;
	mdk_rdev_t *rdev;
	struct md_list_head *tmp;
	int start_recovery = 0;

	MOD_INC_USE_COUNT;

	if (sb->level != 1) {
		printk(INVALID_LEVEL, mdidx(mddev), sb->level);
		goto out;
	}
	/*
	 * copy the already verified devices into our private RAID1
	 * bookkeeping area. [whatever we allocate in raid1_run(),
	 * should be freed in raid1_stop()]
	 */

	conf = raid1_kmalloc(sizeof(raid1_conf_t));
	mddev->private = conf;
	if (!conf) {
		printk(MEM_ERROR, mdidx(mddev));
		goto out;
	}

	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty) {
			printk(ERRORS, partition_name(rdev->dev));
		} else {
			if (!rdev->sb) {
				MD_BUG();
				continue;
			}
		}
		if (rdev->desc_nr == -1) {
			MD_BUG();
			continue;
		}
		descriptor = &sb->disks[rdev->desc_nr];
		disk_idx = descriptor->raid_disk;
		disk = conf->mirrors + disk_idx;

		if (disk_faulty(descriptor)) {
			disk->number = descriptor->number;
			disk->raid_disk = disk_idx;
			disk->dev = rdev->dev;
			disk->sect_limit = MAX_LINEAR_SECTORS;
			disk->operational = 0;
			disk->write_only = 0;
			disk->spare = 0;
			disk->used_slot = 1;
			continue;
		}
		if (disk_active(descriptor)) {
			if (!disk_sync(descriptor)) {
				printk(NOT_IN_SYNC,
					partition_name(rdev->dev));
				continue;
			}
			if ((descriptor->number > MD_SB_DISKS) ||
					 (disk_idx > sb->raid_disks)) {

				printk(INCONSISTENT,
					partition_name(rdev->dev));
				continue;
			}
			if (disk->operational) {
				printk(ALREADY_RUNNING,
					partition_name(rdev->dev),
					disk_idx);
				continue;
			}
			printk(OPERATIONAL, partition_name(rdev->dev),
 					disk_idx);
			disk->number = descriptor->number;
			disk->raid_disk = disk_idx;
			disk->dev = rdev->dev;
			disk->sect_limit = MAX_LINEAR_SECTORS;
			disk->operational = 1;
			disk->write_only = 0;
			disk->spare = 0;
			disk->used_slot = 1;
			conf->working_disks++;
		} else {
		/*
		 * Must be a spare disk ..
		 */
			printk(SPARE, partition_name(rdev->dev));
			disk->number = descriptor->number;
			disk->raid_disk = disk_idx;
			disk->dev = rdev->dev;
			disk->sect_limit = MAX_LINEAR_SECTORS;
			disk->operational = 0;
			disk->write_only = 0;
			disk->spare = 1;
			disk->used_slot = 1;
		}
	}
	if (!conf->working_disks) {
		printk(NONE_OPERATIONAL, mdidx(mddev));
		goto out_free_conf;
	}

	conf->raid_disks = sb->raid_disks;
	conf->nr_disks = sb->nr_disks;
	conf->mddev = mddev;

	for (i = 0; i < MD_SB_DISKS; i++) {
		
		descriptor = sb->disks+i;
		disk_idx = descriptor->raid_disk;
		disk = conf->mirrors + disk_idx;

		if (disk_faulty(descriptor) && (disk_idx < conf->raid_disks) &&
				!disk->used_slot) {

			disk->number = descriptor->number;
			disk->raid_disk = disk_idx;
			disk->dev = MKDEV(0,0);

			disk->operational = 0;
			disk->write_only = 0;
			disk->spare = 0;
			disk->used_slot = 1;
		}
	}

	/*
	 * find the first working one and use it as a starting point
	 * to read balancing.
	 */
	for (j = 0; !conf->mirrors[j].operational; j++)
		/* nothing */;
	conf->last_used = j;

	/*
	 * initialize the 'working disks' list.
	 */
	for (i = conf->raid_disks - 1; i >= 0; i--) {
		if (conf->mirrors[i].operational) {
			conf->mirrors[i].next = j;
			j = i;
		}
	}

	if (conf->working_disks != sb->raid_disks) {
		printk(KERN_ALERT "raid1: md%d, not all disks are operational -- trying to recover array\n", mdidx(mddev));
		start_recovery = 1;
	}

	if (!start_recovery && (sb->state & (1 << MD_SB_CLEAN))) {
		/*
		 * we do sanity checks even if the device says
		 * it's clean ...
		 */
		if (check_consistency(mddev)) {
			printk(RUNNING_CKRAID);
			sb->state &= ~(1 << MD_SB_CLEAN);
		}
	}

	{
		const char * name = "raid1d";

		conf->thread = md_register_thread(raid1d, conf, name);
		if (!conf->thread) {
			printk(THREAD_ERROR, mdidx(mddev));
			goto out_free_conf;
        	}
	}

	if (!start_recovery && !(sb->state & (1 << MD_SB_CLEAN))) {
		const char * name = "raid1syncd";

		conf->resync_thread = md_register_thread(raid1syncd, conf,name);
		if (!conf->resync_thread) {
			printk(THREAD_ERROR, mdidx(mddev));
			goto out_free_conf;
		}

		printk(START_RESYNC, mdidx(mddev));
                conf->resync_mirrors = 1;
                md_wakeup_thread(conf->resync_thread);
        }

	/*
	 * Regenerate the "device is in sync with the raid set" bit for
	 * each device.
	 */
	for (i = 0; i < MD_SB_DISKS; i++) {
		mark_disk_nonsync(sb->disks+i);
		for (j = 0; j < sb->raid_disks; j++) {
			if (!conf->mirrors[j].operational)
				continue;
			if (sb->disks[i].number == conf->mirrors[j].number)
				mark_disk_sync(sb->disks+i);
		}
	}
	sb->active_disks = conf->working_disks;

	if (start_recovery)
		md_recover_arrays();


	printk(ARRAY_IS_ACTIVE, mdidx(mddev), sb->active_disks, sb->raid_disks);
	/*
	 * Ok, everything is just fine now
	 */
	return 0;

out_free_conf:
	kfree(conf);
	mddev->private = NULL;
out:
	MOD_DEC_USE_COUNT;
	return -EIO;
}

#undef INVALID_LEVEL
#undef NO_SB
#undef ERRORS
#undef NOT_IN_SYNC
#undef INCONSISTENT
#undef ALREADY_RUNNING
#undef OPERATIONAL
#undef SPARE
#undef NONE_OPERATIONAL
#undef RUNNING_CKRAID
#undef ARRAY_IS_ACTIVE

static int raid1_stop_resync (mddev_t *mddev)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);

	if (conf->resync_thread) {
		if (conf->resync_mirrors) {
			conf->resync_mirrors = 2;
			md_interrupt_thread(conf->resync_thread);
			printk(KERN_INFO "raid1: mirror resync was not fully finished, restarting next time.\n");
			return 1;
		}
		return 0;
	}
	return 0;
}

static int raid1_restart_resync (mddev_t *mddev)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);

	if (conf->resync_mirrors) {
		if (!conf->resync_thread) {
			MD_BUG();
			return 0;
		}
		conf->resync_mirrors = 1;
		md_wakeup_thread(conf->resync_thread);
		return 1;
	}
	return 0;
}

static int raid1_stop (mddev_t *mddev)
{
	raid1_conf_t *conf = mddev_to_conf(mddev);

	md_unregister_thread(conf->thread);
	if (conf->resync_thread)
		md_unregister_thread(conf->resync_thread);
	kfree(conf);
	mddev->private = NULL;
	MOD_DEC_USE_COUNT;
	return 0;
}

static mdk_personality_t raid1_personality=
{
	"raid1",
	raid1_map,
	raid1_make_request,
	raid1_end_request,
	raid1_run,
	raid1_stop,
	raid1_status,
	NULL,			/* no ioctls */
	0,
	raid1_error,
	raid1_diskop,
	raid1_stop_resync,
	raid1_restart_resync
};

int raid1_init (void)
{
	return register_md_personality (RAID1, &raid1_personality);
}

#ifdef MODULE
int init_module (void)
{
	return raid1_init();
}

void cleanup_module (void)
{
	unregister_md_personality (RAID1);
}
#endif
