#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include "factor.h"

int primes[1000];

void initprimes() {
	int temp[10000];
	int i, j;

	// Initialize the array to all 1's
	for(i = 0; i < 10000; i++) temp[i] = 1;

	// We don't need to go beyond 100 or so
	temp[0] = 0;
	temp[1] = 0;
	for(i = 2; i <= 100; i++) 
		if(temp[i] != 0) 
			for(j = i*i; j < 10000; j += i) temp[j] = 0;

	// record the first 1000 primes
	for(i = 0, j = 0; j < 1000; i++) {
		if(temp[i] != 0) {
			primes[j++] = i;
		}
	}
}

factors factorize(biggy n) {
// This is the old code
/*	factors temp;
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

	return temp;*/

// This is the quadratic sieve code
	
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

biggy factors::value() {
	biggy temp = 1;
	int i, j;

	for(j = 0; j < 1000; j++)
		for(i = 0; i < degrees[j]; i++) temp *= j;

	return temp;
}

factors factors::operator *(const factors f) {
	int j;
	factors temp;

	for(j = 0; j < 1000; j++)
		temp.degrees[j] = *this->degrees[j] + f.degrees[j];
	
	temp.sign = *this->sign * f.sign;

	return temp;
}

int issquare() {
	for(int j = 0; j < 1000; j++) if(*this->degrees[j] % 1) return 0;
	return 1;
}

