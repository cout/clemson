#include <stdio.h>
#include <errno.h>
#include <asm/unistd.h>

_syscall0(void,deepsleep);

main() {
	printf("Say goodnight, Gracie!\n");
	deepsleep();
	printf("Yawn.\n");
}
