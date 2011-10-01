#ifndef _OBJECTS_H
#define _OBJECTS_H

#include "reshape.h"

void objInit();
void objSolidCube();
void objSolidIcosahedron(void);
void objWireIcosahedron(void);
void objTardis();
void objStars();
void objGround(void);
void objSign();
void objLogo();
void objGalaxy();
void objMountains();

#define GLN 37 /* list number */
#define STARS_LIST 38 /* list number */
#define CUBE_FACE_LIST 39
#define WHOLE_CUBE_LIST 40
#define SOLID_ICOSAHEDRON_LIST 41
#define WIRE_ICOSAHEDRON_LIST 42
#define TARDIS_LIST 43

#define CUBE_SUBDIV 2
#define RANDOM_COLORED_CUBE 0 /* 0 for random colored cube, 1 for single-colored cube */

#define GW 8 /* grid width and height */
#define GH 8


#define RANDOM_MAX_HEIGHT 5.0
#define SINES 1
#define RANDOM 2
#define HEIGHT_MODE SINES /* choose how to construct the ground */
#define HILLINESS 1.0 /* larger means steeper hills */

#define NUMSTARS 200
#define STARS_SCALE (FAR_PLANE * 0.9)
#define RANDOM_NUM ((double)rand() / (double)RAND_MAX)
#define STARS_MIN_R 0.6
#define STARS_MIN_G 0.5
#define STARS_MIN_B 0.2

#define SQR(x) ((x)*(x))
#define HEMISTARS /* the stars are not visible below the y=0*/

#endif
