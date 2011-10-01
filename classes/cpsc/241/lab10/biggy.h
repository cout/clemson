
#include "gmp.h"
#include "iostream.h"
#include <math.h>




class biggy {
   public:
	MP_INT data; 

   public:

	biggy(void);
	biggy(MP_INT x);
	biggy(int x);
	biggy(char* x);
	biggy(biggy& x);
        biggy biggy::operator+(biggy x);
        biggy biggy::operator+(int x);
        biggy biggy::operator-(biggy x);
        biggy biggy::operator-(int x);
        biggy biggy::operator*(int x);
        biggy biggy::operator*(biggy x);
        biggy biggy::operator/(int x);
        biggy biggy::operator^(int x);
        biggy biggy::sqrt();
	biggy biggy::abs();
        int   biggy::operator<(int x);
        int   biggy::operator!=(int x);
        int   biggy::operator%(int x);
	void biggy::out(int);
};

biggy gcd (biggy v1, biggy v2);


