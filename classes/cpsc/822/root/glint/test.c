#include <stdio.h>
#include <fcntl.h>

int main() {
	int fd;

	printf("glint test module\n");
	if((fd = open("/dev/glint3", O_RDWR)) == 0) {
		printf("Unable to open /dev/glint3\n");
		return -1;
	}
	printf("glint device opened\n");
	close(fd);
	printf("glint device closed\n");
}
