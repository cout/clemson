//
// Implementation for class datarec.
//
// This class defines data records with the components:
//     name - a string (name without blanks)
//     major - an int (3-digit major code)
//     email - a string (email address)

#include "datarec.h"     // Specification for class datarec

  // Constructor:

    datarec::datarec() { }

  // Destructor:
 
    datarec::~datarec() { }

  // Accessor functions:

    const string & datarec::name() const { // return the name component.
      return NameComponent;
    }

    int            datarec::major() const { // return the major component.
      return MajorComponent;
    }

    const string & datarec::email() const { // return the email component.
      return EmailComponent;
    }

  // Stream I/O:

    static const int nameField = 20;  // size of name field for printing
    static const int majorField = 5;  // size of major field for printing

    static ostream & blanks (int n, ostream & out){
       // Internal function to print n blanks.
       for (int i=0; i<n; i++)
         out << " ";
       return out;
    }

    ostream & datarecHeading (ostream & out) {
      // Print a heading for stream output of datarecs.
      out << "   Name";
      blanks(nameField - 7 + 3, out) << "Major";
      out << "  **  ";
      return blanks(majorField - 5 + 4, out) << "Email Address";
    }

    static ostream & printInt (int n, int w, ostream & out) {
       // Internal function to print n right justified in a field of width w.
       // n is assumed to be nonnegative.
       int size = 1;  // number of characters in n
       int rem  = n;  // for calculating the size of n
       for (; rem>10; size++) rem /= 10;       // Calculate size.
       if (w > size) blanks(w-size, out);           // Print leading blanks.
       return out << n;
     }

    ostream & operator << (ostream & out, const datarec & D) {
      out << D.name();
      if (D.name().length() < nameField)
             blanks(nameField - D.name().length() + 2, out);
      printInt(  D.major(), majorField, out);
      return out <<  "     " << D.email();
    }

    istream & operator >> (istream & in, datarec & D) {
      if (in >> D.NameComponent)
        return in >> D.MajorComponent >> D.EmailComponent;
      else return in;
    }

    int operator < (const datarec & L, const datarec & R) {
      return L.name() < R.name();
    }

    int operator == (const datarec & L, const datarec & R) {
      return L.name() == R.name();
    }
