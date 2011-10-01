#include <unistd.h>
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
    
    /* Make the window modal */
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

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

int yesno_retval;
static void yesno_handler(int val) {
	yesno_retval = val;
}

int message_yesno(gchar *message) {
    GtkWidget *dialog, *label, *yes_button, *no_button;

    yesno_retval = -1;
 
    /* Create the widgets */
    dialog = gtk_dialog_new();
    label = gtk_label_new (message);
    yes_button = gtk_button_new_with_label("Yes");
    no_button = gtk_button_new_with_label("No");

    /* Make the window modal */
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

    /* Connect our signals */
    gtk_signal_connect_object (GTK_OBJECT (yes_button), "clicked",
                               GTK_SIGNAL_FUNC (yesno_handler), (gpointer)1);
    gtk_signal_connect_object (GTK_OBJECT (no_button), "clicked",
                               GTK_SIGNAL_FUNC (yesno_handler), (gpointer)0);

    /* Ensure that the dialog box is destroyed when the user clicks a button */
    gtk_signal_connect_object (GTK_OBJECT (yes_button), "clicked",
                               GTK_SIGNAL_FUNC (gtk_widget_destroy),
                               GTK_OBJECT(dialog));
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area),
                       yes_button);
    gtk_signal_connect_object (GTK_OBJECT (no_button), "clicked",
                               GTK_SIGNAL_FUNC (gtk_widget_destroy),
                               GTK_OBJECT(dialog));
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area),
                       no_button);

    /* Add the label, and show everything we've added to the dialog */
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),
                       label);
    gtk_widget_show_all (dialog);

    while(yesno_retval == -1) {
	usleep(500);
	sync_display();
    }
    return yesno_retval;
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

