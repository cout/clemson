
//                          CpSc 241, Lab 2 (b)
//
//  This program evaluates postfix expressions consisting of positive integer
//  operands and binary operations +, -, *, and /.  Each expression is
//  terminated by a semicolon (;), and the program is terminated by an
//  end-of-file at the beginning of an expression.  
//

#include <iostream.h>  
#include "stack.h"

 //------------------ function doOperation ------------------------------
   int doOperation (char op, stack & operands) {
       // Do the operation indicated by "op", using operands from stack
       // "operands".  Push the result of the operation onto the stack.
       // Return 1 or 0 to indicate error or not.

        int   operand1, operand2;  // operands for the operator

        // Get the operands from the stack.

          if (operands.isEmpty()) return 1;
          operand2 = operands.pop();

          if (operands.isEmpty()) return 1;
          operand1 = operands.pop();

        // Perform the indicated operation.

          switch(op) {
            case '+':  operands.push(operand1 + operand2);
                       break; // leave switch
                       
            case '-':  operands.push(operand1 - operand2);
                       break; // leave switch
                       
            case '*':  operands.push(operand1 * operand2);
                       break; // leave switch
                       
            case '/':  operands.push(operand1 / operand2);
                       break; // leave switch
          }  // end switch

         return 0;
        } // -------------- end doOperation ----------------------------

 //------------------- function endExpression ----------------------------
   int endExpression(stack & operands) {
       // Print the value on the top of stack "operands" and remove it. 
       // Check error conditions.  Return 1 or 0 to indicate error or no error.

          if (operands.isEmpty())  // error if stack is empty
             return 1;
          
          // Stack is not empty; print expression result (stack top).
             cout << "The expression value is: " << operands.pop() << endl
                  << endl;

          if (!operands.isEmpty()) // stack should now be empty
             return 1;
          else return 0;

    } // ------------------- end endExpression ----------------------------


int main() {

   stack operands;  // stack for integer operands
   char  nextChar;  // next input character
   int error=0;     // error indicator

   // Process the expressions (one token at a time).

    cout << endl << "   Postfix expression evaluator for positive integer "
         << "operands " << endl;
    cout << "   and binary operators +, -, *, and /." << endl;
    cout << "   Each expression must be terminated by a semicolon (;)." << endl;
    cout << endl << "Enter the first expression:" << endl << endl;

    while (cin >> nextChar) { // While not end of file, get the next character.

      if (nextChar >= '0' && nextChar <= '9') { 

        // Next token is an operand, so get and stack the operand.
           int   nextOperand;    // next input operand;
           cin.putback(nextChar);// Put the digit back so it can be read as int.
           cin >> nextOperand;   // Get the integer value of the operand.
           operands.push(nextOperand);
      }
      else if (nextChar=='+' || nextChar=='-' || nextChar=='*' || nextChar=='/')
      {
         // Next token is an operator, so perform the operation.
            error = doOperation(nextChar, operands);
            if (error) break;
      }
      else if (nextChar == ';'){ 
         // Next token is ';', so end the expression.
            error = endExpression(operands);
            if (error) break;
            else cout << "Enter next expression:" << endl << endl;
      }
      else { 
         // Next token is something else (invalid):
            cout << "Invalid character in input." << endl;
            error=1;
            break;
      } // end ifs for characters

   } // end while loop for input tokens

   if (error || !operands.isEmpty())
     cout << endl << "*** Error termination ***" << endl << endl;
   else
     cout << endl << "   -- end postfix calculator --" << endl << endl;

return 0;

} // ------------------- end main -----------------------------------
