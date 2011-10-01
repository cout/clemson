#include <unistd.h>
#include <stdio.h>
#include "graphics.h"

int main() {
  printf("Starting Graphics...\n");
  start_graphics();

  printf("Pausing...\n");
  sleep(2);

  printf("Ending Graphics...\n");
  end_graphics(); 

  printf("Done!\n");
  return 0;
}

