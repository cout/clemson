/* spin.c -- module that keeps track of spin and fps */


#ifdef WIN32
#include <time.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include <GL/glut.h>

#include "spin.h"
#include "gettime.h"

float spin;
int update_fps = 1;
long oldtime;

void spin_init() {
	spin = 0.0;
	oldtime = get_time();
}

/* Note: we need the (void) to avoid a compiler warning */
void do_spin(void) {
	long newtime = get_time();
	long dt = newtime - oldtime;
	oldtime = newtime;

	spin = spin + (float)SPIN_RATE * dt;
	while(spin > (float)360.0) spin = spin - (float)360.0;
}
