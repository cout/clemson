#include <stdio.h>
#include <errno.h>
#include <asm/unistd.h>

_syscall1(int, xtimesyscall, long *, value);

int main() {
  long junk = 0;
  xtimesyscall(&junk);
  printf("xtime.tv_sec = %ld\n", junk);

  return 0;
}
