#ifndef GMPWRAPPER_H
#define GMPWRAPPER_H

#include <iostream.h>
#include <gmp.h>

class GMP_Integer {
public:
	GMP_Integer() {
		mpz_init(i);
	}
	GMP_Integer(unsigned int j) {
		mpz_init_set_ui(i, j);
	}
	GMP_Integer(const GMP_Integer &g) {
		mpz_init_set(i, g.i);
	}
	GMP_Integer(const char *s) {
		mpz_init_set_str(i, s, 10);
	}
	~GMP_Integer() {
		mpz_clear(i);
	}

// Arithmetic functions
friend GMP_Integer operator+(const GMP_Integer &g1, const GMP_Integer &g2);
friend GMP_Integer operator+(const GMP_Integer &g1, unsigned int i2);
friend GMP_Integer operator-(const GMP_Integer &g1, const GMP_Integer &g2);
friend GMP_Integer operator-(const GMP_Integer &g1, unsigned int i2);
friend GMP_Integer operator*(const GMP_Integer &g1, const GMP_Integer &g2);
friend GMP_Integer operator*(const GMP_Integer &g1, unsigned int i2);
friend GMP_Integer operator%(const GMP_Integer &g1, const GMP_Integer &g2);
friend unsigned int operator%(const GMP_Integer &g1, unsigned int i2);
friend GMP_Integer operator/(const GMP_Integer &g1, const GMP_Integer &g2);
friend GMP_Integer operator/(const GMP_Integer &g1, unsigned int i2);
friend GMP_Integer sqrt(const GMP_Integer &g);
GMP_Integer& operator++();
GMP_Integer& operator--();
friend GMP_Integer gcd(const GMP_Integer &g1, const GMP_Integer &g2);
friend GMP_Integer mulmod(const GMP_Integer &g1, const GMP_Integer &g2,
                          const GMP_Integer &m);
friend GMP_Integer expmod(const GMP_Integer &g1, const GMP_Integer &g2,
                          const GMP_Integer &m);
GMP_Integer mulmod(const GMP_Integer &g, const GMP_Integer &m);
GMP_Integer expmod(const GMP_Integer &g, const GMP_Integer &m);
GMP_Integer& operator+=(const GMP_Integer &g);
GMP_Integer& operator+=(unsigned int i2);
GMP_Integer& operator-=(const GMP_Integer &g);
GMP_Integer& operator-=(unsigned int i2);
GMP_Integer& operator*=(const GMP_Integer &g);
GMP_Integer& operator*=(unsigned int i2);
GMP_Integer& operator/=(const GMP_Integer &g);
GMP_Integer& operator/=(unsigned int i2);

// Comparison functions
friend bool operator==(const GMP_Integer &g1, const GMP_Integer &g2);
friend bool operator==(const GMP_Integer &g1, unsigned int i2);
friend bool operator>(const GMP_Integer &g1, const GMP_Integer &g2);
friend bool operator>(const GMP_Integer &g1, unsigned int i2);
friend bool operator<(const GMP_Integer &g1, const GMP_Integer &g2);
friend bool operator<(const GMP_Integer &g1, unsigned int i2);

// Assignment functions
GMP_Integer& operator=(const GMP_Integer &g);
GMP_Integer& operator=(unsigned int i);

// Output functions
friend ostream& operator<< (ostream& out, const GMP_Integer& g);

// Miscellaneous functions
GMP_Integer& rand(int size);
bool isprime();
GMP_Integer minverse(const GMP_Integer& m);
 
private:
	mpz_t i;
};

#endif
