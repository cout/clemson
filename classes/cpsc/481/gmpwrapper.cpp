#include <limits.h>
#include "gmpwrapper.h"

// -------------------------------------------------
// Arithmetic functions
// -------------------------------------------------

GMP_Integer operator+(const GMP_Integer &g1, const GMP_Integer &g2) {
	GMP_Integer g;
	mpz_add(g.i, g1.i, g2.i);
	return g;
}

GMP_Integer operator+(const GMP_Integer &g1, unsigned int i2) {
	GMP_Integer g;
	mpz_add_ui(g.i, g1.i, i2);
	return g;
}

GMP_Integer operator-(const GMP_Integer &g1, const GMP_Integer &g2) {
	GMP_Integer g;
	mpz_sub(g.i, g1.i, g2.i);
	return g;
}

GMP_Integer operator-(const GMP_Integer &g1, unsigned int i2) {
	GMP_Integer g;
	mpz_sub_ui(g.i, g1.i, i2);
	return g;
}

GMP_Integer operator*(const GMP_Integer &g1, const GMP_Integer &g2) {
	GMP_Integer g;
	mpz_mul(g.i, g1.i, g2.i);
	return g;
}

GMP_Integer operator*(const GMP_Integer &g1, unsigned int i2) {
	GMP_Integer g;
	mpz_mul_ui(g.i, g1.i, i2);
	return g;
}

GMP_Integer operator%(const GMP_Integer &g1, const GMP_Integer &g2) {
	GMP_Integer g;
	mpz_mod(g.i, g1.i, g2.i);
	return g;
}

unsigned int operator%(const GMP_Integer &g1, unsigned int i2) {
	GMP_Integer g;
	return mpz_mod_ui(g.i, g1.i, i2);
}

GMP_Integer operator/(const GMP_Integer &g1, const GMP_Integer &g2) {
	GMP_Integer g;
	mpz_tdiv_q(g.i, g1.i, g2.i);
	return g;
}

GMP_Integer operator/(const GMP_Integer &g1, unsigned int i2) {
	GMP_Integer g;
	mpz_tdiv_q_ui(g.i, g1.i, i2);
	return g;
}

GMP_Integer& GMP_Integer::operator+=(const GMP_Integer &g) {
	return (*this = *this + g);
}

GMP_Integer& GMP_Integer::operator+=(unsigned int i2) {
	return (*this = *this + i2);
}

GMP_Integer& GMP_Integer::operator-=(const GMP_Integer &g) {
	return (*this = *this - g);
}

GMP_Integer& GMP_Integer::operator-=(unsigned int i2) {
	return (*this = *this - i2);
}

GMP_Integer& GMP_Integer::operator*=(const GMP_Integer &g) {
	return (*this = *this * g);
}

GMP_Integer& GMP_Integer::operator*=(unsigned int i2) {
	return (*this = *this * i2);
}

GMP_Integer& GMP_Integer::operator/=(const GMP_Integer &g) {
	return (*this = *this / g);
}

GMP_Integer& GMP_Integer::operator/=(unsigned int i2) {
	return (*this = *this / i2);
}

GMP_Integer& GMP_Integer::operator++() {
	(*this) += 1;
	return *this;
}

GMP_Integer& GMP_Integer::operator--() {
	(*this) -= 1;
	return *this;
}

GMP_Integer gcd(const GMP_Integer &g1, const GMP_Integer &g2) {
	GMP_Integer g;
	mpz_gcd(g.i, g1.i, g2.i);
	return g;
}

GMP_Integer mulmod(const GMP_Integer &g1, const GMP_Integer &g2,
                   const GMP_Integer &m) {
	GMP_Integer g;
	g = (g1 * g2) % m;
	return g;
}

GMP_Integer expmod(const GMP_Integer &g1, const GMP_Integer &g2,
                   const GMP_Integer &m)  {
	GMP_Integer g;
	mpz_powm(g.i, g1.i, g2.i, m.i);
	return g;
}

GMP_Integer GMP_Integer::mulmod(const GMP_Integer &g, const GMP_Integer &m) {
	return (*this = ::mulmod(*this, g, m));
}

GMP_Integer GMP_Integer::expmod(const GMP_Integer &g, const GMP_Integer &m) {
	return (*this = ::expmod(*this, g, m));
}

GMP_Integer sqrt(const GMP_Integer &g) {
	GMP_Integer h;
	mpz_sqrt(h.i, g.i);
	return h;
}

// -------------------------------------------------
// Comparison functions
// -------------------------------------------------

bool operator==(const GMP_Integer &g1, const GMP_Integer &g2) {
	return mpz_cmp(g1.i, g2.i) == 0;
}

bool operator==(const GMP_Integer &g1, unsigned int i2) {
	return mpz_cmp_ui(g1.i, i2) == 0;
}

bool operator>(const GMP_Integer &g1, const GMP_Integer &g2) {
	return mpz_cmp(g1.i, g2.i) > 0;
}

bool operator>(const GMP_Integer &g1, unsigned int i2) {
	return mpz_cmp_ui(g1.i, i2) > 0;
}

bool operator<(const GMP_Integer &g1, const GMP_Integer &g2) {
	return mpz_cmp(g1.i, g2.i) < 0;
}

bool operator<(const GMP_Integer &g1, unsigned int i2) {
	return mpz_cmp_ui(g1.i, i2) < 0;
}

// -------------------------------------------------
// Assignment functions
// -------------------------------------------------

GMP_Integer& GMP_Integer::operator=(const GMP_Integer &g) {
	mpz_set(i, g.i);
	return *this;
}

GMP_Integer& GMP_Integer::operator=(unsigned int i2) {
	mpz_set_ui(i, i2);
	return *this;
}

// -------------------------------------------------
// Output functions
// -------------------------------------------------

ostream& operator<< (ostream& out, const GMP_Integer& g) {
	GMP_Integer x, y, z;

	// Test for zero
	if(g == 0) {
		out << 0;
		return out;
	}

	// Tets for negative
	if(g < 0) {
		out << '-';
	}

	// Reverse g and put it in x
	z = g;
	while(!(z == 0)) {
		y = z % 0x10;
		x *= 0x10;
		x += y;
		z /= 0x10;
	}

	// Now put x on the stream
	while(!(x == 0)) {
		out << hex << x % 0x10;
		x /= 0x10;
	}

	return out;
}

// -------------------------------------------------
// Miscellaneous functions
// -------------------------------------------------
GMP_Integer& GMP_Integer::rand(int size) {
	mpz_random(i, size);
	return *this;
}

bool GMP_Integer::isprime() {
	// We want to make sure; the manual says 25 gives a reasonable
	// estimate, so this should be more than sufficient.
	return mpz_probab_prime_p(i, 1000);
}

GMP_Integer GMP_Integer::minverse(const GMP_Integer& m) {
	GMP_Integer x;
	mpz_invert(x.i, i, m.i);
	return x;
}

