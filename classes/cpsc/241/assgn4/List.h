#include "Person.h"
#include <iostream.h>
#include <fstream.h>

class List{
public:
   List()	{}
   void readData();
   void writeData();
   bool insert(const Person&);
   void sort();
   bool find(string name);
   bool find(string name, Person& p);
   bool findnext(string name);
   bool findnext(string name, Person& p);
   void del(list<Person>::iterator ptr);
   void del() {del(find_ptr);}
   friend ostream& operator<<(ostream&, const List&);
private:
   list<Person> people;
   list<Person>::iterator find_ptr;
};
