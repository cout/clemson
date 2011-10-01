// Paul Brannan
// CPSC 428 Assignment 6
// April 19, 1999

import java.io.*;
import java.util.*;

// StudentDatabase class, keeps a list of StudentRecords in memory as
// a volatile database
public class StudentDatabase {

	// Instance variables
	private ArrayList db = new ArrayList();

	// Default Constructor -- doesn't need to do anything
	public StudentDatabase() {
	}

	// add a record to the database
	public void addRecordFor(Name n, int major, String className,
		String town) {

		StudentRecord tmp, rec = new StudentRecord(n, major, className,
			town);
		ListIterator it;

		// If the list is empty, add to the beginning
		if(db.isEmpty()) {
			db.add(rec);
			return;
		}

		// If the record is smaller than the first element, then
		// insert at the beginning
		tmp = (StudentRecord)db.get(0);
		if(rec.compareTo(tmp) <= 0) {
			db.add(0, rec);
			return;
		}

		// Find the spot to insert
		for(it = db.listIterator(); it.hasNext(); ) {
			tmp = (StudentRecord)it.next();
			if(rec.compareTo(tmp) > 0) {
				// Add to the list and return
				it.add(rec);
				return;
			}
		}

		// If at the end of the list add to the end.
		db.add(rec);
		return;
	}

	// Return a list of the names in the database
	public ArrayList getNames() {
		ArrayList l = new ArrayList();

		for(Iterator it = db.iterator(); it.hasNext(); ) {
			StudentRecord rec = (StudentRecord)it.next();
			l.add(rec.name());
		}
		return l;
	}

	// Return the record associated with the given name; returns a
	// StudentRecord if successful, null otherwise
	public StudentRecord recordFor(Name n) {
		Iterator it;
		StudentRecord rec;

		// Search through the list for the record
		for(it = db.iterator(); it.hasNext(); ) {
			rec = (StudentRecord)it.next();
			if(rec.name().equals(n)) return rec;
		}

		// Not found -- return null
		return null;
	}

	// Return the major associated with the given name.
	// Returns 0 if unsuccessful.
	public int majorFor(Name n) {
		StudentRecord rec = recordFor(n);
		if(rec == null) return 0; else return rec.major();
	}

	// Return the class associated with the given name.
	// Returns null if unsuccessful.
	public String classFor(Name n) {
		StudentRecord rec = recordFor(n);
		if(rec == null) return null; else return rec.className();
	}

	// Set the class associated with name n to s.
	public boolean setClass(Name n, String s) {
		StudentRecord rec = recordFor(n);	// get the record
		if(rec == null) return false;		// if null return false
		rec.setClass(s);			// otherwise set
		return true;				// and return
	}

	// Return the concatenated string of all records, with a header
	public String toString() {
		String s = "          Student Records\n";
		Iterator it;
		StudentRecord rec;
		for(it = db.iterator(); it.hasNext(); ) {
			rec = (StudentRecord)it.next();
			s = s + rec.toString();
		}
		return s;
	}

	// Test code, from the assignment page.  Appears to be relatively
	// complete, except doesn't test setClass on a nonexistant name, and
	// doesn't test return values.  Added.
	public static void main(String[] args) {

		StudentDatabase db = new StudentDatabase();

		Name n1 = new Name("John", "Finkle");
		Name n2 = new Name("Jean", "Finkle");
		Name n3 = new Name("Adam", "Jones");

		System.out.println("Add to database:");
		System.out.println(n3 + ", 819, Senior, Charlotte, NC");
		System.out.println(n2 + ", 818, Sophomore, Atlanta, GA");
		System.out.println(n1 + ", 820, Freshman, Clemson, SC");
		System.out.println();

		db.addRecordFor(n3, 819, "Senior", "Charlotte, NC");
		db.addRecordFor(n2, 818, "Sophomore", "Atlanta, GA");
		db.addRecordFor(n1, 820, "Freshman", "Clemson, SC");

		System.out.println("The listing:\n");
		System.out.println(db);
		System.out.println();

		System.out.println("Major for " + n1 + ": " + db.majorFor(n1));
		System.out.println("Class for " + n2 + ": " + db.classFor(n2));
		System.out.println();

		Name n4 = new Name("Jan", "Finkle");

		System.out.println("Major for " + n4 + ": " + db.majorFor(n4));
		System.out.println("Class for " + n4 + ": " + db.classFor(n4));
		System.out.println();

		System.out.println(db.setClass(n2, "Junior"));
		System.out.println("Setting class for " + n2 + " to junior...");
		System.out.println("The listing:\n");
		System.out.println(db);

		System.out.println(db.setClass(n4, "Junior"));
	}
}

