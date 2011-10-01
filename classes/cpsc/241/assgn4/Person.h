

#ifndef PERSON__H
#define PERSON__H
  
#include <string>
#include <iostream.h>
#include <list.h>
#include <algo.h>

class Person{
public:
   Person() : name(""), gpa(0.0) {}
   Person(const string & S, float g) : name(S), gpa(g) {}
   ~Person() {}
   string value() { return name; }
   friend ostream & operator<<(ostream &, const Person &);
   friend istream& operator>>(istream& In, Person &);
   friend bool operator< (const Person &, const Person &);
   friend bool operator== (Person p, string name);
   string getname() const { return name; }
   float getgpa() const { return gpa; }
private:
   string name;
   float gpa;
};

#endif
