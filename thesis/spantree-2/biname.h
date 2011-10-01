#ifndef BINAME_H
#define BINAME_H

#include <list>

// Use gmpwrapper for larger trees; intwrapper works for trees up to order 32.
// (on the i386, anyhow)
// #include "gmpwrapper.h"
#include "intwrapper.h"

using namespace std;

class Biname;
typedef list<Biname> Biname_List;

// The biname class
class Biname {
public:
    // Constructors
    Biname() {i = 0;}
    Biname(unsigned int j) {do_copy(j);}
    Biname(const Biname& b) {i = b.i;}

    // Comparison functions
    friend bool operator==(const Biname& b1, const Biname& b2);
    friend bool operator<(const Biname& b1, const Biname& b2);
    friend bool operator>(const Biname& b1, const Biname& b2);

    // Concatenation
    Biname& operator+=(const Biname& b);

    // Assignment functions
    Biname& operator=(const Biname& b);
    Biname& operator=(unsigned int j) {do_copy(j); return *this;}

    // Miscellaneous functions
    Biname& smerg(Biname_List & l);

    // Output functions
    friend ostream& operator<< (ostream& out, const Biname& b);

private:
    GMP_Integer i;

    void do_copy(unsigned int j);
};

#endif
