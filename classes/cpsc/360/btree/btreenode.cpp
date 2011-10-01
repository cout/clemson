#include <stdlib.h>
#include <string.h>

#include "btreenode.h"

int BTreeNode::NodesPerBlock(int blockSize, int elementSize) {
    return((blockSize - sizeof(int) - sizeof(int) - sizeof(bool)) /
        (elementSize + sizeof(int)));
}

int BTreeNode::NodeSize(int elementSize, int maxElements) {
	return maxElements * elementSize + maxElements * sizeof(int) +
		sizeof(int) + sizeof(int) + sizeof(bool);
}

int BTreeNode::NodeSize() {
    return NodeSize(elementSize, maxElements);
}

void BTreeNode::resetnode() {
	// Find the number of elements in the list.
	// There can be no NULL elements, so the last NULL element is
	// the first free element holder.
	(*numElements) = 0;
	for(char *p = elementList; p < (char *)offsetList && *p != 0;
		p += elementSize) (*numElements)++;
}

void BTreeNode::initPointers() {
	offsetList = (int *)((char *)elementList + maxElements * elementSize);
	numElements = (int *)((char *)offsetList + maxElements * sizeof(*offsetList));
	nextLeaf = (int *)((char *)numElements + sizeof(*numElements));
	isLeafNode = (bool *)(&nextLeaf[1]);
}
    
BTreeNode::BTreeNode(int elements, int size) {

	// Initialize lists
	maxElements = elements;
	elementSize = size;
	elementList = new char[NodeSize()];
    initPointers();

    memset(elementList, 0, elementSize * maxElements);
    *numElements = 0;
	*isLeafNode = false;

    doCleanup = true;
}

BTreeNode::BTreeNode(int elements, int size, char *buf) {

	// Initialize lists
	maxElements = elements;
	elementSize = size;
	elementList = (char *)buf;
    initPointers();

	*isLeafNode = false;

    doCleanup = false;
}

BTreeNode::~BTreeNode() {
	if(doCleanup) delete[] elementList;
}

bool BTreeNode::insertElement(const char *element, int offset) {
	if(isFull()) return false;

	int j;
	findElementIndex(element, &j);

    if(j < (*numElements)) {
        memmove(elementList + (j + 1) * elementSize,
		    elementList + j * elementSize,
            ((*numElements) - j) * elementSize);
        memmove(&offsetList[j + 1], &offsetList[j],
            ((*numElements) - j) * sizeof(*offsetList));
    }
	strncpy(elementList + j * elementSize, element, elementSize-1);
	offsetList[j] = offset;
	(*numElements)++;
	return true;
}

bool BTreeNode::deleteElement(const char *element) {
	int n;
	if(!(findElementIndex(element, &n))) return false;
	memmove(elementList + n * elementSize, elementList + (n + 1) *
		elementSize, ((*numElements) - n) * elementSize);
    memmove(&offsetList[n], &offsetList[n + 1],
        ((*numElements) - n) * sizeof(*offsetList));
	(*numElements)--;
	memset(elementList + (*numElements) * elementSize, 0, elementSize);
    offsetList[(*numElements)] = 0;
	return true;
}

bool BTreeNode::searchForElement(const char *element, int *offset) {
	int index;
	bool tmp = findElementIndex(element, &index);
	if(tmp) {
		if(offset != NULL) *offset = offsetList[index];
		return true;
	}
	if(index <= (*numElements)) {
		if(offset != NULL) *offset = offsetList[index];
	} else {
		if(offset != NULL) *offset = *nextLeaf;
	}
	return false;
}

bool BTreeNode::findElementIndex(const char *element, int *index) {
	for(int j = 0; j < (*numElements); j++) {
		int tmp = strncmp(element, elementList + j * elementSize,
			elementSize - 1);
		if(tmp == 0) {
			if(index != NULL) *index = j;
			return true;
		} else if(tmp < 0) {
			if(index != NULL) *index = j;
			return false;
		}
	}
	if(index != NULL) *index = (*numElements);
	return false;
}

// Preconditions:
// 1) this node is full
// 2) *this != n
// 3) maxElements == n.maxElements
bool BTreeNode::doSplit(BTreeNode &n, int offset) {

	// Copy the elements
	memcpy(n.elementList, elementList + elementSize * (maxElements/2),
		elementSize * (maxElements - maxElements/2));
	memcpy(n.offsetList, &offsetList[maxElements/2],
		sizeof(*offsetList) * (maxElements - maxElements/2));

	// Set the number of elements
	*(n.numElements) = maxElements - maxElements/2;
	(*numElements) = maxElements/2;

	// Keep the indexes in order
	*nextLeaf = offset;
	*n.nextLeaf = *nextLeaf;
	*n.isLeafNode = *isLeafNode;

	return true;
}
