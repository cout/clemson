#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include "speller.h"

int main(int argc, char *argv[]) {
	ifstream words("words"), file(argv[1]);

	if(!words) {
		cerr << "Unable to open dictionary file 'words' for reading" << endl;
		exit(1);
	}

#ifdef DEBUG
	cerr << "Initializing speller..." << endl;
#endif
	Speller speller(words);
#ifdef DEBUG
	cerr << "Checking document..." << endl;
#endif
	if(argc==1) {
		speller.checkfile(cin);
	} else {
		if(!file) {
			cerr << "Unable to open file " << argv[1] << " for reading" << endl;
			exit(1);
		}
		speller.checkfile(file);
	}

	return 0;
}
