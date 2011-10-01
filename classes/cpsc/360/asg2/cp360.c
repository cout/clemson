#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE !FALSE
#endif

int main(int argc, char *argv[]) {
	int ifd, ofd;
	struct stat sb;
	char *buf;
	int do_mmap;

	if(argc < 3) {
		fprintf(stderr, "Usage: %s source destination [buffer size]\n",
			argv[0]);
		exit(1);
	}

	if(argc == 3) do_mmap = TRUE;
	else do_mmap = FALSE;

	ifd = open(argv[1], O_RDONLY);
	if(ifd == -1) {
		perror("open(reading)");
		exit(1);
	}
	fstat(ifd, &sb);
	ofd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, sb.st_mode);
	if(ofd == -1) {
		perror("open(writing)");
		exit(1);
	}

	if(do_mmap) {
		char *dst;
		fprintf(stderr, "Using mmap to copy... "); fflush(stderr);
		buf = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, ifd, 0);
		if(buf == MAP_FAILED) {
			perror("mmap");
			exit(1);
		}
		write(ofd, buf, sb.st_size);
		munmap(buf, sb.st_size);
		fprintf(stderr, "done.\n");
	} else {
		int len, bufsize = atoi(argv[3]);
		fprintf(stderr, "Using a buffer of %d bytes to copy...",
			bufsize); fflush(stderr);
		buf = (char *)malloc(bufsize);
		while((len = read(ifd, buf, bufsize)) != 0)
			write(ofd, buf, len);
		free(buf);
		fprintf(stderr, "done.\n");
	}

	close(ifd);
	close(ofd);

	return 0;
}
