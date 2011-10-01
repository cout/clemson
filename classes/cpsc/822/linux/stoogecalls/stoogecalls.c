#include <linux/time.h>
#include <linux/timex.h>
#include <asm/uaccess.h>

extern struct timeval xtime;

int xtimesyscall(long *p) {
  copy_to_user(p, &(xtime.tv_sec), sizeof(time_t));
  return 0;  
}
