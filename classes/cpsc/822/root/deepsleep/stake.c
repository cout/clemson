#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <asm/unistd.h>

_syscall2(int, ssmunch, int, pid, unsigned long, bp);

int main(int argc, char *argv[]) {
	if(!ssmunch(atoi(argv[1]), (1<<SIGKILL)))
		printf("Unable to kill process (not found?).\n");
	return 0;
}
