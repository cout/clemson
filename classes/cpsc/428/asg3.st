"Computer Science 428, assignment 3"
"Paul Brannan"
"February 26, 1999"

Object subclass: #Name
	instanceVariableNames: 'first last '
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg3'!
Name comment:
'Name class.  Contains two instance variables, first and last, that represent
 a first and last name.
'!


!Name methodsFor: 'accessing'!

first
	"Return the first name"
	^first!

last
	"Return the last name"
	^last! !

!Name methodsFor: 'comparing'!

<= aName
	"Compare the names self and aName.  Return true if self <= aName."
	"Determine by comparing the last names and first names, with the"
	"last name taking precedence."
	(self last < aName last) ifTrue: [^true]
	ifFalse: [(self last > aName last) ifTrue: [^false]
	ifFalse: [(self first <= aName first) ifTrue: [^true]
	ifFalse: [^false]]] !

= aName
	"Return true if the first and last names of self and aName are equal."
	^((self last = aName last) & (self first = aName first))! !

!Name methodsFor: 'printing'!

printOn: aStream
	"Print name in format first last to aStream."
	aStream nextPutAll: first; nextPutAll: ' '; nextPutAll: last! !

!Name methodsFor: 'initialization'!

first: aString1 last: aString2
	"Initialize name with first name aString1 and last name aString2."
	first := aString1.
	last := aString2! !

!Name class methodsFor: 'instance creation'!

first: aString1 last: aString2
	"Create a new instance of name with first name aString1 and last"
	"name aString2."
	^((super new) first: aString1 last: aString2)! !

Object subclass: #StudentRecord
	instanceVariableNames: 'name major class hometown '
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg3'!
StudentRecord comment:
'StudentRecord class.  A record that represents a student"s information.
 Uses four instance variables name, major, class, and hometown that
 contain this information.
'!

!StudentRecord methodsFor: 'accessing'!

class: aString
	"Set the student's class to aString."
	class := aString!

className
	"Return the student's class."
	^class!

hometown
	"Return the student's hometown."
	^hometown!

major
	"Return the student's major."
	^major!

name
	"Return the student's name."
	^name! !

!StudentRecord methodsFor: 'printing'!

printOn: aStream
	"Put the record onto aStream using the format from assignment 1."
	aStream nextPutAll: name printString; cr.
	aStream nextPutAll: '   major: '; nextPutAll: major printString; cr.
	aStream nextPutAll: '   class: '; nextPutAll: class; cr.
	aStream nextPutAll: '   hometown: '; nextPutAll: hometown; cr! !

!StudentRecord methodsFor: 'initialization'!

name: aName major: anInteger class: aString1 homeTown: aString2
	"Set the record's name to aName, major to anInteger, class to"
	"aString1, and homeTown to aString2."
	name := Name first: aName first last: aName last.
	major := anInteger.
	class := aString1.
	hometown := aString2! !

!StudentRecord methodsFor: 'comparing'!

<= aRecord
	"Return true if self <= aRecord.  Use the record's name to determine"
	"this."
	^(self name <= aRecord name)! !

!StudentRecord class methodsFor: 'instance creation'!

name: aName major: anInteger class: aString1 homeTown: aString2
	"Create a new instance of StudentRecord with values as given."
	^((super new) name: aName major: anInteger class: aString1
		homeTown: aString2)! !

Object subclass: #StudentDatabase
	instanceVariableNames: 'records'
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg3'!
StudentDatabase comment:
'StudentDatabse class.  Contains an instance variable records that represents
 a SortedCollection of StudentRecords.
'!

!StudentDatabase methodsFor: 'accessing'!

addRecordFor: aName major: anInteger class: aString1 homeTown: aString2
	"Add a record to the database using the values given."
	records add: (StudentRecord name: aName major: anInteger
		class: aString1 homeTown: aString2)!

classFor: aName
	"Return the class of the student associated with name aName."
	|aRecord|
	aRecord := (self recordFor: aName).
	(aRecord = nil) ifTrue: [^nil].
	^(aRecord class)!

majorFor: aName
	"Return the major of the student associated with aName."
	|aRecord|
	aRecord := (self recordFor: aName).
	(aRecord = nil) ifTrue: [^nil].
	^(aRecord major)!

recordFor: aName
	"Return the entire record of the student associated with aName."
	(records isEmpty) ifTrue: [^nil]
	ifFalse: [records do: [:rec | (rec name = aName) ifTrue: [^rec]]].
	^nil!

setClassFor: aName to: aString
	"Set the class class of the student associated with aName to aString."
	((self recordFor: aName) = nil) ifTrue: [^nil]
	ifFalse: [records := (records collect: [:rec | (rec name = aName) ifTrue: [rec class: aString]. rec]).
	^self]! !

!StudentDatabase methodsFor: 'printing'!

printOn: aStream
	"Print all the records in the database to aStream."
	(records isEmpty) ifTrue: [^nil].
	aStream nextPutAll: '     Student Records'; cr; cr.
	records do: [:rec | aStream nextPutAll: rec printString; cr]! !

!StudentDatabase methodsFor: 'initialization'!

init
	"Set records to be a sorted collection for initialization."
	records := SortedCollection new.! !

!StudentDatabase class methodsFor: 'instance creation'!

new
	"Create a new instance of StudentDatabase, using init as an"
	"initializer." 
	^(super new init)! !

