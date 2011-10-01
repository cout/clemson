#ifndef __SECIDX_H

#define __SECIDX_H

#include <stack>
#include <vector>

using namespace std;

#include "btree.h"
#include "dbfile.h"

#define BLOCKSIZE 4096

class SecondaryIndex {
public:
	SecondaryIndex(int primarykeysize, int secondarykeysize);
    ~SecondaryIndex();
	bool insertRecord(const char *secondarykey, const char *primarykey);
	bool deleteRecord(const char *secondarykey, const char *primarykey);
	bool open(const char *skeyfile, const char *pkeyfile, bool destruct = true);
	bool close();

    bool findFirstRecord(const char *secondarykey, char *primarykey);
    bool findNextRecord(char *primarykey);
	bool findRecord(const char *secondarykey, char *primarykey);

private:
    static int NodeSize(int pkeysize, int skeysize);

    DBFile pkey;
    BTree skey;
    int primarykeysize, secondarykeysize;
    BlockMan blockman;
    char *buf;
    int findOffset;
};

#endif
