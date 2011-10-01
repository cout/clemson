#include "Hex.h"

extern const unsigned int MAX_DIGITS;

Hex& Hex::operator++() {
	number++;
	return *this;
}

Hex& Hex::operator--() {
	number--;
	return *this;
}

const Hex& Hex::operator=(const Hex& b) {
	if (this == &b) return *this;
	number = b.number;

	return *this;
}

Hex operator+(const Hex& LHS, const Hex& RHS) {
	Hex t(LHS.number + RHS.number);
	return t;
}

Hex operator-(const Hex& LHS, const Hex& RHS) {
	Hex t(LHS.number - RHS.number);
	return t;
}

Hex operator*(const Hex& LHS, const Hex& RHS) {
	Hex t(LHS.number * RHS.number);
	return t;
} 

Hex operator/(const Hex& LHS, const Hex& RHS) {
	Hex t(LHS.number / RHS.number);
	return t;
}

void convert(int base, int this_number, char number[]) {
	char temp[MAX_DIGITS];
	int ctr = 0;
	while (this_number) {
		if(this_number % base <= 9)
			temp[ctr++] = this_number % base + '0';
			else temp[ctr++] = this_number % base + 'A' - 10;
		this_number = this_number / base;
	}
	int n = --ctr;
	for (int i = 0; i <= ctr; i++) number[i] = temp[n--];
	number[ctr+1] = '\0';
}

ostream & operator << (ostream& Out, const Hex& this_number) {
	char number[MAX_DIGITS];
	convert(16, this_number.number, number);
	Out << number;
	return Out;
}
