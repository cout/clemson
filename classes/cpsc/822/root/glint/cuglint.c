/* The GLINT prototype module */

#include <linux/modversions.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/module.h>

#ifndef GLINT_MAJOR
#define GLINT_MAJOR 50
#endif

int glint_open(struct inode *node, struct file *fp) {
	printk("glint: open\n");
	return 0;
}

int glint_release(struct inode *node, struct file *fp) {
	printk("glint: release\n");
	return 0;
}

int glint_ioctl(struct inode *node, struct file *fp,
	unsigned int cmd, unsigned long arg) {
	return 0;
}

struct file_operations glint_fops = {
	NULL,			/* lseek */
	NULL,			/* read */
	NULL,			/* write */
	NULL,			/* readdir */
	NULL,			/* poll */
	glint_ioctl,		/* ioctl */
	NULL,			/* nmap */
	glint_open,		/* open */
	NULL,			/* flush */
	glint_release,		/* release */
	NULL,			/* fsync */
	NULL,			/* fasync */
	NULL,			/* check_media_change */
	NULL,			/* revalidate */
	NULL			/* lock */
};

int init_module() {
	if(register_chrdev(GLINT_MAJOR, "glint", &glint_fops)) {
		printk("glint: unable to get major %d\n", GLINT_MAJOR);
		return -EIO;
	}

	printk("glint: successfully registered\n");
	return 0;
}

void cleanup_module() {
	unregister_chrdev(GLINT_MAJOR, "glint");
	printk("glint: unregistered\n");
}

