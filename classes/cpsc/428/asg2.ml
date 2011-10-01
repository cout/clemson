(* CPSC 428 Spring 1999 Assignment 2                                    *)
(* Paul Brannan                                                         *)
(* This program uses some basic ML techniques for                       *)
(* storing and printing out a list of records.                          *)
(* Based on the example program from the class notes                    *)

use "/home/turner/public/cs428/ml/simpleio.ml";

(*----------------------------------------------------------------------*)
(* Type definitions:                                                    *)
(*----------------------------------------------------------------------*)

(* A preson's name, consisting of first and last names *)
type nameType = string*string;

(* A single student record, consisting of name, major code, class, and *)
(* home town *)
type studentRecord = nameType*int*string*string;

(* A BST of student records *)
datatype studentRecordBST =
   Empty |
   Node of studentRecord*studentRecordBST*studentRecordBST;

(*----------------------------------------------------------------------*)
(* Accessing and other utility functions:                               *)
(*----------------------------------------------------------------------*)

fun firstName(n: nameType) = #1(n);  (* Return the first name of a name. *)
fun lastName(n: nameType) = #2(n);   (* Return the last name of a name. *)

fun name(r: studentRecord) = #1(r);  (* Return the name from a record. *)
fun major(r: studentRecord) = #2(r); (* Return the major code from a record. *)
fun class(r: studentRecord) = #3(r); (* Return the class from a record. *)
fun town(r: studentRecord) = #4(r);  (* Return the hometown from a record. *)

exception Empty_btree;
fun root(Empty) = raise Empty_btree | root(Node(x, right, left)) = x;
fun left(Empty) = raise Empty_btree | left(Node(x, left, right)) = left;
fun right(Empty) = raise Empty_btree | right(Node(x, left, right)) = right;

(* nameLT compares two names and returns true if n1 < n2, false otherwise *)
fun nameLT(n1: nameType, n2: nameType) =
   if lastName(n1) < lastName(n2) then true
   else if lastName(n1) > lastName(n2) then false
      else if firstName(n1) < firstName(n2) then true
      else false;

(* nameEQ compares two names and returns true if n1 = n2, false otherwise *)
fun nameEQ(n1: nameType, n2: nameType) =
   if lastName(n1) = lastName(n2) andalso firstName(n1) = firstName(n2) then
      true
   else
      false;

(* recordLT compares two records and returns true if r1 < r2, false otherwise *)
fun recordLT(r1: studentRecord, r2: studentRecord) =
   if nameLT(name(r1), name(r2)) then true
   else if nameLT(name(r2), name(r1)) then false
      else if major(r1) < major(r2) then true
         else if major(r1) > major(r2) then false
            else if class(r1) < class(r2) then true
               else if class(r1) > class(r2) then false
                  else if town(r1) < town(r2) then true
                  else false;

(*----------------------------------------------------------------------*)
(* Modification functions for studentRecordBST:                         *)
(*----------------------------------------------------------------------*)

(* insertRecord inserts a record into a sorted list of studentRecords *)
fun insertRecord(Empty, r: studentRecord) = Node(r, Empty, Empty)
|   insertRecord(Node(root, left, right), r) =
       if recordLT(r, root) then Node(root, insertRecord(left, r), right)
       else Node(root, left, insertRecord(right, r));

(*----------------------------------------------------------------------*)
(* Printing-related functions for studentRecordBST:                     *)
(*----------------------------------------------------------------------*)

(* nameString returns a concanated string of first/last names for printing *)
fun nameString(n: nameType) =
   firstName(n) ^ " " ^ lastName(n);

(* majorString returns a string for the major code *)
fun majorString(m: int) =
   Int.toString(m);

(* printStudentRecord prints a single formatted student record *)
fun printStudentRecord(r: studentRecord) = (
   print(nameString(name(r)) ^ "\n");
   print("   major: " ^ majorString(major(r)) ^ "\n");
   print("   class: " ^ class(r) ^ "\n");
   print("   hometown: " ^ town(r) ^ "\n");
   print("\n")
);

(* printStudentRecordBST prints a list of student records using recursion *)
fun printStudentRecordBST(Empty) = ()
|   printStudentRecordBST(L: studentRecordBST) = (
       printStudentRecordBST(left(L));
       printStudentRecord(root(L));
       printStudentRecordBST(right(L))
);
   
(* printStudentRecords prints a header and a list of student records *)
fun printStudentRecords(L: studentRecordBST) = (
   print("\n     Student Records\n\n");
   printStudentRecordBST(L);
   L
);

(*----------------------------------------------------------------------*)
(* Search functions for studentRecordBST:                               *)
(*----------------------------------------------------------------------*)

exception NotFound of nameType;

(* searchStudentRecordBST searches for a given entry.  Returns true if *)
(* found, false otherwise. *)
fun searchStudentRecordBST(Empty, n: nameType) =
   raise NotFound(n)
|   searchStudentRecordBST(Node(root, left, right), n) =
      if nameEQ(n, name(root)) then root
      else if nameLT(n, name(root)) then searchStudentRecordBST(left, n)
         else searchStudentRecordBST(right, n);

(*----------------------------------------------------------------------*)
(* Input/Output functions for studentRecordBST:                         *)
(*----------------------------------------------------------------------*)

(* writeNameTo writes a name to a file *)
fun writeNameTo(F: outstream, n: nameType) = (
   outputLine(F, firstName(n));
   outputLine(F, lastName(n))
);

(* writeMajorTo writes a major code to a file *)
fun writeMajorTo(F: outstream, m: int) =
   outputLine(F, Int.toString(m));

(* writeClassTo writes a class string to a file *)
fun writeClassTo(F: outstream, c: string) =
   outputLine(F, c);

(* writeTownTo writes a hometown string to a file *)
fun writeTownTo(F: outstream, t: string) =
   outputLine(F, t);

(* writeStudentRecordTo writes a studentRecord to a file *)
fun writeStudentRecordTo(F: outstream, r: studentRecord) = (
   writeNameTo(F, name(r));
   writeMajorTo(F, major(r));
   writeClassTo(F, class(r));
   writeTownTo(F, town(r))
);

(* writeStudentRecordBSTTo writes a studentRecordBST to a file *)
fun writeStudentRecordBSTTo(F: outstream, Empty) = F
|   writeStudentRecordBSTTo(F: outstream, L: studentRecordBST) = (
       writeStudentRecordTo(F, root(L));
       writeStudentRecordBSTTo(F, left(L));
       writeStudentRecordBSTTo(F, right(L))
);

(* nameFrom reads a name from a file *)
fun nameFrom(F: instream): nameType =
   (inputLine(F), inputLine(F));

(* majorFrom reads a major code from a file *)
fun majorFrom(F: instream): int =
   let
      val major = inputInteger(F)
   in
      (skipWhiteSpace(F); major)
   end;

(* classFrom reads a class string from a file *)
fun classFrom(F: instream): string = 
   (inputLine(F));

(* townFrom reads a hometown string from a file *)
fun townFrom(F: instream): string =
   (inputLine(F));

(* studentRecordFrom reads a studentRecord from a file *)
fun studentRecordFrom(F:instream): studentRecord =
   (nameFrom(F), majorFrom(F), classFrom(F), townFrom(F));

(* studentRecordBSTFrom reads a file into a studentRecordBST *)
fun studentRecordBSTFrom(F: instream, L: studentRecordBST): studentRecordBST =
      studentRecordBSTFrom(F, insertRecord(L, studentRecordFrom(F)))
   handle EndOfFile => L;

(* studentRecordsFrom reads a studentRecordBSTfrom a file *)
fun studentRecordsFrom(F: instream): studentRecordBST =
   studentRecordBSTFrom(F, Empty);

(*------------------------------------------------------------------------*)
(* Dialog functions                                                       *)
(*------------------------------------------------------------------------*)

(* getWord prints prompt s and return the next input word.*)
fun getWord(s: string) =
   (outputWord(stdOut,s); flushOut(stdOut); inputWord(stdIn));

(* getInteger prints prompt s and returns the next input integer *)
fun getInteger(s: string) =
   (outputWord(stdOut, s); flushOut(stdOut); inputInteger(stdIn));

(* getString prints prompt s and returns an input string *)
fun getString(s: string) =
   (outputString(stdOut, s); flushOut(stdOut); inputString(stdIn));

(* getLine prints prommpt s and returns an input string, with spaces *)
fun getLine(s: string) =
   (outputString(stdOut, s); flushOut(stdOut); inputLine(stdIn));

(* loadStudentRecordBST() loads a student record BST from a given filename *)
fun loadStudentRecordBST() =
   let val F = openIn(getWord("\nFile name: "))
       val L = studentRecordsFrom(F)
   in
      (closeIn(F); L)
   end;

(* storeStudentRecordBST stores a student record list to a given filename *)
fun storeStudentRecordBST(L: studentRecordBST): studentRecordBST =
   let val F = openOut(getWord("\nFile name: "))
   in
      (writeStudentRecordBSTTo(F, L); closeOut(F); L)
   end;

(* getName gets a name from the user *)
fun getName() = (getWord("\nFirst name: "), getWord("\nLast name: "));

(* getMajor gets a major code from the user *)
fun getMajor() = getInteger("Major code: ");

(* getClass gets a class string from the user *)
fun getClass() = getLine("\nClass: ");

(* getTown gets a hometown string from the user *)
fun getTown() = getLine("\nHome town: ");

(* getStudentRecord gets a studentRecord from the user *)
fun getStudentRecord() = (getName(), getMajor(), getClass(), getTown());

(* addStudentRecordDialog adds a studentRecord to the current database *)
fun addStudentRecordDialog(L: studentRecordBST) =
      insertRecord(L, getStudentRecord())
   handle
      badDataException => (print("\n Invalid data.  New record ignored.\n");
                           inputLine(stdIn);        (* flush input line *)
                           L);

(* searchRecordsByName inputs a name to search for and performs the search *)
fun searchRecordsByName(L: studentRecordBST) =
      (printStudentRecord(searchStudentRecordBST(L, getName())); L)
   handle
      NotFound(n) => (print("\nCould not find " ^ nameString(n) ^
                            " in the current database.\n"); L);

(* action prompts for and returns the next action code from stdIn. *)
fun action(): int = (
   print("\nPlease select one of:");
   print("\n0 - Terminate the program.");
   print("\n1 - Load the database from a file.");
   print("\n2 - Print a listing of the database.");
   print("\n3 - Print the student record for a given student name.");
   print("\n4 - Add a student record to the database.");
   print("\n5 - Store the database to a file.");
   getInteger("\nNext action: ")
);

exception ExitDialog;

(* processQuery processes the action specified by the user. *)
fun processQuery(L) =
   (case action() of
      0 => raise ExitDialog |
      1 => loadStudentRecordBST() |
      2 => printStudentRecords(L) |
      3 => searchRecordsByName(L) |
      4 => addStudentRecordDialog(L) |
      5 => storeStudentRecordBST(L) )
   handle Match => (print("\n ** Invalid option **\n"); L) |
          EndOfFile => raise ExitDialog;

(* doDialog performs successive actions specified by the user. *)
fun doDialog(L: studentRecordBST) = 
   doDialog(processQuery(L))
      handle ExitDialog => (print("\nMay the force be with you.\n"); L);

(* studentRecordDialog starts the query process *)
fun studentRecordDialog() = doDialog(Empty);

