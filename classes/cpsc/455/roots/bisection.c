#include <math.h>

#define sign(x) fabs(x) / x

int i;

double bs (double xl, double xr, double func(double), double tolerance) {
        double d = (xl + xr) / 2, f = (*func)(d);
        if ((fabs(f) <= tolerance) || (++i >= 30000)) return d;
        if (sign(f) == sign((*func)(xl)))
                return bs(d, xr, func, tolerance);
        return bs(xl, d, func, tolerance);
}

double bisection(double xl, double xr, double func(double), double tolerance) {
        i = 0;
        return bs(xl, xr, func, tolerance);
}

