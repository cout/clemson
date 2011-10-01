#ifndef _RESHAPE_H
#define _RESHAPE_H

#define FAR_PLANE 5000.0
#define NEAR_PLANE 0.1
#define FOV_ANGLE 70.0

void disp_really_reshape(int w, int h);
void ae_really_reshape(int w, int h);
void set_initial_size(int w, int h, int aew, int aeh);
int disp_window_width(void);
int disp_window_height(void);
int ae_window_width(void);
int ae_window_height(void);
void really_look_at_it(void);

#endif
