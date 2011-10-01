//                        class listNode definition
//
//  This defines a class of nodes for a linked list.  The data field in
//  the node is for integer data.
//

class listNode {

  public:

    // Constructor:
       listNode(int n, listNode *p=0 );
          // Construct a listNode with data field n and next field p (optional).

    // Access operators.
       int getData();   // Return the data value in the listNode.

       listNode * getNext();  // Return a pointer to the next listNode.
    
       void setNext(listNode * p);  // Set the next pointer to p.

  private:

     int data;        // the data field for the listNode
     listNode* next;  // the link (pointer) field for the node
};
