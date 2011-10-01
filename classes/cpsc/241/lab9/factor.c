#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include "factor.h"

int primes[10000];

void initprimes() {
	int temp[1000000];
	int i, j;

	// Initialize the array to all 1's
	for(i = 0; i < 1000000; i++) temp[i] = 1;

	// We don't need to go beyond 100 or so
	temp[0] = 0;
	temp[1] = 0;
	for(i = 2; i <= 10000; i++) 
		if(temp[i] != 0) 
			for(j = i*i; j < 1000000; j += i) temp[j] = 0;

	// record the first 10000 primes
	for(i = 0, j = 0; j < 10000; i++) {
		if(temp[i] != 0) {
			primes[j++] = i;
		}
	}
}

factors factorize(int n) {
	factors temp;
	int j;

	// special case
	if(n == 0) {
		temp.setsign(0);
		return temp;
	}

	// Initialize the primes
	initprimes();

	// Initialize the degrees
	for(j = 0; j < 1000; j++) temp.setdegree(j, 0);

	// Initialize the sign
	temp.setsign(abs(n)/n);
	n = abs(n);

	for(j = 0; j < n; j++) {
		while (n % primes[j] == 0) {
			temp.setdegree(j, temp.getdegree(j) + 1);
			n /= primes[j];
		}
	}

	return temp;
}

int factors::getsign() {
	return sign;
}

int factors::setsign(int s) {
	return (sign = s);
}

int factors::getdegree(int i) {
	return degrees[i];
}

void factors::setdegree(int i, int val) {
	degrees[i] = val;
}

int factors::value() {
	int temp = 1;
	int i, j;

	for(j = 0; j < 1000; j++)
		for(i = 0; i < degrees[j]; i++) temp *= j;

	return temp;
}
