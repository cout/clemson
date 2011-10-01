// Bignum class
// Allows integers to be used that grow in size to take all available memory
// Makes use of the linked list class

#include <iostream.h>
#include "List.h"

#ifndef __Bignum__

#define __Bignum__ __Bignum__

class Bignum {
public:
	Bignum(long n = 0);			// default constructor
	Bignum(const Bignum &n);		// copy constructor

	~Bignum()	{ nums.clear(); }	// destructor (important!)

	// Miscellaneous operators
	friend ostream& operator<< (ostream &out, const Bignum &n);
	const Bignum& operator= (const Bignum &n);
	friend bool operator!= (const Bignum &n1, const Bignum &n2);
	Bignum& operator++ ();
	friend Bignum operator+ (const Bignum &n1, const Bignum &n2);
	friend Bignum operator* (const Bignum &n1, const Bignum &n2);

	// operator^ could be overloaded, but it has the wrong precedence
	friend Bignum pow (const Bignum &n1, const Bignum &n2);

	// counts the distribution of digits in the number
	void distrib(Bignum d[]);
private:
	List nums;				// linked list of digits
};

#endif
