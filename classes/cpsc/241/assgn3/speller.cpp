#include "speller.h"
#include <ctype.h>
#include <string.h>

Speller::Speller(istream& in) {
#ifdef DEBUG
	cerr << "Initializing ternary search tree..." << endl;
#endif
	inittst(in);
#ifdef DEBUG
	cerr << "Initializing array delim..." << endl;
#endif
	initdelim(" .,;:'\"\\`-&\t\r\n");
#ifdef DEBUG
	cerr << "Initialization of speller done." << endl;
#endif
}

void Speller::inittst(istream &in) {
	char s[256];
	while(!in.eof()) {
		in >> s;
		tst.insert(s);
	}
}

void Speller::checkfile(istream &in) {
	char s[32768];
	unsigned int line = 0;
	char *t, *j, *max;

	while(in) {
		in.getline(s, 32768);
		line++;
		j = t = s;
		max = s + in.gcount();
		while(j < max) {
			if(delim[*j]) {
				*j = 0;
				checkword(t, line);
				t = j + 1;
				while((j < max) && delim[*j]) j++;
			} else {
				j++;
			}
		}
	}
}

void Speller::checkword(char *s, unsigned int line) {
	switch(findword(s)) {
		case 1:
			cout << line << ": " << s << " misspelled." << endl;
			break;
		case 2:
			cout << line << ": " << s << " should be uppercase." << endl;
			break;
	}
}

int Speller::findword(char *s) {
	if(tst.search(s)) return 0;	// if found then it's spelled right

	char temp = s[0];		// save first letter of the words
	if((s[0] >= 'A') && (s[0] <= 'Z')) {
		s[0] = s[0] - 'A' + 'a';	// convert to lowercase
		if(tst.search(s)) {		// and search again
			s[0] = temp;
			return 0;
		}
	} else if((s[0] >= 'a') && (s[0] <= 'z')) {
		s[0] = s[0] - 'a' + 'A';
		if(tst.search(s)) {
			s[0] = temp;
			return 2;
		}
		s[0] = temp;
	} else return 1;

	unsigned int len = strlen(s) - 1;
	if(s[len] == 's') {
		s[len] = 0;
		if(tst.search(s)) {
			s[len] = 's';
			return 0;
		}
		if(s[len - 1] == 'e') {
			s[len - 1] = 0;
			if(tst.search(s)) {
				s[len - 1] = 'e';
				s[len] = 's';
				return 0;
			}
			if(s[len - 2] == 'i') {
				s[len - 2] = 'y';
				if(tst.search(s)) {
					s[len - 2] = 'i';
					s[len - 1] = 'e';
					s[len] = 's';
					return 0;
				}
				s[len - 2] = 'i';
			}
			s[len - 1] = 'e';
		}
		s[len] = 's';
	}
	return 1;
}

void Speller::initdelim (char *d) {
#ifdef DEBUG
	cerr << "Clearing array delim..." << endl;
#endif
	memset(delim, 0, sizeof(delim));
#ifdef DEBUG
	cerr << "Setting values in array delim..." << endl;
#endif
	while(*d != 0) {delim[*d] = true; d++;}
}

