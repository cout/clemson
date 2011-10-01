// Bignum
// Paul Brannan
// Assignment 2 for Dr. Malloy's CPSC241 class
// 3/2/98

#include <iostream.h>
#include "Bignum.h"

int main() {
	Bignum n(1234567890), n2(15);		// Some initial values
	Bignum d[10];				// Holds distribution of digits

	// Do some verification tests
	cout << "Verifying constructor:" << endl;
	cout << n << endl;
	cout << "Verifying operator+:" << endl;
	cout << n << " + " << n2 << " = " << n + n2 << endl;
	cout << "Verifying operator*:" << endl;
	cout << n << " * " << n2 << " = " << n * n2 << endl;
	cout << "Verifying pow():" << endl;
	cout << 2 << " ^ " << 40 << " = " << pow((Bignum)2, 40) << endl;

	// Print a message
	cout << endl << "These numbers should check out with bc.  Now ";
	cout << "2^4000 will be calculated.  This" << endl;
	cout << "will take some time (up to 5 minutes, as this code is ";
	cout << "not very optimized), so" << endl;
	cout << "please be patient.  You may even want to go get a cup ";
	cout << "of coffee or go order " << endl << "some pizza." << endl;

	// Calculate 2^4000 and the distribution of digits
	n = pow((Bignum)2, 4000);
	cout << 2 << " ^ " << 4000 << " = " << n << endl;
	n.distrib(d);
	cout << "Distribution of digits:" << endl;
	for(int j = 0; j < 10; j++) cout << j << ": " << d[j] << ", ";
	cout << endl;
}
