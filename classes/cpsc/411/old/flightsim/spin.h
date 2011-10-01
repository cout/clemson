#ifndef _SPIN_H
#define _SPIN_H

#define SPIN_RATE 0.045 /* degrees per millisecond */

extern float /*fps,*/ spin; 
/* extern int update_fps; */
/* extern long oldtime, oldfpstime; */

void spin_init();
void do_spin(void);

#endif
