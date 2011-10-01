#include <iostream.h>
#include <limits.h>
#include "rsa.h"

void strprint(const RSA_Str& s) {
	int prev=1;
	for(unsigned int i = 0; i < s.length(); i++) {
		if((unsigned char)s[i] >= ' ' && (unsigned char)s[i] <= '~') {
			cout << s[i];
			prev = 0;
		}
		else {
			if(!prev) cout << ' ';
			// The double-cast is necessary for formatting
			cout << "["
			     << hex << (unsigned int)(unsigned char)s[i]
			     << "] ";
			prev = 1;
		}
	}
}

void do_test(RSA_Int keyp, RSA_Int keyq, RSA_Int ekey, char *str) {
	RSA m(keyp, keyq, ekey);
	RSA_Str s(str);

	cout << "keyp: 0x" << keyp << " keyq: 0x" << keyq << endl;
	cout << "Public key: 0x" << m.publickey % INT_MAX << endl;
	cout << "Private key: 0x" << m.privatekey % INT_MAX << endl;
	cout << "Modulus: 0x" << m.modulus_n % INT_MAX << endl;

	// Make sure we can encrypt the entire alphabet
	int flag=0;
	int max = (m.modulus_n > 100)?100:(m.modulus_n%100);
	for(int j = 2; j < max; j++) {
		if(!(expmod(expmod(j, m.publickey, m.modulus_n),
 		                      m.privatekey, m.modulus_n) == j))
			flag = j;
	}
	if(flag) cout << "Something's wrong!  The keys don't work! ("
	              << flag << ")" << endl;

	cout << "h: 0x" << expmod('h', m.publickey, m.modulus_n) % INT_MAX << endl;
	cout << "Original message: ";
	strprint(s);
	cout << endl;

	m.encrypt(s);
	cout << "Encrypted message: ";
	strprint(s);
	cout << endl;

	m.decrypt(s);
	cout << "Decrypted message: ";
	strprint(s);
	cout << endl;
}

main() {
	cout << "Using primes for key generation:" << endl;
	do_test(43, 59, 13, "hello, world");
	cout << endl;

	cout << "Using non-primes for key generation:" << endl;
	do_test((unsigned)100, (unsigned)200, (unsigned)13, "hello, world");
	cout << endl;

	cout << "Generating our own primes:" << endl;
	cout << "(perhaps you would like some coffee?)" << endl;
	do_test((unsigned)0, (unsigned)0, (unsigned)0, "hello, world");
	cout << endl;

	cout << "Trying some large primes:" << endl;
	cout << "(these won't match, as the message is too short for the key.)"
	     << endl;
	do_test("622288097498926496141095869268883999563096063592498055290461",
		"511704374946917490638851104912462284144240813125071454126151",
		(unsigned)0, "hello, world");
	return 0;
}
