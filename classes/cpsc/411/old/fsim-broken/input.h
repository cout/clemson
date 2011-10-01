#ifndef MOUSE_H
#define MOUSE_H

void change_look_given_AE_in_pixels(int x, int y);
void look_at_it(void);
void init_look(void);
double getRoll(void);
double getElevation(void);
double getAzimuth(void);
void idle(void);
void mouse_button(int button, int state, int x, int y);
void keyfunc(unsigned char key, int x, int y);
void ok_idle_function(void (*func)(void));
void track_motion(int x, int y);

extern int fsim_mode;

#endif
