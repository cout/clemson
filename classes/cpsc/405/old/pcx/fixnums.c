#include <assert.h>
#include <string.h>
#include <math.h>
#include "fixnums.h"

/* This should fix our big-endian/little-endian problem */
unsigned int fixint(unsigned int i) {
        unsigned char x[4], c1, c2, c3, c4;
        unsigned int y;
        memcpy(x, &i, sizeof(x));
        c1 = x[0];
        c2 = x[1];
        c3 = x[2];
        c4 = x[3];
        y = c1 + 256 * (c2 + 256 * (c3 + 256 * c4));
#ifdef WIN32
        assert(i == y);
#endif
        return y;
}

/* fix float fixes floats.  Input is a x86 float, output is a
 * machine-specific float.  This function does not handle
 * subnormals, infinity, or NaN.
 */
float fixfloat(float f) {
        float g = 0.0;
        int i, m, s, e;

        /* First, fix the endianness */
        i = fixint(*(unsigned int *)&f);

        /* Divide i into parts */
        m = i & 0x007FFFFF;
        s = (i & 0x80000000) >> 31;
        e = (i & 0x7F800000) >> 23;

	i = (s << 31) | (e << 23) | m;
	g = *(float *)&i;

#ifdef WIN32
        assert(f == g);
#endif
        return g;
}
