#include <stdio.h>

class hash {
public:
	hash(size s);
	~hash();
	void insert(char *s);
	int insert(
private:
	char **h;
}

hash(size s) {
	h = new char* [s];
	for(int j = 0; j < s; j++) h[j] = NULL;
}

~hash() {
	for(int j = 0; j < s; j++) if(h[j] != NULL) delete[] h[j];
	delete[] h;
}


