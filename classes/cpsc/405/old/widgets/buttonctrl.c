#include <gtk/gtk.h>
#include "buttonctrl.h"

int buttonctrl_type[2] = {CAMERA_ZOOM, OBJECT_ROTATE};

void on_leftbutton_camrot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = CAMERA_ROTATE;
}

void on_leftbutton_camzoom_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = CAMERA_ZOOM;
}

void on_leftbutton_camroll_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = CAMERA_ROLL;
}

void on_leftbutton_objrot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = OBJECT_ROTATE;
}

void on_leftbutton_movexz_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = OBJECT_MOVEXZ;
}

void on_leftbutton_movexy_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = OBJECT_MOVEXY;
}

void on_leftbutton_scenerot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = SCENE_ROTATE;
}

void on_midbutton_camrot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = CAMERA_ROTATE;
}

void on_midbutton_camzoom_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = CAMERA_ZOOM;
}

void on_midbutton_camroll_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = CAMERA_ROLL;
}

void on_midbutton_objrot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = OBJECT_ROTATE;
}

void on_midbutton_movexz_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = OBJECT_MOVEXZ;
}

void on_midbutton_movexy_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = OBJECT_MOVEXY;
}

void on_midbutton_scenerot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = SCENE_ROTATE;
}

void on_leftbutton_lightrot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = LIGHT_ROTATE;
}

void on_leftbutton_lightmovexz_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = LIGHT_MOVEXZ;
}

void on_leftbutton_lightmovexy_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[0] = LIGHT_MOVEXY;
}

void on_midbutton_lightrot_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = LIGHT_ROTATE;
}

void on_midbutton_lightmovexz_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = LIGHT_MOVEXZ;
}

void on_midbutton_lightmovexy_clicked(GtkWidget *w, gpointer user_data) {
	buttonctrl_type[1] = LIGHT_MOVEXY;
}

