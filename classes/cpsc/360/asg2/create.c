#include <stdio.h>
main() {
	int j;
	char *buf = "CPSC 360 is fun!  ";
	int len = 32*1024*1024/strlen(buf);
	for(j = 0; j < len; j++) fputs(buf, stdout);
	return 0;
}
