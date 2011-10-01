#include <gtk/gtk.h>
#include <glade/glade.h>

GtkWidget *eventbox1;
GtkWidget *particle_system_dlg;
GladeXML *self;

void init_widgets(GladeXML *s) {
	self = s;
	eventbox1 = glade_xml_get_widget(self, "eventbox1");
	particle_system_dlg = glade_xml_get_widget(self, "particle_system_dlg");
}
