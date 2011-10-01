"Assignment 4"
"Paul Brannan"
"Loosely based on the phonebook example in the VisualWorks tutorial"

Object subclass: #StudentDatabase
	instanceVariableNames: 'records '
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg4'!
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
	^(aRecord className)!

getNameList
	"Return a list of all the names in the database."
	^((records collect: [:rec | rec name]) asSortedCollection)!

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
	"Set the class of the student associated with aName to aString."
	((self recordFor: aName) = nil) ifTrue: [^nil]
	ifFalse: [records := ((records collect: [:rec | (rec name = aName) ifTrue: [rec class: aString]. rec]) asSortedCollection).
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
	records := SortedCollection new! !

!StudentDatabase methodsFor: 'testing'!

isEmpty
	"Returns true if the database is empty, false otherwise."
	^(records isEmpty)! !
"-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- "!

StudentDatabase class
	instanceVariableNames: ''!


!StudentDatabase class methodsFor: 'instance creation'!

new
	"Create a new instance of StudentDatabase, using init as an"
	"initializer." 
	^(super new init)! !

Object subclass: #Name
	instanceVariableNames: 'first last '
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg4'!
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
	^(last < aName last) or: [last = aName last and:
		[ first <= aName first] ]!

= aName
	"Return true if the first and last names of self and aName are equal."
	^((last = aName last) and: [first = aName first])! !

!Name methodsFor: 'printing'!

printOn: aStream
	"Print name in format first last to aStream."
	aStream nextPutAll: first; nextPutAll: ' '; nextPutAll: last! !

!Name methodsFor: 'initialization'!

first: aString1 last: aString2
	"Initialize name with first name aString1 and last name aString2."
	first := aString1.
	last := aString2! !
"-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- "!

Name class
	instanceVariableNames: ''!


!Name class methodsFor: 'instance creation'!

first: aString1 last: aString2
	"Create a new instance of name with first name aString1 and last"
	"name aString2."
	^((super new) first: aString1 last: aString2)! !

Object subclass: #StudentRecord
	instanceVariableNames: 'name major class hometown '
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg4'!
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
	^(name <= aRecord name)! !
"-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- "!

StudentRecord class
	instanceVariableNames: ''!


!StudentRecord class methodsFor: 'instance creation'!

name: aName major: anInteger class: aString1 homeTown: aString2
	"Create a new instance of StudentRecord with values as given."
	^((super new) name: aName major: anInteger class: aString1
		homeTown: aString2)! !

ApplicationModel subclass: #StudentDatabaseGUI
	instanceVariableNames: 'studentInfo names DB studentNameHolder studentClassHolder majorHolder classHolder firstNameHolder lastNameHolder homeTownHolder '
	classVariableNames: ''
	poolDictionaries: ''
	category: 'asg4'!


!StudentDatabaseGUI methodsFor: 'actions'!

add
	"Bring up the appropriate dialog box to add a student to the database."

	"Initialize the dialog box's fields."
	firstNameHolder := nil.
	lastNameHolder := nil.
	majorHolder := nil.
	classHolder := nil.
	homeTownHolder := nil.

	(self openDialogInterface: #addDialogSpec)
		ifTrue: [DB addRecordFor: (Name first: firstNameHolder value last: lastNameHolder value) major: majorHolder value class: classHolder value homeTown: homeTownHolder value.
			names list: DB getNameList]!

changeClass
	"Bring up the appropriate dialog box to add a student to the database."

	"Check to see if there is a name selected."
	names selection isNil
		ifTrue: [^Dialog warn: 'Select a student to change class for.'].

	"Check to see if the name holders didn't get initialized -- why is this happening?"
	studentNameHolder isNil ifTrue: [studentNameHolder := Name new asValue].
	studentClassHolder isNil ifTrue: [studentClassHolder := String new asValue].

	"Initialize the dialog box's fields."
	studentNameHolder value: (names selection printString).
	studentClassHolder value: ((DB classFor: (names selection)) displayString).

	"Open the dialog box and change the class if the user selected accept."
	(self openDialogInterface: #changeClassDialogSpec)
		ifTrue: [DB setClassFor: (names selection) to: studentClassHolder value]!

show
	"Check to see if there is a name selected."
	names selection isNil
		ifTrue: [^Dialog warn: 'Select a student to display.'].

	"Show the record currently selected in the student information."
	studentInfo value: (DB recordFor: (names selection)) printString!

showAll
	"Shows all the records in the database."
	^(studentInfo value: self studentInfoString)! !

!StudentDatabaseGUI methodsFor: 'aspects'!

classHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^classHolder isNil
		ifTrue:
			[classHolder := String new asValue]
		ifFalse:
			[classHolder]!

firstNameHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^firstNameHolder isNil
		ifTrue:
			[firstNameHolder := String new asValue]
		ifFalse:
			[firstNameHolder]!

homeTownHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^homeTownHolder isNil
		ifTrue:
			[homeTownHolder := String new asValue]
		ifFalse:
			[homeTownHolder]!

lastNameHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^lastNameHolder isNil
		ifTrue:
			[lastNameHolder := String new asValue]
		ifFalse:
			[lastNameHolder]!

majorHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^majorHolder isNil
		ifTrue:
			[majorHolder := 0 asValue]
		ifFalse:
			[majorHolder]!

names
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^names isNil
		ifTrue:
			[names := SelectionInList with: DB getNameList]
		ifFalse:
			[names]!

studentClassHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^studentClassHolder isNil
		ifTrue:
			[studentClassHolder := String new asValue]
		ifFalse:
			[studentClassHolder]!

studentInfo
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^studentInfo isNil
		ifTrue:
			[studentInfo := String new asValue]
		ifFalse:
			[studentInfo]!

studentNameHolder
	"This method was generated by UIDefiner.  Any edits made here
	may be lost whenever methods are automatically defined.  The
	initialization provided below may have been preempted by an
	initialize method."

	^studentNameHolder isNil
		ifTrue:
			[studentNameHolder := Name new asValue]
		ifFalse:
			[studentNameHolder]! !

!StudentDatabaseGUI methodsFor: 'initialization'!

init
	"Initialize DB to be a StudentDatabase."
	DB := StudentDatabase new! !

!StudentDatabaseGUI methodsFor: 'data formatting'!

studentInfoString
	"Return a string to be put into the student information box."
	(DB isEmpty) ifTrue: [^'There are no records in the database.'].
	^(DB printString)! !
"-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- "!

StudentDatabaseGUI class
	instanceVariableNames: ''!


!StudentDatabaseGUI class methodsFor: 'instance creation'!

new
	"Create a new instance of StudentDatabaseGUI using init as an initializer."
	^(super new init)! !

!StudentDatabaseGUI class methodsFor: 'interface specs'!

addDialogSpec
	"UIPainter new openOnClass: self andSelector: #addDialogSpec"

	<resource: #canvas>
	^#(#FullSpec 
		#window: 
		#(#WindowSpec 
			#label: 'Add Student Record' 
			#bounds: #(#Rectangle 493 274 796 403 ) ) 
		#component: 
		#(#SpecCollection 
			#collection: #(
				#(#InputFieldSpec 
					#layout: #(#Rectangle 104 8 296 24 ) 
					#model: #firstNameHolder ) 
				#(#InputFieldSpec 
					#layout: #(#Rectangle 104 32 296 48 ) 
					#model: #lastNameHolder ) 
				#(#InputFieldSpec 
					#layout: #(#Rectangle 64 56 120 72 ) 
					#model: #majorHolder 
					#type: #number ) 
				#(#InputFieldSpec 
					#layout: #(#Rectangle 200 56 296 72 ) 
					#model: #classHolder ) 
				#(#InputFieldSpec 
					#layout: #(#Rectangle 96 80 288 96 ) 
					#model: #homeTownHolder ) 
				#(#LabelSpec 
					#layout: #(#Point 8 8 ) 
					#label: 'First name:' ) 
				#(#LabelSpec 
					#layout: #(#Point 8 32 ) 
					#label: 'Last name:' ) 
				#(#LabelSpec 
					#layout: #(#Point 8 56 ) 
					#label: 'Major:' ) 
				#(#LabelSpec 
					#layout: #(#Point 144 56 ) 
					#label: 'Class:' ) 
				#(#LabelSpec 
					#layout: #(#Point 8 80 ) 
					#label: 'Home Town:' ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 72 104 136 136 ) 
					#model: #accept 
					#label: 'Accept' 
					#isDefault: true 
					#defaultable: true ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 168 104 223 134 ) 
					#model: #cancel 
					#label: 'Cancel' 
					#defaultable: true ) ) ) )!

changeClassDialogSpec
	"UIPainter new openOnClass: self andSelector: #changeClassDialogSpec"

	<resource: #canvas>
	^#(#FullSpec 
		#window: 
		#(#WindowSpec 
			#label: 'Change class' 
			#bounds: #(#Rectangle 554 316 834 397 ) ) 
		#component: 
		#(#SpecCollection 
			#collection: #(
				#(#InputFieldSpec 
					#layout: #(#Rectangle 56 8 272 24 ) 
					#model: #studentNameHolder 
					#isReadOnly: true ) 
				#(#LabelSpec 
					#layout: #(#Point 8 8 ) 
					#label: 'Name:' ) 
				#(#InputFieldSpec 
					#layout: #(#Rectangle 120 32 216 48 ) 
					#model: #studentClassHolder ) 
				#(#LabelSpec 
					#layout: #(#Point 56 32 ) 
					#label: 'Class:' ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 56 56 120 88 ) 
					#model: #accept 
					#label: 'Accept' 
					#isDefault: true 
					#defaultable: true ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 168 64 224 80 ) 
					#model: #cancel 
					#label: 'Cancel' 
					#defaultable: true ) ) ) )!

windowSpec
	"UIPainter new openOnClass: self andSelector: #windowSpec"

	<resource: #canvas>
	^#(#FullSpec 
		#window: 
		#(#WindowSpec 
			#label: 'Student Database' 
			#bounds: #(#Rectangle 500 253 891 533 ) ) 
		#component: 
		#(#SpecCollection 
			#collection: #(
				#(#SequenceViewSpec 
					#layout: #(#Rectangle 8 16 168 248 ) 
					#model: #names 
					#useModifierKeys: true 
					#selectionType: #highlight ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 8 256 64 272 ) 
					#model: #add 
					#label: 'Add' 
					#isDefault: false 
					#defaultable: true ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 72 256 176 272 ) 
					#model: #changeClass 
					#label: 'Change Class' 
					#defaultable: true ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 184 256 240 272 ) 
					#model: #show 
					#label: 'Show' 
					#defaultable: true ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 248 256 320 272 ) 
					#model: #showAll 
					#label: 'Show All' 
					#defaultable: true ) 
				#(#ActionButtonSpec 
					#layout: #(#Rectangle 328 256 384 272 ) 
					#model: #closeRequest 
					#label: 'Quit' 
					#defaultable: true ) 
				#(#LabelSpec 
					#layout: #(#Point 64 0 ) 
					#label: 'Names' ) 
				#(#LabelSpec 
					#layout: #(#Point 200 0 ) 
					#label: 'Student Information' ) 
				#(#TextEditorSpec 
					#layout: #(#Rectangle 176 16 376 248 ) 
					#model: #studentInfo 
					#tabable: true 
					#isReadOnly: true ) ) ) )! !

