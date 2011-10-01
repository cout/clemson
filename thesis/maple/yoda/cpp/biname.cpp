#include <list>
#include <algorithm>
#include <iterator>
#include "biname.h"

// Strictly speaking, in a biname, a 0 indicates going down a level,
// and a 1 indicates going up a level.  In this implementation, we are
// using the opposite representation.  This would result in the maxtree
// if we were to use the comparison function to find the mintree.
// Thus, we do all operations backward so that to the outside world
// we are acting as normal.

// ----------------------------------------------------
// Comparison functions
// ----------------------------------------------------

bool operator==(const Biname& b1, const Biname& b2) {
	// Return TRUE iff b1 == b2
	return (b1.i == b2.i);
}

// This is equivalent to the LLT function; it tests to see if one
// biname is lexicographically less than another biname
bool operator<(const Biname& b1, const Biname& b2) {
	// Pseudocode:
	//   1) Test to see if b1 and b2 have the same length
	//      TRUE: return b1.i < b2.i
	//      FALSE: go to step 2
	//   2) Create a temporary b3 which is equal to min(b1, b2)
	//   3) Create a temporary b4 which is equal to max(b1, b2)
	//   4) Shift b3.i to the left to have the same length as b4.i
	//   5) return the appropriate value after comparing b3.i < b4.i

	int b1size = sizeinbase(b1.i, 2);
	int b2size = sizeinbase(b2.i, 2);
	if(b1size == b2size) {
		// This operation intentionally backward
		return b1.i > b2.i;
	} else {
		GMP_Integer g1, g2;
		if(b1.i < b2.i) {
			g1 = b1.i << (b2size - b1size);
			g2 = b2.i;
		} else {
			g1 = b1.i;
			g2 = b2.i << (b1size - b2size);
		}
		return (g1 < g2);
	}
}

bool operator>(const Biname& b1, const Biname& b2) {
	// Return FALSE if b1 < b2 OR b1 == b2
	return !(b1 == b2 || b1 < b2);
}

bool operator!=(const Biname& b1, const Biname& b2) {
	return b1 != b2;
}

// ----------------------------------------------------
// Concatenation
// ----------------------------------------------------

Biname& Biname::operator+=(const Biname& b) {
	// Pseudocode:
	// Shift i to the left to accomodate space for b.i
	// i |= b.i
	int bits_to_shift = sizeinbase(b.i,2);
	i <<= bits_to_shift;
	i |= b.i;
	return *this;
}

// ----------------------------------------------------
// Assignment
// ----------------------------------------------------

Biname& Biname::operator=(const Biname& b) {
	i = b.i;
	return *this;
}

void Biname::do_copy(unsigned int j) {
	// Since the parameter here is an int, it was constructed by
	// the outside world.  Thus, we must reverse the bits to
	// produce a number compatible with our internal representation.

	// Count the number of ones in the integer
	int onecount = 0;
	for(int jtmp = j; jtmp != 0; jtmp >>= 1) if(jtmp&1) onecount++;

	// Add digits on to the biname until we have exhausted the ones
	i = 0;
	int tmp;
	while(onecount > 0) {
		tmp = j & 1;
		if(!tmp) onecount--;
		i <<= 1;
		i |= tmp;
		j >>= 1;
	}
}

// ----------------------------------------------------
// Misc
// ----------------------------------------------------

// smerg merges a list of binames into a single list r in lexicographic
// order, and places a 0 to the left and a 1 to the right of the merged
// list.  This is useful for traversing a tree.
Biname& Biname::smerg(list<Biname> l) {
	l.sort();
	
	list<Biname>::iterator it;
	i = 1;
	for(it = l.begin(); it != l.end(); it++) {
		*this += *it;
	}
	i <<= 1;

	return *this;
}


// ----------------------------------------------------
// Output
// ----------------------------------------------------

ostream& operator<< (ostream& out, const Biname& b) {
	// We need to convert b.i before we display it.
	GMP_Integer g,h;

	g = b.i;
	h = 0;
	while(g != 0) {
		h <<= 1;
		h |= (g & 1);
		g >>= 1;
	}
	
	out << h;

	return out;
}
