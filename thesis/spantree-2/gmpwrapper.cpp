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
// Logical and bit-manipulation functions
// -------------------------------------------------

GMP_Integer operator<<(const GMP_Integer& g1, unsigned int n) {
    GMP_Integer g;
    mpz_mul_2exp(g.i, g1.i, n);
    return g;
}

GMP_Integer operator>>(const GMP_Integer& g1, unsigned int n) {
    GMP_Integer g;
    mpz_tdiv_q_2exp(g.i, g1.i, n);
    return g;
}

// FIX ME!! There should be a faster way to do this.
GMP_Integer& GMP_Integer::operator<<=(unsigned int n) {
    return (*this = *this << n);
}

// FIX ME!! There should be a faster way to do this.
GMP_Integer& GMP_Integer::operator>>=(unsigned int n) {
    return (*this = *this >> n);
}

GMP_Integer operator|(const GMP_Integer &g1, const GMP_Integer &g2) {
    GMP_Integer g;
    mpz_ior(g.i, g1.i, g2.i);
    return g;
}

GMP_Integer& GMP_Integer::operator|=(const GMP_Integer &g) {
    return (*this = *this | g);
}

GMP_Integer operator&(const GMP_Integer &g1, const GMP_Integer &g2) {
    GMP_Integer g;
    mpz_and(g.i, g1.i, g2.i);
    return g;
}

GMP_Integer& GMP_Integer::operator&=(const GMP_Integer &g) {
    return (*this = *this & g);
}

int operator&(const GMP_Integer &g1, unsigned int n) {
    int x = mpz_get_ui(g1.i);
    return x&n;
}

GMP_Integer& GMP_Integer::operator&=(unsigned int n) {
    return (*this = *this & n);
}

// -------------------------------------------------
// Comparison functions
// -------------------------------------------------

bool operator==(const GMP_Integer &g1, const GMP_Integer &g2) {
    return mpz_cmp(g1.i, g2.i) == 0;
}

bool operator!=(const GMP_Integer &g1, const GMP_Integer &g2) {
    return mpz_cmp(g1.i, g2.i) != 0;
}

bool operator!=(const GMP_Integer &g1, unsigned int n) {
    return mpz_cmp_ui(g1.i, n) != 0;
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
    int base = 0;
    if(out.flags() & ios::oct) base = 8;
    if(out.flags() & ios::dec) base = 10;
    if(out.flags() & ios::hex) base = 16;
    // if(base == 0) base = out.base();

        int size = mpz_sizeinbase(g.i, base) + 2;
    char *buf = new char[size];
    mpz_get_str(buf, base, g.i);
    out << buf;

    return out;
}

// -------------------------------------------------
// Miscellaneous functions
// -------------------------------------------------
int sizeinbase(const GMP_Integer &g, int base) {
    return mpz_sizeinbase(g.i, base);
}
