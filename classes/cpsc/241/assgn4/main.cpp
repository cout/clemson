//*********************************************************
// This the main program for the Phone Management Software
//
// This code was written by Brian Malloy 
// Any use of this code other than by the express written
// permission of the author is strictly forbidden.
//
// Code first constructed on Oct 2nd, 1997
// Later revised on April 13th, 1998
//*********************************************************


#include <iostream.h>
#include <limits.h>

#include "Person.h"
#include "Menu.h"
#include "IteratorString.h"
#include "List.h"

main() {
   char* commands[]= {"exit", "help", "list", "load", "save",
         "insert", "delete", "find", "new", "modify", "sort", ""};

   Menu M( "phonebook> ", commands);

   M.giveHelp();

   List my_list;
   Person p;

   string command_line;
   cout << M;
   getline(cin, command_line);
   IteratorString It( command_line, " ");

   int cmd = M.getCommand(command_line);
   while (cmd != 0) {
        switch (cmd) {
           case 1 : M.giveHelp();                        break;
           case 2 : cout << my_list;                     break;
           case 3 : my_list.readData();                  break;
           case 10: cout << "Sorting...";
                    my_list.sort();
                    cout << endl;                        break;
           case 4 : my_list.writeData();                 break;
           case 5 : cout << "Name <enter> GPA <enter>: ";
                    cin >> p; my_list.insert(p);         break;
           case 6 : cout << "Name: ";
                    getline(cin, command_line);
                    if(my_list.find(command_line, p)) {
                        cout << p << endl;
                        cout << "Delete this record? ";
                        char c;
                        cin >> c;
			cin.ignore(INT_MAX, '\n');
                        if(toupper(c)=='Y') {
                            my_list.del();
                            cout << "Deleted." << endl;
                        }
                    } else {
                        cout << "Not found." << endl;
                    }
                                                         break;
           case 7 : cout << "Name: ";
                    getline(cin, command_line);
                    if(my_list.find(command_line, p)) {
                        cout << p << endl;
			while(my_list.findnext(command_line, p))
			    cout << p << endl;
		    }
                    else cout << "Not found" << endl;    break;
           case 8 : cout << "   not implemented yet!\n"; break;
           case 9 : cout << "   not implemented yet!\n"; break;
           default: cout << "   not a valid command. \n";
        }
      cout << M;
      getline(cin, command_line);
      cmd = M.getCommand(command_line);
   }
   cout << "Terminating phonebook management program." << endl;
   return 0;    
}
