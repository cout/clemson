#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <asm/unistd.h>

_syscall1(double, get_ide_request_data, int, n);

int main() {
	double sum_of_services = get_ide_request_data(1);
	double sum_of_waits = get_ide_request_data(2);
	double num_requests = get_ide_request_data(3);

	printf("Sum of services:     %f\n", sum_of_services);
	printf("Sum of waits:        %f\n", sum_of_waits);
	printf("Number of requests:  %u\n", (int)num_requests);
	printf("Mean service time:   %f\n", sum_of_services/num_requests);
	printf("Mean wait time:      %f\n", sum_of_waits/num_requests);

	return 0;
}
