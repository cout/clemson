// This is file Menu.h
// It manages the interface between the user and the program.

#include <string>
#include <vector.h>

class Menu{
public:
   Menu(char* inPrompt, char* cmds[]); 
  ~Menu() {}

   int getCommand(const string &);
   void giveHelp();

   // the next function outputs the prompt:
   friend ostream& operator<<(ostream &, const Menu&);

private:
   vector<string> commands;
   string prompt;
   int numberOfCommands;
};
