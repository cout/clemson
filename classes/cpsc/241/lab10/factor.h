/* factor.h
Data Structures and prototypes for lab 8 */

#include "gmp.h"
#include "biggy.h"

extern int primes[1000]; /*contains the primes.  primes[0]=2, primes[1]=3,
							primes[2]=5.      */
class factors {
	private:
	   int degrees[1000];  /*degrees[i] contains the degree of
			        prime[i] in this factorization.            */
           int sign; /*contains the sign of this number */
	public:
                int getsign(void);
                int setsign(int s);
                void setdegree(int i,int val); /*Set degree[i] to value
                                                 val */
                int getdegree(int i); /*returns  degree[i] */
		biggy value(void); /* returns the value of the number factored.
                                  by multiplying all factors*/
		uncomment when doing that lab:
		factors factors::operator *(const factors f);
		int issquare(void);
		/*factors sqrt(void);*/
};

void initprimes();  /* Use the sieve of aristothenes to initialize the primes
							  array. */
factors factorize(biggy n);  /* factorize the biggy n using the
                          availible primes.  If this is not possible,
                         return a list full of -1's. */

