#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "calc.h"

#define NOT_USED	-1

int main() {
  int fp;
  struct calc_struct v1 = { 3.0, NOT_USED, NOT_USED };
  struct calc_struct v2 = { 2.0, NOT_USED, NOT_USED };
  struct calc_struct add = { 0.0, NOT_USED , '+' };
  double res = -1.0;

  if ((fp = open("/dev/calc", O_RDWR)) == 0) {
    fprintf(stderr, "Cannot Open Calc Device\n");
    exit(0);
  }

  /* Add two numbers together */
  ioctl(fp, CALCULATOR_CLEAR, NULL);
  ioctl(fp, CALCULATOR_PERFORM, &v1);
  ioctl(fp, CALCULATOR_PERFORM, &v2);
  ioctl(fp, CALCULATOR_PERFORM, &add);
  ioctl(fp, CALCULATOR_TOP, &res);
  ioctl(fp, CALCULATOR_DONE, NULL);
  printf("Result is %f\n\n", res);

  close(fp);

  return 0;
}
