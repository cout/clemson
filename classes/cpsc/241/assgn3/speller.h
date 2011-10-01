#include "ternary.h"
#include <iostream.h>

class Speller {
public:
	Speller(istream &in);
	void inittst(istream &in);
	void checkfile(istream &in);
	void checkword(char *s, unsigned int line);
	int findword(char *s);
	void initdelim (char *d);
private:
	bool delim[256];
	TTree tst;
};
