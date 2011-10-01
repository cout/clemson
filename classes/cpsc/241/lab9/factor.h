/* factor.h
Data Structures and prototypes for lab 8 */

extern int primes[10000]; /*contains the primes.  primes[0]=2, primes[1]=3,
							primes[2]=5.      */

class factors {
	private:
	   int degrees[10000];  /*degrees[i] contains the degree of
			        prime[i] in this factorization.            */
           int sign; /*contains the sign of this number */
	public:
                int getsign(void);
                int setsign(int s);
                void setdegree(int i,int val); /*Set degree[i] to value
                                                 val */
                int getdegree(int i); /*returns  degree[i] */
		int value(void); /* returns the value of the number factored.
                                  by multiplying all factors*/
		/* Additional member functions for use in Lab 10:
		uncomment when doing that lab:
		factors factors::operator *(const factors f);
		int isquare(void);
		factors sqrt(void);
		*/
};

void initprimes();  /* Use the sieve of aristothenes to initialize the primes
							  array. */
factors factorize(int n);  /* factorize the integer n using the
                          availible primes.  If this is not possible,
                         return a list full of -1's. */

