// Node.cpp

#include "Node.h"

ostream & operator<<(ostream & out, const Node & node) {
   out << node.number;
   return out;
}       
