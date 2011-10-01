#ifndef MD2_H
#define MD2_H

#include <stdio.h>
#include "texgen.h"

enum md2_return_values {
	MD2_OK,
	MD2ERR_OPEN,
	MD2ERR_VERSION,
	MD2ERR_MAGIC,
	MD2ERR_RANGE,
	MD2ERR_VERTEX,
	MD2ERR_NORMALS,
	MD2ERR_DISPLAYLIST
};

typedef struct {
	int skin;
	int framestart;
	int numframes;
} md2_model_t;

int parse_md2(const char *file, const char *skin, md2_model_t *md2, int tt);
int parse_md2_fp(FILE *fp, const char *skin, md2_model_t *md2, int tt);

#endif
