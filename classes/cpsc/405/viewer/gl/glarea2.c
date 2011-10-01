#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtkwidget.h>
#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include "ctlpt.h"
#include "splines.h"

void initgl2(void) {
   /*Specify clear color(black)*/
   glClearColor(0,0,0,0);
   glClearDepth(1.0);
   /*Turn on depth testing (z-buffering)*/
   glEnable(GL_DEPTH_TEST);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glShadeModel(GL_FLAT);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);
   glDisable(GL_LIGHTING);
   /*Turn on blending (for transparency)*/
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

gint glarea2_expose(GtkWidget* widget, GdkEventExpose* event) {
   GLfloat vertsp, horisp, i;
   ctlpt_type* ctlpts = (ctlpt_type*)gtk_object_get_data(GTK_OBJECT(widget),				"ctlpts");
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(widget),				"params");
   if(event->count > 0) {
	return TRUE;  }
   gtk_widget_grab_focus(GTK_WIDGET(widget));
   if (gtk_gl_area_begingl(GTK_GL_AREA(widget))) {
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0,0,widget->allocation.width,widget->allocation.height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,widget->allocation.width,					   (-widget->allocation.height/2.0),(widget->allocation.height/2.0));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vertsp = widget->allocation.width / 16.0;
	horisp = widget->allocation.height / 16.0;
	for(i=0.0; i<=widget->allocation.width; i=i+vertsp) {
	   glPushMatrix();
	   if(i == 0.0) {
		glColor3f(0,0,1);
		glLineWidth(3.0);  }
	   else {
		glColor3f(1,1,1);
		glLineWidth(1.0);  }
	   glBegin(GL_LINES);
		glVertex2f(i, (-widget->allocation.height/2.0));
		glVertex2f(i, widget->allocation.height/2.0);
	   glEnd();
	   glPopMatrix();    }
	for(i=-widget->allocation.height/2.0; 								i<=widget->allocation.height/2.0; i=i+horisp) {
	   glPushMatrix();
	   if(i == 0.0){
		glColor3f(0,0,1); }
	   else glColor3f(1,1,1);
	   glBegin(GL_LINES);
		glVertex2f(0.0, i);
		glVertex2f(widget->allocation.width, i);
	   glEnd();
	   glPopMatrix();    }
	showpoints(ctlpts);

	/* Draw the curve if doing so is enabled */
	if(params->drawcurve) {
		int curve_steps = (int)(params->curvestep);
		draw_1D_spline(ctlpts, curve_steps);
	}

	gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
	gtk_gl_area_endgl(GTK_GL_AREA(widget));   }
   return TRUE;
}

gint glarea2_configure(GtkWidget* widget, GdkEventConfigure* event) {
   if(gtk_gl_area_begingl(GTK_GL_AREA(widget))) {
	glViewport(0,0,widget->allocation.width, widget->allocation.height);
	gtk_gl_area_endgl(GTK_GL_AREA(widget));  }
   return TRUE;
}

gint glarea2_button_press(GtkWidget* widget, GdkEventButton* event) {
   int i;
   GLfloat dify, difx, vertsp, horisp, tempx, tempy;
   ctlpt_type* ctlpts = (ctlpt_type*)gtk_object_get_data(GTK_OBJECT(widget),				"ctlpts");
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(widget),				"params");
   if(event->button == 1)  {
	for(i=0; i<ctlpts->numctlpts; i++) {
	   dify = ((widget->allocation.height/2.0)-event->y) - 					ctlpts->ctrlpoints[i][1];
	   difx = event->x - ctlpts->ctrlpoints[i][0];
	   if((dify <= 5 && dify >= -5) && (difx <= 5 && difx >= -5)) {
	 	/* g_print("selected point %d\n", i);*/
		ctlpts->selected = i+1;
		return TRUE;			}   }
	ctlpts->selected = 0;
	if(ctlpts->numctlpts >= MAXPTS) return TRUE;
	ctlpts->ctrlpoints[ctlpts->numctlpts][0] = event->x;
	ctlpts->ctrlpoints[ctlpts->numctlpts][1] = 						(widget->allocation.height/2.0)-event->y;
	if(params->snapping) {
	   vertsp = widget->allocation.width / 16.0;
	   horisp = widget->allocation.height / 16.0;
	   tempx = ctlpts->ctrlpoints[ctlpts->numctlpts][0] / vertsp;
	   tempy = ctlpts->ctrlpoints[ctlpts->numctlpts][1] / horisp;
	   ctlpts->ctrlpoints[ctlpts->numctlpts][0] = 						rint((double)tempx) * vertsp;
	   ctlpts->ctrlpoints[ctlpts->numctlpts][1] =						rint((double)tempy) * horisp;          }
	ctlpts->numctlpts++;
	/*g_print("(%f, %f)\n", ctlpts->ctrlpoints[ctlpts->numctlpts-1][0],			ctlpts->ctrlpoints[ctlpts->numctlpts-1][1]);*/
	gtk_widget_draw(widget, (GdkRectangle*)NULL); 
	return TRUE;    }
   return FALSE;
}

gint glarea2_motion_notify(GtkWidget* widget, GdkEventMotion* event) {
   gint x, y;
   GdkModifierType state;
   GLfloat vertsp, horisp, tempx, tempy;
   ctlpt_type* ctlpts = (ctlpt_type*)gtk_object_get_data(GTK_OBJECT(widget), 				"ctlpts");
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(widget),				"params");
   if(event->is_hint) {
	gdk_window_get_pointer(event->window, &x, &y, &state);   }
   else {
	x = event->x;
	y = event->y;
	state = (GdkModifierType)event->state;   }
   if(state & GDK_BUTTON1_MASK) { 
	if (ctlpts->selected) {
	   ctlpts->ctrlpoints[ctlpts->selected-1][0] = event->x;
	   ctlpts->ctrlpoints[ctlpts->selected-1][1] = 							(widget->allocation.height/2.0) - event->y;
	   if(params->snapping) {
		vertsp = widget->allocation.width / 16.0;
		horisp = widget->allocation.height / 16.0;
		tempx = ctlpts->ctrlpoints[ctlpts->selected-1][0] / vertsp;
		tempy = ctlpts->ctrlpoints[ctlpts->selected-1][1] / horisp;
		ctlpts->ctrlpoints[ctlpts->selected-1][0] = 						rint((double)tempx) * vertsp;
		ctlpts->ctrlpoints[ctlpts->selected-1][1] =						rint((double)tempy) * horisp;		}
	   gtk_widget_draw(widget, (GdkRectangle*)NULL);      }    }
   return TRUE;
}

void snap_toggle(GtkWidget* widget, GtkWidget* glarea) {
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(glarea),				"params");
   if(GTK_TOGGLE_BUTTON(widget)->active) {
	params->snapping = TRUE;   }
   else 
	params->snapping = FALSE;
}

void curvescale_change(GtkWidget* widget, GtkWidget* glarea) {
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(glarea),				"params");
   params->curvestep = (int)(GTK_ADJUSTMENT(widget)->value);
   gtk_adjustment_set_value(GTK_ADJUSTMENT(widget), params->curvestep);
   /*g_print("curve step is %f\n", params->curvestep);*/
   gtk_widget_draw(glarea, (GdkRectangle*)NULL);
}

void sweepscale_change(GtkWidget* widget, GtkWidget* glarea) {
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(glarea),				"params");
   params->sweepstep = (int)(GTK_ADJUSTMENT(widget)->value);
   gtk_adjustment_set_value(GTK_ADJUSTMENT(widget), params->sweepstep);
   /*g_print("sweep step is %f\n", params->sweepstep);*/
}

void curvebutton_clicked(GtkWidget *w, GtkWidget *glarea) {
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(glarea),				"params");
   params->drawcurve = !params->drawcurve;
   /*g_print("drawcurve is now %d\n", params->drawcurve);*/
   gtk_widget_draw(glarea, (GdkRectangle*)NULL);
}

void sweepbutton_clicked(GtkWidget *w, GtkWidget *glarea) {
   param_type* params = (param_type*)gtk_object_get_data(GTK_OBJECT(glarea),				"params");
   ctlpt_type *ctlpts = (ctlpt_type*)gtk_object_get_data(GTK_OBJECT(glarea),
                        "ctlpts");
   draw_spline_volume(ctlpts, params->curvestep, params->sweepstep);
}

gint create_gl2_window() {
   GtkWidget* glwindow, *glarea, *glbox, *glbox2, *snapbutton, *curvebutton;
   GtkWidget* sweepbutton, *curvescale, *sweepscale, *label1;
   GtkObject* adj1, *adj2;
   ctlpt_type* ctlpts;
   param_type* params;
   glwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(glwindow), "Modeler");
   gtk_container_border_width(GTK_CONTAINER(glwindow), 5);
   /*Destroy this window when exiting from gtk_main()*/
   gtk_quit_add_destroy(1, GTK_OBJECT(glwindow));
   /*Create new OpenGL widget with GTK's window mode*/
   glarea = gtk_gl_area_new_vargs(NULL,										GDK_GL_DOUBLEBUFFER,								GDK_GL_RGBA,									GDK_GL_DEPTH_SIZE, 1,								GDK_GL_NONE);
   if(!glarea) {
	/*g_print("Can't create GtkGlArea widget\n");*/
	return FALSE;  }
   /*Set up events and signals for OpenGL widget*/
   gtk_widget_set_events(GTK_WIDGET(glarea),								GDK_EXPOSURE_MASK |								GDK_BUTTON_PRESS_MASK |								GDK_BUTTON_RELEASE_MASK |							GDK_KEY_PRESS_MASK |								GDK_KEY_RELEASE_MASK |								GDK_POINTER_MOTION_MASK |							GDK_POINTER_MOTION_HINT_MASK);
   gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",					      GTK_SIGNAL_FUNC(glarea2_expose), NULL);
   gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",					      GTK_SIGNAL_FUNC(glarea2_configure), NULL);
   gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event",					      GTK_SIGNAL_FUNC(glarea2_button_press), NULL);
   gtk_signal_connect(GTK_OBJECT(glarea), "motion_notify_event",				      GTK_SIGNAL_FUNC(glarea2_motion_notify), NULL);
   gtk_widget_set_usize(glarea, 400, 400);
   gtk_quit_add_destroy(1, GTK_OBJECT(glarea));
   ctlpts = malloc(sizeof(ctlpt_type));
   initctlpt(ctlpts);
   gtk_object_set_data(GTK_OBJECT(glarea), "ctlpts", ctlpts);
   params = malloc(sizeof(param_type));
   initparam(params);
   gtk_object_set_data(GTK_OBJECT(glarea), "params", params);
   glbox = gtk_hbox_new(FALSE, 0);
   gtk_container_add(GTK_CONTAINER(glwindow), glbox);
   gtk_box_pack_start(GTK_BOX(glbox), glarea, TRUE, TRUE, 0);
   gtk_widget_show(glarea);
   glbox2 = gtk_vbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(glbox), glbox2, TRUE, TRUE, 0);
   snapbutton = gtk_toggle_button_new_with_label("Snap to Grid");
   gtk_signal_connect(GTK_OBJECT(snapbutton), "toggled",					GTK_SIGNAL_FUNC(snap_toggle), glarea);
   gtk_box_pack_start(GTK_BOX(glbox2), snapbutton, FALSE, FALSE, 10);
   gtk_widget_show(snapbutton);
   label1 = gtk_label_new("Curve Step:");
   gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(glbox2), label1, FALSE, FALSE, 0);
   gtk_widget_show(label1);
   adj1 = gtk_adjustment_new(1.0, 1.0, 50.0, 1.0, 0.0, 0.0);
   gtk_signal_connect(GTK_OBJECT(adj1), "value_changed",					GTK_SIGNAL_FUNC(curvescale_change), glarea);
   curvescale = gtk_hscale_new(GTK_ADJUSTMENT(adj1));
   gtk_box_pack_start(GTK_BOX(glbox2), curvescale, FALSE, FALSE, 0);
   gtk_widget_show(curvescale);
   curvebutton = gtk_button_new_with_label("Draw Spline Curve");
   gtk_signal_connect(GTK_OBJECT(curvebutton), "clicked",					GTK_SIGNAL_FUNC(curvebutton_clicked), glarea);
   gtk_box_pack_start(GTK_BOX(glbox2), curvebutton, FALSE, FALSE, 10);
   gtk_widget_show(curvebutton);
   label1 = gtk_label_new("Sweep Step:");
   gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(glbox2), label1, FALSE, FALSE, 0);
   gtk_widget_show(label1);
   adj2 = gtk_adjustment_new(3.0, 3.0, 100.0, 1.0, 0.0, 0.0);
   gtk_signal_connect(GTK_OBJECT(adj2), "value_changed",					GTK_SIGNAL_FUNC(sweepscale_change), glarea);
   sweepscale = gtk_hscale_new(GTK_ADJUSTMENT(adj2));
   gtk_box_pack_start(GTK_BOX(glbox2), sweepscale, FALSE, FALSE, 0);
   gtk_widget_show(sweepscale);
   sweepbutton = gtk_button_new_with_label("Sweep");
   gtk_signal_connect(GTK_OBJECT(sweepbutton), "clicked",					GTK_SIGNAL_FUNC(sweepbutton_clicked), glarea);
   gtk_box_pack_start(GTK_BOX(glbox2), sweepbutton, FALSE, FALSE, 10);
   gtk_widget_show(sweepbutton);
   gtk_widget_show(glbox2);
   gtk_widget_show(glbox);
   gtk_widget_show(glwindow);
   /*set focus to glarea widget and initialize OpenGl*/
   GTK_WIDGET_SET_FLAGS(glarea, GTK_CAN_FOCUS);
   gtk_widget_grab_focus(GTK_WIDGET(glarea));
   initgl2();
   return(TRUE);
}
