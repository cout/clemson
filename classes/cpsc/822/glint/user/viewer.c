#include <unistd.h>
#include <stdio.h>
#include "graph3d.h"

static Graph3dColor gr_grey = {64, 64, 64};

int main(int argc, char *argv[]) {

  int i, j, errcode;
  int x, y;
  int width, height;
  Graph3dColor c;
  FILE *fp;
  unsigned char *image_bytes;
  char buf[1024];
  char *s, *parse;
  int max_color, size;

  if((fp = fopen(argv[1], "rb")) == NULL) {
	printf("Unable to open file %s\n", argv[1]);
	exit(1);
  }

  printf("Starting Graphics...\n");
  if(errcode = start_graph3d()) {
	printf("failed! (errcode = %d)\n", errcode);
	exit(0);
  }

  /* Import graphics file */

        /* Ignore the comments */
        fgets(buf, sizeof(buf), fp);
        do {
                fgets(buf, sizeof(buf), fp);
        } while(buf[0] == '#');

        for(parse = s = buf; *parse != ' '; parse++) ;
        *parse = 0;
        width = atoi(s);

        parse++;
        for(s = parse; *parse != '\n'; parse++) ;
        *parse = 0;
        height = atoi(s);

        fgets(buf, sizeof(buf), fp);
        max_color = atoi(buf);

        size = width * height;

        image_bytes = (unsigned char *)calloc(3, size);
        fread(image_bytes, 1, 3*size, fp);
        fclose(fp);
   
  /* Grey background */
  draw_dma_rect(0, 0, gr_grey, 800, 600, gr_grey);

  /* Draw the image */
  for(y = 0; y < height; y++) {
	for(x = 0; x < width; x++) {
		c.r = image_bytes[(y*width+x)*3 + 0];
		c.g = image_bytes[(y*width+x)*3 + 1];
		c.b = image_bytes[(y*width+x)*3 + 2];
		draw_point(x, y, c);
	}
	graph3d_flush();
  }

  printf("Press a key\n");
  getchar();

  end_graph3d(); 
  free(image_bytes);
  printf("Done!!!!\n");
  return 0;
}

