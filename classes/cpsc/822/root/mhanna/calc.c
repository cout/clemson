#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/malloc.h>
#include <linux/delay.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/mman.h>
#include <asm/uaccess.h>

#include "calc.h"

int calc_open(struct inode *inode, struct file *fp); 
int calc_release(struct inode *inode, struct file *fp); 
int calc_ioctl(struct inode *inode, struct file *fp, unsigned int cmd, unsigned long arg); 
int init_module(void); 
int cleanup_module(void);
 
struct file_operations calc_ops = {
  NULL, /* lseek */
  NULL, /* read */
  NULL, /* write */
  NULL, /* readdir */
  NULL, /* poll */
  calc_ioctl, /* ioctl */
  NULL, /* mmap */
  calc_open, /* open */
  NULL, /* flush */
  calc_release, /* release */
  NULL, /* fsync */
  NULL, /* fasync */
  NULL, /* check_media_change */
  NULL, /* revalidate */
  NULL /* lock */
};

double calc_vars[256] = { 0.0 };
double stack[256];
int top = -1;

int calc_open(struct inode *inode, struct file *fp) {
  printk("Opening the device...\n");

  /* Calculator in use? */
  if (top >= 0)
    return 1;

  return 0;
}

int calc_release(struct inode *inode, struct file *fp) {
  printk("Releasing the device...\n");
  return 0;
}

int calc_ioctl(struct inode *inode, struct file *fp, unsigned int cmd, unsigned long arg) {
  struct calc_struct *info;
  double v1, v2, res;

  printk("Performing an ioctl function...\n");

  switch (cmd) {
    case CALCULATOR_CLEAR :
    case CALCULATOR_DONE :
      for (top = 255; top >= 0; top--)
        calc_vars[top] = 0;
      break;
    case CALCULATOR_TOP :
      *((double *)arg) = stack[top];
      break;
    case CALCULATOR_VARIABLE :
      info = (struct calc_struct *)arg;
      if (info->var >= 0) {
        calc_vars[info->var] = info->val;
      }
      break;
    case CALCULATOR_PERFORM :
      info = (struct calc_struct *)arg;
      if ((info->var < 0) && (info->op < 0)) {
        /* It is a number */
        stack[++top] = info->val;
      } else if (info->var >= 0) {
        /* It is a variable reference */
        stack[++top] = calc_vars[info->var];
      } else if (info->op >= 0) {
        /* Perform a math op */
        v1 = stack[top--];
        v2 = stack[top--];

        switch (info->op) {
          case '+' :
            	res = v1 + v2;
		break;
	  case '-' :
		res = v1 - v2;
		break;
	  case '*' :
		res = v1 * v2;
		break;
	  case '/' :
		res = v1 / v2;
		break;
          default :
		stack[++top] = v2;
		res = v1;
		break;
        }
	stack[++top] = res;
      }
      break;
    default :
       /* Nothing to do */
  }

  return 0;
}

int init_module(void) {
  if (!register_chrdev(240, "calc", &calc_ops))
    printk("Unable to get major for /dev/calc\n");

  printk("Registering /dev/calc\n");

  return 0;
}

int cleanup_module(void) {
  unregister_chrdev(240, "calc");
  printk("Unregistering /dev/calc\n");

  return 0;
}
