#include <stdio.h>
#include <gtk/gtk.h>
#include "status.h"
#include "../gtkmisc.h"

static GtkWidget *progress = NULL;
static GtkWidget *status = NULL;
gint timer;
gfloat pct = 0.0;
int statcount = 0;

void on_progressbar1_event(GtkWidget *w) {
	progress = w;
}

void on_statusbar1_event(GtkWidget *w) {
	status = w;
}

void status_update(gfloat f) {
	char s[20];
	if((pct > 0.0 && pct < 0.99) && pct > f - 0.014) return;
	pct = f;
	/* gtk_progress_bar_update(GTK_PROGRESS_BAR(progress), pct); */
	sprintf(s, "%3.1f%%", f*100);
	gtk_statusbar_pop(GTK_STATUSBAR(progress), 1);
	gtk_statusbar_push(GTK_STATUSBAR(progress), 1, s);
	sync_display();
}

void status_clear() {
	gtk_statusbar_pop(GTK_STATUSBAR(progress), 1);
	pct = 0.0;
}

void status_push(char *s) {
	gtk_statusbar_push(GTK_STATUSBAR(status), 1, s);
	statcount++;
	sync_display();
}

void status_pop() {
	if(statcount <= 0) return;
	gtk_statusbar_pop(GTK_STATUSBAR(status), 1);
	statcount--;
	sync_display();
}
