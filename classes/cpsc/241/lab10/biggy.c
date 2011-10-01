#include "biggy.h"
extern "C" {
#include "gmp.h"
}
#include "iostream.h"
#include <math.h>


 biggy::biggy(void){
		mpz_init(&data);
		}

 biggy::biggy(MP_INT x){
		mpz_init_set(&data,&x);
		}

 biggy::biggy(int x){
		mpz_init_set_ui(&data,x);
		}

 biggy::biggy(char* x){
		mpz_init_set_str(&data,x,10);
		}

 biggy::biggy(biggy& x){
		char* c = new char[70];		// !!!!!!!!!!! 
		mpz_get_str(c,10,&x.data);
		mpz_init_set_str(&data,c,10);
		}

 biggy biggy::operator+(biggy x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_add(&temp,&data,&x.data);
		return temp;
		}

 biggy biggy::operator+(int x)
		{
		MP_INT temp;
		mpz_init(&temp);
		if (x>0) 		mpz_add_ui(&temp,&data,x);
		else			mpz_sub_ui(&temp,&data,-x);
		return temp;
		}
 biggy biggy::operator-(biggy x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_sub(&temp,&data,&x.data);
		return temp;
		}

 biggy biggy::operator-(int x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_sub_ui(&temp,&data,x);
		return temp;
		}
 biggy biggy::operator*(int x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_mul_ui(&temp,&data,x);
		return temp;
		}
 biggy biggy::operator*(biggy x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_mul(&temp,&data,&x.data);
		return temp;
		}
 biggy biggy::operator/(int x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_div_ui(&temp,&data,x);
		return temp;
		}

 biggy biggy::operator^(int x)
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_pow_ui(&temp,&data,x);
		return temp;
		}

 int biggy::operator<(int x)
		{
		return (mpz_cmp_ui(&data,x) < 0 );
		}

 int biggy::operator!=(int x)
		{
		return (mpz_cmp_ui(&data,x) != 0 );
		}

 int biggy::operator%(int x)
		{
		MP_INT temp;
		mpz_init(&temp);

		char* c;
		c = new char[70];
		mpz_get_str(c,10,&temp);
		mpz_get_str(c,10,&data);
		mpz_mmod_ui(&temp,&data,x);
		mpz_get_str(c,10,&temp);
		return mpz_get_si(&temp); 
		}

 biggy biggy::abs()
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_abs(&temp,&data);
		return temp; 
		}
 biggy biggy::sqrt()
		{
		MP_INT temp;
		mpz_init(&temp);
		mpz_sqrt(&temp,&data);
		return temp;
		}


        
 void biggy::out(int CR){
		char* c;
		c = new char[70];
		mpz_get_str(c,10,&data);
		cout << c;
		if (CR)		cout << endl;
		}


 
biggy gcd(biggy v1, biggy v2) {
 		MP_INT temp;
		mpz_init(&temp);
		mpz_gcd(&temp,v1,v2);
		return temp;
		}

