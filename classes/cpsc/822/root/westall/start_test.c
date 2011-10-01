#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <asm/unistd.h>

_syscall1(double, get_ide_request_data, int, n);

int main() {
	get_ide_request_data(0);
	return 0;
}
