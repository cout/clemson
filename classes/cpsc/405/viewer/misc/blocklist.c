#include "blocklist.h"

/* Find which block a point is in, given a point p and a block center c */
int get_block(Point *p, Center *c) {
	return ((((p[0] < c[0]) & 1) << 0) |
	        (((p[1] < c[1]) & 1) << 1) |
	        (((p[2] < c[2]) & 1) << 2));
}

/* Return a list of polygons in a given block */
Block* get_poly_list(Point *p, Blocklist *b) {
	block = get_block(p, b->center);
	if(b->data[block].split) return get_poly_list(p, b->data[block]);
	return (Block*)b->data[block];
}

/* Split a block */
/* b is the blocklist of the parent block, and blocknum is the number of the
 * block to be split.
 */
void block_split(Blocklist *b, int blocknum) {
	int j;
	Block* tmpblock = (Block*)b->data[blocknum];
	b->data[blocknum] = (Blocklist*)malloc(sizeof(Blocklist));
	for(j = 0; j < tmpblock->num_elements; j++) {
		/* Add each id in tmpblock to the appropriate newly created
		 * block.
		 */
	}
	free(tmpblock);
}

/* Add an ID to the blocklist (octtree) */
void add_poly(Point *p, int id, Blocklist *b) {
	block = get_block(p, b->center);
	if(b->data[block].split) {
		add_poly(p, id, b->data[block]);
	} else {
		Block *bl = (Block*)b->data[block];
		for(j = 0; j < bl->num_elements)
			if(bl->id[j] == id) return;
		if(bl->num_elements >= MAX_BLOCK_IDS) {
			block_split(b, block);
			add_poly(p, id, b->data[block]);
		}
		bl->id[bl->num_elements++] = id;
	}
}

