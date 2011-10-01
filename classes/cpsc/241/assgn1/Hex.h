#ifndef __HEX_CLASS__

#define __HEX_CLASS__ 1

#include <iostream.h>

class Hex{
public:

	// copy constructor
	Hex (const Hex& ThisNumber) : number(ThisNumber.number) {};

	// default constructor
	Hex(int n = 0) : number(n) {}

	// increment and decrement
	Hex& operator++();
	Hex& operator--();
	const Hex& operator=(const Hex& b);

	// various operations
	friend Hex operator+(const Hex& LHS, const Hex& RHS);
	friend Hex operator-(const Hex& LHS, const Hex& RHS);
	friend Hex operator*(const Hex& LHS, const Hex& RHS);
	friend Hex operator/(const Hex& LHS, const Hex& RHS);
	friend ostream& operator << (ostream& Out, const Hex& ThisNumber);

private:
	int number;
};

#endif
