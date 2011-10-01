#include "pstring.h"

PString::operator=(const char *s) {
	v.clear();
	for(; *s != 0; s++) v.push_back(*s);
}

PString::operator=(const PString& s) {
}

PString::operator+=(const char *s) {
	for(; *s != 0; s++) v.push_back(*s);
}

PString::operator+=(const PString& s) {
}

char& operator[](unsigned int i) {
	return v[i];
}

const char& operator[] (unsigned int i) const {
	return v[i];
}
