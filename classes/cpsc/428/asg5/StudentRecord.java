// Paul Brannan
// CPSC 428 Assignment 6
// April 19, 1999

import java.util.*;

// StudentRecord class, represents information related to a student,
// for storage in the StudentDatabase.
public class StudentRecord {

	// Instance variables
	private Name name;
	private int major;
	private String className;
	private String hometown;

	// Default constructor
	public StudentRecord(Name name, int major, String className,
		String town) {

		this.name = name;
		this.major = major;
		this.className = className;
		this.hometown = town;
	}

	// Return the name field
	public Name name() {
		return name;
	}

	// Return the major field
	public int major() {
		return major;
	}

	// Return the class field -- we can't use class as the instance
	// variable here!
	public String className() {
		return className;
	}

	// Set the class to s
	public String setClass(String s) {
		return className = s;
	}

	// Return 0 if name equals r.name, > 0 if name > r.name, and < 0 if
	// name < r.name
	public int compareTo(StudentRecord r) {
		return name.compareTo(r.name);
	}

	// Return the string representation of StudentRecord
	public String toString() {
		return name.toString() + "\n" +
			"   major code: " + major + "\n" +
			"   class: " + className + "\n" +
			"   home town: " + hometown + "\n";
	}

	public static void main(String[] args) {
		// test name
		Name n1 = new Name("First", "Last");
		Name n2 = new Name("Weird", "Al");
		Name n3 = new Name("First", "Last");

		System.out.println("First: " + n1.first());
		System.out.println("Last: " + n1.last());

		// test comparions of name
		if(n1.equals(n2))
			System.out.println(n1 + " equals " + n2);
		if(n1.equals(n3))
			System.out.println(n1 + " equals " + n3);
		System.out.println(n1.compareTo(n2) + " (should be > 0)");
		System.out.println(n2.compareTo(n3) + " (should be < 0)");
		System.out.println(n3.compareTo(n1) + " (should be == 0)");

		// print a name
		System.out.println("A name: " + n2.toString());

		// test StudentRecord
		StudentRecord s1 = new StudentRecord(n1, 100, "sr", "townsville");
		StudentRecord s2 = new StudentRecord(n2, 200, "jr", "?");

		StudentRecord s3 = new StudentRecord(n3, 100, "so", "townsville");

		System.out.println(s1.name());
		System.out.println(s1.major());
		System.out.println(s1.className());

		// test comparisons of StudentRecord
		System.out.println(s1.compareTo(s2) + " (should be > 0)");
		System.out.println(s2.compareTo(s3) + " (should be < 0)");
		System.out.println(s3.compareTo(s1) + " (should be == 0)");

		// test setClass
		System.out.println(s1.name() + " has been demoted to a freshman.");
		s1.setClass("fr");

		// print a StudentRecord
		System.out.println(s1);
	}
}

