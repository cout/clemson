// This is file Menu.cpp

#include "Menu.h"
#include <assert.h>

ostream& operator<<(ostream & Out, const Menu& M){
   Out << M.prompt;
   return Out;
}

Menu::Menu(char* in_prompt, char* cmds[]) {
   prompt = string(in_prompt);
   numberOfCommands = 0;

   while (strcmp(cmds[numberOfCommands], "") != 0)
      commands.push_back( string(cmds[numberOfCommands++]) );
}

void Menu::giveHelp() { 
   cout << "There are " << numberOfCommands << " valid commands: \n";
   vector<string>::const_iterator ptr = commands.begin();
   int count = 0;
   for ( ; ptr != commands.end(); ++ptr ) {
      cout << '\t' << (*ptr);
      if ( count++ == numberOfCommands/2 ) cout << endl;
   }
   cout << endl;
}

int Menu::getCommand(const string & command_line) {

   //First, try the obvious/typical ways to get out:
   if (command_line == "bye") return 0;
   else if (command_line == "b") return 0;
   else if (command_line == "stop") return 0;
   else if (command_line == "quit") return 0;

   // It's not one of the above, search the list of commands
   // using a common loop! We're returning an int anyway:
   for (int i = 0; i < numberOfCommands; i++) {
      if (commands[i] == command_line) return i;
   }
   return numberOfCommands;
} 
