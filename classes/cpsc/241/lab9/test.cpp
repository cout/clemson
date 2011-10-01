#include <iostream.h>
#include "factor.h"

int main() {
	initprimes();
	for(int j = 0; j < 100; j++) cout << primes[j] << " ";
	cout << endl;

	return 0;
}
