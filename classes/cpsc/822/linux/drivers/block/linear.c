/*
   linear.c : Multiple Devices driver for Linux
              Copyright (C) 1994-96 Marc ZYNGIER
	      <zyngier@ufr-info-p7.ibp.fr> or
	      <maz@gloups.fdn.fr>

   Linear mode management functions.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
   
   You should have received a copy of the GNU General Public License
   (for example /usr/src/linux/COPYING); if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  
*/

#include <linux/module.h>

#include <linux/raid/md.h>
#include <linux/malloc.h>

#include <linux/raid/linear.h>

#define MAJOR_NR MD_MAJOR
#define MD_DRIVER
#define MD_PERSONALITY

static int linear_run (mddev_t *mddev)
{
	linear_conf_t *conf;
	struct linear_hash *table;
	mdk_rdev_t *rdev;
	int size, i, j, nb_zone;
	unsigned int curr_offset;

	MOD_INC_USE_COUNT;

	conf = kmalloc (sizeof (*conf), GFP_KERNEL);
	if (!conf)
		goto out;
	mddev->private = conf;

	if (md_check_ordering(mddev)) {
		printk("linear: disks are not ordered, aborting!\n");
		goto out;
	}
	/*
	 * Find the smallest device.
	 */

	conf->smallest = NULL;
	curr_offset = 0;
	ITERATE_RDEV_ORDERED(mddev,rdev,j) {
		dev_info_t *disk = conf->disks + j;

		disk->dev = rdev->dev;
		disk->size = rdev->size;
		disk->offset = curr_offset;

		curr_offset += disk->size;

		if (!conf->smallest || (disk->size < conf->smallest->size))
			conf->smallest = disk;
	}

	nb_zone = conf->nr_zones =
		md_size[mdidx(mddev)] / conf->smallest->size +
		((md_size[mdidx(mddev)] % conf->smallest->size) ? 1 : 0);
  
	conf->hash_table = kmalloc (sizeof (struct linear_hash) * nb_zone,
					GFP_KERNEL);
	if (!conf->hash_table)
		goto out;

	/*
	 * Here we generate the linear hash table
	 */
	table = conf->hash_table;
	i = 0;
	size = 0;
	for (j = 0; j < mddev->nb_dev; j++) {
		dev_info_t *disk = conf->disks + j;

		if (size < 0) {
			table->dev1 = disk;
			table++;
		}
		size += disk->size;

		while (size) {
			table->dev0 = disk;
			size -= conf->smallest->size;
			if (size < 0)
				break;
			table->dev1 = NULL;
			table++;
		}
	}
	table->dev1 = NULL;

	return 0;

out:
	if (conf)
		kfree(conf);
	MOD_DEC_USE_COUNT;
	return 1;
}

static int linear_stop (mddev_t *mddev)
{
	linear_conf_t *conf = mddev_to_conf(mddev);
  
	kfree(conf->hash_table);
	kfree(conf);

	MOD_DEC_USE_COUNT;

	return 0;
}


static int linear_map (mddev_t *mddev, kdev_t dev, kdev_t *rdev,
		       unsigned long *rsector, unsigned long size)
{
	linear_conf_t *conf = mddev_to_conf(mddev);
	struct linear_hash *hash;
	dev_info_t *tmp_dev;
	long block;

	block = *rsector >> 1;
	hash = conf->hash_table + (block / conf->smallest->size);
  
	if (block >= (hash->dev0->size + hash->dev0->offset))
	{
		if (!hash->dev1)
		{
			printk ("linear_map : hash->dev1==NULL for block %ld\n",
						block);
			return -1;
		}
		tmp_dev = hash->dev1;
	} else
		tmp_dev = hash->dev0;
    
	if (block >= (tmp_dev->size + tmp_dev->offset)
				|| block < tmp_dev->offset)
	printk ("Block %ld out of bounds on dev %s size %d offset %d\n",
		block, kdevname(tmp_dev->dev), tmp_dev->size, tmp_dev->offset);
  
	*rdev = tmp_dev->dev;
	*rsector = (block - tmp_dev->offset) << 1;

	return 0;
}

static int linear_status (char *page, mddev_t *mddev)
{
	int sz=0;

#undef MD_DEBUG
#ifdef MD_DEBUG
	int j;
	linear_conf_t *conf = mddev_to_conf(mddev);
  
	sz += sprintf(page+sz, "      ");
	for (j = 0; j < conf->nr_zones; j++)
	{
		sz += sprintf(page+sz, "[%s",
			partition_name(conf->hash_table[j].dev0->dev));

		if (conf->hash_table[j].dev1)
			sz += sprintf(page+sz, "/%s] ",
			  partition_name(conf->hash_table[j].dev1->dev));
		else
			sz += sprintf(page+sz, "] ");
	}
	sz += sprintf(page+sz, "\n");
#endif
	sz += sprintf(page+sz, " %dk rounding", mddev->param.chunk_size/1024);
	return sz;
}


static mdk_personality_t linear_personality=
{
	"linear",
	linear_map,
	NULL,
	NULL,
	linear_run,
	linear_stop,
	linear_status,
	NULL,
	0,
	NULL,
	NULL,
	NULL,
	NULL
};

#ifndef MODULE

md__initfunc(void linear_init (void))
{
	register_md_personality (LINEAR, &linear_personality);
}

#else

int init_module (void)
{
	return (register_md_personality (LINEAR, &linear_personality));
}

void cleanup_module (void)
{
	unregister_md_personality (LINEAR);
}

#endif

