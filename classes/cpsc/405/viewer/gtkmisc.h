#ifndef GTKMISC_H
#define GTKMISC_H

#include <gtk/gtk.h>

void sync_display();
void quick_message(gchar *s);
int message_yesno(gchar *message);
void create_file_selection(GtkSignalFunc store_filename, char *pattern);

#endif

