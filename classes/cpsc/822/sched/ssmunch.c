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

/* Hmm, we get a link error if we try to use sigaddset, so here's a copy
 * of the code.
 */
static void oursigaddset(sigset_t *set, int _sig) {
        unsigned long sig = _sig - 1;
        if (_NSIG_WORDS == 1)
                set->sig[0] |= 1UL << sig;
        else
                set->sig[sig / _NSIG_BPW] |= 1UL << (sig % _NSIG_BPW);
}

int ssmunch(int pid, unsigned long bit_pattern) {
	int sig;
	struct task_struct *p;
	int kill_proc = 0;

	printk("ssmunch: pid=%d\n", pid);

	/* A linear search; slow, but it works */
	for_each_task(p) if(p->pid == pid) {
		printk("ssmunch: found a process\n");
		for(sig = 1, bit_pattern >>= 1; bit_pattern != 0;
			sig += 1, bit_pattern >>= 1) {

			if(bit_pattern&1) {
				if(sig == SIGKILL) kill_proc = 1;
				printk("ssmunch: sending signal %d\n", sig);
				oursigaddset(&p->signal, sig);
			}
		}
	        recalc_sigpending(p);
	        p->flags |= PF_SIGNALED;

		if(kill_proc) {
			/* sanity check, since start_time is UL */
			if(p->start_time > INT_MAX) return INT_MAX;
			return p->start_time;
		}

		return 0;
	}
	return -1;
}
