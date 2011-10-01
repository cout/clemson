#include <stdio.h>
#include <stdlib.h>
#include "io.h"

int io_getword(char *s, int size, FILE *fp) {
	int c, j;

	/* We won't overwrite anything if there is no buffer */
	if(size == 0) s = 0;

	for(c = fgetc(fp); c == ' ' || c == '\t'; c = fgetc(fp));
	for(j = 0; j < size-1; ) {
		switch(c) {
			case EOF:
				if(s) s[j] = 0;
				if(j > 0) return IO_OK;
				return IO_EOF;
			case ' ':
			case '\t':
				if(s) s[j] = 0;
				return IO_OK;
			case '\n':
				if(s) s[j] = 0;
				if(j > 0) {
					ungetc(c, fp);
					return IO_OK;
				}
				return IO_EOL;
			default:
				if(s) s[j++] = c;
		}
		c = fgetc(fp);
	}

	return IO_OVERFLOW;
}

int io_getfloat(float *f, FILE *fp) {
	int retval;
	char s[128];

	retval = io_getword(s, sizeof(s), fp);
	*f = atof(s);
	return retval;
}

int io_getint(int *i, FILE *fp) {
	int retval;
	char s[128];

	retval = io_getword(s, sizeof(s), fp);
	*i = atoi(s);
	return retval;
}
