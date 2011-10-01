
#include <unistd.h>
#include <stdio.h>
#include "graph3d.h"
#include "matrix.h"

static Vector origin = {0.0, 0.0, 0.0, 0.0};

static Vector v1 = {-1, 0, 0, 1};
static Vector v2 = {0, -1, 0, 1};
static Vector v3 = {0, 0, 1, 1};
static Vector n1 = {-1, -1, -1, 1};
static Vector n2 = {-1, 1, -1, 1};
static Vector n3 = {1, 1, 1, 1};

int main() {
  int i, j, errcode;

  printf("Starting Graphics...\n");
  if(errcode = start_graph3d()) {
	printf("failed! (errcode = %d)\n", errcode);
	exit(0);
  }

  printf("Drawing something, but I'm not sure what...\n");

  for(j = 0; j < 100; j++) {
    for(i = 0; i < 100; i++) {
      draw_2d_tri(rand()%800, rand()%600, gr_red,
                  rand()%800, rand()%600, gr_green,
                  rand()%800, rand()%600, gr_blue);
    }
    flush_dma();
  }

  end_graph3d(); 
  printf("Done!!!!\n");
  return 0;
}

