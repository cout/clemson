#include <asm/ptimer.h>

/* For lab 6 */

#define MHZ 133
#define BIGNUMBER (65536.0*65536.0)
#define ONE_MILLION 1000000.0

static unsigned long low, high;
void get_ptimer(PTimer *p) {
	asm("RDTSC");
	asm("movl %eax,low");
	asm("movl %edx,high");
	p->low = low;
	p->high = high;
}

double calc_timer(PTimer start, PTimer end) {
	double total_secs;
	if(start.low > end.low) {
		total_secs = BIGNUMBER + end.low - start.low;
		total_secs += BIGNUMBER * (end.high - start.high - 1);
	} else {
		total_secs = end.low - start.low;
		total_secs += BIGNUMBER * (end.high - start.high);
	}
	total_secs /= MHZ*ONE_MILLION;
	return total_secs;
}

int main() {
	PTimer start;
	PTimer end;

	get_ptimer(&start);
	printf("Low: %d, high: %d\n", start.low, start.high);
	sleep(5);
	get_ptimer(&end);
	printf("Low: %d, high: %d\n", end.low, end.high);
	printf("Total time: %f\n", calc_timer(start, end));
	
	return 0;
}
