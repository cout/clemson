#include <sys/types.h>
#include <sys/stat.h>

#include "btreedb.h"

BTreeDB::BTreeDB(int keysize, int recsize):
db(recsize),
btree(keysize, BTreeNode::NodesPerBlock(BLOCKSIZE, keysize)) {
    elementSize = recsize;
}

bool BTreeDB::open(const char *dbfile, const char *idxfile, bool destruct) {
    struct stat buf;
    int fileisthere = !stat(dbfile, &buf);

	if(!db.open(dbfile, destruct)) return false;
    if(destruct || !fileisthere) {
        char *buf = new char[elementSize];
        memset(buf, 0, elementSize);
        db.writeRecord(0, buf);
        blockman.setnextblock(1);
        delete[] buf;
    } else {
        char *buf = new char[elementSize];
        int j = 1;
        // this also has the effect of initializing the buffers
        while(db.readRecord(j, buf)) {
            if(db.isDeleted(j)) blockman.addblock(j);
            j++;
        }
        blockman.setnextblock(j);
        delete[] buf;
    }

    if(!btree.open(idxfile, destruct)) {
        db.close();
        return false;
    }

	return true;
}

bool BTreeDB::close() {
    bool dbclose = db.close();
    bool btreeclose = btree.close();
	return(dbclose && btreeclose);
}

bool BTreeDB::insertKey(const char *key, int block) {
    return btree.insertRecord(key, block);
}

bool BTreeDB::insertRecord(const char *key, void *record) {
    int block = blockman.getblock();
    bool dbins = db.writeRecord(block, record);
    bool btreeins = insertKey(key, block);
    return (btreeins && dbins);
}

bool BTreeDB::deleteRecord(const char *key) {
    int block;
    if(btree.findRecord(key, &block) == false) return false;
    bool btreedel = btree.deleteRecord(key);
    bool dbdel = db.deleteRecord(block);
    blockman.addblock(block);
    return (btreedel && dbdel);
}

bool BTreeDB::findRecord(const char *key, void *record) {
    int block;
    if(btree.findRecord(key, &block) == false) return false;
    if(record) return db.readRecord(block, record);
    return true;
}

int BTreeDB::firstRecord(void *buf) {
    rec = 1;
    if(!db.readRecord(rec, buf)) return 0;
    return rec;
}

int BTreeDB::nextRecord(void *buf) {
    rec++;
    if(!db.readRecord(rec, buf)) return 0;
    return rec;
}
