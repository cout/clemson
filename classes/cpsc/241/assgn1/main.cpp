#include <iostream.h>
#include "Hex.h"

#define HEX_AUTHOR "Paul Brannan"
#define HEX_DATE   "2/5/98"

// We shouldn't need any more digits than this
extern const unsigned int MAX_DIGITS = 100;

main() {

	// Display banner
	cout << "Hex by " << HEX_AUTHOR << endl;
	cout << HEX_DATE << endl << endl;
	cout << "Starting execution..." << endl << endl;

	// Initialize values
	Hex x(3), y(17), z(7);

	// Print initial values
	cout << "x = 0x" << x << endl;
	cout << "y = 0x" << y << endl;
	cout << "z = 0x" << z << endl << endl;

	// Do some operations to show off
	cout << "x + y = 0x" << x + y << endl;
	cout << "y - z = 0x" << y - z << endl;
	cout << "z * x = 0x" << z * x << endl;
	cout << "y / x = 0x" << y / x << endl;

	cout << "++z is 0x" << ++z << endl;
	cout << "--y is 0x" << --y << endl;

	x = Hex(20);
	cout << "x = Hex(20) returns x = 0x" << x << endl;

	y = 10;
	cout << "y = 10 returns y = 0x" << y << endl;

	cout << endl << "Terminating!" << endl;
}
