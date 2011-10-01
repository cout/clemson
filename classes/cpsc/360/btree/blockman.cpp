#include "blockman.h"

BlockMan::BlockMan() {
    nextblock = 0;
}

// return an offset to a free block of memory
int BlockMan::getblock() {
	int index;
	if(freeList.empty()) {
		index = nextblock;
		nextblock++;
		return index;
	}
	index = freeList.top();
	freeList.pop();
	return index;
}
