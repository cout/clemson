#include "IteratorString.h"

ostream& operator<<(ostream& Out, const IteratorString& It){
   Out	<< "Str = " << It.Str << endl
	<< "delim = " << It.delim << endl
	<< "index = " << It.index << endl;
   return Out;
}

IteratorString& IteratorString::operator=(const IteratorString & it) { 
   Str = it.Str; 
   index = it.index;
   return *this;
}

IteratorString& IteratorString::operator=(const string& S) { 
   Str = S; 
   index = 0;
   return *this;
}

string IteratorString::operator () (){
   char buf[80];
   int bufIndex = 0;
   while ((index < Str.length() && !okay(Str[index])) ) index++;
   unsigned int tempIndex = index;
   while ( tempIndex < Str.length() && okay(Str[tempIndex]) )
        buf[bufIndex++] = Str[tempIndex++];
   buf[bufIndex] = '\0';
   string temp(buf);
   return temp;
}

void IteratorString::init(char* d){
   int len = strlen(d);
   delim = new char[len+1];
   strcpy(delim, d);
   delim[len] = '\0';
}

IteratorString::IteratorString(char* d){
   init(d);
   index = 0; 
}

IteratorString::IteratorString(const string& S, char* d) {
   Str = S;
   init(d);
   index = 0; 
}

IteratorString::IteratorString(const IteratorString & I) {
   init(I.delim);
   index = I.index;
   Str = I.Str;
}

bool IteratorString::okay(char ch) const {
   for (unsigned int i = 0; i < strlen(delim); i++)
      if (delim[i] == ch) return false;
   return true;
}

void IteratorString::operator++ () {
   while ((index < Str.length() && !okay(Str[index])) ) index++;
   while ( index < Str.length() && okay(Str[index]) ) index++; 
   while ((index < Str.length() && !okay(Str[index])) 
          && (Str[index] != '=') ) index++;
}
