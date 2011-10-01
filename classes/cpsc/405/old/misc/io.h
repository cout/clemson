#ifndef IO_H
#define IO_H

enum Io_Return_Values {
	IO_OK,
	IO_EOL,
	IO_EOF,
	IO_OVERFLOW
};

int io_getword(char *s, int size, FILE *fp);
int io_getfloat(float *f, FILE *fp);
int io_getint(int *i, FILE *fp);

#endif
