#ifndef __BTREEDB_H

#define __BTREEDB_H

#include <stack>
#include <vector>

using namespace std;

#include "btree.h"
#include "dbfile.h"

class BTreeDB {
public:
	BTreeDB(int keysize, int recsize);
    bool insertKey(const char *key, int block);
	bool insertRecord(const char *key, void *record);
	bool deleteRecord(const char *key);
	bool open(const char *dbfile, const char *idxfile, bool destruct = true);
	bool close();

	bool findRecord(const char *key, void *record);
    int firstRecord(void *buf);
    int nextRecord(void *buf);

private:
	DBFile db;
    BTree btree;
    BlockMan blockman;

    int elementSize, rec;
};

#endif
