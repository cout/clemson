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

extern int ssmunch_multi;

/* Hmm, we get a link error if we try to use sigaddset, so here's a copy
 * of the code.  It's static so it doesn't interfere with anything else.
 */
static void sigaddset(sigset_t *set, int _sig) {
        unsigned long sig = _sig - 1;
        if (_NSIG_WORDS == 1)
                set->sig[0] |= 1UL << sig;
        else
                set->sig[sig / _NSIG_BPW] |= 1UL << (sig % _NSIG_BPW);
}

/* Hmm, same thing for find_task_by_pid?  Something's odd here... */
static struct task_struct *find_task_by_pid(int pid) {
        struct task_struct *p, **htable = &pidhash[pid_hashfn(pid)];

        for(p = *htable; p && p->pid != pid; p = p->pidhash_next) ;
        return p;
}

/* This function doesn't have a prototype anywhere?  Perhaps I shouldn't be
 * using it?
 */
extern void release(struct task_struct *p);

int ssmunch(int pid, unsigned long bit_pattern) {
	int sig;
	struct task_struct *p;
	int kill_proc = 0;

	printk("ssmunch: pid=%d\n", pid);

	p = find_task_by_pid(pid);
	if(!p) return -1;

	printk("ssmunch: found a process\n");

	/* One problem with this.  If our process is defunct or asleep,
	 * then the next signal will probably be SIGCHLD or SIGALRM, and it
	 * will probably belong to a different process.  So we will see a
	 * multiple signal message from do_signal, when the signals aren't
	 * actually processed.  Such is life.
	 */
	ssmunch_multi = bit_pattern;
	
	for(sig = 1, bit_pattern >>= 1; bit_pattern != 0;
		sig += 1, bit_pattern >>= 1) {

		if(bit_pattern&1) {
			if(sig == SIGKILL) kill_proc = 1;
			printk("ssmunch: sending signal %d\n", sig);
			sigaddset(&p->signal, sig);
		}
	}
        recalc_sigpending(p);
        p->flags |= PF_SIGNALED;

	if(kill_proc) {
		/* This won't do anything if p==current; in that case
		 * we will get killed normally.  Unfortunately, there
		 * is one case in which the process won't get killed,
		 * that is, when a process tries to kill itself with
		 * ssmunch and it's parent process is asleep.  We'll
		 * then end up with a defunct process that can be
		 * killed by ssmunch.
		 *
		 * Incidentally, we probably shouldn't allow init to be
		 * killed.  But, alas, that wasn't in the spec...
		 */
		wake_up_process(p);
		release(p);
		schedule();

		/* sanity check, since start_time is UL */
		if(p->start_time > INT_MAX) return INT_MAX;
		return p->start_time;
	}
	return 0;
}
