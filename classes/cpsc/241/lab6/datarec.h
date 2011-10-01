//
// Specification for class datarec.
//
// This class defines data records with the components:
//     name - a string (name without blanks)
//     major - an int (3-digit major code)
//     email - a string (email address)

#ifndef __DATAREC__

#define __DATAREC__ __DATAREC__

#include <stringClass.h>             // string ADT

class datarec {

public:

  // Constructor:

    datarec();

  // Destructor:
 
    ~datarec();

  // Accessor functions:

    const string & name() const;  // return the name component.
    int            major() const; // return the major component.
    const string & email() const; // return the email component.

private:

    friend istream & operator >> (istream &, datarec &);

  // Data components:

    string NameComponent;
    int    MajorComponent;
    string EmailComponent;
};

// Non member functions:

   ostream &  datarecHeading (ostream &);
        // Print a heading for ostream output of datarecs.

  ostream & operator << (ostream &, const datarec &);

  int operator < (const datarec &, const datarec &);

  int operator == (const datarec &, const datarec &);

#endif
