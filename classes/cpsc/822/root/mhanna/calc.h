#ifndef CALC_H
#define CALC_H

#define CALCULATOR_CLEAR	0
#define CALCULATOR_TOP		1	
#define CALCULATOR_PERFORM	2
#define CALCULATOR_DONE		3 
#define CALCULATOR_VARIABLE	4 

struct calc_struct {
  double val;
  int var;
  int op;
};

#endif
