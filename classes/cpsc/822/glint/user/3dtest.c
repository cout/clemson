#include <unistd.h>
#include <stdio.h>
#include "graph3d.h"
#include "matrix.h"

static Vector origin = {0.0, 0.0, 0.0, 0.0};

static Vector v1 = {-1, 0, -1, 1};
static Vector v2 = {0, -1, -1, 1};
static Vector v3 = {0, 0, -1, 1};
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

  // Initialize 3d graphics
  graph3d_set_depth(1.0);
  graph3d_set_height(100.0);
  graph3d_set_ambient(gr_red);
  graph3d_set_diffuse(gr_green);
  graph3d_set_lightpos(origin);
  graph3d_load_identity();
  graph3d_scale(0.001, 0.001, 0.001);

  for(j = 0; j < 1; j++) {
    draw_3d_tri(v1, gr_red, n1, v2, gr_green, n2, v3, gr_blue, n3);
    graph3d_flush();
    graph3d_rotate(1.0, 0.1, 0.2, 0.3);
  }

  printf("Please press enter");
  getchar();

  printf("Ending Graphics...\n");

  end_graph3d(); 
  printf("Done!!!!\n");
  return 0;
}

