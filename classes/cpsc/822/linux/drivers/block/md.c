/*
   md.c : Multiple Devices driver for Linux
          Copyright (C) 1998, 1999 Ingo Molnar

     completely rewritten, based on the MD driver code from Marc Zyngier

   Changes:

   - RAID-1/RAID-5 extensions by Miguel de Icaza, Gadi Oxman, Ingo Molnar
   - boot support for linear and striped mode by Harald Hoyer <HarryH@Royal.Net>
   - kerneld support by Boris Tobotras <boris@xtalk.msk.su>
   - kmod support by: Cyrus Durgin
   - RAID0 bugfixes: Mark Anthony Lisher <markal@iname.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   You should have received a copy of the GNU General Public License
   (for example /usr/src/linux/COPYING); if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
*/

#include <linux/raid/md.h>
#include <linux/raid/xor.h>

#ifdef CONFIG_KMOD
#include <linux/kmod.h>
#endif

#define __KERNEL_SYSCALLS__
#include <linux/unistd.h>

#include <asm/unaligned.h>

extern asmlinkage int sys_sched_yield(void);
extern asmlinkage int sys_setsid(void);

extern unsigned long io_events[MAX_BLKDEV];

#define MAJOR_NR MD_MAJOR
#define MD_DRIVER

#include <linux/blk.h>

#ifdef CONFIG_MD_BOOT
extern kdev_t name_to_kdev_t(char *line) md__init;
#endif

static mdk_personality_t *pers[MAX_PERSONALITY] = {NULL, };

/*
 * these have to be allocated separately because external
 * subsystems want to have a pre-defined structure
 */
struct hd_struct md_hd_struct[MAX_MD_DEVS];
static int md_blocksizes[MAX_MD_DEVS];
static int md_maxreadahead[MAX_MD_DEVS];
static mdk_thread_t *md_recovery_thread = NULL;

int md_size[MAX_MD_DEVS] = {0, };

static void md_geninit (struct gendisk *);

static struct gendisk md_gendisk=
{
	MD_MAJOR,
	"md",
	0,
	1,
	MAX_MD_DEVS,
	md_geninit,
	md_hd_struct,
	md_size,
	MAX_MD_DEVS,
	NULL,
	NULL
};

/*
 * Current RAID-1,4,5 parallel reconstruction 'guaranteed speed limit'
 * is 100 KB/sec, so the extra system load does not show up that much.
 * Increase it if you want to have more _guaranteed_ speed. Note that
 * the RAID driver will use the maximum available bandwith if the IO
 * subsystem is idle.
 *
 * you can change it via /proc/sys/dev/speed-limit
 */

static int sysctl_speed_limit = 100;

static struct ctl_table_header *md_table_header;

static ctl_table md_table[] = {
	{DEV_MD_SPEED_LIMIT, "speed-limit",
	 &sysctl_speed_limit, sizeof(int), 0644, NULL, &proc_dointvec},
	{0}
};

static ctl_table md_dir_table[] = {
        {DEV_MD, "md", NULL, 0, 0555, md_table},
        {0}
};

static ctl_table md_root_table[] = {
        {CTL_DEV, "dev", NULL, 0, 0555, md_dir_table},
        {0}
};

static void md_register_sysctl(void)
{
        md_table_header = register_sysctl_table(md_root_table, 1);
}

void md_unregister_sysctl(void)
{
        unregister_sysctl_table(md_table_header);
}

/*
 * The mapping between kdev and mddev is not necessary a simple
 * one! Eg. HSM uses several sub-devices to implement Logical
 * Volumes. All these sub-devices map to the same mddev.
 */
dev_mapping_t mddev_map [MAX_MD_DEVS] = { {NULL, 0}, };

void add_mddev_mapping (mddev_t * mddev, kdev_t dev, void *data)
{
	unsigned int minor = MINOR(dev);

	if (MAJOR(dev) != MD_MAJOR) {
		MD_BUG();
		return;
	}
	if (mddev_map[minor].mddev != NULL) {
		MD_BUG();
		return;
	}
	mddev_map[minor].mddev = mddev;
	mddev_map[minor].data = data;
}

void del_mddev_mapping (mddev_t * mddev, kdev_t dev)
{
	unsigned int minor = MINOR(dev);

	if (MAJOR(dev) != MD_MAJOR) {
		MD_BUG();
		return;
	}
	if (mddev_map[minor].mddev != mddev) {
		MD_BUG();
		return;
	}
	mddev_map[minor].mddev = NULL;
	mddev_map[minor].data = NULL;
}

/*
 * Enables to iterate over all existing md arrays
 */
static MD_LIST_HEAD(all_mddevs);

static mddev_t * alloc_mddev (kdev_t dev)
{
	mddev_t * mddev;

	if (MAJOR(dev) != MD_MAJOR) {
		MD_BUG();
		return 0;
	}
	mddev = (mddev_t *) kmalloc(sizeof(*mddev), GFP_KERNEL);
	if (!mddev)
		return NULL;
		
	memset(mddev, 0, sizeof(*mddev));

	mddev->__minor = MINOR(dev);
	mddev->reconfig_sem = MUTEX;
	mddev->recovery_sem = MUTEX;
	mddev->resync_sem = MUTEX;
	MD_INIT_LIST_HEAD(&mddev->disks);
	/*
	 * The 'base' mddev is the one with data NULL.
	 * personalities can create additional mddevs 
	 * if necessary.
	 */
	add_mddev_mapping(mddev, dev, 0);
	md_list_add(&mddev->all_mddevs, &all_mddevs);

	return mddev;
}

static void free_mddev (mddev_t *mddev)
{
	if (!mddev) {
		MD_BUG();
		return;
	}

	/*
	 * Make sure nobody else is using this mddev
	 * (careful, we rely on the global kernel lock here)
	 */
	while (md_atomic_read(&mddev->resync_sem.count) != 1)
		schedule();
	while (md_atomic_read(&mddev->recovery_sem.count) != 1)
		schedule();

	del_mddev_mapping(mddev, MKDEV(MD_MAJOR, mdidx(mddev)));
	md_list_del(&mddev->all_mddevs);
	MD_INIT_LIST_HEAD(&mddev->all_mddevs);
	kfree(mddev);
}


struct gendisk * find_gendisk (kdev_t dev)
{
	struct gendisk *tmp = gendisk_head;

	while (tmp != NULL) {
		if (tmp->major == MAJOR(dev))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

mdk_rdev_t * find_rdev_nr(mddev_t *mddev, int nr)
{
	mdk_rdev_t * rdev;
	struct md_list_head *tmp;

	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->desc_nr == nr)
			return rdev;
	}
	return NULL;
}

mdk_rdev_t * find_rdev(mddev_t * mddev, kdev_t dev)
{
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;

	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->dev == dev)
			return rdev;
	}
	return NULL;
}

static MD_LIST_HEAD(device_names);

char * partition_name (kdev_t dev)
{
	struct gendisk *hd;
	static char nomem [] = "<nomem>";
	dev_name_t *dname;
	struct md_list_head *tmp = device_names.next;

	while (tmp != &device_names) {
		dname = md_list_entry(tmp, dev_name_t, list);
		if (dname->dev == dev)
			return dname->name;
		tmp = tmp->next;
	}

	dname = (dev_name_t *) kmalloc(sizeof(*dname), GFP_KERNEL);

	if (!dname)
		return nomem;
	/*
	 * ok, add this new device name to the list
	 */
	hd = find_gendisk (dev);

	if (!hd)
		sprintf (dname->name, "[dev %s]", kdevname(dev));
	else
		disk_name (hd, MINOR(dev), dname->name);

	dname->dev = dev;
	md_list_add(&dname->list, &device_names);

	return dname->name;
}

static unsigned int calc_dev_sboffset (kdev_t dev, mddev_t *mddev,
						int persistent)
{
	unsigned int size = 0;

	if (blk_size[MAJOR(dev)])
		size = blk_size[MAJOR(dev)][MINOR(dev)];
	if (persistent)
		size = MD_NEW_SIZE_BLOCKS(size);
	return size;
}

static unsigned int calc_dev_size (kdev_t dev, mddev_t *mddev, int persistent)
{
	unsigned int size;

	size = calc_dev_sboffset(dev, mddev, persistent);
	if (!mddev->sb) {
		MD_BUG();
		return size;
	}
	if (mddev->sb->chunk_size)
		size &= ~(mddev->sb->chunk_size/1024 - 1);
	return size;
}

/*
 * We check wether all devices are numbered from 0 to nb_dev-1. The
 * order is guaranteed even after device name changes.
 *
 * Some personalities (raid0, linear) use this. Personalities that
 * provide data have to be able to deal with loss of individual
 * disks, so they do their checking themselves.
 */
int md_check_ordering (mddev_t *mddev)
{
	int i, c;
	mdk_rdev_t *rdev;
	struct md_list_head *tmp;

	/*
	 * First, all devices must be fully functional
	 */
	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty) {
			printk("md: md%d's device %s faulty, aborting.\n",
				mdidx(mddev), partition_name(rdev->dev));
			goto abort;
		}
	}

	c = 0;
	ITERATE_RDEV(mddev,rdev,tmp) {
		c++;
	}
	if (c != mddev->nb_dev) {
		MD_BUG();
		goto abort;
	}
	if (mddev->nb_dev != mddev->sb->raid_disks) {
		printk("md: md%d, array needs %d disks, has %d, aborting.\n",
			mdidx(mddev), mddev->sb->raid_disks, mddev->nb_dev);
		goto abort;
	}
	/*
	 * Now the numbering check
	 */
	for (i = 0; i < mddev->nb_dev; i++) {
		c = 0;
		ITERATE_RDEV(mddev,rdev,tmp) {
			if (rdev->desc_nr == i)
				c++;
		}
		if (c == 0) {
			printk("md: md%d, missing disk #%d, aborting.\n",
				mdidx(mddev), i);
			goto abort;
		}
		if (c > 1) {
			printk("md: md%d, too many disks #%d, aborting.\n",
				mdidx(mddev), i);
			goto abort;
		}
	}
	return 0;
abort:
	return 1;
}

static unsigned int zoned_raid_size (mddev_t *mddev)
{
	unsigned int mask;
	mdk_rdev_t * rdev;
	struct md_list_head *tmp;

	if (!mddev->sb) {
		MD_BUG();
		return -EINVAL;
	}
	/*
	 * do size and offset calculations.
	 */
	mask = ~(mddev->sb->chunk_size/1024 - 1);
printk("mask %08x\n", mask);

	ITERATE_RDEV(mddev,rdev,tmp) {
printk(" rdev->size: %d\n", rdev->size);
		rdev->size &= mask;
printk(" masked rdev->size: %d\n", rdev->size);
		md_size[mdidx(mddev)] += rdev->size;
printk("  new md_size: %d\n", md_size[mdidx(mddev)]);
	}
	return 0;
}

static void remove_descriptor (mdp_disk_t *disk, mdp_super_t *sb)
{
	if (disk_active(disk)) {
		sb->working_disks--;
	} else {
		if (disk_spare(disk)) {
			sb->spare_disks--;
			sb->working_disks--;
		} else	{
			sb->failed_disks--;
		}
	}
	sb->nr_disks--;
	disk->major = 0;
	disk->minor = 0;
	mark_disk_removed(disk);
}

#define BAD_MAGIC KERN_ERR \
"md: invalid raid superblock magic on %s\n"

#define BAD_MINOR KERN_ERR \
"md: %s: invalid raid minor (%x)\n"

#define OUT_OF_MEM KERN_ALERT \
"md: out of memory.\n"

#define NO_SB KERN_ERR \
"md: disabled device %s, could not read superblock.\n"

#define BAD_CSUM KERN_WARNING \
"md: invalid superblock checksum on %s\n"

static int alloc_array_sb (mddev_t * mddev)
{
	if (mddev->sb) {
		MD_BUG();
		return 0;
	}

	mddev->sb = (mdp_super_t *) __get_free_page (GFP_KERNEL);
	if (!mddev->sb)
		return -ENOMEM;
	md_clear_page((unsigned long)mddev->sb);
	return 0;
}

static int alloc_disk_sb (mdk_rdev_t * rdev)
{
	if (rdev->sb)
		MD_BUG();

	rdev->sb = (mdp_super_t *) __get_free_page(GFP_KERNEL);
	if (!rdev->sb) {
		printk (OUT_OF_MEM);
		return -EINVAL;
	}
	md_clear_page((unsigned long)rdev->sb);

	return 0;
}

static void free_disk_sb (mdk_rdev_t * rdev)
{
	if (rdev->sb) {
		free_page((unsigned long) rdev->sb);
		rdev->sb = NULL;
		rdev->sb_offset = 0;
		rdev->size = 0;
	} else {
		if (!rdev->faulty)
			MD_BUG();
	}
}

static void mark_rdev_faulty (mdk_rdev_t * rdev)
{
	unsigned long flags;

	if (!rdev) {
		MD_BUG();
		return;
	}
	save_flags(flags);
	cli();
	free_disk_sb(rdev);
	rdev->faulty = 1;
	restore_flags(flags);
}

static int read_disk_sb (mdk_rdev_t * rdev)
{
	int ret = -EINVAL;
	struct buffer_head *bh = NULL;
	kdev_t dev = rdev->dev;
	mdp_super_t *sb;
	u32 sb_offset;

	if (!rdev->sb) {
		MD_BUG();
		goto abort;
	}	
	
	/*
	 * Calculate the position of the superblock,
	 * it's at the end of the disk
	 */
	sb_offset = calc_dev_sboffset(rdev->dev, rdev->mddev, 1);
	rdev->sb_offset = sb_offset;
	printk("(read) %s's sb offset: %d", partition_name(dev),
							 sb_offset);
	fsync_dev(dev);
	set_blocksize (dev, MD_SB_BYTES);
	bh = bread (dev, sb_offset / MD_SB_BLOCKS, MD_SB_BYTES);

	if (bh) {
		sb = (mdp_super_t *) bh->b_data;
		memcpy (rdev->sb, sb, MD_SB_BYTES);
	} else {
		printk (NO_SB,partition_name(rdev->dev));
		goto abort;
	}
	printk(" [events: %08lx]\n", (unsigned long)get_unaligned(&rdev->sb->events));
	ret = 0;
abort:
	if (bh)
		brelse (bh);
	return ret;
}

static unsigned int calc_sb_csum (mdp_super_t * sb)
{
	unsigned int disk_csum, csum;

	disk_csum = sb->sb_csum;
	sb->sb_csum = 0;
	csum = csum_partial((void *)sb, MD_SB_BYTES, 0);
	sb->sb_csum = disk_csum;
	return csum;
}

/*
 * Check one RAID superblock for generic plausibility
 */

static int check_disk_sb (mdk_rdev_t * rdev)
{
	mdp_super_t *sb;
	int ret = -EINVAL;

	sb = rdev->sb;
	if (!sb) {
		MD_BUG();
		goto abort;
	}

	if (sb->md_magic != MD_SB_MAGIC) {
		printk (BAD_MAGIC, partition_name(rdev->dev));
		goto abort;
	}

	if (sb->md_minor >= MAX_MD_DEVS) {
		printk (BAD_MINOR, partition_name(rdev->dev),
							sb->md_minor);
		goto abort;
	}

	if (calc_sb_csum(sb) != sb->sb_csum)
		printk(BAD_CSUM, partition_name(rdev->dev));
	ret = 0;
abort:
	return ret;
}

static kdev_t dev_unit(kdev_t dev)
{
	unsigned int mask;
	struct gendisk *hd = find_gendisk(dev);

	if (!hd)
		return 0;
	mask = ~((1 << hd->minor_shift) - 1);

	return MKDEV(MAJOR(dev), MINOR(dev) & mask);
}

static mdk_rdev_t * match_dev_unit(mddev_t *mddev, kdev_t dev)
{
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;

	ITERATE_RDEV(mddev,rdev,tmp)
		if (dev_unit(rdev->dev) == dev_unit(dev))
			return rdev;

	return NULL;
}

static int match_mddev_units(mddev_t *mddev1, mddev_t *mddev2)
{
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;

	ITERATE_RDEV(mddev1,rdev,tmp)
		if (match_dev_unit(mddev2, rdev->dev))
			return 1;

	return 0;
}

static MD_LIST_HEAD(all_raid_disks);
static MD_LIST_HEAD(pending_raid_disks);

static void bind_rdev_to_array (mdk_rdev_t * rdev, mddev_t * mddev)
{
	mdk_rdev_t *same_pdev;

	if (rdev->mddev) {
		MD_BUG();
		return;
	}
	same_pdev = match_dev_unit(mddev, rdev->dev);
	if (same_pdev)
		printk( KERN_WARNING
"md%d: WARNING: %s appears to be on the same physical disk as %s. True\n"
"     protection against single-disk failure might be compromised.\n",
 			mdidx(mddev), partition_name(rdev->dev),
				partition_name(same_pdev->dev));
		
	md_list_add(&rdev->same_set, &mddev->disks);
	rdev->mddev = mddev;
	mddev->nb_dev++;
	printk("bind<%s,%d>\n", partition_name(rdev->dev), mddev->nb_dev);
}

static void unbind_rdev_from_array (mdk_rdev_t * rdev)
{
	if (!rdev->mddev) {
		MD_BUG();
		return;
	}
	md_list_del(&rdev->same_set);
	MD_INIT_LIST_HEAD(&rdev->same_set);
	rdev->mddev->nb_dev--;
	printk("unbind<%s,%d>\n", partition_name(rdev->dev),
						 rdev->mddev->nb_dev);
	rdev->mddev = NULL;
}

/*
 * prevent the device from being mounted, repartitioned or
 * otherwise reused by a RAID array (or any other kernel
 * subsystem), by opening the device. [simply getting an
 * inode is not enough, the SCSI module usage code needs
 * an explicit open() on the device]
 */
static int lock_rdev (mdk_rdev_t *rdev)
{
	int err = 0;

	/*
	 * First insert a dummy inode.
	 */
	if (rdev->inode)
		MD_BUG();
	rdev->inode = get_empty_inode();
 	/*
	 * we dont care about any other fields
	 */
	rdev->inode->i_dev = rdev->inode->i_rdev = rdev->dev;
	insert_inode_hash(rdev->inode);

	memset(&rdev->filp, 0, sizeof(rdev->filp));
	rdev->filp.f_mode = 3; /* read write */
	err = blkdev_open(rdev->inode, &rdev->filp);
	if (err) {
		printk("blkdev_open() failed: %d\n", err);
		clear_inode(rdev->inode);
		rdev->inode = NULL;
	}
	return err;
}

static void unlock_rdev (mdk_rdev_t *rdev)
{
	blkdev_release(rdev->inode);
	if (!rdev->inode)
		MD_BUG();
	clear_inode(rdev->inode);
	rdev->inode = NULL;
}

static void export_rdev (mdk_rdev_t * rdev)
{
	printk("export_rdev(%s)\n",partition_name(rdev->dev));
	if (rdev->mddev)
		MD_BUG();
	unlock_rdev(rdev);
	free_disk_sb(rdev);
	md_list_del(&rdev->all);
	MD_INIT_LIST_HEAD(&rdev->all);
	if (rdev->pending.next != &rdev->pending) {
		printk("(%s was pending)\n",partition_name(rdev->dev));
		md_list_del(&rdev->pending);
		MD_INIT_LIST_HEAD(&rdev->pending);
	}
	rdev->dev = 0;
	rdev->faulty = 0;
	kfree(rdev);
}

static void kick_rdev_from_array (mdk_rdev_t * rdev)
{
	unbind_rdev_from_array(rdev);
	export_rdev(rdev);
}

static void export_array (mddev_t *mddev)
{
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;
	mdp_super_t *sb = mddev->sb;

	if (mddev->sb) {
		mddev->sb = NULL;
		free_page((unsigned long) sb);
	}

	ITERATE_RDEV(mddev,rdev,tmp) {
		if (!rdev->mddev) {
			MD_BUG();
			continue;
		}
		kick_rdev_from_array(rdev);
	}
	if (mddev->nb_dev)
		MD_BUG();
}

#undef BAD_CSUM
#undef BAD_MAGIC
#undef OUT_OF_MEM
#undef NO_SB

static void print_desc(mdp_disk_t *desc)
{
	printk(" DISK<N:%d,%s(%d,%d),R:%d,S:%d>\n", desc->number,
		partition_name(MKDEV(desc->major,desc->minor)),
		desc->major,desc->minor,desc->raid_disk,desc->state);
}

static void print_sb(mdp_super_t *sb)
{
	int i;

	printk("  SB: (V:%d.%d.%d) ID:<%08x.%08x.%08x.%08x> CT:%08x\n",
		sb->major_version, sb->minor_version, sb->patch_version,
		sb->set_uuid0, sb->set_uuid1, sb->set_uuid2, sb->set_uuid3,
		sb->ctime);
	printk("     L%d S%08d ND:%d RD:%d md%d LO:%d CS:%d\n", sb->level,
		sb->size, sb->nr_disks, sb->raid_disks, sb->md_minor,
		sb->layout, sb->chunk_size);
	printk("     UT:%08x ST:%d AD:%d WD:%d FD:%d SD:%d CSUM:%08x E:%08lx\n",
		sb->utime, sb->state, sb->active_disks, sb->working_disks,
		sb->failed_disks, sb->spare_disks,
		sb->sb_csum, (unsigned long)get_unaligned(&sb->events));

	for (i = 0; i < MD_SB_DISKS; i++) {
		mdp_disk_t *desc;

		desc = sb->disks + i;
		printk("     D %2d: ", i);
		print_desc(desc);
	}
	printk("     THIS: ");
	print_desc(&sb->this_disk);

}

static void print_rdev(mdk_rdev_t *rdev)
{
	printk(" rdev %s: O:%s, SZ:%08d F:%d DN:%d ",
		partition_name(rdev->dev), partition_name(rdev->old_dev),
		rdev->size, rdev->faulty, rdev->desc_nr);
	if (rdev->sb) {
		printk("rdev superblock:\n");
		print_sb(rdev->sb);
	} else
		printk("no rdev superblock!\n");
}

void md_print_devices (void)
{
	struct md_list_head *tmp, *tmp2;
	mdk_rdev_t *rdev;
	mddev_t *mddev;

	printk("\n");
	printk("       **********************************\n");
	printk("       * <COMPLETE RAID STATE PRINTOUT> *\n");
	printk("       **********************************\n");
	ITERATE_MDDEV(mddev,tmp) {
		printk("md%d: ", mdidx(mddev));

		ITERATE_RDEV(mddev,rdev,tmp2)
			printk("<%s>", partition_name(rdev->dev));

		if (mddev->sb) {
			printk(" array superblock:\n");
			print_sb(mddev->sb);
		} else
			printk(" no array superblock.\n");

		ITERATE_RDEV(mddev,rdev,tmp2)
			print_rdev(rdev);
	}
	printk("       **********************************\n");
	printk("\n");
}

static int sb_equal ( mdp_super_t *sb1, mdp_super_t *sb2)
{
	int ret;
	mdp_super_t *tmp1, *tmp2;

	tmp1 = kmalloc(MD_SB_BYTES,GFP_KERNEL);
	tmp2 = kmalloc(MD_SB_BYTES,GFP_KERNEL);

	if (!tmp1 || !tmp2) {
		ret = 0;
		goto abort;
	}

	memcpy(tmp1, sb1, MD_SB_BYTES);
	memcpy(tmp2, sb2, MD_SB_BYTES);

	/*
	 * nr_disks is not constant
	 */
	tmp1->nr_disks = 0;
	tmp2->nr_disks = 0;

	if (memcmp(tmp1, tmp2, MD_SB_GENERIC_CONSTANT_WORDS * 4))
		ret = 0;
	else
		ret = 1;

abort:
	if (tmp1)
		kfree(tmp1);
	if (tmp2)
		kfree(tmp2);

	return ret;
}

static int uuid_equal(mdk_rdev_t *rdev1, mdk_rdev_t *rdev2)
{
	if (	(rdev1->sb->set_uuid0 == rdev2->sb->set_uuid0) &&
		(rdev1->sb->set_uuid1 == rdev2->sb->set_uuid1) &&
		(rdev1->sb->set_uuid2 == rdev2->sb->set_uuid2) &&
		(rdev1->sb->set_uuid3 == rdev2->sb->set_uuid3))

		return 1;

	return 0;
}

static mdk_rdev_t * find_rdev_all (kdev_t dev)
{
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;

	tmp = all_raid_disks.next;
	while (tmp != &all_raid_disks) {
		rdev = md_list_entry(tmp, mdk_rdev_t, all);
		if (rdev->dev == dev)
			return rdev;
		tmp = tmp->next;
	}
	return NULL;
}

#define GETBLK_FAILED KERN_ERR \
"md: getblk failed for device %s\n"

static int write_disk_sb(mdk_rdev_t * rdev)
{
	struct buffer_head *bh;
	kdev_t dev;
	u32 sb_offset, size;
	mdp_super_t *sb;

	if (!rdev->sb) {
		MD_BUG();
		return -1;
	}
	if (rdev->faulty) {
		MD_BUG();
		return -1;
	}
	if (rdev->sb->md_magic != MD_SB_MAGIC) {
		MD_BUG();
		return -1;
	}

	dev = rdev->dev;
	sb_offset = calc_dev_sboffset(dev, rdev->mddev, 1);
	if (rdev->sb_offset != sb_offset) {
		printk("%s's sb offset has changed from %d to %d, skipping\n", partition_name(dev), rdev->sb_offset, sb_offset);
		goto skip;
	}
	/*
	 * If the disk went offline meanwhile and it's just a spare, then
	 * it's size has changed to zero silently, and the MD code does
	 * not yet know that it's faulty.
	 */
	size = calc_dev_size(dev, rdev->mddev, 1);
	if (size != rdev->size) {
		printk("%s's size has changed from %d to %d since import, skipping\n", partition_name(dev), rdev->size, size);
		goto skip;
	}

	printk("(write) %s's sb offset: %d\n", partition_name(dev), sb_offset);
	fsync_dev(dev);
	set_blocksize(dev, MD_SB_BYTES);
	bh = getblk(dev, sb_offset / MD_SB_BLOCKS, MD_SB_BYTES);
	if (!bh) {
		printk(GETBLK_FAILED, partition_name(dev));
		return 1;
	}
	memset(bh->b_data,0,bh->b_size);
	sb = (mdp_super_t *) bh->b_data;
	memcpy(sb, rdev->sb, MD_SB_BYTES);

	mark_buffer_uptodate(bh, 1);
	mark_buffer_dirty(bh, 1);
	ll_rw_block(WRITE, 1, &bh);
	wait_on_buffer(bh);
	brelse(bh);
	fsync_dev(dev);
skip:
	return 0;
}
#undef GETBLK_FAILED KERN_ERR

static void set_this_disk(mddev_t *mddev, mdk_rdev_t *rdev)
{
	int i, ok = 0;
	mdp_disk_t *desc;

	for (i = 0; i < MD_SB_DISKS; i++) {
		desc = mddev->sb->disks + i;
#if 0
		if (disk_faulty(desc)) {
			if (MKDEV(desc->major,desc->minor) == rdev->dev)
				ok = 1;
			continue;
		}
#endif
		if (MKDEV(desc->major,desc->minor) == rdev->dev) {
			rdev->sb->this_disk = *desc;
			rdev->desc_nr = desc->number;
			ok = 1;
			break;
		}
	}

	if (!ok) {
		MD_BUG();
	}
}

static int sync_sbs(mddev_t * mddev)
{
	mdk_rdev_t *rdev;
	mdp_super_t *sb;
        struct md_list_head *tmp;

        ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty)
			continue;
		sb = rdev->sb;
		memcpy(sb, mddev->sb, MD_SB_BYTES);
		set_this_disk(mddev, rdev);
		sb->sb_csum = calc_sb_csum(sb);
	}
	return 0;
}

int md_update_sb(mddev_t * mddev)
{
	int first, err, count = 100;
        struct md_list_head *tmp;
	mdk_rdev_t *rdev;
	__u64 ev;

repeat:
	mddev->sb->utime = CURRENT_TIME;
	ev = get_unaligned(&mddev->sb->events);
	++ev;
	put_unaligned(ev,&mddev->sb->events);
	if (ev == (__u64)0) {
		/*
		 * oops, this 64-bit counter should never wrap.
		 * Either we are in around ~1 trillion A.C., assuming
		 * 1 reboot per second, or we have a bug:
		 */
		MD_BUG();
		--ev;
		put_unaligned(ev,&mddev->sb->events);
	}
	sync_sbs(mddev);

	/*
	 * do not write anything to disk if using
	 * nonpersistent superblocks
	 */
	if (mddev->sb->not_persistent)
		return 0;

	printk(KERN_INFO "md: updating md%d RAID superblock on device\n",
					mdidx(mddev));

	first = 1;
	err = 0;
        ITERATE_RDEV(mddev,rdev,tmp) {
		if (!first) {
			first = 0;
			printk(", ");
		}
		if (rdev->faulty)
			printk("(skipping faulty ");
		printk("%s ", partition_name(rdev->dev));
		if (!rdev->faulty) {
			printk("[events: %08lx]",
			       (unsigned long)get_unaligned(&rdev->sb->events));
			err += write_disk_sb(rdev);
		} else
			printk(")\n");
	}
	printk(".\n");
	if (err) {
		printk("errors occured during superblock update, repeating\n");
		if (--count)
			goto repeat;
		printk("excessive errors occured during superblock update, exiting\n");
	}
	return 0;
}

/*
 * Import a device. If 'on_disk', then sanity check the superblock
 *
 * mark the device faulty if:
 *
 *   - the device is nonexistent (zero size)
 *   - the device has no valid superblock
 *
 * a faulty rdev _never_ has rdev->sb set.
 */
static int md_import_device (kdev_t newdev, int on_disk)
{
	int err;
	mdk_rdev_t *rdev;
	unsigned int size;

	if (find_rdev_all(newdev))
		return -EEXIST;

	rdev = (mdk_rdev_t *) kmalloc(sizeof(*rdev), GFP_KERNEL);
	if (!rdev) {
		printk("could not alloc mem for %s!\n", partition_name(newdev));
		return -ENOMEM;
	}
	memset(rdev, 0, sizeof(*rdev));

	if (!fs_may_mount(newdev)) {
		printk("md: can not import %s, has active inodes!\n",
			partition_name(newdev));
		err = -EBUSY;
		goto abort_free;
	}

	if ((err = alloc_disk_sb(rdev)))
		goto abort_free;

	rdev->dev = newdev;
	if (lock_rdev(rdev)) {
		printk("md: could not lock %s, zero-size? Marking faulty.\n",
			partition_name(newdev));
		err = -EINVAL;
		goto abort_free;
	}
	rdev->desc_nr = -1;
	rdev->faulty = 0;

	size = 0;
	if (blk_size[MAJOR(newdev)])
		size = blk_size[MAJOR(newdev)][MINOR(newdev)];
	if (!size) {
		printk("md: %s has zero size, marking faulty!\n",
				partition_name(newdev));
		err = -EINVAL;
		goto abort_free;
	}

	if (on_disk) {
		if ((err = read_disk_sb(rdev))) {
			printk("md: could not read %s's sb, not importing!\n",
					partition_name(newdev));
			goto abort_free;
		}
		if ((err = check_disk_sb(rdev))) {
			printk("md: %s has invalid sb, not importing!\n",
					partition_name(newdev));
			goto abort_free;
		}

		rdev->old_dev = MKDEV(rdev->sb->this_disk.major,
					rdev->sb->this_disk.minor);
		rdev->desc_nr = rdev->sb->this_disk.number;
	}
	md_list_add(&rdev->all, &all_raid_disks);
	MD_INIT_LIST_HEAD(&rdev->pending);

	if (rdev->faulty && rdev->sb)
		free_disk_sb(rdev);
	return 0;

abort_free:
	if (rdev->sb) {
		if (rdev->inode)
			unlock_rdev(rdev);
		free_disk_sb(rdev);
	}
	kfree(rdev);
	return err;
}

/*
 * Check a full RAID array for plausibility
 */

#define INCONSISTENT KERN_ERR \
"md: fatal superblock inconsistency in %s -- removing from array\n"

#define OUT_OF_DATE KERN_ERR \
"md: superblock update time inconsistency -- using the most recent one\n"

#define OLD_VERSION KERN_ALERT \
"md: md%d: unsupported raid array version %d.%d.%d\n"

#define NOT_CLEAN_IGNORE KERN_ERR \
"md: md%d: raid array is not clean -- starting background reconstruction\n"

#define UNKNOWN_LEVEL KERN_ERR \
"md: md%d: unsupported raid level %d\n"

static int analyze_sbs (mddev_t * mddev)
{
	int out_of_date = 0, i;
	struct md_list_head *tmp, *tmp2;
	mdk_rdev_t *rdev, *rdev2, *freshest;
	mdp_super_t *sb;

	/*
	 * Verify the RAID superblock on each real device
	 */
	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty) {
			MD_BUG();
			goto abort;
		}
		if (!rdev->sb) {
			MD_BUG();
			goto abort;
		}
		if (check_disk_sb(rdev))
			goto abort;
	}

	/*
	 * The superblock constant part has to be the same
	 * for all disks in the array.
	 */
	sb = NULL;

	ITERATE_RDEV(mddev,rdev,tmp) {
		if (!sb) {
			sb = rdev->sb;
			continue;
		}
		if (!sb_equal(sb, rdev->sb)) {
			printk (INCONSISTENT, partition_name(rdev->dev));
			kick_rdev_from_array(rdev);
			continue;
		}
	}

	/*
	 * OK, we have all disks and the array is ready to run. Let's
	 * find the freshest superblock, that one will be the superblock
	 * that represents the whole array.
	 */
	if (!mddev->sb)
		if (alloc_array_sb(mddev))
			goto abort;
	sb = mddev->sb;
	freshest = NULL;

	ITERATE_RDEV(mddev,rdev,tmp) {
		__u64 ev1, ev2;
		/*
		 * if the checksum is invalid, use the superblock
		 * only as a last resort. (decrease it's age by
		 * one event)
		 */
		if (calc_sb_csum(rdev->sb) != rdev->sb->sb_csum) {
			__u64 ev = get_unaligned(&rdev->sb->events);
			if (ev != (__u64)0) {
				--ev;
				put_unaligned(ev,&rdev->sb->events);
			}
		}

		printk("%s's event counter: %08lx\n", partition_name(rdev->dev),
		       (unsigned long)get_unaligned(&rdev->sb->events));
		if (!freshest) {
			freshest = rdev;
			continue;
		}
		/*
		 * Find the newest superblock version
		 */
		ev1 = get_unaligned(&rdev->sb->events);
		ev2 = get_unaligned(&freshest->sb->events);
		if (ev1 != ev2) {
			out_of_date = 1;
			if (ev1 > ev2)
				freshest = rdev;
		}
	}
	if (out_of_date) {
		printk(OUT_OF_DATE);
		printk("freshest: %s\n", partition_name(freshest->dev));
	}
	memcpy (sb, freshest->sb, MD_SB_BYTES);

	/*
	 * at this point we have picked the 'best' superblock
	 * from all available superblocks.
	 * now we validate this superblock and kick out possibly
	 * failed disks.
	 */
	ITERATE_RDEV(mddev,rdev,tmp) {
		/*
		 * Kick all non-fresh devices faulty
		 */
		__u64 ev1, ev2;
		ev1 = get_unaligned(&rdev->sb->events);
		ev2 = get_unaligned(&sb->events);
		++ev1;
		if (ev1 < ev2) {
			printk("md: kicking non-fresh %s from array!\n",
						partition_name(rdev->dev));
			kick_rdev_from_array(rdev);
			continue;
		}
	}

	/*
	 * Fix up changed device names ... but only if this disk has a
	 * recent update time. Use faulty checksum ones too.
	 */
	ITERATE_RDEV(mddev,rdev,tmp) {
		__u64 ev1, ev2, ev3;
		if (rdev->faulty) { /* REMOVEME */
			MD_BUG();
			goto abort;
		}
		ev1 = get_unaligned(&rdev->sb->events);
		ev2 = get_unaligned(&sb->events);
		ev3 = ev2;
		--ev3;
		if ((rdev->dev != rdev->old_dev) &&
		    ((ev1 == ev2) || (ev1 == ev3))) {
			mdp_disk_t *desc;

			printk("md: device name has changed from %s to %s since last import!\n", partition_name(rdev->old_dev), partition_name(rdev->dev));
			if (rdev->desc_nr == -1) {
				MD_BUG();
				goto abort;
			}
			desc = &sb->disks[rdev->desc_nr];
			if (rdev->old_dev != MKDEV(desc->major, desc->minor)) {
				MD_BUG();
				goto abort;
			}
			desc->major = MAJOR(rdev->dev);
			desc->minor = MINOR(rdev->dev);
			desc = &rdev->sb->this_disk;
			desc->major = MAJOR(rdev->dev);
			desc->minor = MINOR(rdev->dev);
		}
	}

	/*
	 * Remove unavailable and faulty devices ...
	 *
	 * note that if an array becomes completely unrunnable due to
	 * missing devices, we do not write the superblock back, so the
	 * administrator has a chance to fix things up. The removal thus
	 * only happens if it's nonfatal to the contents of the array.
	 */
	for (i = 0; i < MD_SB_DISKS; i++) {
		int found;
		mdp_disk_t *desc;
		kdev_t dev;

		desc = sb->disks + i;
		dev = MKDEV(desc->major, desc->minor);

		/*
		 * We kick faulty devices/descriptors immediately.
		 */
		if (disk_faulty(desc)) {
			found = 0;
			ITERATE_RDEV(mddev,rdev,tmp) {
				if (rdev->desc_nr != desc->number)
					continue;
				printk("md%d: kicking faulty %s!\n",
					mdidx(mddev),partition_name(rdev->dev));
				kick_rdev_from_array(rdev);
				found = 1;
				break;
			}
			if (!found) {
				if (dev == MKDEV(0,0))
					continue;
				printk("md%d: removing former faulty %s!\n",
					mdidx(mddev), partition_name(dev));
			}
			remove_descriptor(desc, sb);
			continue;
		}

		if (dev == MKDEV(0,0))
			continue;
		/*
		 * Is this device present in the rdev ring?
		 */
		found = 0;
		ITERATE_RDEV(mddev,rdev,tmp) {
			if (rdev->desc_nr == desc->number) {
				found = 1;
				break;
			}
		}
		if (found) 
			continue;

		printk("md%d: former device %s is unavailable, removing from array!\n", mdidx(mddev), partition_name(dev));
		remove_descriptor(desc, sb);
	}

	/*
	 * Double check wether all devices mentioned in the
	 * superblock are in the rdev ring.
	 */
	for (i = 0; i < MD_SB_DISKS; i++) {
		mdp_disk_t *desc;
		kdev_t dev;

		desc = sb->disks + i;
		dev = MKDEV(desc->major, desc->minor);

		if (dev == MKDEV(0,0))
			continue;

		if (disk_faulty(desc)) {
			MD_BUG();
			goto abort;
		}

		rdev = find_rdev(mddev, dev);
		if (!rdev) {
			MD_BUG();
			goto abort;
		}
	}

	/*
	 * Do a final reality check.
	 */
	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->desc_nr == -1) {
			MD_BUG();
			goto abort;
		}
		/*
		 * is the desc_nr unique?
		 */
		ITERATE_RDEV(mddev,rdev2,tmp2) {
			if ((rdev2 != rdev) &&
					(rdev2->desc_nr == rdev->desc_nr)) {
				MD_BUG();
				goto abort;
			}
		}
		/*
		 * is the device unique?
		 */
		ITERATE_RDEV(mddev,rdev2,tmp2) {
			if ((rdev2 != rdev) &&
					(rdev2->dev == rdev->dev)) {
				MD_BUG();
				goto abort;
			}
		}
	}

	/*
	 * Check if we can support this RAID array
	 */
	if (sb->major_version != MD_MAJOR_VERSION ||
			sb->minor_version > MD_MINOR_VERSION) {

		printk (OLD_VERSION, mdidx(mddev), sb->major_version,
				sb->minor_version, sb->patch_version);
		goto abort;
	}

	if ((sb->state != (1 << MD_SB_CLEAN)) && ((sb->level == 1) ||
			(sb->level == 4) || (sb->level == 5)))
		printk (NOT_CLEAN_IGNORE, mdidx(mddev));

	return 0;
abort:
	return 1;
}

#undef INCONSISTENT
#undef OUT_OF_DATE
#undef OLD_VERSION
#undef OLD_LEVEL

static int device_size_calculation (mddev_t * mddev)
{
	int data_disks = 0, persistent;
	unsigned int readahead;
	mdp_super_t *sb = mddev->sb;
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;

	/*
	 * Do device size calculation. Bail out if too small.
	 * (we have to do this after having validated chunk_size,
	 * because device size has to be modulo chunk_size)
	 */ 
	persistent = !mddev->sb->not_persistent;
	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty)
			continue;
		if (rdev->size) {
			MD_BUG();
			continue;
		}
		rdev->size = calc_dev_size(rdev->dev, mddev, persistent);
		if (rdev->size < sb->chunk_size / 1024) {
			printk (KERN_WARNING
				"Dev %s smaller than chunk_size: %dk < %dk\n",
				partition_name(rdev->dev),
				rdev->size, sb->chunk_size / 1024);
			return -EINVAL;
		}
	}

	switch (sb->level) {
		case -3:
			data_disks = 1;
			break;
		case -2:
			data_disks = 1;
			break;
		case -1:
			zoned_raid_size(mddev);
			data_disks = 1;
			break;
		case 0:
			zoned_raid_size(mddev);
			data_disks = sb->raid_disks;
			break;
		case 1:
			data_disks = 1;
			break;
		case 4:
		case 5:
			data_disks = sb->raid_disks-1;
			break;
		default:
			printk (UNKNOWN_LEVEL, mdidx(mddev), sb->level);
			goto abort;
	}
	if (!md_size[mdidx(mddev)])
		md_size[mdidx(mddev)] = sb->size * data_disks;

	readahead = MD_READAHEAD;
	if ((sb->level == 0) || (sb->level == 4) || (sb->level == 5))
		readahead = mddev->sb->chunk_size * 4 * data_disks;
		if (readahead < data_disks * MAX_SECTORS*512*2) 
			readahead = data_disks * MAX_SECTORS*512*2;
	else {
		if (sb->level == -3)
			readahead = 0;
	}
	md_maxreadahead[mdidx(mddev)] = readahead;

	printk(KERN_INFO "md%d: max total readahead window set to %dk\n",
		mdidx(mddev), readahead/1024);

	printk(KERN_INFO
		"md%d: %d data-disks, max readahead per data-disk: %dk\n",
			mdidx(mddev), data_disks, readahead/data_disks/1024);
	return 0;
abort:
	return 1;
}


#define TOO_BIG_CHUNKSIZE KERN_ERR \
"too big chunk_size: %d > %d\n"

#define TOO_SMALL_CHUNKSIZE KERN_ERR \
"too small chunk_size: %d < %ld\n"

#define BAD_CHUNKSIZE KERN_ERR \
"no chunksize specified, see 'man raidtab'\n"

static int do_md_run (mddev_t * mddev)
{
	int pnum, err;
	int chunk_size;
	struct md_list_head *tmp;
	mdk_rdev_t *rdev;


	if (!mddev->nb_dev) {
		MD_BUG();
		return -EINVAL;
	}
  
	if (mddev->pers)
		return -EBUSY;

	/*
	 * Resize disks to align partitions size on a given
	 * chunk size.
	 */
	md_size[mdidx(mddev)] = 0;

	/*
	 * Analyze all RAID superblock(s)
	 */ 
	if (analyze_sbs(mddev)) {
		MD_BUG();
		return -EINVAL;
	}

	chunk_size = mddev->sb->chunk_size;
	pnum = level_to_pers(mddev->sb->level);

	mddev->param.chunk_size = chunk_size;
	mddev->param.personality = pnum;

	if (chunk_size > MAX_CHUNK_SIZE) {
		printk(TOO_BIG_CHUNKSIZE, chunk_size, MAX_CHUNK_SIZE);
		return -EINVAL;
	}
	/*
	 * chunk-size has to be a power of 2 and multiples of PAGE_SIZE
	 */
	if ( (1 << ffz(~chunk_size)) != chunk_size) {
		MD_BUG();
		return -EINVAL;
	}
	if (chunk_size < PAGE_SIZE) {
		printk(TOO_SMALL_CHUNKSIZE, chunk_size, PAGE_SIZE);
		return -EINVAL;
	}

	if (pnum >= MAX_PERSONALITY) {
		MD_BUG();
		return -EINVAL;
	}

	if ((pnum != RAID1) && (pnum != LINEAR) && !chunk_size) {
		/*
		 * 'default chunksize' in the old md code used to
		 * be PAGE_SIZE, baaad.
		 * we abort here to be on the safe side. We dont
		 * want to continue the bad practice.
		 */
		printk(BAD_CHUNKSIZE);
		return -EINVAL;
	}

	if (!pers[pnum])
	{
#ifdef CONFIG_KMOD
		char module_name[80];
		sprintf (module_name, "md-personality-%d", pnum);
		request_module (module_name);
		if (!pers[pnum])
#endif
			return -EINVAL;
	}
  
	if (device_size_calculation(mddev))
		return -EINVAL;

	/*
	 * Drop all container device buffers, from now on
	 * the only valid external interface is through the md
	 * device.
	 */
	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty)
			continue;
		fsync_dev(rdev->dev);
		invalidate_buffers(rdev->dev);
	}
  
	mddev->pers = pers[pnum];
  
	err = mddev->pers->run(mddev);
	if (err) {
		printk("pers->run() failed ...\n");
		mddev->pers = NULL;
		return -EINVAL;
	}

	mddev->sb->state &= ~(1 << MD_SB_CLEAN);
	md_update_sb(mddev);

	/*
	 * md_size has units of 1K blocks, which are
	 * twice as large as sectors.
	 */
	md_hd_struct[mdidx(mddev)].start_sect = 0;
	md_hd_struct[mdidx(mddev)].nr_sects = md_size[mdidx(mddev)] << 1;
  
	read_ahead[MD_MAJOR] = 1024;
	return (0);
}

#undef TOO_BIG_CHUNKSIZE
#undef BAD_CHUNKSIZE

#define OUT(x) do { err = (x); goto out; } while (0)

static int restart_array (mddev_t *mddev)
{
	int err = 0;
 
	/*
	 * Complain if it has no devices
	 */
	if (!mddev->nb_dev)
		OUT(-ENXIO);

	if (mddev->pers) {
		if (!mddev->ro)
			OUT(-EBUSY);

		mddev->ro = 0;
		set_device_ro(mddev_to_kdev(mddev), 0);

		printk (KERN_INFO
			"md%d switched to read-write mode.\n", mdidx(mddev));
		/*
		 * Kick recovery or resync if necessary
		 */
		md_recover_arrays();
		if (mddev->pers->restart_resync)
			mddev->pers->restart_resync(mddev);
	} else
		err = -EINVAL;
 
out:
	return err;
}

#define STILL_MOUNTED KERN_WARNING \
"md: md%d still mounted.\n"

static int do_md_stop (mddev_t * mddev, int ro)
{
	int err = 0, resync_interrupted = 0;
	kdev_t dev = mddev_to_kdev(mddev);
 
	if (!ro && !fs_may_mount (dev)) {
		printk (STILL_MOUNTED, mdidx(mddev));
		OUT(-EBUSY);
	}
  
	/*
	 * complain if it's already stopped
	 */
	if (!mddev->nb_dev)
		OUT(-ENXIO);

	if (mddev->pers) {
		/*
		 * It is safe to call stop here, it only frees private
		 * data. Also, it tells us if a device is unstoppable
		 * (eg. resyncing is in progress)
		 */
		if (mddev->pers->stop_resync)
			if (mddev->pers->stop_resync(mddev))
				resync_interrupted = 1;

		if (mddev->recovery_running)
			md_interrupt_thread(md_recovery_thread);

		/*
		 * This synchronizes with signal delivery to the
		 * resync or reconstruction thread. It also nicely
		 * hangs the process if some reconstruction has not
		 * finished.
		 */
		down(&mddev->recovery_sem);
		up(&mddev->recovery_sem);

		/*
		 *  sync and invalidate buffers because we cannot kill the
		 *  main thread with valid IO transfers still around.
		 *  the kernel lock protects us from new requests being
		 *  added after invalidate_buffers().
		 */
		fsync_dev (mddev_to_kdev(mddev));
		fsync_dev (dev);
		invalidate_buffers (dev);

		if (ro) {
			if (mddev->ro)
				OUT(-ENXIO);
			mddev->ro = 1;
		} else {
			if (mddev->ro)
				set_device_ro(dev, 0);
			if (mddev->pers->stop(mddev)) {
				if (mddev->ro)
					set_device_ro(dev, 1);
				OUT(-EBUSY);
			}
			if (mddev->ro)
				mddev->ro = 0;
		}
		if (mddev->sb) {
			/*
			 * mark it clean only if there was no resync
			 * interrupted.
			 */
			if (!mddev->recovery_running && !resync_interrupted) {
				printk("marking sb clean...\n");
				mddev->sb->state |= 1 << MD_SB_CLEAN;
			}
			md_update_sb(mddev);
		}
		if (ro)
			set_device_ro(dev, 1);
	}
 
	/*
	 * Free resources if final stop
	 */
	if (!ro) {
		export_array(mddev);
		md_size[mdidx(mddev)] = 0;
		md_hd_struct[mdidx(mddev)].nr_sects = 0;
		free_mddev(mddev);

		printk (KERN_INFO "md%d stopped.\n", mdidx(mddev));
	} else
		printk (KERN_INFO
			"md%d switched to read-only mode.\n", mdidx(mddev));
out:
	return err;
}

#undef OUT

/*
 * We have to safely support old arrays too.
 */
int detect_old_array (mdp_super_t *sb)
{
	if (sb->major_version > 0)
		return 0;
	if (sb->minor_version >= 90)
		return 0;

	return -EINVAL;
}


static void autorun_array (mddev_t *mddev)
{
	mdk_rdev_t *rdev;
        struct md_list_head *tmp;
	int err;

	if (mddev->disks.prev == &mddev->disks) {
		MD_BUG();
		return;
	}

	printk("running: ");

        ITERATE_RDEV(mddev,rdev,tmp) {
		printk("<%s>", partition_name(rdev->dev));
	}
	printk("\nnow!\n");

	err = do_md_run (mddev);
	if (err) {
		printk("do_md_run() returned %d\n", err);
		/*
		 * prevent the writeback of an unrunnable array
		 */
		mddev->sb_dirty = 0;
		do_md_stop (mddev, 0);
	}
}

/*
 * lets try to run arrays based on all disks that have arrived
 * until now. (those are in the ->pending list)
 *
 * the method: pick the first pending disk, collect all disks with
 * the same UUID, remove all from the pending list and put them into
 * the 'same_array' list. Then order this list based on superblock
 * update time (freshest comes first), kick out 'old' disks and
 * compare superblocks. If everything's fine then run it.
 */
static void autorun_devices (void)
{
	struct md_list_head candidates;
	struct md_list_head *tmp;
	mdk_rdev_t *rdev0, *rdev;
	mddev_t *mddev;
	kdev_t md_kdev;


	printk("autorun ...\n");
	while (pending_raid_disks.next != &pending_raid_disks) {
		rdev0 = md_list_entry(pending_raid_disks.next,
					 mdk_rdev_t, pending);

		printk("considering %s ...\n", partition_name(rdev0->dev));
		MD_INIT_LIST_HEAD(&candidates);
		ITERATE_RDEV_PENDING(rdev,tmp) {
			if (uuid_equal(rdev0, rdev)) {
				if (!sb_equal(rdev0->sb, rdev->sb)) {
					printk("%s has same UUID as %s, but superblocks differ ...\n", partition_name(rdev->dev), partition_name(rdev0->dev));
					continue;
				}
				printk("  adding %s ...\n", partition_name(rdev->dev));
				md_list_del(&rdev->pending);
				md_list_add(&rdev->pending, &candidates);
			}
		}
		/*
		 * now we have a set of devices, with all of them having
		 * mostly sane superblocks. It's time to allocate the
		 * mddev.
		 */
		md_kdev = MKDEV(MD_MAJOR, rdev0->sb->md_minor);
		mddev = kdev_to_mddev(md_kdev);
		if (mddev) {
			printk("md%d already running, cannot run %s\n",
				 mdidx(mddev), partition_name(rdev0->dev));
			ITERATE_RDEV_GENERIC(candidates,pending,rdev,tmp)
				export_rdev(rdev);
			continue;
		}
		mddev = alloc_mddev(md_kdev);
		printk("created md%d\n", mdidx(mddev));
		ITERATE_RDEV_GENERIC(candidates,pending,rdev,tmp) {
			bind_rdev_to_array(rdev, mddev);
			md_list_del(&rdev->pending);
			MD_INIT_LIST_HEAD(&rdev->pending);
		}
		autorun_array(mddev);
	}
	printk("... autorun DONE.\n");
}

/*
 * import RAID devices based on one partition
 * if possible, the array gets run as well.
 */

#define BAD_VERSION KERN_ERR \
"md: %s has RAID superblock version 0.%d, autodetect needs v0.90 or higher\n"

#define OUT_OF_MEM KERN_ALERT \
"md: out of memory.\n"

#define NO_DEVICE KERN_ERR \
"md: disabled device %s\n"

#define AUTOADD_FAILED KERN_ERR \
"md: auto-adding devices to md%d FAILED (error %d).\n"

#define AUTOADD_FAILED_USED KERN_ERR \
"md: cannot auto-add device %s to md%d, already used.\n"

#define AUTORUN_FAILED KERN_ERR \
"md: auto-running md%d FAILED (error %d).\n"

#define MDDEV_BUSY KERN_ERR \
"md: cannot auto-add to md%d, already running.\n"

#define AUTOADDING KERN_INFO \
"md: auto-adding devices to md%d, based on %s's superblock.\n"

#define AUTORUNNING KERN_INFO \
"md: auto-running md%d.\n"

static int autostart_array (kdev_t startdev)
{
	int err = -EINVAL, i;
	mdp_super_t *sb = NULL;
	mdk_rdev_t *start_rdev = NULL, *rdev;

	if (md_import_device(startdev, 1)) {
		printk("could not import %s!\n", partition_name(startdev));
		goto abort;
	}

	start_rdev = find_rdev_all(startdev);
	if (!start_rdev) {
		MD_BUG();
		goto abort;
	}
	if (start_rdev->faulty) {
		printk("can not autostart based on faulty %s!\n",
						partition_name(startdev));
		goto abort;
	}
	md_list_add(&start_rdev->pending, &pending_raid_disks);

	sb = start_rdev->sb;

	err = detect_old_array(sb);
	if (err) {
		printk("array version is too old to be autostarted, use raidtools 0.90 mkraid --upgrade\nto upgrade the array without data loss!\n");
		goto abort;
	}

	for (i = 0; i < MD_SB_DISKS; i++) {
		mdp_disk_t *desc;
		kdev_t dev;

		desc = sb->disks + i;
		dev = MKDEV(desc->major, desc->minor);

		if (dev == MKDEV(0,0))
			continue;
		if (dev == startdev)
			continue;
		if (md_import_device(dev, 1)) {
			printk("could not import %s, trying to run array nevertheless.\n", partition_name(dev));
			continue;
		}
		rdev = find_rdev_all(dev);
		if (!rdev) {
			MD_BUG();
			goto abort;
		}
		md_list_add(&rdev->pending, &pending_raid_disks);
	}

	/*
	 * possibly return codes
	 */
	autorun_devices();
	return 0;

abort:
	if (start_rdev)
		export_rdev(start_rdev);
	return err;
}

#undef BAD_VERSION
#undef OUT_OF_MEM
#undef NO_DEVICE
#undef AUTOADD_FAILED_USED
#undef AUTOADD_FAILED
#undef AUTORUN_FAILED
#undef AUTOADDING
#undef AUTORUNNING

struct {
	int set;
	int noautodetect;

} raid_setup_args md__initdata = { 0, 0 };

/*
 * Searches all registered partitions for autorun RAID arrays
 * at boot time.
 */
md__initfunc(void autodetect_raid(void))
{
#ifdef CONFIG_AUTODETECT_RAID
	struct gendisk *disk;
	mdk_rdev_t *rdev;
	int i;

	if (raid_setup_args.noautodetect) {
		printk(KERN_INFO "skipping autodetection of RAID arrays\n");
		return;
	}
	printk(KERN_INFO "autodetecting RAID arrays\n");

	for (disk = gendisk_head ; disk ; disk = disk->next) {
		for (i = 0; i < disk->max_p*disk->max_nr; i++) {
			kdev_t dev = MKDEV(disk->major,i);

			if (disk->part[i].type == LINUX_OLD_RAID_PARTITION) {
				printk(KERN_ALERT
"md: %s's partition type has to be changed from type 0x86 to type 0xfd\n"
"    to maintain interoperability with other OSs! Autodetection support for\n"
"    type 0x86 will be deleted after some migration timeout. Sorry.\n",
					partition_name(dev));
				disk->part[i].type = LINUX_RAID_PARTITION;
			}
			if (disk->part[i].type != LINUX_RAID_PARTITION)
				continue;

			if (md_import_device(dev,1)) {
				printk(KERN_ALERT "could not import %s!\n",
							partition_name(dev));
				continue;
			}
			/*
			 * Sanity checks:
			 */
			rdev = find_rdev_all(dev);
			if (!rdev) {
				MD_BUG();
				continue;
			}
			if (rdev->faulty) {
				MD_BUG();
				continue;
			}
			md_list_add(&rdev->pending, &pending_raid_disks);
		}
	}

	autorun_devices();
#endif
}

static int get_version (void * arg)
{
	mdu_version_t ver;

	ver.major = MD_MAJOR_VERSION;
	ver.minor = MD_MINOR_VERSION;
	ver.patchlevel = MD_PATCHLEVEL_VERSION;

	if (md_copy_to_user(arg, &ver, sizeof(ver)))
		return -EFAULT;

	return 0;
}

#define SET_FROM_SB(x) info.x = mddev->sb->x
static int get_array_info (mddev_t * mddev, void * arg)
{
	mdu_array_info_t info;

	if (!mddev->sb)
		return -EINVAL;

	SET_FROM_SB(major_version);
	SET_FROM_SB(minor_version);
	SET_FROM_SB(patch_version);
	SET_FROM_SB(ctime);
	SET_FROM_SB(level);
	SET_FROM_SB(size);
	SET_FROM_SB(nr_disks);
	SET_FROM_SB(raid_disks);
	SET_FROM_SB(md_minor);
	SET_FROM_SB(not_persistent);

	SET_FROM_SB(utime);
	SET_FROM_SB(state);
	SET_FROM_SB(active_disks);
	SET_FROM_SB(working_disks);
	SET_FROM_SB(failed_disks);
	SET_FROM_SB(spare_disks);

	SET_FROM_SB(layout);
	SET_FROM_SB(chunk_size);

	if (md_copy_to_user(arg, &info, sizeof(info)))
		return -EFAULT;

	return 0;
}
#undef SET_FROM_SB

#define SET_FROM_SB(x) info.x = mddev->sb->disks[nr].x
static int get_disk_info (mddev_t * mddev, void * arg)
{
	mdu_disk_info_t info;
	unsigned int nr;

	if (!mddev->sb)
		return -EINVAL;

	if (md_copy_from_user(&info, arg, sizeof(info)))
		return -EFAULT;

	nr = info.number;
	if (nr >= mddev->sb->nr_disks)
		return -EINVAL;

	SET_FROM_SB(major);
	SET_FROM_SB(minor);
	SET_FROM_SB(raid_disk);
	SET_FROM_SB(state);

	if (md_copy_to_user(arg, &info, sizeof(info)))
		return -EFAULT;

	return 0;
}
#undef SET_FROM_SB

#define SET_SB(x) mddev->sb->disks[nr].x = info.x

static int add_new_disk (mddev_t * mddev, void * arg)
{
	int err, size, persistent;
	mdu_disk_info_t info;
	mdk_rdev_t *rdev;
	unsigned int nr;
	kdev_t dev;

	if (!mddev->sb)
		return -EINVAL;

	if (md_copy_from_user(&info, arg, sizeof(info)))
		return -EFAULT;

	nr = info.number;
	if (nr >= mddev->sb->nr_disks)
		return -EINVAL;

	dev = MKDEV(info.major,info.minor);

	if (find_rdev_all(dev)) {
		printk("device %s already used in a RAID array!\n", 
				partition_name(dev));
		return -EBUSY;
	}

	SET_SB(number);
	SET_SB(major);
	SET_SB(minor);
	SET_SB(raid_disk);
	SET_SB(state);
 
	if ((info.state & (1<<MD_DISK_FAULTY))==0) {
		err = md_import_device (dev, 0);
		if (err) {
			printk("md: error, md_import_device() returned %d\n", err);
			return -EINVAL;
		}
		rdev = find_rdev_all(dev);
		if (!rdev) {
			MD_BUG();
			return -EINVAL;
		}
 
		rdev->old_dev = dev;
		rdev->desc_nr = info.number;
 
		bind_rdev_to_array(rdev, mddev);
 
		persistent = !mddev->sb->not_persistent;
		if (!persistent)
			printk("nonpersistent superblock ...\n");
		if (!mddev->sb->chunk_size)
			printk("no chunksize?\n");
 
		size = calc_dev_size(dev, mddev, persistent);
		rdev->sb_offset = calc_dev_sboffset(dev, mddev, persistent);
 
		if (!mddev->sb->size || (mddev->sb->size > size))
			mddev->sb->size = size;
	}
 
	/*
	 * sync all other superblocks with the main superblock
	 */
	sync_sbs(mddev);

	return 0;
}
#undef SET_SB

static int hot_remove_disk (mddev_t * mddev, kdev_t dev)
{
	int err;
	mdk_rdev_t *rdev;
	mdp_disk_t *disk;

	if (!mddev->pers)
		return -ENODEV;

	printk("trying to remove %s from md%d ... \n",
		partition_name(dev), mdidx(mddev));

	if (!mddev->pers->diskop) {
		printk("md%d: personality does not support diskops!\n",
								 mdidx(mddev));
		return -EINVAL;
	}

	rdev = find_rdev(mddev, dev);
	if (!rdev)
		return -ENXIO;

	if (rdev->desc_nr == -1) {
		MD_BUG();
		return -EINVAL;
	}
	disk = &mddev->sb->disks[rdev->desc_nr];
	if (disk_active(disk))
		goto busy;
	if (disk_removed(disk)) {
		MD_BUG();
		return -EINVAL;
	}
	
	err = mddev->pers->diskop(mddev, &disk, DISKOP_HOT_REMOVE_DISK);
	if (err == -EBUSY)
		goto busy;
	if (err) {
		MD_BUG();
		return -EINVAL;
	}

	remove_descriptor(disk, mddev->sb);
	kick_rdev_from_array(rdev);
	mddev->sb_dirty = 1;
	md_update_sb(mddev);

	return 0;
busy:
	printk("cannot remove active disk %s from md%d ... \n",
		partition_name(dev), mdidx(mddev));
	return -EBUSY;
}

static int hot_add_disk (mddev_t * mddev, kdev_t dev)
{
	int i, err, persistent;
	unsigned int size;
	mdk_rdev_t *rdev;
	mdp_disk_t *disk;

	if (!mddev->pers)
		return -ENODEV;

	printk("trying to hot-add %s to md%d ... \n",
		partition_name(dev), mdidx(mddev));

	if (!mddev->pers->diskop) {
		printk("md%d: personality does not support diskops!\n",
								 mdidx(mddev));
		return -EINVAL;
	}

	persistent = !mddev->sb->not_persistent;
	size = calc_dev_size(dev, mddev, persistent);

	if (size < mddev->sb->size) {
		printk("md%d: disk size %d blocks < array size %d\n",
				mdidx(mddev), size, mddev->sb->size);
		return -ENOSPC;
	}

	rdev = find_rdev(mddev, dev);
	if (rdev)
		return -EBUSY;

	err = md_import_device (dev, 0);
	if (err) {
		printk("md: error, md_import_device() returned %d\n", err);
		return -EINVAL;
	}
	rdev = find_rdev_all(dev);
	if (!rdev) {
		MD_BUG();
		return -EINVAL;
	}
	if (rdev->faulty) {
		printk("md: can not hot-add faulty %s disk to md%d!\n",
				partition_name(dev), mdidx(mddev));
		err = -EINVAL;
		goto abort_export;
	}
	bind_rdev_to_array(rdev, mddev);

	/*
	 * The rest should better be atomic, we can have disk failures
	 * noticed in interrupt contexts ...
	 */
	cli();
	rdev->old_dev = dev;
	rdev->size = size;
	rdev->sb_offset = calc_dev_sboffset(dev, mddev, persistent);

	disk = mddev->sb->disks + mddev->sb->raid_disks;
	for (i = mddev->sb->raid_disks; i < MD_SB_DISKS; i++) {
		disk = mddev->sb->disks + i;

		if (!disk->major && !disk->minor)
			break;
		if (disk_removed(disk))
			break;
	}
	if (i == MD_SB_DISKS) {
		sti();
		printk("md%d: can not hot-add to full array!\n", mdidx(mddev));
		err = -EBUSY;
		goto abort_unbind_export;
	}

	if (disk_removed(disk)) {
		/*
		 * reuse slot
		 */
		if (disk->number != i) {
			sti();
			MD_BUG();
			err = -EINVAL;
			goto abort_unbind_export;
		}
	} else {
		disk->number = i;
	}

	disk->raid_disk = disk->number;
	disk->major = MAJOR(dev);
	disk->minor = MINOR(dev);

	if (mddev->pers->diskop(mddev, &disk, DISKOP_HOT_ADD_DISK)) {
		sti();
		MD_BUG();
		err = -EINVAL;
		goto abort_unbind_export;
	}

	mark_disk_spare(disk);
	mddev->sb->nr_disks++;
	mddev->sb->spare_disks++;
	mddev->sb->working_disks++;

	mddev->sb_dirty = 1;

	sti();
	md_update_sb(mddev);

	/*
	 * Kick recovery, maybe this spare has to be added to the
	 * array immediately.
	 */
	md_recover_arrays();

	return 0;

abort_unbind_export:
	unbind_rdev_from_array(rdev);

abort_export:
	export_rdev(rdev);
	return err;
}

#define SET_SB(x) mddev->sb->x = info.x
static int set_array_info (mddev_t * mddev, void * arg)
{
	mdu_array_info_t info;

	if (mddev->sb) {
		printk("array md%d already has a superblock!\n", 
				mdidx(mddev));
		return -EBUSY;
	}

	if (md_copy_from_user(&info, arg, sizeof(info)))
		return -EFAULT;

	if (alloc_array_sb(mddev))
		return -ENOMEM;

	mddev->sb->major_version = MD_MAJOR_VERSION;
	mddev->sb->minor_version = MD_MINOR_VERSION;
	mddev->sb->patch_version = MD_PATCHLEVEL_VERSION;
	mddev->sb->ctime = CURRENT_TIME;

	SET_SB(level);
	SET_SB(size);
	SET_SB(nr_disks);
	SET_SB(raid_disks);
	SET_SB(md_minor);
	SET_SB(not_persistent);

	SET_SB(state);
	SET_SB(active_disks);
	SET_SB(working_disks);
	SET_SB(failed_disks);
	SET_SB(spare_disks);

	SET_SB(layout);
	SET_SB(chunk_size);

	mddev->sb->md_magic = MD_SB_MAGIC;

	/*
	 * Generate a 128 bit UUID
	 */
	get_random_bytes(&mddev->sb->set_uuid0, 4);
	get_random_bytes(&mddev->sb->set_uuid1, 4);
	get_random_bytes(&mddev->sb->set_uuid2, 4);
	get_random_bytes(&mddev->sb->set_uuid3, 4);

	return 0;
}
#undef SET_SB

static int set_disk_info (mddev_t * mddev, void * arg)
{
	printk("not yet");
	return -EINVAL;
}

static int clear_array (mddev_t * mddev)
{
	printk("not yet");
	return -EINVAL;
}

static int write_raid_info (mddev_t * mddev)
{
	printk("not yet");
	return -EINVAL;
}

static int protect_array (mddev_t * mddev)
{
	printk("not yet");
	return -EINVAL;
}

static int unprotect_array (mddev_t * mddev)
{
	printk("not yet");
	return -EINVAL;
}

static int set_disk_faulty (mddev_t *mddev, kdev_t dev)
{
	int ret;

	fsync_dev(mddev_to_kdev(mddev));
	ret = md_error(mddev_to_kdev(mddev), dev);
	return ret;
}

static int md_ioctl (struct inode *inode, struct file *file,
                     unsigned int cmd, unsigned long arg)
{
	unsigned int minor;
	int err = 0;
	struct hd_geometry *loc = (struct hd_geometry *) arg;
	mddev_t *mddev = NULL;
	kdev_t dev;

	if (!md_capable_admin())
		return -EACCES;

	dev = inode->i_rdev;
	minor = MINOR(dev);
	if (minor >= MAX_MD_DEVS)
		return -EINVAL;

	/*
	 * Commands dealing with the RAID driver but not any
	 * particular array:
	 */
	switch (cmd)
	{
		case RAID_VERSION:
			err = get_version((void *)arg);
			goto done;

		case PRINT_RAID_DEBUG:
			err = 0;
			md_print_devices();
			goto done_unlock;
      
		case BLKGETSIZE:   /* Return device size */
			if (!arg) {
				err = -EINVAL;
				goto abort;
			}
			err = md_put_user(md_hd_struct[minor].nr_sects,
						(long *) arg);
			goto done;

		case BLKFLSBUF:
			fsync_dev(dev);
			invalidate_buffers(dev);
			goto done;

		case BLKRASET:
			if (arg > 0xff) {
				err = -EINVAL;
				goto abort;
			}
			read_ahead[MAJOR(dev)] = arg;
			goto done;
    
		case BLKRAGET:
			if (!arg) {
				err = -EINVAL;
				goto abort;
			}
			err = md_put_user (read_ahead[
				MAJOR(dev)], (long *) arg);
			goto done;
		default:
	}

	/*
	 * Commands creating/starting a new array:
	 */

	mddev = kdev_to_mddev(dev);

	switch (cmd)
	{
		case SET_ARRAY_INFO:
		case START_ARRAY:
			if (mddev) {
				printk("array md%d already exists!\n",
								mdidx(mddev));
				err = -EEXIST;
				goto abort;
			}
		default:
	}

	switch (cmd)
	{
		case SET_ARRAY_INFO:
			mddev = alloc_mddev(dev);
			if (!mddev) {
				err = -ENOMEM;
				goto abort;
			}
			/*
			 * alloc_mddev() should possibly self-lock.
			 */
			err = lock_mddev(mddev);
			if (err) {
				printk("ioctl, reason %d, cmd %d\n", err, cmd);
				goto abort;
			}
			err = set_array_info(mddev, (void *)arg);
			if (err) {
				printk("couldnt set array info. %d\n", err);
				goto abort;
			}
			goto done_unlock;

		case START_ARRAY:
			/*
			 * possibly make it lock the array ...
			 */
			err = autostart_array((kdev_t)arg);
			if (err) {
				printk("autostart %s failed!\n",
					partition_name((kdev_t)arg));
				goto abort;
			}
			goto done;
      
		default:
	}
      
	/*
	 * Commands querying/configuring an existing array:
	 */

	if (!mddev) {
		err = -ENODEV;
		goto abort;
	}
	err = lock_mddev(mddev);
	if (err) {
		printk("ioctl lock interrupted, reason %d, cmd %d\n",err, cmd);
		goto abort;
	}

	/*
	 * Commands even a read-only array can execute:
	 */
	switch (cmd)
	{
		case GET_ARRAY_INFO:
			err = get_array_info(mddev, (void *)arg);
			goto done_unlock;

		case GET_DISK_INFO:
			err = get_disk_info(mddev, (void *)arg);
			goto done_unlock;
      
		case RESTART_ARRAY_RW:
			err = restart_array(mddev);
			goto done_unlock;

		case STOP_ARRAY:
			err = do_md_stop (mddev, 0);
			goto done_unlock;
      
		case STOP_ARRAY_RO:
			err = do_md_stop (mddev, 1);
			goto done_unlock;
      
	/*
	 * We have a problem here : there is no easy way to give a CHS
	 * virtual geometry. We currently pretend that we have a 2 heads
	 * 4 sectors (with a BIG number of cylinders...). This drives
	 * dosfs just mad... ;-)
	 */
		case HDIO_GETGEO:
			if (!loc) {
				err = -EINVAL;
				goto abort_unlock;
			}
			err = md_put_user (2, (char *) &loc->heads);
			if (err)
				goto abort_unlock;
			err = md_put_user (4, (char *) &loc->sectors);
			if (err)
				goto abort_unlock;
			err = md_put_user (md_hd_struct[mdidx(mddev)].nr_sects/8,
						(short *) &loc->cylinders);
			if (err)
				goto abort_unlock;
			err = md_put_user (md_hd_struct[minor].start_sect,
						(long *) &loc->start);
			goto done_unlock;
	}

	/*
	 * The remaining ioctls are changing the state of the
	 * superblock, so we do not allow read-only arrays
	 * here:
	 */
	if (mddev->ro) {
		err = -EROFS;
		goto abort_unlock;
	}

	switch (cmd)
	{
		case CLEAR_ARRAY:
			err = clear_array(mddev);
			goto done_unlock;
      
		case ADD_NEW_DISK:
			err = add_new_disk(mddev, (void *)arg);
			goto done_unlock;
      
		case HOT_REMOVE_DISK:
			err = hot_remove_disk(mddev, (kdev_t)arg);
			goto done_unlock;
      
		case HOT_ADD_DISK:
			err = hot_add_disk(mddev, (kdev_t)arg);
			goto done_unlock;
      
		case SET_DISK_INFO:
			err = set_disk_info(mddev, (void *)arg);
			goto done_unlock;
      
		case WRITE_RAID_INFO:
			err = write_raid_info(mddev);
			goto done_unlock;
      
		case UNPROTECT_ARRAY:
			err = unprotect_array(mddev);
			goto done_unlock;
      
		case PROTECT_ARRAY:
			err = protect_array(mddev);
			goto done_unlock;

		case SET_DISK_FAULTY:
			err = set_disk_faulty(mddev, (kdev_t)arg);
			goto done_unlock;
      
		case RUN_ARRAY:
		{
			mdu_param_t param;

			err = md_copy_from_user(&param, (mdu_param_t *)arg,
							 sizeof(param));
			if (err)
				goto abort_unlock;

			err = do_md_run (mddev);
			/*
			 * we have to clean up the mess if
			 * the array cannot be run for some
			 * reason ...
			 */
			if (err) {
				mddev->sb_dirty = 0;
				do_md_stop (mddev, 0);
			}
			goto done_unlock;
		}
      
		default:
			printk(KERN_WARNING "%s(pid %d) used obsolete MD ioctl, upgrade your software to use new ictls.\n", current->comm, current->pid);
			err = -EINVAL;
			goto abort_unlock;
	}

done_unlock:
abort_unlock:
	if (mddev)
		unlock_mddev(mddev);
	else
		printk("huh11?\n");

	return err;
done:
	if (err)
		printk("huh12?\n");
abort:
	return err;
}


#if LINUX_VERSION_CODE < LinuxVersionCode(2,1,0)

static int md_open (struct inode *inode, struct file *file)
{
	/*
	 * Always succeed
	 */
	return (0);
}

static void md_release (struct inode *inode, struct file *file)
{
	sync_dev(inode->i_rdev);
}


static int md_read (struct inode *inode, struct file *file,
						char *buf, int count)
{
	mddev_t *mddev = kdev_to_mddev(MD_FILE_TO_INODE(file)->i_rdev);

	if (!mddev || !mddev->pers)
		return -ENXIO;

	return block_read (inode, file, buf, count);
}

static int md_write (struct inode *inode, struct file *file,
						const char *buf, int count)
{
	mddev_t *mddev = kdev_to_mddev(MD_FILE_TO_INODE(file)->i_rdev);

	if (!mddev || !mddev->pers)
		return -ENXIO;

	return block_write (inode, file, buf, count);
}

static struct file_operations md_fops=
{
	NULL,
	md_read,
	md_write,
	NULL,
	NULL,
	md_ioctl,
	NULL,
	md_open,
	md_release,
	block_fsync
};

#else

static int md_open (struct inode *inode, struct file *file)
{
	/*
	 * Always succeed
	 */
	return (0);
}

static int md_release (struct inode *inode, struct file *file)
{
	sync_dev(inode->i_rdev);
	return 0;
}

static ssize_t md_read (struct file *file, char *buf, size_t count,
			loff_t *ppos)
{
	mddev_t *mddev = kdev_to_mddev(MD_FILE_TO_INODE(file)->i_rdev);

	if (!mddev || !mddev->pers)
		return -ENXIO;

	return block_read(file, buf, count, ppos);
}

static ssize_t md_write (struct file *file, const char *buf,
			 size_t count, loff_t *ppos)
{
	mddev_t *mddev = kdev_to_mddev(MD_FILE_TO_INODE(file)->i_rdev);

	if (!mddev || !mddev->pers)
		return -ENXIO;

	return block_write(file, buf, count, ppos);
}

static struct file_operations md_fops=
{
	NULL,
	md_read,
	md_write,
	NULL,
	NULL,
	md_ioctl,
	NULL,
	md_open,
	NULL,
	md_release,
	block_fsync
};

#endif

int md_map (kdev_t dev, kdev_t *rdev,
			 unsigned long *rsector, unsigned long size)
{
	int err;
	mddev_t *mddev = kdev_to_mddev(dev);

	if (!mddev || !mddev->pers) {
		err = -ENXIO;
		goto out;
	}

	err = mddev->pers->map(mddev, dev, rdev, rsector, size);
out:
	return err;
}
  
int md_make_request (struct buffer_head * bh, int rw)
{
	int err;
	mddev_t *mddev = kdev_to_mddev(bh->b_dev);

	if (!mddev || !mddev->pers) {
		err = -ENXIO;
		goto out;
	}

	if (mddev->pers->make_request) {
		if (buffer_locked(bh)) {
			err = 0;
			goto out;
		}
		set_bit(BH_Lock, &bh->b_state);
		if (rw == WRITE || rw == WRITEA) {
			if (!buffer_dirty(bh)) {
				bh->b_end_io(bh, buffer_uptodate(bh));
				err = 0;
				goto out;
			}
		}
		if (rw == READ || rw == READA) {
			if (buffer_uptodate(bh)) {
				bh->b_end_io(bh, buffer_uptodate(bh));
				err = 0;
				goto out;
			}
		}
		err = mddev->pers->make_request(mddev, rw, bh);
	} else {
		make_request (MAJOR(bh->b_rdev), rw, bh);
		err = 0;
	}
out:
	return err;
}

static void do_md_request (void)
{
	printk(KERN_ALERT "Got md request, not good...");
	return;
}

int md_thread(void * arg)
{
	mdk_thread_t *thread = arg;

	md_lock_kernel();
	exit_mm(current);
	exit_files(current);
	exit_fs(current);

	/*
	 * Detach thread
	 */
	sys_setsid();
	sprintf(current->comm, thread->name);
	md_init_signals();
	md_flush_signals();
	thread->tsk = current;

	/*
	 * md_thread is a 'system-thread', it's priority should be very
	 * high. We avoid resource deadlocks individually in each
	 * raid personality. (RAID5 does preallocation) We also use RR and
	 * the very same RT priority as kswapd, thus we will never get
	 * into a priority inversion deadlock.
	 *
	 * we definitely have to have equal or higher priority than
	 * bdflush, otherwise bdflush will deadlock if there are too
	 * many dirty RAID5 blocks.
	 */
	current->policy = SCHED_OTHER;
	current->priority = 40;

	up(thread->sem);

	for (;;) {
		cli();
		if (!test_bit(THREAD_WAKEUP, &thread->flags)) {
			if (!thread->run)
				break;
			interruptible_sleep_on(&thread->wqueue);
		}
		sti();
		clear_bit(THREAD_WAKEUP, &thread->flags);
		if (thread->run) {
			thread->run(thread->data);
			run_task_queue(&tq_disk);
		}
		if (md_signal_pending(current)) {
			printk("%8s(%d) flushing signals.\n", current->comm,
				current->pid);
			md_flush_signals();
		}
	}
	sti();
	up(thread->sem);
	return 0;
}

void md_wakeup_thread(mdk_thread_t *thread)
{
	set_bit(THREAD_WAKEUP, &thread->flags);
	wake_up(&thread->wqueue);
}

mdk_thread_t *md_register_thread (void (*run) (void *),
						void *data, const char *name)
{
	mdk_thread_t *thread;
	int ret;
	struct semaphore sem = MUTEX_LOCKED;
	
	thread = (mdk_thread_t *) kmalloc
				(sizeof(mdk_thread_t), GFP_KERNEL);
	if (!thread)
		return NULL;
	
	memset(thread, 0, sizeof(mdk_thread_t));
	init_waitqueue(&thread->wqueue);
	
	thread->sem = &sem;
	thread->run = run;
	thread->data = data;
	thread->name = name;
	ret = kernel_thread(md_thread, thread, 0);
	if (ret < 0) {
		kfree(thread);
		return NULL;
	}
	down(&sem);
	return thread;
}

void md_interrupt_thread (mdk_thread_t *thread)
{
	if (!thread->tsk) {
		MD_BUG();
		return;
	}
	printk("interrupting MD-thread pid %d\n", thread->tsk->pid);
	send_sig(SIGKILL, thread->tsk, 1);
}

void md_unregister_thread (mdk_thread_t *thread)
{
	struct semaphore sem = MUTEX_LOCKED;
	
	thread->sem = &sem;
	thread->run = NULL;
	thread->name = NULL;
	if (!thread->tsk) {
		MD_BUG();
		return;
	}
	md_interrupt_thread(thread);
	down(&sem);
}

void md_recover_arrays (void)
{
	if (!md_recovery_thread) {
		MD_BUG();
		return;
	}
	md_wakeup_thread(md_recovery_thread);
}


int md_error (kdev_t dev, kdev_t rdev)
{
	mddev_t *mddev = kdev_to_mddev(dev);
	mdk_rdev_t * rrdev;
	int rc;

	if (!mddev) {
		MD_BUG();
		return 0;
	}
	rrdev = find_rdev(mddev, rdev);
	mark_rdev_faulty(rrdev);
	/*
	 * if recovery was running, stop it now.
	 */
	if (mddev->pers->stop_resync)
		mddev->pers->stop_resync(mddev);
	if (mddev->recovery_running)
		md_interrupt_thread(md_recovery_thread);
	if (mddev->pers->error_handler) {
		rc = mddev->pers->error_handler(mddev, rdev);
		md_recover_arrays();
		return rc;
	}
#if 0
	/*
	 * Drop all buffers in the failed array.
	 * _not_. This is called from IRQ handlers ...
	 */
	invalidate_buffers(rdev);
#endif
	return 0;
}

static int status_unused (char * page)
{
	int sz = 0, i = 0;
	mdk_rdev_t *rdev;
	struct md_list_head *tmp;

	sz += sprintf(page + sz, "unused devices: ");

	ITERATE_RDEV_ALL(rdev,tmp) {
		if (!rdev->same_set.next && !rdev->same_set.prev) {
			/*
			 * The device is not yet used by any array.
			 */
			i++;
			sz += sprintf(page + sz, "%s ",
				partition_name(rdev->dev));
		}
	}
	if (!i)
		sz += sprintf(page + sz, "<none>");

	sz += sprintf(page + sz, "\n");
	return sz;
}


static int status_resync (char * page, mddev_t * mddev)
{
	int sz = 0;
	unsigned int blocksize, max_blocks, resync, res, dt, tt, et;

	resync = mddev->curr_resync;
	blocksize = blksize_size[MD_MAJOR][mdidx(mddev)];
	max_blocks = blk_size[MD_MAJOR][mdidx(mddev)] / (blocksize >> 10);

	/*
	 * Should not happen.
	 */		
	if (!max_blocks) {
		MD_BUG();
		return 0;
	}
	res = resync*100/max_blocks;
	if (!mddev->recovery_running)
		/*
		 * true resync
		 */
		sz += sprintf(page + sz, " resync=%u%%", res);
	else
		/*
		 * recovery ...
		 */
		sz += sprintf(page + sz, " recovery=%u%%", res);

	/*
	 * We do not want to overflow, so the order of operands and
	 * the * 100 / 100 trick are important. We do a +1 to be
	 * safe against division by zero. We only estimate anyway.
	 *
	 * dt: time until now
	 * tt: total time
	 * et: estimated finish time
	 */
	dt = ((jiffies - mddev->resync_start) / HZ);
	tt = (dt * (max_blocks / (resync/100+1)))/100;
	if (tt > dt)
		et = tt - dt;
	else
		/*
		 * ignore rounding effects near finish time
		 */
		et = 0;
	
	sz += sprintf(page + sz, " finish=%u.%umin", et / 60, (et % 60)/6);

	return sz;
}

int get_md_status (char *page)
{
	int sz = 0, j, size;
	struct md_list_head *tmp, *tmp2;
	mdk_rdev_t *rdev;
	mddev_t *mddev;

	sz += sprintf(page + sz, "Personalities : ");
	for (j = 0; j < MAX_PERSONALITY; j++)
	if (pers[j])
		sz += sprintf(page+sz, "[%s] ", pers[j]->name);

	sz += sprintf(page+sz, "\n");


	sz += sprintf(page+sz, "read_ahead ");
	if (read_ahead[MD_MAJOR] == INT_MAX)
		sz += sprintf(page+sz, "not set\n");
	else
		sz += sprintf(page+sz, "%d sectors\n", read_ahead[MD_MAJOR]);
  
	ITERATE_MDDEV(mddev,tmp) {
		sz += sprintf(page + sz, "md%d : %sactive", mdidx(mddev),
						mddev->pers ? "" : "in");
		if (mddev->pers) {
			if (mddev->ro)	
				sz += sprintf(page + sz, " (read-only)");
			sz += sprintf(page + sz, " %s", mddev->pers->name);
		}

		size = 0;
		ITERATE_RDEV(mddev,rdev,tmp2) {
			sz += sprintf(page + sz, " %s[%d]",
				partition_name(rdev->dev), rdev->desc_nr);
			if (rdev->faulty) {
				sz += sprintf(page + sz, "(F)");
				continue;
			}
			size += rdev->size;
		}

		if (mddev->nb_dev) {
			if (mddev->pers)
				sz += sprintf(page + sz, " %d blocks",
						 md_size[mdidx(mddev)]);
			else
				sz += sprintf(page + sz, " %d blocks", size);
		}

		if (!mddev->pers) {
			sz += sprintf(page+sz, "\n");
			continue;
		}

		sz += mddev->pers->status (page+sz, mddev);

		if (mddev->curr_resync)
			sz += status_resync (page+sz, mddev);
		else {
			if (md_atomic_read(&mddev->resync_sem.count) != 1)
				sz += sprintf(page + sz, " resync=DELAYED");
		}
		sz += sprintf(page + sz, "\n");
	}
	sz += status_unused (page + sz);

	return (sz);
}

int register_md_personality (int pnum, mdk_personality_t *p)
{
	if (pnum >= MAX_PERSONALITY)
		return -EINVAL;

	if (pers[pnum])
		return -EBUSY;
  
	pers[pnum] = p;
	printk(KERN_INFO "%s personality registered\n", p->name);
	return 0;
}

int unregister_md_personality (int pnum)
{
	if (pnum >= MAX_PERSONALITY)
		return -EINVAL;

	printk(KERN_INFO "%s personality unregistered\n", pers[pnum]->name);
	pers[pnum] = NULL;
	return 0;
} 

static mdp_disk_t *get_spare(mddev_t *mddev)
{
	mdp_super_t *sb = mddev->sb;
	mdp_disk_t *disk;
	mdk_rdev_t *rdev;
	struct md_list_head *tmp;

	ITERATE_RDEV(mddev,rdev,tmp) {
		if (rdev->faulty)
			continue;
		if (!rdev->sb) {
			MD_BUG();
			continue;
		}
		disk = &sb->disks[rdev->desc_nr];
		if (disk_faulty(disk)) {
			MD_BUG();
			continue;
		}
		if (disk_active(disk))
			continue;
		return disk;
	}
	return NULL;
}

static int is_mddev_idle (mddev_t *mddev)
{
	mdk_rdev_t * rdev;
	struct md_list_head *tmp;
	int idle;
	unsigned long curr_events;

	idle = 1;
	ITERATE_RDEV(mddev,rdev,tmp) {
		curr_events = io_events[MAJOR(rdev->dev)];

		if (curr_events != rdev->last_events) {
//			printk("!I(%d)", curr_events-rdev->last_events);
			rdev->last_events = curr_events;
			idle = 0;
		}
	}
	return idle;
}

/*
 * parallel resyncing thread. 
 */

/*
 * Determine correct block size for this device.
 */
unsigned int device_bsize (kdev_t dev)
{
	unsigned int i, correct_size;

	correct_size = BLOCK_SIZE;
	if (blksize_size[MAJOR(dev)]) {
		i = blksize_size[MAJOR(dev)][MINOR(dev)];
		if (i)
			correct_size = i;
	}

	return correct_size;
}

static struct wait_queue *resync_wait = (struct wait_queue *)NULL;

#define RA_ORDER (1)
#define RA_PAGE_SIZE (PAGE_SIZE*(1<<RA_ORDER))
#define MAX_NR_BLOCKS (RA_PAGE_SIZE/sizeof(struct buffer_head *))

int md_do_sync(mddev_t *mddev, mdp_disk_t *spare)
{
	mddev_t *mddev2;
        struct buffer_head **bh;
	unsigned int max_blocks, blocksize, curr_bsize,
		i, ii, j, k, chunk, window, nr_blocks, err, serialize;
	kdev_t read_disk = mddev_to_kdev(mddev);
	int major = MAJOR(read_disk), minor = MINOR(read_disk);
	unsigned long starttime;
	int max_read_errors = 2*MAX_NR_BLOCKS,
		 max_write_errors = 2*MAX_NR_BLOCKS;
	struct md_list_head *tmp;

retry_alloc:
	bh = (struct buffer_head **) md__get_free_pages(GFP_KERNEL, RA_ORDER);
	if (!bh) {
		printk(KERN_ERR
		"could not alloc bh array for reconstruction ... retrying!\n");
		goto retry_alloc;
	}

	err = down_interruptible(&mddev->resync_sem);
	if (err)
		goto out_nolock;

recheck:
	serialize = 0;
	ITERATE_MDDEV(mddev2,tmp) {
		if (mddev2 == mddev)
			continue;
		if (mddev2->curr_resync && match_mddev_units(mddev,mddev2)) {
			printk(KERN_INFO "md: serializing resync, md%d has overlapping physical units with md%d!\n", mdidx(mddev), mdidx(mddev2));
			serialize = 1;
			break;
		}
	}
	if (serialize) {
		interruptible_sleep_on(&resync_wait);
		if (md_signal_pending(current)) {
			md_flush_signals();
			err = -EINTR;
			goto out;
		}
		goto recheck;
	}

	mddev->curr_resync = 1;

	blocksize = device_bsize(read_disk);
	max_blocks = blk_size[major][minor] / (blocksize >> 10);

	printk(KERN_INFO "md: syncing RAID array md%d\n", mdidx(mddev));
	printk(KERN_INFO "md: minimum _guaranteed_ reconstruction speed: %d KB/sec.\n",
						sysctl_speed_limit);
	printk(KERN_INFO "md: using maximum available idle IO bandwith for reconstruction.\n");

	/*
	 * Resync has low priority.
	 */
	current->priority = 1;

	is_mddev_idle(mddev); /* this also initializes IO event counters */
	starttime = jiffies;
	mddev->resync_start = starttime;

	/*
	 * Tune reconstruction:
	 */
	window = md_maxreadahead[mdidx(mddev)]/1024;
	nr_blocks = window / (blocksize >> 10);
	if (!nr_blocks || (nr_blocks > MAX_NR_BLOCKS))
		nr_blocks = MAX_NR_BLOCKS;
	printk(KERN_INFO "md: using %dk window.\n",window);

	for (j = 0; j < max_blocks; j += nr_blocks) {

		if (j)
			mddev->curr_resync = j;
		/*
		 * B careful. When some1 mounts a non-'blocksize' filesystem
		 * then we get the blocksize changed right under us. Go deal
		 * with it transparently, recalculate 'blocksize', 'j' and
		 * 'max_blocks':
		 */
		curr_bsize = device_bsize(read_disk);
		if (curr_bsize != blocksize) {
			printk(KERN_INFO "md%d: blocksize changed\n",
								mdidx(mddev));
retry_read:
			if (curr_bsize > blocksize)
				/*
				 * this is safe, rounds downwards.
				 */
				j /= curr_bsize/blocksize;
			else
				j *= blocksize/curr_bsize;

			blocksize = curr_bsize;
			nr_blocks = window / (blocksize >> 10);
			if (!nr_blocks || (nr_blocks > MAX_NR_BLOCKS))
				nr_blocks = MAX_NR_BLOCKS;
			max_blocks = blk_size[major][minor] / (blocksize >> 10);
			printk("nr_blocks changed to %d (blocksize %d, j %d, max_blocks %d)\n",
					nr_blocks, blocksize, j, max_blocks);
			/*
			 * We will retry the current block-group
			 */
		}

		/*
		 * Cleanup routines expect this
		 */
		for (k = 0; k < nr_blocks; k++)
			bh[k] = NULL;

		chunk = nr_blocks;
		if (chunk > max_blocks-j)
			chunk = max_blocks-j;

		/*
		 * request buffer heads ...
		 */
		for (i = 0; i < chunk; i++) {
			bh[i] = getblk (read_disk, j+i, blocksize);
			if (!bh[i])
				goto read_error;
			if (!buffer_dirty(bh[i]))
				mark_buffer_lowprio(bh[i]);
		}

		/*
		 * read buffer heads ...
		 */
		ll_rw_block (READ, chunk, bh);
		run_task_queue(&tq_disk);
		
		/*
		 * verify that all of them are OK ...
		 */
		for (i = 0; i < chunk; i++) {
			ii = chunk-i-1;
			wait_on_buffer(bh[ii]);
			if (!buffer_uptodate(bh[ii]))
				goto read_error;
		}

retry_write:
		for (i = 0; i < chunk; i++)
			mark_buffer_dirty_lowprio(bh[i]);

		ll_rw_block(WRITE, chunk, bh);
		run_task_queue(&tq_disk);

		for (i = 0; i < chunk; i++) {
			ii = chunk-i-1;
			wait_on_buffer(bh[ii]);

			if (spare && disk_faulty(spare)) {
				for (k = 0; k < chunk; k++)
					brelse(bh[k]);
				printk(" <SPARE FAILED!>\n ");
				err = -EIO;
				goto out;
			}

			if (!buffer_uptodate(bh[ii])) {
				curr_bsize = device_bsize(read_disk);
				if (curr_bsize != blocksize) {
					printk(KERN_INFO
						"md%d: blocksize changed during write\n",
						mdidx(mddev));
					for (k = 0; k < chunk; k++)
						if (bh[k]) {
							if (buffer_lowprio(bh[k]))
								mark_buffer_clean(bh[k]);
							brelse(bh[k]);
						}
					goto retry_read;
				}
				printk(" BAD WRITE %8d>\n", j);
				/*
				 * Ouch, write error, retry or bail out.
				 */
				if (max_write_errors) {
					max_write_errors--;
					printk ( KERN_WARNING "md%d: write error while reconstructing, at block %u(%d).\n", mdidx(mddev), j, blocksize);
					goto retry_write;
				}
				printk ( KERN_ALERT
				  "too many write errors, stopping reconstruction.\n");
				for (k = 0; k < chunk; k++)
					if (bh[k]) {
						if (buffer_lowprio(bh[k]))
							mark_buffer_clean(bh[k]);
						brelse(bh[k]);
					}
				err = -EIO;
				goto out;
			}
		}

		/*
		 * This is the normal 'everything went OK' case
		 * do a 'free-behind' logic, we sure dont need
		 * this buffer if it was the only user.
		 */
		for (i = 0; i < chunk; i++)
			if (buffer_dirty(bh[i]))
				brelse(bh[i]);
			else
				bforget(bh[i]);


		if (md_signal_pending(current)) {
			/*
			 * got a signal, exit.
			 */
			mddev->curr_resync = 0;
			printk("md_do_sync() got signal ... exiting\n");
			md_flush_signals();
			err = -EINTR;
			goto out;
		}

		/*
		 * this loop exits only if either when we are slower than
		 * the 'hard' speed limit, or the system was IO-idle for
		 * a jiffy.
		 * the system might be non-idle CPU-wise, but we only care
		 * about not overloading the IO subsystem. (things like an
		 * e2fsck being done on the RAID array should execute fast)
		 */
repeat:
		if (md_need_resched(current))
			schedule();

		if ((blocksize/1024)*j/((jiffies-starttime)/HZ + 1) + 1
						> sysctl_speed_limit) {
			current->priority = 1;

			if (!is_mddev_idle(mddev)) {
				current->state = TASK_INTERRUPTIBLE;
				md_schedule_timeout(HZ/2);
				if (!md_signal_pending(current))
					goto repeat;
			}
		} else
			current->priority = 40;
	}
	fsync_dev(read_disk);
	printk(KERN_INFO "md: md%d: sync done.\n",mdidx(mddev));
	err = 0;
	/*
	 * this also signals 'finished resyncing' to md_stop
	 */
out:
	up(&mddev->resync_sem);
out_nolock:
	free_pages((unsigned long)bh, RA_ORDER);
	mddev->curr_resync = 0;
	wake_up(&resync_wait);
	return err;

read_error:
	/*
	 * set_blocksize() might change the blocksize. This
	 * should not happen often, but it happens when eg.
	 * someone mounts a filesystem that has non-1k
	 * blocksize. set_blocksize() doesnt touch our
	 * buffer, but to avoid aliasing problems we change
	 * our internal blocksize too and retry the read.
	 */
	curr_bsize = device_bsize(read_disk);
	if (curr_bsize != blocksize) {
		printk(KERN_INFO "md%d: blocksize changed during read\n",
			mdidx(mddev));
		for (k = 0; k < chunk; k++)
			if (bh[k]) {
				if (buffer_lowprio(bh[k]))
					mark_buffer_clean(bh[k]);
				brelse(bh[k]);
			}
		goto retry_read;
	}

	/*
	 * It's a real read problem. We retry and bail out
	 * only if it's excessive.
	 */
	if (max_read_errors) {
		max_read_errors--;
		printk ( KERN_WARNING "md%d: read error while reconstructing, at block %u(%d).\n", mdidx(mddev), j, blocksize);
		for (k = 0; k < chunk; k++)
			if (bh[k]) {
				if (buffer_lowprio(bh[k]))
					mark_buffer_clean(bh[k]);
				brelse(bh[k]);
			}
		goto retry_read;
	}
	printk ( KERN_ALERT "too many read errors, stopping reconstruction.\n");
	for (k = 0; k < chunk; k++)
		if (bh[k]) {
			if (buffer_lowprio(bh[k]))
				mark_buffer_clean(bh[k]);
			brelse(bh[k]);
		}
	err = -EIO;
	goto out;
}

#undef MAX_NR_BLOCKS

/*
 * This is a kernel thread which syncs a spare disk with the active array
 *
 * the amount of foolproofing might seem to be a tad excessive, but an
 * early (not so error-safe) version of raid1syncd synced the first 0.5 gigs
 * of my root partition with the first 0.5 gigs of my /home partition ... so
 * i'm a bit nervous ;)
 */
void md_do_recovery (void *data)
{
	int err;
	mddev_t *mddev;
	mdp_super_t *sb;
	mdp_disk_t *spare;
	unsigned long flags;
	struct md_list_head *tmp;

	printk(KERN_INFO "md: recovery thread got woken up ...\n");
restart:
	ITERATE_MDDEV(mddev,tmp) {
		sb = mddev->sb;
		if (!sb)
			continue;
		if (mddev->recovery_running)
			continue;
		if (sb->active_disks == sb->raid_disks)
			continue;
		if (!sb->spare_disks) {
			printk(KERN_ERR "md%d: no spare disk to reconstruct array! -- continuing in degraded mode\n", mdidx(mddev));
			continue;
		}
		/*
		 * now here we get the spare and resync it.
		 */
		if ((spare = get_spare(mddev)) == NULL)
			continue;
		printk(KERN_INFO "md%d: resyncing spare disk %s to replace failed disk\n", mdidx(mddev), partition_name(MKDEV(spare->major,spare->minor)));
		if (!mddev->pers->diskop)
			continue;
		if (mddev->pers->diskop(mddev, &spare, DISKOP_SPARE_WRITE))
			continue;
		down(&mddev->recovery_sem);
		mddev->recovery_running = 1;
		err = md_do_sync(mddev, spare);
		if (err == -EIO) {
			printk(KERN_INFO "md%d: spare disk %s failed, skipping to next spare.\n", mdidx(mddev), partition_name(MKDEV(spare->major,spare->minor)));
			if (!disk_faulty(spare)) {
				mddev->pers->diskop(mddev,&spare,DISKOP_SPARE_INACTIVE);
				mark_disk_faulty(spare);
				mark_disk_nonsync(spare);
				mark_disk_inactive(spare);
				sb->spare_disks--;
				sb->working_disks--;
				sb->failed_disks++;
			}
		} else
			if (disk_faulty(spare))
				mddev->pers->diskop(mddev, &spare,
						DISKOP_SPARE_INACTIVE);
		if (err == -EINTR) {
			/*
			 * Recovery got interrupted ...
			 * signal back that we have finished using the array.
			 */
			mddev->pers->diskop(mddev, &spare,
							 DISKOP_SPARE_INACTIVE);
			up(&mddev->recovery_sem);
			mddev->recovery_running = 0;
			continue;
		} else {
			mddev->recovery_running = 0;
			up(&mddev->recovery_sem);
		}
		save_flags(flags);
		cli();
		if (!disk_faulty(spare)) {
			/*
			 * the SPARE_ACTIVE diskop possibly changes the
			 * pointer too
			 */
			mddev->pers->diskop(mddev, &spare, DISKOP_SPARE_ACTIVE);
			mark_disk_sync(spare);
			mark_disk_active(spare);
			sb->active_disks++;
			sb->spare_disks--;
		}
		restore_flags(flags);
		mddev->sb_dirty = 1;
		md_update_sb(mddev);
		goto restart;
	}
	printk(KERN_INFO "md: recovery thread finished ...\n");
	
}

int md_notify_reboot(struct notifier_block *this,
					unsigned long code, void *x)
{
	struct md_list_head *tmp;
	mddev_t *mddev;

	if ((code == MD_SYS_DOWN) || (code == MD_SYS_HALT)
				  || (code == MD_SYS_POWER_OFF)) {

		printk(KERN_INFO "stopping all md devices.\n");

		ITERATE_MDDEV(mddev,tmp)
			do_md_stop (mddev, 1);
		/*
		 * certain more exotic SCSI devices are known to be
		 * volatile wrt too early system reboots. While the
		 * right place to handle this issue is the given
		 * driver, we do want to have a safe RAID driver ...
		 */
		md_mdelay(1000*1);
	}
	return NOTIFY_DONE;
}

struct notifier_block md_notifier = {
	md_notify_reboot,
	NULL,
	0
};

md__initfunc(void raid_setup(char *str, int *ints))
{
	char tmpline[100];
	int len, pos, nr, i;

	len = strlen(str) + 1;
	nr = 0;
	pos = 0;

	for (i = 0; i < len; i++) {
		char c = str[i];

		if (c == ',' || !c) {
			tmpline[pos] = 0;
			if (!strcmp(tmpline,"noautodetect"))
				raid_setup_args.noautodetect = 1;
			nr++;
			pos = 0;
			continue;
		}
		tmpline[pos] = c;
		pos++;
	}
	raid_setup_args.set = 1;
	return;
}

#ifdef CONFIG_MD_BOOT
struct {
	int set;
	int ints[100];
	char str[100];
} md_setup_args md__initdata = {
	0,{0},{0}
};

/* called from init/main.c */
md__initfunc(void md_setup(char *str,int *ints))
{
	int i;
	for(i=0;i<=ints[0];i++) {
		md_setup_args.ints[i] = ints[i];
		strcpy(md_setup_args.str, str);
/*      printk ("md: ints[%d]=%d.\n", i, ints[i]);*/
	}
	md_setup_args.set=1;
	return;
}

md__initfunc(void do_md_setup(char *str,int *ints))
{
#if 0
	int minor, pers, chunk_size, fault;
	kdev_t dev;
	int i=1;

	printk("i plan to phase this out --mingo\n");

	if(ints[0] < 4) {
		printk (KERN_WARNING "md: Too few Arguments (%d).\n", ints[0]);
		return;
	}
   
	minor=ints[i++];
   
	if ((unsigned int)minor >= MAX_MD_DEVS) {
		printk (KERN_WARNING "md: Minor device number too high.\n");
		return;
	}

	pers = 0;
	
	switch(ints[i++]) {  /* Raidlevel  */
	case -1:
#ifdef CONFIG_MD_LINEAR
		pers = LINEAR;
		printk (KERN_INFO "md: Setting up md%d as linear device.\n",
									minor);
#else 
	        printk (KERN_WARNING "md: Linear mode not configured." 
			"Recompile the kernel with linear mode enabled!\n");
#endif
		break;
	case 0:
		pers = STRIPED;
#ifdef CONFIG_MD_STRIPED
		printk (KERN_INFO "md: Setting up md%d as a striped device.\n",
								minor);
#else 
	        printk (KERN_WARNING "md: Striped mode not configured." 
			"Recompile the kernel with striped mode enabled!\n");
#endif
		break;
/*      not supported yet
	case 1:
		pers = RAID1;
		printk ("md: Setting up md%d as a raid1 device.\n",minor);
		break;
	case 5:
		pers = RAID5;
		printk ("md: Setting up md%d as a raid5 device.\n",minor);
		break;
*/
	default:	   
		printk (KERN_WARNING "md: Unknown or not supported raid level %d.\n", ints[--i]);
		return;
	}

	if (pers) {

		chunk_size = ints[i++]; /* Chunksize  */
		fault = ints[i++]; /* Faultlevel */
   
		pers = pers | chunk_size | (fault << FAULT_SHIFT);   
   
		while( str && (dev = name_to_kdev_t(str))) {
			do_md_add (minor, dev);
			if((str = strchr (str, ',')) != NULL)
				str++;
		}

		do_md_run (minor, pers);
		printk (KERN_INFO "md: Loading md%d.\n",minor);
	}
#endif
}
#endif

void hsm_init (void);
void translucent_init (void);
void linear_init (void);
void raid0_init (void);
void raid1_init (void);
void raid5_init (void);

md__initfunc(int md_init (void))
{
	static char * name = "mdrecoveryd";

	printk (KERN_INFO "md driver %d.%d.%d MAX_MD_DEVS=%d, MAX_REAL=%d\n",
			MD_MAJOR_VERSION, MD_MINOR_VERSION,
			MD_PATCHLEVEL_VERSION, MAX_MD_DEVS, MAX_REAL);

	if (register_blkdev (MD_MAJOR, "md", &md_fops))
	{
		printk (KERN_ALERT "Unable to get major %d for md\n", MD_MAJOR);
		return (-1);
	}

	blk_dev[MD_MAJOR].request_fn = DEVICE_REQUEST;
	blk_dev[MD_MAJOR].current_request = NULL;
	read_ahead[MD_MAJOR] = INT_MAX;
	md_gendisk.next = gendisk_head;

	gendisk_head = &md_gendisk;

	md_recovery_thread = md_register_thread(md_do_recovery, NULL, name);
	if (!md_recovery_thread)
		printk(KERN_ALERT "bug: couldn't allocate md_recovery_thread\n");

	md_register_reboot_notifier(&md_notifier);
	md_register_sysctl();

#ifdef CONFIG_MD_HSM
	hsm_init ();
#endif
#ifdef CONFIG_MD_TRANSLUCENT
	translucent_init ();
#endif
#ifdef CONFIG_MD_LINEAR
	linear_init ();
#endif
#ifdef CONFIG_MD_STRIPED
	raid0_init ();
#endif
#ifdef CONFIG_MD_MIRRORING
	raid1_init ();
#endif
#ifdef CONFIG_MD_RAID5
	raid5_init ();
#endif
#if defined(CONFIG_MD_RAID5) || defined(CONFIG_MD_RAID5_MODULE)
        /*
         * pick a XOR routine, runtime.
         */
	calibrate_xor_block();
#endif

	return (0);
}

#ifdef CONFIG_MD_BOOT
md__initfunc(void md_setup_drive(void))
{
	if(md_setup_args.set)
		do_md_setup(md_setup_args.str, md_setup_args.ints);
}
#endif

MD_EXPORT_SYMBOL(md_size);
MD_EXPORT_SYMBOL(register_md_personality);
MD_EXPORT_SYMBOL(unregister_md_personality);
MD_EXPORT_SYMBOL(partition_name);
MD_EXPORT_SYMBOL(md_error);
MD_EXPORT_SYMBOL(md_recover_arrays);
MD_EXPORT_SYMBOL(md_register_thread);
MD_EXPORT_SYMBOL(md_unregister_thread);
MD_EXPORT_SYMBOL(md_update_sb);
MD_EXPORT_SYMBOL(md_map);
MD_EXPORT_SYMBOL(md_wakeup_thread);
MD_EXPORT_SYMBOL(md_do_sync);
MD_EXPORT_SYMBOL(md_print_devices);
MD_EXPORT_SYMBOL(find_rdev_nr);
MD_EXPORT_SYMBOL(md_check_ordering);
MD_EXPORT_SYMBOL(md_interrupt_thread);
MD_EXPORT_SYMBOL(mddev_map);

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry proc_md = {
	PROC_MD, 6, "mdstat",
	S_IFREG | S_IRUGO, 1, 0, 0,
	0, &proc_array_inode_operations,
};
#endif

static void md_geninit (struct gendisk *gdisk)
{
	int i;
  
	for(i = 0; i < MAX_MD_DEVS; i++) {
		md_blocksizes[i] = 1024;
		md_maxreadahead[i] = MD_READAHEAD;
		md_gendisk.part[i].start_sect = -1; /* avoid partition check */
		md_gendisk.part[i].nr_sects = 0;
	}

	printk("md.c: sizeof(mdp_super_t) = %d\n", (int)sizeof(mdp_super_t));

	blksize_size[MD_MAJOR] = md_blocksizes;
	md_set_global_readahead(md_maxreadahead);

#ifdef CONFIG_PROC_FS
	proc_register(&proc_root, &proc_md);
#endif
}

