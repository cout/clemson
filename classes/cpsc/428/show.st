'Methods show and showcr to display values in the Transcript.' !

!Object methodsFor: 'printing'!

show
	"Display the object in the transcript window; leave the cursor at the end of the object's
        print string."

	( self isMemberOf: ByteString )  ifTrue: [Transcript show: self]
                                                   ifFalse: [Transcript show: self printString]! 


showcr
	"Display the object in the transcript window, and start a new line"

	self show.
        Transcript cr ! !
