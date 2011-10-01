#ifndef STATUS_H
#define STATUS_H

#include <gtk/gtk.h>

void status_update(gfloat pct);
void status_clear();
void status_push(char *s);
void status_pop();

#endif
