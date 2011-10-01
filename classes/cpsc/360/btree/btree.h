#ifndef __BTREE_H

#define __BTREE_H

#include <stack>
#include <vector>

using namespace std;

#include "btreenode.h"
#include "dbfile.h"
#include "blockman.h"

// MAXKEY is two ASCII 255 (blank) characters
const char MAXKEY[] = {(char)255, (char)255, (char)0};

class BTree {
public:
	BTree(int keysize, int num);
	bool insertRecord(const char *key, int offset);
	bool deleteRecord(const char *key);
	bool open(const char *filename, bool destruct = true);
	bool close();
	bool findRecord(const char *key, int *offset);

private:
	DBFile db;
	int elementSize;
	int numElements;
	stack<int, vector<int> > indexStack;
    BlockMan blockman;
};

#endif
