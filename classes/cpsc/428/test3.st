Smalltalk at: #N1 put: ''!
Smalltalk at: #N2 put: ''!
Smalltalk at: #N3 put: ''!
Smalltalk at: #N4 put: ''!
Smalltalk at: #R1 put: ''!
Smalltalk at: #R2 put: ''!
Smalltalk at: #DB put: ''!

N1 := Name first: 'Jane' last: 'Jones'.
N2 := Name first: 'Sue' last: 'Brown'.
N3 := Name first: 'John' last: 'Smith'.
N4 := Name first: 'Jim' last: 'Adams'.

R1 := StudentRecord name: N1 major: 820 class: 'frosh' homeTown: 'Clemson, SC'.
R2 := StudentRecord name: N2 major: 818 class: 'junior' homeTown: 'Atlanta, GA'.

DB := StudentDatabase new.

N1 first showcr.
N2 last showcr.
N1 showcr.
(N1 <= N2) showcr.
(N1 = (Name first: 'Jane' last: 'Jones')) showcr.
(N1 <= N3) showcr.

R1 name showcr.
R1 major showcr.
R1 className showcr.
R1 class: 'soph'.
(R1 <= R2) showcr.
(R2 <= R1) showcr.
R1 showcr.

DB addRecordFor: N1 major: 820 class: 'frosh' homeTown: 'Clemson, SC'.
DB addRecordFor: N2 major: 818 class: 'junior' homeTown: 'Atlanta, GA'.
DB addRecordFor: N3 major: 819 class: 'senior' homeTown: 'Greenville, SC'.
DB addRecordFor: N4 major: 435 class: 'soph' homeTown: 'Charlotte, NC'.
(DB recordFor: N2) showcr.
(DB majorFor: N3) showcr.
(DB classFor: (Name first: 'Who' last: 'Knows') ) showcr.
(DB classFor: N3) showcr.
(DB setClassFor: N1 to: 'blah').
DB showcr
