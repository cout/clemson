#ifndef RSA_H
#define RSA_H

#include <string>
#include "gmpwrapper.h"

#define RSA_Int GMP_Integer
#define RSA_Str string

class RSA {
public:
	RSA() {
		RSA_Int keyp((unsigned)0), keyq((unsigned)0);
		genkeys(keyp, keyq, (unsigned)0, (unsigned)0);
	}
	RSA(RSA_Int& p, RSA_Int& q, const RSA_Int& ek = (unsigned)0,
	    unsigned int ks = 0) {
		genkeys(p, q, ek, ks);
	}

	void encrypt(RSA_Str& string);
	void decrypt(RSA_Str& string);

//private:
	void genkeys(RSA_Int &keyp, RSA_Int &keyq,
	             const RSA_Int &ekey, int ks);

	RSA_Int modulus_n;
	RSA_Int publickey, privatekey;
	bool isgood(const RSA_Int&, const RSA_Int&, const RSA_Int&);
};

#endif
