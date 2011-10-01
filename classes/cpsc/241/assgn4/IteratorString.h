
#ifndef ITERATORString__H
#define ITERATORString__H

#include <string>
#include <iostream.h>

class IteratorString{
public:
   IteratorString() 	{}
   IteratorString(char* d);
   IteratorString(const string& S, char* d);
   IteratorString(const IteratorString &);
   ~IteratorString() 	{ delete []delim; }
   string restOfLine()	{ return Str.substr(index, Str.length()); }
   string theString()	{ return Str; }
   void operator++();
   inline void rewind() { index = 0; }
   string operator () ();
   IteratorString& operator=(const string& S);
   IteratorString& operator=(const IteratorString&);
   inline bool atEnd() const { return ( index >= Str.length() ); }
   friend ostream& operator<<(ostream& Out, const IteratorString& It); 

private:
   unsigned int index;
   string  Str;
   char* delim;
   bool okay(char ch) const;
   void IteratorString::init(char* d);
};

#endif
