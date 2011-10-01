#include <iostream>
#include "biname.h"

int main() {
	cout << "This should test the Biname class for proper "
		"functionality." << endl;

	Biname b;
	cout << "Should print 0:" << endl;
	cout << b << endl;

	Biname b1(0x3);
	cout << "Should print 0x3 (0011b):" << endl;
	cout << "0x" << hex << b1 << endl;

	b = 0x17;
	cout << "Should print 0x17 (00010111b):" << endl;
	cout << "0x" << hex << b << endl << endl;

	cout << "0x17 < 0x3: ";
	cout << dec << (b < b1) << endl;

	cout << "0x3 < 0x17: ";
	cout << dec << (b1 < b) << endl;

	b = 5;
	cout << "0x5 < 0x3: ";
	cout << dec << (b < b1) << endl;

	cout << "0x3 < 0x5: ";
	cout << dec << (b1 < b) << endl;

	b = 0x3f;
	b1 = 0xfe1f;
	cout << "0x3f < 0xfe1f: ";
	cout << dec << (b < b1) << endl;

	b = 0x3f;
	b1 = 0xfff;
	cout << "0x3f < 0xfff: ";
	cout << dec << (b < b1) << endl << endl;

	cout << "Testing smerg on an empty list." << endl;
	list<Biname> l;
	b.smerg(l);
	cout << "0x" << hex << b << endl;

	cout << "Testing smerg on a list with 0 in it." << endl;
	l.push_back(0);
	b.smerg(l);
	cout << "0x" << hex << b << endl;

	cout << "We will now merge the binames 0x3, 0x17, and 0x1" << endl;
	cout << "The merged list should be 0x0b9b" << endl;
	l.clear();
	l.push_back(0x3);
	l.push_back(0x17);
	l.push_back(0x1);
	b.smerg(l);
	cout << "0x" << hex << b << endl;

	return 0;
}

