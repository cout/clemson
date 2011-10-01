#include <GL/glut.h>
#include "lighting.h"

void do_lighting() {
        #define R_warm 255.0/255.0
        #define G_warm 150.0/255.0
        #define B_warm 0.0/255.0
        #define R_cool 50.0/255.0
        #define G_cool 75.0/255.0
        #define B_cool 0.0/255.0
        #define EYE -25.0
        #define FOCAL 50.0

        GLfloat hi_diffuse[] = { (R_warm-R_cool)/2.0,
                                 (G_warm-G_cool)/2.0,
                                 (B_warm-B_cool)/2.0 };
        GLfloat lo_diffuse[] = { (R_cool-R_warm)/2.0,
                                 (G_cool-G_warm)/2.0,
                                 (B_cool-B_warm)/2.0 };
        GLfloat white_dif[] = { 0.6, 0.6, 0.6 };

        GLfloat hi_position[] = {  FOCAL,  -FOCAL, EYE, 1 };
        GLfloat lo_position[] = { -FOCAL, FOCAL, EYE, 1 };
        GLfloat white1_position[] = {50, 50, -50, 1};
        GLfloat white2_position[] = {0, 20, 0, 1};
        GLfloat white3_position[] = {20, -30, 0, 1};
        GLfloat ambient[] = { 0.5, 0.5, 0.5 };

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

        glLightfv(GL_LIGHT0, GL_DIFFUSE, hi_diffuse);
        glLightfv(GL_LIGHT0, GL_POSITION, hi_position);
        glEnable( GL_LIGHT0 );

        glLightfv(GL_LIGHT1, GL_DIFFUSE, hi_diffuse);
        glLightfv(GL_LIGHT1, GL_POSITION, lo_position);
        glEnable( GL_LIGHT1 );

        glLightfv(GL_LIGHT2, GL_DIFFUSE, white_dif);
        glLightfv(GL_LIGHT2, GL_POSITION, white1_position);
        glEnable(GL_LIGHT2);

        glLightfv(GL_LIGHT3, GL_DIFFUSE, white_dif);
        glLightfv(GL_LIGHT3, GL_POSITION, white2_position);
        glEnable(GL_LIGHT3);

        glLightfv(GL_LIGHT4, GL_DIFFUSE, white_dif);
        /*glEnable(GL_LIGHT4);*/

        glEnable(GL_LIGHTING);
}

