#include "_time.h"
#include <gtk/gtk.h>

guint32 get_time() {
	GTimeVal t;
	g_get_current_time(&t);
	return (t.tv_sec * 1000) + (t.tv_usec / 1000);
}

