#include <gtk/gtk.h>
#include <glade/glade.h>

GtkWidget *eventbox1;

void init_widgets(GladeXML *self) {
	eventbox1 = glade_xml_get_widget(self, "eventbox1");
}
