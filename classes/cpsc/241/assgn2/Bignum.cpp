#include <iostream.h>
#include "Bignum.h"

// Default constructor
Bignum::Bignum(long n) {
	do {
		nums.insertAtRear(n % 10);
		n /= 10;
	} while(n != 0);
}

// Copy constructor
Bignum::Bignum(const Bignum &n) {
	Node *ptr = n.nums.getFirst();		// Copy all elements
	while(ptr != 0) {
		nums.insertAtRear(ptr->getValue());
		ptr = n.nums.getNext();
	}
}

// Overloaded output operator
ostream& operator<< (ostream &out, const Bignum &n) {
	Node *ptr = n.nums.getLast();
	do out << ptr->getValue(); while((ptr = n.nums.getPrev()) != 0);
	return out;
}

// Overloaded assignment operator
const Bignum& Bignum::operator= (const Bignum &n) {
	nums.clear();				// First, clear out the digits
        Node *ptr = n.nums.getFirst();		// Copy all elements
        while(ptr != 0) {
                nums.insertAtRear(ptr->getValue());
                ptr = n.nums.getNext();
        }
	return *this;
}

// Overloaded != operator
// Useful for "for" statements
bool operator!= (const Bignum &n1, const Bignum &n2) {
	// If the numbers aren't the same size, then they aren't equal
	if(n1.nums.size() != n2.nums.size()) return true;

	// Check all the digits to see if the numbers are equal
	Node *ptr1 = n1.nums.getFirst();
	Node *ptr2 = n2.nums.getFirst();
	while(ptr1 != 0) {
		if(ptr1->getValue() != ptr2->getValue()) return true;
		ptr1 = n1.nums.getNext();
		ptr2 = n2.nums.getNext();
	}

	// If the numbers are equal, we return false
	return false;
}

// Overloaded prefix ++
Bignum& Bignum::operator++ () {
	*this = *this + 1;			// Increment the value
	return *this;
}

// Overloaded addition operator
Bignum operator+ (const Bignum &n1, const Bignum &n2) {
	int carry = 0, minisum;			// Holds carry and partial sum
	Bignum sum;				// Holds the sum

	sum.nums.clear();			// Clear the digits, since we
						// have a "0" already in there
	Node *ptr1 = n1.nums.getFirst();	// Start at the beginning
	Node *ptr2 = n2.nums.getFirst();
	while((ptr1 != 0) || (ptr2 != 0)) {
		minisum = carry;		// Bring over the carry bit
		if(ptr1 != 0) {			// Don't add if we're at the end
			minisum += ptr1->getValue();
			ptr1 = n1.nums.getNext();
		}
		if(ptr2 != 0) {			// Don't add if we're at the end
			minisum += ptr2->getValue();
			ptr2 = n2.nums.getNext();
		}
		carry = minisum / 10;		// Calculate the new carry
		minisum %= 10;			// Take the carry out
		sum.nums.insertAtRear(minisum);	// Add onto the current sum
	}
	// We don't want a 0 on the end of the number, but we do want the last
	// carry value
	if(carry != 0) sum.nums.insertAtRear(carry);
	return sum;
}

// Overloaded multiplication operator
Bignum operator* (const Bignum &n1, const Bignum &n2) {
	Bignum temp(0), j;
	// Calculate product by multiple sums
	for(j = 0; j != n2; ++j) temp = temp + n1;
	return temp;
}

// Overloaded pow() function -- raises n1 to the power of n2
Bignum pow (const Bignum &n1, const Bignum &n2) {
	Bignum temp(1), j;
	// Calculate power by many multiplies
	for(j = 0; j != n2; ++j) temp = temp * n1;
	return temp;
}

// Calculate the distribution of digits in a number
// Takes an array[10] of Bignum that gets modified to contain the distribution
// of the digits 0-9
void Bignum::distrib(Bignum d[]) {
	Node *ptr = nums.getFirst();
	while(ptr != 0) {
		++d[ptr->getValue()];
		ptr = nums.getNext();
	}
}
