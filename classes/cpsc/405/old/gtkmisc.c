#include <gtk/gtk.h>
#include "gtkmisc.h"

void sync_display () {
	while (gtk_events_pending ())
		gtk_main_iteration ();
	gdk_flush ();
}

/* Function to open a dialog box displaying the message provided. */

 void quick_message(gchar *message) {

    GtkWidget *dialog, *label, *okay_button;
    
    /* Create the widgets */
    
    dialog = gtk_dialog_new();
    label = gtk_label_new (message);
    okay_button = gtk_button_new_with_label("Okay");
    
    /* Ensure that the dialog box is destroyed when the user clicks ok. */
    
    gtk_signal_connect_object (GTK_OBJECT (okay_button), "clicked",
                               GTK_SIGNAL_FUNC (gtk_widget_destroy),
                               GTK_OBJECT(dialog));
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area),
                       okay_button);

    /* Add the label, and show everything we've added to the dialog. */

    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
                       label);
    gtk_widget_show_all (dialog);
}

void create_file_selection(GtkSignalFunc store_filename, char *pattern) {
	GtkWidget *file_selector;

        /* Create the selector */
        file_selector = gtk_file_selection_new("Please select a file for"
                "editing.");
        gtk_file_selection_complete(GTK_FILE_SELECTION(file_selector), pattern);        gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(file_selector)->
                ok_button), "clicked", GTK_SIGNAL_FUNC(store_filename), 
		file_selector);

        /* Ensure that the dialog box is destroyed when the user clicks a
         * button.
         */
        gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(file_selector)
                ->ok_button), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
                (gpointer) file_selector);
        gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(file_selector)
                ->cancel_button), "clicked",
                GTK_SIGNAL_FUNC(gtk_widget_destroy), (gpointer) file_selector);

        /* Display that dialog */
        gtk_widget_show(file_selector);
}

