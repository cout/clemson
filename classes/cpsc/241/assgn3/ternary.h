#define BUFSIZE 1024

class TNode {
	friend class TTree;
private:
	char splitchar;
	TNode *lokid, *eqkid, *hikid;
};

class TTree {
public:
	TTree() {root = 0; bufn = freen = 0; bs = BUFSIZE;}
	int search(char *s);
	void insert(char *s);
	TNode* rinsert(TNode *p, char *s);
private:
	TNode *root;
	TNode *buf;
	int bufn, freen, bs;
	void *freearr[10000];
};
