#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <asm/unistd.h>

_syscall2(int, ssmunch, int, pid, unsigned long, bp);

void handle_user_signal(int i) {
	signal(i, handle_user_signal);
	printf("Got a signal (%d)\n", i);
}

int main() {
	printf("We should receive 8 signals.\n");
	signal(SIGUSR1, handle_user_signal);
	signal(SIGUSR2, handle_user_signal);
 	// signal(SIGPIPE, SIG_IGN);
	raise(SIGUSR1);
	raise(SIGUSR2);
	printf("--------------------\n");
	sleep(1);

	ssmunch(getpid(), (1<<SIGUSR1) | (1<<SIGUSR2));
	printf("--------------------\n");
	sleep(1);

	ssmunch(getpid(), (1<<SIGUSR1));
	printf("--------------------\n");
	sleep(1);

	ssmunch(getpid(), (1<<SIGUSR2));
	printf("--------------------\n");
	sleep(1);

	ssmunch(getpid(), (1<<SIGUSR1) | (1<<SIGUSR2) | (1<<SIGKILL));
	sleep(1);

	printf("You should not see this message.\n");
	return 0;
}
