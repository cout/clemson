#include <unistd.h>
#include <stdio.h>
#include "graphics.h"

static GraphicsColor gr_red = {255, 0, 0};
static GraphicsColor gr_green = {0, 255, 0};
static GraphicsColor gr_blue = {0, 0, 255};

int main() {
  printf("Starting Graphics...\n");
  if(start_graphics()) {
	printf("failed!\n");
	exit(0);
  }

  printf("Drawing something, but I'm not sure what...\n");
  // set_draw_color(0, 0, 255);
  // draw_rect(50,50,400,200);
  // draw_tri(1, 25, 30, 300, 500, 1);
  draw_color_tri(500, 1, gr_red, 1, 25, gr_green, 30, 300, gr_blue);
  // draw_color_tri(300, 300, gr_red, 1, 1, gr_green, 10, 500, gr_blue);
  // draw_color_tri(1, 1, gr_red, 300, 300, gr_green, 10, 500, gr_blue);

  printf("Please press enter");
  getchar();

  printf("Ending Graphics...\n");
  end_graphics(); 

  printf("Done!\n");
  return 0;
}

