
#include "List.h"

ostream& operator<<(ostream& out, const List& my_list) {
   if (my_list.people.size() == 0) {
      out << "The phone list is empty." << endl;
      return out;
   }
   list<Person>::const_iterator ptr = my_list.people.begin();
   out << "name" << "\t\t" << "grade" << endl;
   for ( ; ptr != my_list.people.end(); ++ptr) {
      out << *ptr << endl;
   }
   out << endl;
   return out;
}


void List::readData() {
   ifstream fin("phone.dat");
   if (!fin) {
      cout << "can't open input file";
      return;
   }
   cout << "reading from phone.dat ..." << endl;
   char ch;
   Person *this_person;
   while ( 1 ) {
      this_person = new Person;
      fin >> *this_person;
      fin.get(ch);
      if ( fin.eof() ) {
         fin.close();
         delete this_person;
         break; 
      }
      insert(*this_person);
   }    
}

void List::writeData() {
	ofstream fout("phone.dat");
	if(!fout) {
		cout << "can't open output file";
		return;
	}
	cout << "writing to phone.dat ..." << endl;
	list<Person>::const_iterator ptr = people.begin();
	for ( ; ptr != people.end(); ++ptr) {
		fout << (*ptr).getname() << endl;
		fout << (*ptr).getgpa() << endl;
	}
	fout << endl;
}

bool List::insert(const Person &p) {
	people.push_back(p);
	sort();
	return true;
}

void List::sort() {
	people.sort();
}

bool List::find(string name) {
   if (people.size() == 0) return 0;
   find_ptr = people.begin();
   for ( ; find_ptr != people.end(); ++find_ptr) {
       if(*find_ptr == name) return true;
   }
   return false;
}

bool List::find(string name, Person& p) {
   if (people.size() == 0) return 0;
   find_ptr = people.begin();
   for ( ; find_ptr != people.end(); ++find_ptr) {
       if(*find_ptr == name) {
           p = *find_ptr;
           return true;
       }
   }
   return false;
}

bool List::findnext(string name) {
   if (people.size() == 0) return 0;
   for (++find_ptr; find_ptr != people.end(); ++find_ptr) {
       if(*find_ptr == name) return true;
   }
   return false;
}

bool List::findnext(string name, Person& p) {
   if (people.size() == 0) return 0;
   for (++find_ptr; find_ptr != people.end(); ++find_ptr) {
       if(*find_ptr == name) {
           p = *find_ptr;
           return true;
       }
   }
   return false;
}

void List::del(list<Person>::iterator ptr) {
	people.erase(ptr);
}
