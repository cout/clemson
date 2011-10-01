#ifndef PSTRING_H
#define PSTRING_H

#include <vector>

class PString {
public:
	PString(char *s="") {
		for(; *s != 0; s++) v.push_back(*s);
	}
	~Pstring() {
		delete v;
	}
	PString& operator=(const char *s);
	PString& operator=(const PString& s);
	PString& operator+=(const char *s);
	PString& operator+=(const PString& s);
	char& operator[](unsigned int i);
	const char& operator[] (unsigned int i) const;
	
private:
	vector<char> v;
#endif

