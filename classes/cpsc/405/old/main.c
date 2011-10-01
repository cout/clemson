#include <string.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include "widgets/widgets.h"
#include "gl/gl.h"

int main (int argc, char **argv) {
	GladeXML *xml;

	gtk_init(&argc, &argv);
	glade_init();

	xml = glade_xml_new("viewer.glade", NULL);

	if (!xml) {
		g_warning("Error creating the interface");
		return 1;
	}

	glade_xml_signal_autoconnect(xml);
	init_widgets(xml);
	gl_init();
	gtk_main();

	return 0;
}
