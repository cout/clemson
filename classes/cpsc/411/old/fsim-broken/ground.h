#ifndef GROUND_H
#define GROUND_H

#define GW 2 /* grid width and height */
#define GH 2

#define GLN 37 /* list number */

#define RANDOM_MAX_HEIGHT 5.0
#define SINES 1
#define RANDOM 2
#define HEIGHT_MODE RANDOM /* choose how to construct the ground */
void init_ground_list(void);
void draw_ground(void);

#endif
