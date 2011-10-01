#include <stdio.h>
#include <errno.h>
#include <asm/unistd.h>

_syscall1(int, turbo, int, pid);

int main(int argc, char *argv[]) {
	if(argc < 2) return 1;
	turbo(atoi(argv[1]));
	return 0;
}
