/* For lab 6 */

#include <asm/ptimer.h>
#include <linux/kernel.h>

extern unsigned long cpu_hz;
#define MHZ cpu_hz
#define BIGNUMBER (65536.0*65536.0)
#define ONE_MILLION 1000000.0

static unsigned long low, high;
void get_ptimer(PTimer *p) {
	asm("RDTSC");
	asm("movl %eax, low");
	asm("movl %edx, high");
	p->low = low; p->high = high;
}

double calc_ptimer(PTimer start, PTimer end) {
	double total_secs;
	if(start.low > end.low) {
		total_secs = BIGNUMBER + end.low - start.low;
		total_secs += BIGNUMBER * (end.high - start.high - 1);
	} else {
		total_secs = end.low - start.low;
		total_secs += BIGNUMBER * (end.high - start.high);
	}
	total_secs /= cpu_hz;
	return total_secs;
}
