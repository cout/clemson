#include <string.h>

#include "secidx.h"

SecondaryIndex::SecondaryIndex(int pkeysize, int skeysize):
pkey(pkeysize + sizeof(int)),
skey(skeysize, BTreeNode::NodesPerBlock(BLOCKSIZE, skeysize)) {
    // pkey is a DBFile
    // skey is a BTree
    primarykeysize = pkeysize;
    secondarykeysize = skeysize;
    buf = new char[pkeysize + sizeof(int)];
    memset(buf, 0, pkeysize + sizeof(int));
}

SecondaryIndex::~SecondaryIndex() {
    delete[] buf;
}

bool SecondaryIndex::insertRecord(const char *secondarykey, const char *primarykey) {
    int oldoffset, offset;
    if(skey.findRecord(secondarykey, &offset)) {
        // the key is already in the tree
        do {
            pkey.readRecord(offset, buf);
            if(!strncmp(buf, primarykey, primarykeysize)) return false;
            oldoffset = offset;
            offset = *(int *)(buf + primarykeysize);
        } while(offset != 0);
        offset = *(int *)(buf + primarykeysize) = blockman.getblock();
        pkey.writeRecord(oldoffset, buf);
        strncpy(buf, primarykey, primarykeysize - 1);
        *(int *)(buf + primarykeysize) = 0;
        pkey.writeRecord(offset, buf);
    } else {
        // the key is not already in the tree
        offset = blockman.getblock();
        skey.insertRecord(secondarykey, offset);
        strncpy(buf, primarykey, primarykeysize - 1);
        *(int *)(buf + primarykeysize) = 0;
        pkey.writeRecord(offset, buf);
    }
    return true;
}

bool SecondaryIndex::deleteRecord(const char *secondarykey, const char *primarykey) {
    int prevoffset = 0, offset = 0, nextoffset = 0;
    if(!skey.findRecord(secondarykey, &nextoffset)) return false;
    do {
        if(nextoffset == 0) return false;
        prevoffset = offset;
        offset = nextoffset;
        pkey.readRecord(offset, buf);
        nextoffset = *(int *)(buf + primarykeysize);
    } while(strncmp(buf, primarykey, primarykeysize));
    blockman.addblock(offset);
    if(prevoffset != 0) {
        pkey.readRecord(prevoffset, buf);
        *(int *)(buf + primarykeysize) = nextoffset;
        pkey.writeRecord(prevoffset, buf);
    } else {
        skey.deleteRecord(secondarykey);
    }
    return true;
}

bool SecondaryIndex::open(const char *skeyfile, const char *pkeyfile, bool destruct) {
	if(!skey.open(skeyfile)) return false;
    if(!pkey.open(pkeyfile)) {
        skey.close();
        return false;
    }
    blockman.setnextblock(1);
	return true;
}

bool SecondaryIndex::close() {
    bool skeyclose = skey.close();
    bool pkeyclose = pkey.close();
	return(skeyclose && pkeyclose);
}

bool SecondaryIndex::findFirstRecord(const char *secondarykey, char *primarykey) {
    if(!skey.findRecord(secondarykey, &findOffset)) return false;
    pkey.readRecord(findOffset, buf);
    findOffset = *(int *)(buf + primarykeysize);
    strncpy(primarykey, buf, primarykeysize - 1);
    primarykey[primarykeysize - 1] = 0;
    return true;
}

bool SecondaryIndex::findNextRecord(char *primarykey) {
    if(findOffset == 0) return false;
    pkey.readRecord(findOffset, buf);
    findOffset = *(int *)(buf + primarykeysize);
    strncpy(primarykey, buf, primarykeysize - 1);
    primarykey[primarykeysize - 1] = 0;
    return true;
}

bool SecondaryIndex::findRecord(const char *secondarykey, char *primarykey) {
    char *key = new char[primarykeysize];
    if(!findFirstRecord(secondarykey, key)) return false;
    while(strncmp(key, primarykey, primarykeysize - 1)) {
        if(!findNextRecord(key)) {
            delete[] key;
            return false;
        }
    }
    delete[] key;
    return true;
}