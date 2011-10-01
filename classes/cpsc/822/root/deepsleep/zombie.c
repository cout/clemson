#include <stdio.h>

main() {
	int pid;
	switch(pid=fork()) {
		case 0:
			printf("try to kill pid %d\n", getpid());
			exit(0);
		default:
			printf("without killing %d\n", getpid());
			for(;;) sleep(1);
	}
	return 0;
}
