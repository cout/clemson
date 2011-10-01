#include <math.h>
#include <limits.h>

/*
 * Rootfinder:
 *      J. Wegstein
 *      National Bureau of Standards, Washington 25, D. C.
 *
 *      This procedure computes a value of g=x satis-
 *      fying the equation x = f(x). The procedure calling
 *      statement gives the function, an initial approxi-
 *      mation a=/= 0 to the root, and a tolerance
 *      parameter Epsilon for determining the number of sig-
 *      nificant figures in the solution. This accelerated
 *      iteration or secant method is described by the
 *      author in _Communications_, June, 1958.;
 *
 *
 * Implemented by:
 *      Jose R. Valverde
 *      European Bioinformatics Institute.
 *      Jose.Valverde@ebi.ac.uk                 (currently)
 *
 *      JRValverde@enlil.iib.uk                 (oldies, still valid)
 *      JRamon@mvax.fmed.uam.es
 *
 * Disclaimer:
 *      I made this out of my wits. The European Bioinformatics Institute
 *      does not guarantee or otherwise support or accept any liability
 *      in any manner whatsoever (nor explicit or implicit or ever) with
 *      respect to this software.
 *
 * Jose R. Valverde. 2 February 1995
 */

/* Modified by Paul Brannan for compatibility with test functions
 * and to exit if the number of iterations is greater than 30000
 */

double secant(double a, double dummy, double (*f)(double), double epsilon)
{
    /* Root */
    double b, c, d, e,g;
    int i;

    b = a;
    c = (*f)(b);
    g = c;
    if (c == a)
        return (g);
    d = a;
    b = c;
    e = c;
    for (i=0; i<30000; i++) {          /* Hob: */
        c = (*f)(b);
        g = (d * c - b * e) / (c - e - b + d);
        if (fabs((g - b)/g) <= epsilon)
            return (g);
        e = c;
        d = b;
        b = g;
    }
    return(g);
}
