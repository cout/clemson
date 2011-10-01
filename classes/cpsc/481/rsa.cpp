#include <iostream.h>
#include <limits.h>
#include <stdlib.h>
#include "rsa.h"

// this is a simple test for whether our primes are really prime
// yes, this is cheating a bit, since we already know what the message is
bool RSA::isgood(const RSA_Int& keyp, const RSA_Int& keyq,
                   const RSA_Int& pmqm) {
	// generate a public key
	for(RSA_Int i = pmqm + 1; i < modulus_n; ++i) {
		if(gcd(i, pmqm) == 1) {
			publickey = i;
			break;
		}
	}
	// and a private key
	privatekey = publickey.minverse(pmqm);

	// now encrypt "hello, world"
	RSA_Str s("hello, world");
	encrypt(s);
	decrypt(s);
	if(s == "hello, world") return true; else return false;
}

void RSA::genkeys(RSA_Int &keyp, RSA_Int &keyq,
                  const RSA_Int &ekey, int ks) {
	RSA_Int i, pmqm, maxint;

	srandom(time(NULL));
	if(keyp == 0 && keyq == 0) {
		do {
			// We dont have a set of primes, so generate them
			// There seems to be a problem with the probably prime
			// algorithm, so we have implemented a cheap check below
			// Note that if we have more than one limb, we risk
			// not getting a prime; this is because the primality
			// check is expensive, and so we cut corners.
			do {
				keyp.rand(1);
			} while(!keyp.isprime());
			do {
				keyq.rand(1);
			} while(!keyq.isprime());

			// pmqm is phi(modulus_n)
			modulus_n = keyp * keyq;
			pmqm = (keyp - 1) * (keyq - 1);

		} while(!isgood(keyp, keyq, pmqm));
	} else {
		// pmqm is phi(modulus_n)
		modulus_n = keyp * keyq;
		pmqm = (keyp - 1) * (keyq - 1);
	}

	// First find the public key
	if(ekey == 0) {
		// Create a public key since we weren't given one
		for(;;) {
			i.rand(100);
			i = i % modulus_n;
			if(gcd(i, pmqm) == 1) {
				publickey = i;
				break;
			}
		}
		// what if i >= modulus_n now?
	} else {
		publickey = ekey;
	}

	// Now find the private key
/*	for(i = 4; i < modulus_n; ++i) {
		if(mulmod(publickey, i, pmqm) == 1) {
			privatekey = i;
			break;
		}
	}*/
	privatekey = publickey.minverse(pmqm);
	if(!(mulmod(publickey, privatekey, pmqm) == 1)) {
		cout << "Uh oh, the keys don't fit! "
		     << "(" << mulmod(publickey, i, pmqm) << ")" << endl;
	}
}

void RSA::encrypt(RSA_Str& msg) {
	RSA_Int m, n, o;
	int i, len=msg.length();
	unsigned int c;

	// extract an integer from the message
	for(i = 0; i < len; i++) {
		c = (unsigned)msg[i] % 0x100;
		m *= 0x100;
		m += c;
	}

	// encrypt the extracted integer
	for(; !(m==0); ) {
		n = m % modulus_n;
		n.expmod(publickey, modulus_n);
		o *= modulus_n;
		o += n;
		m /= modulus_n;
	}

	// now put the encrypted integer back into the message
	for(i = 0; i < len; i++) {
		msg[i] = o % 0x100;
		o /= 0x100;
	}
	msg += o % 0x100;
}

void RSA::decrypt(RSA_Str& msg) {
	RSA_Int m, n, o;
	int i, len=msg.length();
	unsigned int c;

	// now put the encrypted integer back into the message
	// note that we must reverse the order here
	for(i = len - 1; i >= 0; i--) {
		c = (unsigned)msg[i] % 0x100;
		m *= 0x100;
		m += c;
	}

	// decrypt the extracted integer
	for(; !(m==0);) {
		n = m % modulus_n;
		n.expmod(privatekey, modulus_n);
		o *= modulus_n;
		o += n;
		m /= modulus_n;
	}

	// extract an integer from the message
	// note that we must reverse the order here
	o *= 0x100;
	for(i = len-1; i >= 0; i--) {
		msg[i] = o % 0x100;
		o /= 0x100;
	}
	msg.erase(len-1, len-1);
}

