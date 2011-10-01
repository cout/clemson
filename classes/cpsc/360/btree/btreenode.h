#ifndef __BTREENODE_H

#define __BTREENODE_H

class BTreeNode {
public:
	BTreeNode(int elements, int size);
	BTreeNode(int elements, int size, char *buf);
	~BTreeNode();

	bool insertElement(const char *element, int offset);
	bool deleteElement(const char *element);

	bool searchForElement(const char *element, int *offset);
	bool findElementIndex(const char *element, int *index);

	void makeLeaf(int next) {*isLeafNode = true; *nextLeaf = next;}
	void makeIndex() {*isLeafNode = false;}
	int getNextLeaf() {return *nextLeaf;}
	void setNextLeaf(int index) {*nextLeaf = index;}
	bool isFull() {return *numElements >= maxElements;}
    bool isHalfFull() {return *numElements < maxElements/2;}
	bool isEmpty() {return *numElements == 0;}
	bool isLeaf() {return *isLeafNode;}

    char *getbuf() {return elementList;}

    static int NodesPerBlock(int blockSize, int elementSize);
    static int NodeSize(int size, int elements);
	int NodeSize();

	bool doSplit(BTreeNode &n, int offset);

    char *minElement() {return isEmpty()?NULL:elementList;}
    char *maxElement() {return isEmpty()?NULL:
        (elementList + (*numElements - 1) * elementSize);}

    void resetnode();

private:
    void initPointers();

    // this is what gets saved
	char *elementList;
	int *offsetList;
	bool *isLeafNode;
	int *nextLeaf;
	int *numElements;
    ////

	int elementSize;
	int maxElements;
    bool doCleanup;
};

#endif
