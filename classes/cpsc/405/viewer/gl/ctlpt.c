#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtkwidget.h>
#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ctlpt.h"

void initctlpt(ctlpt_type* cp) {
   cp->selected = FALSE;
   cp->numctlpts = 0;
}

void initparam(param_type* p) {
   p->snapping = 0;
   p->curvestep = 1.0;
   p->sweepstep = 3.0;
   p->drawcurve = 0;
}

void showpoints(ctlpt_type* cp) {
   int i;
   glPointSize(5.0);
   for(i=0; i<cp->numctlpts; i++) {
	glPushMatrix();
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	   glVertex2f(cp->ctrlpoints[i][0], cp->ctrlpoints[i][1]);
	glEnd();
	glPopMatrix();    }
   for(i=0; i<cp->numctlpts-1; i++) {
	glPushMatrix();
	glColor3f(1,0,0);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	   glVertex2f(cp->ctrlpoints[i][0], cp->ctrlpoints[i][1]);
	   glVertex2f(cp->ctrlpoints[i+1][0], cp->ctrlpoints[i+1][1]);
	glEnd();
	glPopMatrix();   }
}
