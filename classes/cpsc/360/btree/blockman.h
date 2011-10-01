#ifndef __BLOCKMAN_H

#define __BLOCKMAN_H

#include <stack>
#include <vector>

using namespace std;

class BlockMan {
public:
	BlockMan();
	int getblock();
    void setnextblock(int block) {nextblock = block;}
    int getnextblock() {return nextblock;}
    void addblock(int block) {freeList.push(block);}

private:
	stack<int, vector<int> > freeList;
    int nextblock;
};

#endif

