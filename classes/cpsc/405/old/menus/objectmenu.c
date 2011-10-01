#include <gtk/gtk.h>
#include <GL/gl.h>
#include "../gl/gl.h"
#include "../gtkmisc.h"
#include "objectmenu.h"
#include "../gl/textures.h"

#define BINV(x) x=!x

GtkWidget *colorseldlg = NULL;

void on_invert_normals_activate(GtkMenuItem *m, gpointer data) {
	BINV(aw_object[curr_obj].invert_normals);
	aw_object[curr_obj].object_changed = TRUE;
	gl_redraw_all();
}

void color_changed(GtkWidget *w, GtkColorSelection *colorsel) {
	gdouble color[3];

	gtk_color_selection_get_color(colorsel, color);

	if(curr_obj < 0) quick_message("No object selected.");
	aw_object[curr_obj].color[0] = color[0];
	aw_object[curr_obj].color[1] = color[1];
	aw_object[curr_obj].color[2] = color[2];
	aw_object[curr_obj].object_changed = TRUE;

	gl_redraw_all();
}

void on_select_colors_activate(GtkMenuItem *m, gpointer data) {
	GtkWidget *colorsel;

	if(colorseldlg == NULL) {
	colorseldlg = gtk_color_selection_dialog_new(
			"Select a new color for this object");
		colorsel = GTK_COLOR_SELECTION_DIALOG(colorseldlg)->colorsel;
		gtk_signal_connect(GTK_OBJECT(colorsel), "color_changed",
			(GtkSignalFunc)color_changed, (gpointer)colorsel);
		gtk_signal_connect_object(GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(
			colorseldlg) ->ok_button), "clicked",
			GTK_SIGNAL_FUNC(gtk_widget_hide),
			(gpointer)colorseldlg);
		gtk_signal_connect_object(GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(
			colorseldlg) ->cancel_button), "clicked",
			GTK_SIGNAL_FUNC(gtk_widget_hide),
			(gpointer)colorseldlg);
	}
	update_color();
	gtk_widget_show(colorseldlg);
}

void update_color() {
	GtkWidget *colorsel;
	gdouble color[3];

	if(curr_obj < 0) return;

	if(!colorseldlg) return;
	colorsel = GTK_COLOR_SELECTION_DIALOG(colorseldlg)->colorsel;
	color[0] = aw_object[curr_obj].color[0];
	color[1] = aw_object[curr_obj].color[1];
	color[2] = aw_object[curr_obj].color[2];
	gtk_color_selection_set_color(GTK_COLOR_SELECTION(colorsel), color);
}

static void load_file(GtkWidget *w, GtkFileSelection *selector) {
	int texnum;
        gchar *filename;

	if(curr_obj < 0) {
		quick_message("No object selected.");
	        gtk_widget_hide(GTK_WIDGET(selector));
		return;
	}

	filename = gtk_file_selection_get_filename(selector);
	texnum = bind_tex(filename, 256, 256, 0);
	if(texnum == 0) {
		quick_message("Unable to load image.");
	        gtk_widget_hide(GTK_WIDGET(selector));
		return;
	}

	if(aw_object[curr_obj].texnum)
		glDeleteTextures(1, (GLuint*)&aw_object[curr_obj].texnum);
	aw_object[curr_obj].texnum = texnum;
	aw_object[curr_obj].object_changed = TRUE;

        gtk_widget_hide(GTK_WIDGET(selector));
        gl_redraw_all();
}

void on_bind_texture_activate(GtkMenuItem *m, gpointer data) {
	if(curr_obj < 0) {
		quick_message("No object selected.");
		return;
	}
	create_file_selection(load_file, "");
}

