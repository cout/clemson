#include "blocklist.h"
#include "math/matrix.h"

#ifndef BOOL
#define BOOL char;
#endif

#define MAX_BLOCK_IDS 4

typedef Vector Point;

typedef struct {
	BOOL split;
	int id[MAX_BLOCK_IDS];
	int num_elements;
} Block;

typedef struct {
	BOOL split;
	Blocklist *data[8];
	Point center;
	int blocksize;
} Blocklist;
