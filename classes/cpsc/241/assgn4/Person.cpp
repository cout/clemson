#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <limits.h>

#include "Person.h"

istream& operator>>(istream& In, Person & S){
   getline(In, S.name);
   In >> S.gpa;
   In.ignore(INT_MAX, '\n');
   return In;
}

ostream & operator<<(ostream & Out, const Person & P) {
   Out << P.name << "\t" << setw(6);
   Out.precision(1);
   Out.setf(ios::fixed, ios::floatfield);
   Out << P.gpa;
   return Out;
}

bool operator< (const Person &p1, const Person &p2) {
	if(p1.name == p2.name) return p1.gpa < p2.gpa;
	return p1.name < p2.name;
}

bool operator== (Person p, string name) {
	unsigned int j;
	for(j = 0; j < p.name.length(); j++) p.name[j] = toupper(p.name[j]);
	for(j = 0; j < name.length(); j++) name[j] = toupper(name[j]);
	return p.name.find(name) != string::npos;
}
