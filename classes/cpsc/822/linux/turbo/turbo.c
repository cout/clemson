#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/signal.h>
#include <linux/limits.h>

#include <asm/uaccess.h>

/* Hmm, same thing for find_task_by_pid?  Something's odd here... */
static struct task_struct *find_task_by_pid(int pid) {
	        struct task_struct *p, **htable = &pidhash[pid_hashfn(pid)];
	        for(p = *htable; p && p->pid != pid; p = p->pidhash_next) ;
		        return p;
}


void turbo(int pid) {
	struct task_struct *p;

	printk("Turbo: scheduling process %d as turbo\n", pid);
	p = find_task_by_pid(pid);
	if(!p) return;
	if(p->state != TASK_RUNNING) return;
	p->policy = SCHED_TURBO;
	schedule();
}
