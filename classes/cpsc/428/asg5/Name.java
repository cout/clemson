// Paul Brannan
// CPSC 428 Assignment 6
// April 19, 1999

// The name class, I know it's a repeat of what's on the web
public class Name {

        // Instance variables
        protected String first;
        protected String last;

        // Default constructor
        Name(String first, String last) {
                this.first = first;
                this.last = last;
        }

        // Return the first name
        public String first() {
                return first;
        }

        // Return the last name
        public String last() {
                return last;
	}

        // Return 0 if this==n, a number > 0 if this > n, or a number
        // < 0 if this < n.
        public int compareTo(Name n) {
                int tmp = this.last.compareTo(n.last);
                if(tmp != 0) return tmp;
                return this.first.compareTo(n.first);
        }

        // Return true if this==n, false otherwise
        public boolean equals(Name n) {
                return compareTo(n) == 0;
        }

        // Return a string representation of Name
        public String toString() {
                return first + " " + last;
        }
}
