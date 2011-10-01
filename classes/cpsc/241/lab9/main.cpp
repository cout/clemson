#include <iostream.h>
#include <stdlib.h>
#include "factor.h"

int main(int argc, char *argv[]) {
	if(argc < 2) {
		cout << "Usage: " << argv[0] << "<number>" << endl;
		exit(1);
	}

	int n = atoi(argv[1]);
	factors f = factorize(n);

	if(f.getsign() == 0) cout << "Unable to factor";
	else if(n == 1) cout << "1";
	else {
		int flag = 0;
		for(int j = 0; j < 999; j++) {
			if(f.getdegree(j) != 0) {
				if(flag == 1) {
					cout << '*';
				} else {
					flag = 1;
				}
				cout << primes[j] << '^' << f.getdegree(j);
			}
		}
	}
	cout << endl;

	return 0;
}
