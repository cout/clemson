(* CPSC 428 Spring 1999 Assignment 1              *)
(* Paul Brannan                                   *)
(* This program uses some basic ML techniques for *)
(* storing and printing out a list of records.    *)

(* A preson's name, consisting of first and last names *)
type name = string*string;

(* A single student record, consisting of name, major code, class, and *)
(* home town *)
type studentRecord = name*int*string*string;

(* A list of student records *)
type studentRecordList = studentRecord list;

(* nameLT compares two names and returns true if n1 < n2, false otherwise *)
fun nameLT(n1: name, n2: name) =
   if #2(n1) < #2(n2) then true
   else if #2(n1) > #2(n2) then false
      else if #1(n1) < #1(n2) then true
      else false;

(* recordLT compares two records and returns true if r1 < r2, false otherwise *)
fun recordLT(r1: studentRecord, r2: studentRecord) =
   if nameLT(#1(r1), #1(r2)) then true
   else if nameLT(#1(r2), #1(r1)) then false
      else if #2(r1) < #2(r2) then true
         else if #2(r1) > #2(r2) then false
            else if #3(r1) < #3(r2) then true
               else if #3(r1) > #3(r2) then false
                  else if #4(r1) < #4(r2) then true
                  else false;

(* insertRecord inserts a record into a sorted list of studentRecords *)
fun insertRecord(L:studentRecordList, r:studentRecord) =
   if null(L) then [r]
   else
      if recordLT(r, hd(L)) then r::L
      else
         hd(L)::insertRecord(tl(L), r);

(* nameString returns a concanated string of first/last names for printing *)
fun nameString(n: nameType) =
   #1(n) ^ " " ^ #2(n);

(* printStudentRecord prints a single formatted student record *)
fun printStudentRecord(r: studentRecord) = (
   print(nameString(#1(r)) ^ "\n");
   print("   major: " ^ Int.toString(#2(r)) ^ "\n");
   print("   class: " ^ #3(r) ^ "\n");
   print("   hometown: " ^ #4(r) ^ "\n");
   print("\n")
);

(* printStudentRecordList prints a list of student records using recursion *)
fun printStudentRecordList(L: studentRecordList) =
   if not(null(L)) then (
      printStudentRecord(hd(L));
      print("\n");
      printStudentRecordList(tl(L))
   ) else ();

(* printStudentRecords prints a header and a list of student records *)
fun printStudentRecords(L: studentRecordList) = (
   print("     Student Records\n");
   print("\n");
   printStudentRecordList(L)
);

