#ifndef SPLINES_H
#define SPLINES_H

#include "ctlpt.h"

void draw_1D_spline(ctlpt_type *ctlpts, int curve_steps);
void draw_1D_spline_GL(ctlpt_type *ctlpts, int curve_steps);
void draw_spline_volume(ctlpt_type *ctlpts, int curve_steps, int sweep_steps);

#endif
