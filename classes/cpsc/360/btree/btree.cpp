#include <assert.h>

#include "btree.h"

BTree::BTree(int size, int num):
db(BTreeNode::NodeSize(size, num),
   (BLOCKSIZE>BTreeNode::NodeSize(size, num))?
    BLOCKSIZE:(BTreeNode::NodeSize(size, num))) {

	elementSize = size;
	numElements = num;

}

// loosely based on the algorithm from the book
bool BTree::insertRecord(const char *key, int offset) {
	// get the root node
	BTreeNode tmpnode(numElements, elementSize);
	char *buf = tmpnode.getbuf();
    db.readRecord(0, buf);

	// determine if we are trying to insert the MAXKEY
	if(!strncmp(key, MAXKEY, elementSize)) return false;

	// find the node we are supposed to insert at
	int index = 0;
    do {
        indexStack.push(index);
		if(tmpnode.searchForElement(key, &index)) {
			// clear the stack
			while(!indexStack.empty()) indexStack.pop();
			return false;	// already in tree?
		}
        if(index == 0) return false;
        if(!db.readRecord(index, buf)) return false;
    } while(!tmpnode.isLeaf());

	// place the node into the tree or split then insert
    char *keybuf = NULL;
	for(;;) {
		if(!tmpnode.isFull()) {
			// If not full insert and we are done
			tmpnode.insertElement(key, offset);
			db.writeRecord(index, buf);
			break;
		} else {
			// The node is full -- we must perform a split

			// Allocate space for a new node
			int newindex = blockman.getblock();
			BTreeNode newnode(numElements, elementSize);
			char *newbuf = newnode.getbuf();

			// divide the elements
			tmpnode.doSplit(newnode, newindex);

			// Insert the new key
			if(strncmp(key, tmpnode.maxElement(), elementSize) < 0)
				tmpnode.insertElement(key, offset);
			else
				newnode.insertElement(key, offset);

			db.writeRecord(newindex, newbuf);

			if(indexStack.empty()) {
				// split the root
				index = blockman.getblock();
				db.writeRecord(index, buf);

				BTreeNode newrootnode(numElements, elementSize);
				newrootnode.insertElement(tmpnode.maxElement(),
					index);
				newrootnode.insertElement(newnode.maxElement(),
					newindex);
				db.writeRecord(0, newrootnode.getbuf());
				break;
			} else {
       			db.writeRecord(index, buf);

                if(!keybuf) {
                    keybuf = new char[elementSize];
                    keybuf[elementSize-1] = 0;
                    key = keybuf;
                }
                strncpy(keybuf, tmpnode.maxElement(), elementSize-1);
	    		offset = index;

                db.readRecord(1, buf);
                assert(tmpnode.isLeaf());

                // keep travelling up the tree
				index = indexStack.top();
				indexStack.pop();
				db.readRecord(index, buf);

			}

			// Change the offset of the new node
			tmpnode.deleteElement(newnode.maxElement());
			tmpnode.insertElement(newnode.maxElement(), newindex);
		}
	}

    if(keybuf) delete[] keybuf;

	// clear the stack
	while(!indexStack.empty()) indexStack.pop();

	return true;
}

bool BTree::deleteRecord(const char *key) {
	// get the root node
	BTreeNode tmpnode(numElements, elementSize);
	char *buf = tmpnode.getbuf();
	db.readRecord(0, buf);
	int index = 0;

	// find the node we are supposed to delete
	do {
		indexStack.push(index);
		tmpnode.searchForElement(key, &index);
        if(index == 0) {
            while(!indexStack.empty()) indexStack.pop();
            return false;
        }
        if(!db.readRecord(index, buf)) {
            while(!indexStack.empty()) indexStack.pop();
            return false;
        }
    } while(!tmpnode.isLeaf());

	// delete the record from the node, if it is found
    if(!tmpnode.searchForElement(key, &index)) {
        while(!indexStack.empty()) indexStack.pop();
        return false;
    }

    tmpnode.deleteElement(key);

    while(!indexStack.empty()) indexStack.pop();

	return true;
}

bool BTree::open(const char *filename, bool destruct) {
	if(!db.open(filename, destruct)) return false;
	
    if(destruct) {
        BTreeNode rootnode(numElements, elementSize);
        BTreeNode leafnode(numElements, elementSize);

        rootnode.insertElement(MAXKEY, 1);
        leafnode.insertElement(MAXKEY, 0);
        rootnode.makeIndex();
        leafnode.makeLeaf(0);

        db.writeRecord(0, rootnode.getbuf());
        db.writeRecord(1, leafnode.getbuf());

        blockman.setnextblock(2);
    } else {
        int j = 2;
        char *buf = new char[elementSize];
        while(db.readRecord(j, buf)) j++;
        blockman.setnextblock(j);
    }

	return true;
}

bool BTree::close() {
	return(db.close());
}

bool BTree::findRecord(const char *key, int *index) {
	BTreeNode tmpnode(numElements, elementSize);
	// get the root node
	char *buf = tmpnode.getbuf();
	db.readRecord(0, buf);

	// find the node we are supposed to delete
	do {
		tmpnode.searchForElement(key, index);
		if(*index == 0) return false;	// something's wrong
		db.readRecord(*index, buf);
	} while(!tmpnode.isLeaf());

	if(!tmpnode.searchForElement(key, index)) return false;
	return true;
}
