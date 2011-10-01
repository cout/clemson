#ifndef GMPWRAPPER_H
#define GMPWRAPPER_H

#include <iostream>
#include <gmp.h>

class GMP_Int {
public:
    GMP_Int() {
        mpz_init(i);
    }
    GMP_Int(unsigned int j) {
        mpz_init_set_ui(i, j);
    }
    GMP_Int(const GMP_Int &g) {
        mpz_init_set(i, g.i);
    }
    void clear() {
        mpz_clear(i);
    }

    // Arithmetic functions
    friend GMP_Int operator+(const GMP_Int &g1, const GMP_Int &g2);
    friend GMP_Int operator+(const GMP_Int &g1, unsigned int i2);
    friend GMP_Int operator-(const GMP_Int &g1, const GMP_Int &g2);
    friend GMP_Int operator-(const GMP_Int &g1, unsigned int i2);
    friend GMP_Int operator*(const GMP_Int &g1, const GMP_Int &g2);
    friend GMP_Int operator*(const GMP_Int &g1, unsigned int i2);
    friend GMP_Int operator%(const GMP_Int &g1, const GMP_Int &g2);
    friend unsigned int operator%(const GMP_Int &g1, unsigned int i2);
    friend GMP_Int operator/(const GMP_Int &g1, const GMP_Int &g2);
    friend GMP_Int operator/(const GMP_Int &g1, unsigned int i2);
    friend GMP_Int sqrt(const GMP_Int &g);
    GMP_Int& operator++();
    GMP_Int& operator--();
    friend GMP_Int gcd(const GMP_Int &g1, const GMP_Int &g2);
    friend GMP_Int mulmod(const GMP_Int &g1, const GMP_Int &g2,
                          const GMP_Int &m);
    friend GMP_Int expmod(const GMP_Int &g1, const GMP_Int &g2,
                          const GMP_Int &m);
    friend int sizeinbase(const GMP_Int &g, int base);
    GMP_Int mulmod(const GMP_Int &g, const GMP_Int &m);
    GMP_Int expmod(const GMP_Int &g, const GMP_Int &m);
    GMP_Int& operator+=(const GMP_Int &g);
    GMP_Int& operator+=(unsigned int i2);
    GMP_Int& operator-=(const GMP_Int &g);
    GMP_Int& operator-=(unsigned int i2);
    GMP_Int& operator*=(const GMP_Int &g);
    GMP_Int& operator*=(unsigned int i2);
    GMP_Int& operator/=(const GMP_Int &g);
    GMP_Int& operator/=(unsigned int i2);

    // Logical and bit-manipulation functions
    friend GMP_Int operator>>(const GMP_Int &g1, unsigned int n);
    friend GMP_Int operator<<(const GMP_Int &g1, unsigned int n);
    GMP_Int& operator>>=(unsigned int n);
    GMP_Int& operator<<=(unsigned int n);
    friend GMP_Int operator|(const GMP_Int &g1, const GMP_Int &g2);
    GMP_Int& operator|=(const GMP_Int &g);
    friend GMP_Int operator&(const GMP_Int &g1, const GMP_Int &g2);
    GMP_Int& operator&=(const GMP_Int &g);
    friend int operator&(const GMP_Int &g1, unsigned int n);
    GMP_Int& operator&=(unsigned int n);

    // Comparison functions
    friend bool operator==(const GMP_Int &g1, const GMP_Int &g2);
    friend bool operator!=(const GMP_Int &g1, const GMP_Int &g2);
    friend bool operator==(const GMP_Int &g1, unsigned int i2);
    friend bool operator!=(const GMP_Int &g1, unsigned int i2);
    friend bool operator>(const GMP_Int &g1, const GMP_Int &g2);
    friend bool operator>(const GMP_Int &g1, unsigned int i2);
    friend bool operator<(const GMP_Int &g1, const GMP_Int &g2);
    friend bool operator<(const GMP_Int &g1, unsigned int i2);

    // Assignment functions
    GMP_Int& operator=(const GMP_Int &g);
    GMP_Int& operator=(unsigned int i);

    // Output functions
    friend ostream& operator<< (ostream& out, const GMP_Int& g);

private:
    mpz_t i;
};

typedef GMP_Int GMP_Integer;

#endif
