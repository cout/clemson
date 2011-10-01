//                        class listNode implementation
//
//  Class listNode is a class of nodes for a linked list.  The data field in
//  the node is for integer data.
//

#include "listNode.h"

    // Constructor:
       listNode::listNode(int n, listNode *p) : data(n), next(p) { }
         // Construct a listNode with data field n and next field p.

    // Access operators.
       int listNode::getData(){   // Return the data value in the listNode.
          return data;
       }

       listNode * listNode::getNext(){ // Return the pointer field in the node.
          return next;
       }
    
       void listNode::setNext(listNode * p){  // Set the next pointer to p.
          next = p;
       }

