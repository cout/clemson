#ifndef INTWRAPPER_H
#define INTWRAPPER_H

// This lets us pretend that we are using large integers when we are really
// using regular integers
#define GMP_Integer int

inline int sizeinbase(unsigned int i, unsigned int base) {
    if(i == 0) return 1;
    int count = 1;
    for(; i != 1; i /= base) count++;
    return count;
}

#endif
