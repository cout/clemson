#ifndef CTLPT_H
#define CTLPT_H

#include <GL/gl.h>

#define MAXPTS (20)

typedef struct {
   GLfloat ctrlpoints[MAXPTS][2];
   int selected;
   int numctlpts; } ctlpt_type;

typedef struct {
   int snapping;
   float curvestep;
   float sweepstep;
   int drawcurve;
} param_type;

void initctlpt(ctlpt_type* cp);
void initparam(param_type* p);
void showpoints(ctlpt_type* cp);

#endif
