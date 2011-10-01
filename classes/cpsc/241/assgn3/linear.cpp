#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if(argc != 2) {
		cerr << "Usage: " << argv[0] << " <word>" << endl;
		exit(1);
	}

	ifstream words("words");
	if(!words) {
		cerr << "Error: cannot open file words for reading.\n" << endl;
		exit(1);
	}

	char s[255];
	cout << '\"' << argv[1] << "\" ";
	while(!words.eof()) {
		words >> s;
		if(!strcmp(s, argv[1])) {
			cout << "found in the dictionary." << endl;
			return 0;
		}
	}
	cout << "not found in the dictionary." << endl;

	return 0;
}
