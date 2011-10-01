#ifndef _INPUT_H
#define _INPUT_H

#define MULTIPLIER 1.0
#define AZI_RATE 0.000045
#define ELEV_RATE 0.000045
#define ROLL_RATE 0.000045

#define ROLL_INCREMENT 0.10
#define ZOOM_IN_INCREMENT 0.2
#define ZOOM_OUT_INCREMENT 0.2

/* #define ROLL_INCREMENT 0.10 */
/* #define ZOOM_IN_INCREMENT 1.0 */
/* #define ZOOM_OUT_INCREMENT 1.0 */
void change_look_given_AE_in_pixels(int x, int y);
void mouse_motion(int x, int y);
void mouse_button(int button, int state, int x, int y);
void look_at_it(void);
void init_look(void);
double getRoll(void);
double getElevation(void);
double getAzimuth(void);
void idle(void);
void keyfunc(unsigned char key, int x, int y);
void ok_idle_function(void (*func)(void));
int AEGraph_in_disp_win(void);
int compass_in_disp_win(void);
double eye_x(void);
double eye_y(void);
double eye_z(void);

extern int mousex, mousey;
extern int reality_mode;

#endif
