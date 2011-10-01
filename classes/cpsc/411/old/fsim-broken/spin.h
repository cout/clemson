#ifndef __SPIN_H
#define __SPIN_H

/* Spin rate is degrees per millisecond */
#define SPIN_RATE 0.045

extern float fps, spin;
extern int update_fps;
extern long oldtime, oldfpstime;

long get_time();
void spin_init();
void do_spin(void);

#endif