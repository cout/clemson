SRC=	main.c gtkmisc.c \
	widgets/window.c widgets/widgets.c widgets/buttonctrl.c \
	widgets/right_vbox.c widgets/scale.c widgets/status.c \
	gl/gl.c gl/aw.c gl/selection.c gl/textures.c gl/normals.c gl/splines.c \
	gl/trackball/trackball.c gl/glarea2.c gl/ctlpt.c gl/particle.c \
	misc/_string.c misc/io.c misc/dynarray.c misc/_time.c misc/geometry.c \
	menus/filemenu.c menus/editmenu.c menus/optionmenu.c \
	menus/objectmenu.c menus/particlemenu.c \
	math/matrix.c
OBJ1=$(SRC:.c=.o)
OBJ=$(OBJ1:.cpp=.o)
OUT=viewer

INCLUDES=-I/usr/local/include -I.
CFLAGS=-g -O2 -Wall `libglade-config --cflags` `gtk-config --cflags` \
	`glib-config --cflags` `imlib-config --cflags`
CC=gcc

CCFLAGS=$(CFLAGS)
CCC=CC

LIBS=-lGLU -lGL -lgtkgl
LDFLAGS=`gtk-config --libs` `libglade-config --libs` \
	`imlib-config --libs-gdk` -lm

##### RULES #####

.SUFFIXES: .c .c .ln

default: setup dep $(OUT)

# Machine-specific definitions:
setup:
	if test "$(CC)" = "cc"; then \
		LDFLAGS="$(LDFLAGS) -Wl,-woff,84 -Wl,-woff,85 -Wl,-woff,127"; \
	else \
		CFLAGS="$(CFLAGS) -Wall"; \
	fi

.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.cpp.o: 
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@


$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	/bin/rm -f $(OBJ) $(OUT)
# DO NOT DELETE

main.o: /usr/include/unistd.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
main.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
main.o: /usr/include/bits/confname.h /usr/include/getopt.h
main.o: /usr/include/string.h /usr/include/glade/glade.h
main.o: /usr/include/glade/glade-xml.h /usr/include/glib.h
main.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
main.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
main.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
main.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
main.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
main.o: /usr/include/gtk/gtkdata.h /usr/include/gdk/gdk.h
main.o: /usr/include/gdk/gdktypes.h /usr/include/gdk/gdkcursors.h
main.o: /usr/include/gdk/gdkrgb.h /usr/include/gtk/gtkobject.h
main.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
main.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
main.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkwidget.h
main.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkadjustment.h
main.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtktooltips.h
main.o: /usr/include/gtk/gtk.h /usr/include/gtk/gtkaccellabel.h
main.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
main.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
main.o: /usr/include/gtk/gtkcontainer.h /usr/include/gtk/gtkaspectframe.h
main.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
main.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
main.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
main.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
main.o: /usr/include/gtk/gtkmarshal.h /usr/include/gtk/gtkcheckbutton.h
main.o: /usr/include/gtk/gtktogglebutton.h
main.o: /usr/include/gtk/gtkcheckmenuitem.h /usr/include/gtk/gtkmenuitem.h
main.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
main.o: /usr/include/gtk/gtkhscrollbar.h /usr/include/gtk/gtkscrollbar.h
main.o: /usr/include/gtk/gtkrange.h /usr/include/gtk/gtkvscrollbar.h
main.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
main.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
main.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
main.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
main.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
main.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
main.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
main.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
main.o: /usr/include/gtk/gtkdrawingarea.h /usr/include/gtk/gtkdialog.h
main.o: /usr/include/gtk/gtkdnd.h /usr/include/gtk/gtkselection.h
main.o: /usr/include/gtk/gtkfeatures.h /usr/include/gtk/gtkfilesel.h
main.o: /usr/include/gtk/gtkfixed.h /usr/include/gtk/gtkfontsel.h
main.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
main.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
main.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
main.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
main.o: /usr/include/gtk/gtkruler.h /usr/include/gtk/gtkhseparator.h
main.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
main.o: /usr/include/gtk/gtkinputdialog.h /usr/include/gtk/gtkitemfactory.h
main.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
main.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
main.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
main.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
main.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
main.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
main.o: /usr/include/gtk/gtkprogressbar.h /usr/include/gtk/gtkradiobutton.h
main.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
main.o: /usr/include/gtk/gtkscrolledwindow.h /usr/include/gtk/gtkviewport.h
main.o: /usr/include/gtk/gtksocket.h /usr/include/gtk/gtkspinbutton.h
main.o: /usr/include/gtk/gtkstatusbar.h /usr/include/gtk/gtktearoffmenuitem.h
main.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
main.o: /usr/include/gtk/gtktipsquery.h /usr/include/gtk/gtktoolbar.h
main.o: /usr/include/gtk/gtktree.h /usr/include/gtk/gtktreeitem.h
main.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
main.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
main.o: /usr/include/gtk/gtkvseparator.h widgets/widgets.h gl/gl.h
main.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
main.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h gl/aw.h
main.o: misc/dynarray.h math/matrix.h gl/particle.h math/matrix.h
gtkmisc.o: /usr/include/unistd.h /usr/include/features.h
gtkmisc.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gtkmisc.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
gtkmisc.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gtkmisc.o: /usr/include/bits/confname.h /usr/include/getopt.h
gtkmisc.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
gtkmisc.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gtkmisc.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gtkmisc.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gtkmisc.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gtkmisc.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gtkmisc.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gtkmisc.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gtkmisc.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkobject.h
gtkmisc.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
gtkmisc.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
gtkmisc.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkaccellabel.h
gtkmisc.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
gtkmisc.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkadjustment.h
gtkmisc.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
gtkmisc.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
gtkmisc.o: /usr/include/gtk/gtkcontainer.h /usr/include/gtk/gtkaspectframe.h
gtkmisc.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
gtkmisc.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
gtkmisc.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
gtkmisc.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
gtkmisc.o: /usr/include/gtk/gtkmarshal.h /usr/include/gtk/gtkcheckbutton.h
gtkmisc.o: /usr/include/gtk/gtktogglebutton.h
gtkmisc.o: /usr/include/gtk/gtkcheckmenuitem.h /usr/include/gtk/gtkmenuitem.h
gtkmisc.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
gtkmisc.o: /usr/include/gtk/gtkhscrollbar.h /usr/include/gtk/gtkscrollbar.h
gtkmisc.o: /usr/include/gtk/gtkrange.h /usr/include/gtk/gtkvscrollbar.h
gtkmisc.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
gtkmisc.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
gtkmisc.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
gtkmisc.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
gtkmisc.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
gtkmisc.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
gtkmisc.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
gtkmisc.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
gtkmisc.o: /usr/include/gtk/gtkdrawingarea.h /usr/include/gtk/gtkdialog.h
gtkmisc.o: /usr/include/gtk/gtkdnd.h /usr/include/gtk/gtkselection.h
gtkmisc.o: /usr/include/gtk/gtkfeatures.h /usr/include/gtk/gtkfilesel.h
gtkmisc.o: /usr/include/gtk/gtkfixed.h /usr/include/gtk/gtkfontsel.h
gtkmisc.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
gtkmisc.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
gtkmisc.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
gtkmisc.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
gtkmisc.o: /usr/include/gtk/gtkruler.h /usr/include/gtk/gtkhseparator.h
gtkmisc.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
gtkmisc.o: /usr/include/gtk/gtkinputdialog.h
gtkmisc.o: /usr/include/gtk/gtkitemfactory.h
gtkmisc.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
gtkmisc.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
gtkmisc.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
gtkmisc.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
gtkmisc.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
gtkmisc.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
gtkmisc.o: /usr/include/gtk/gtkprogressbar.h
gtkmisc.o: /usr/include/gtk/gtkradiobutton.h
gtkmisc.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gtkmisc.o: /usr/include/gtk/gtkscrolledwindow.h
gtkmisc.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
gtkmisc.o: /usr/include/gtk/gtkspinbutton.h /usr/include/gtk/gtkstatusbar.h
gtkmisc.o: /usr/include/gtk/gtktearoffmenuitem.h /usr/include/gtk/gtktext.h
gtkmisc.o: /usr/include/gtk/gtkthemes.h /usr/include/gtk/gtktipsquery.h
gtkmisc.o: /usr/include/gtk/gtktoolbar.h /usr/include/gtk/gtktooltips.h
gtkmisc.o: /usr/include/gtk/gtktree.h /usr/include/gtk/gtktreeitem.h
gtkmisc.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
gtkmisc.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
gtkmisc.o: /usr/include/gtk/gtkvseparator.h gtkmisc.h
widgets/window.o: /usr/include/stdio.h /usr/include/features.h
widgets/window.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
widgets/window.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
widgets/window.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
widgets/window.o: /usr/include/bits/types.h /usr/include/libio.h
widgets/window.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
widgets/window.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
widgets/window.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
widgets/window.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
widgets/window.o: /usr/include/bits/posix1_lim.h
widgets/window.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
widgets/window.o: /usr/include/bits/posix2_lim.h
widgets/window.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
widgets/window.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
widgets/window.o: /usr/include/gtk/gtkaccelgroup.h
widgets/window.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
widgets/window.o: /usr/include/gtk/gtktypeutils.h
widgets/window.o: /usr/include/gtk/gtktypebuiltins.h
widgets/window.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
widgets/window.o: /usr/include/gtk/gtkaccellabel.h
widgets/window.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
widgets/window.o: /usr/include/gtk/gtkwidget.h
widgets/window.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
widgets/window.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
widgets/window.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
widgets/window.o: /usr/include/gtk/gtkaspectframe.h
widgets/window.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
widgets/window.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
widgets/window.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
widgets/window.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
widgets/window.o: /usr/include/gtk/gtkmarshal.h
widgets/window.o: /usr/include/gtk/gtkcheckbutton.h
widgets/window.o: /usr/include/gtk/gtktogglebutton.h
widgets/window.o: /usr/include/gtk/gtkcheckmenuitem.h
widgets/window.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
widgets/window.o: /usr/include/gtk/gtkclist.h
widgets/window.o: /usr/include/gtk/gtkhscrollbar.h
widgets/window.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
widgets/window.o: /usr/include/gtk/gtkvscrollbar.h
widgets/window.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
widgets/window.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
widgets/window.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
widgets/window.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
widgets/window.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
widgets/window.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
widgets/window.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
widgets/window.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
widgets/window.o: /usr/include/gtk/gtkdrawingarea.h
widgets/window.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
widgets/window.o: /usr/include/gtk/gtkselection.h
widgets/window.o: /usr/include/gtk/gtkfeatures.h
widgets/window.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
widgets/window.o: /usr/include/gtk/gtkfontsel.h
widgets/window.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
widgets/window.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
widgets/window.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
widgets/window.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
widgets/window.o: /usr/include/gtk/gtkruler.h
widgets/window.o: /usr/include/gtk/gtkhseparator.h
widgets/window.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
widgets/window.o: /usr/include/gtk/gtkinputdialog.h
widgets/window.o: /usr/include/gtk/gtkitemfactory.h
widgets/window.o: /usr/include/gtk/gtkmenufactory.h
widgets/window.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
widgets/window.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
widgets/window.o: /usr/include/gtk/gtkmenushell.h
widgets/window.o: /usr/include/gtk/gtkmenubar.h
widgets/window.o: /usr/include/gtk/gtkoptionmenu.h
widgets/window.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
widgets/window.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
widgets/window.o: /usr/include/gtk/gtkprogressbar.h
widgets/window.o: /usr/include/gtk/gtkradiobutton.h
widgets/window.o: /usr/include/gtk/gtkradiomenuitem.h
widgets/window.o: /usr/include/gtk/gtkrc.h
widgets/window.o: /usr/include/gtk/gtkscrolledwindow.h
widgets/window.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
widgets/window.o: /usr/include/gtk/gtkspinbutton.h
widgets/window.o: /usr/include/gtk/gtkstatusbar.h
widgets/window.o: /usr/include/gtk/gtktearoffmenuitem.h
widgets/window.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
widgets/window.o: /usr/include/gtk/gtktipsquery.h
widgets/window.o: /usr/include/gtk/gtktoolbar.h
widgets/window.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
widgets/window.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
widgets/window.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
widgets/window.o: /usr/include/gtk/gtkvscale.h
widgets/window.o: /usr/include/gtk/gtkvseparator.h gl/gl.h
widgets/window.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
widgets/window.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h
widgets/window.o: gl/aw.h misc/dynarray.h math/matrix.h gl/particle.h
widgets/window.o: math/matrix.h widgets/status.h widgets/widgets.h
widgets/window.o: /usr/include/glade/glade.h /usr/include/glade/glade-xml.h
widgets/window.o: menus/objectmenu.h
widgets/widgets.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
widgets/widgets.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
widgets/widgets.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
widgets/widgets.o: /usr/include/features.h /usr/include/sys/cdefs.h
widgets/widgets.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
widgets/widgets.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
widgets/widgets.o: /usr/include/bits/posix2_lim.h
widgets/widgets.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
widgets/widgets.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
widgets/widgets.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
widgets/widgets.o: /usr/include/gtk/gtkaccelgroup.h
widgets/widgets.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
widgets/widgets.o: /usr/include/gtk/gtktypeutils.h
widgets/widgets.o: /usr/include/gtk/gtktypebuiltins.h
widgets/widgets.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
widgets/widgets.o: /usr/include/gtk/gtkaccellabel.h
widgets/widgets.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
widgets/widgets.o: /usr/include/gtk/gtkwidget.h
widgets/widgets.o: /usr/include/gtk/gtkadjustment.h
widgets/widgets.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
widgets/widgets.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
widgets/widgets.o: /usr/include/gtk/gtkcontainer.h
widgets/widgets.o: /usr/include/gtk/gtkaspectframe.h
widgets/widgets.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
widgets/widgets.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
widgets/widgets.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
widgets/widgets.o: /usr/include/gtk/gtkcalendar.h
widgets/widgets.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
widgets/widgets.o: /usr/include/gtk/gtkcheckbutton.h
widgets/widgets.o: /usr/include/gtk/gtktogglebutton.h
widgets/widgets.o: /usr/include/gtk/gtkcheckmenuitem.h
widgets/widgets.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
widgets/widgets.o: /usr/include/gtk/gtkclist.h
widgets/widgets.o: /usr/include/gtk/gtkhscrollbar.h
widgets/widgets.o: /usr/include/gtk/gtkscrollbar.h
widgets/widgets.o: /usr/include/gtk/gtkrange.h
widgets/widgets.o: /usr/include/gtk/gtkvscrollbar.h
widgets/widgets.o: /usr/include/gtk/gtkcolorsel.h
widgets/widgets.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
widgets/widgets.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
widgets/widgets.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
widgets/widgets.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
widgets/widgets.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
widgets/widgets.o: /usr/include/gtk/gtkeventbox.h /usr/include/gtk/gtkcombo.h
widgets/widgets.o: /usr/include/gtk/gtkcompat.h /usr/include/gtk/gtkctree.h
widgets/widgets.o: /usr/include/gtk/gtkcurve.h
widgets/widgets.o: /usr/include/gtk/gtkdrawingarea.h
widgets/widgets.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
widgets/widgets.o: /usr/include/gtk/gtkselection.h
widgets/widgets.o: /usr/include/gtk/gtkfeatures.h
widgets/widgets.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
widgets/widgets.o: /usr/include/gtk/gtkfontsel.h
widgets/widgets.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
widgets/widgets.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
widgets/widgets.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
widgets/widgets.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
widgets/widgets.o: /usr/include/gtk/gtkruler.h
widgets/widgets.o: /usr/include/gtk/gtkhseparator.h
widgets/widgets.o: /usr/include/gtk/gtkseparator.h
widgets/widgets.o: /usr/include/gtk/gtkimage.h
widgets/widgets.o: /usr/include/gtk/gtkinputdialog.h
widgets/widgets.o: /usr/include/gtk/gtkitemfactory.h
widgets/widgets.o: /usr/include/gtk/gtkmenufactory.h
widgets/widgets.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
widgets/widgets.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
widgets/widgets.o: /usr/include/gtk/gtkmenushell.h
widgets/widgets.o: /usr/include/gtk/gtkmenubar.h
widgets/widgets.o: /usr/include/gtk/gtkoptionmenu.h
widgets/widgets.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
widgets/widgets.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
widgets/widgets.o: /usr/include/gtk/gtkprogressbar.h
widgets/widgets.o: /usr/include/gtk/gtkradiobutton.h
widgets/widgets.o: /usr/include/gtk/gtkradiomenuitem.h
widgets/widgets.o: /usr/include/gtk/gtkrc.h
widgets/widgets.o: /usr/include/gtk/gtkscrolledwindow.h
widgets/widgets.o: /usr/include/gtk/gtkviewport.h
widgets/widgets.o: /usr/include/gtk/gtksocket.h
widgets/widgets.o: /usr/include/gtk/gtkspinbutton.h
widgets/widgets.o: /usr/include/gtk/gtkstatusbar.h
widgets/widgets.o: /usr/include/gtk/gtktearoffmenuitem.h
widgets/widgets.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
widgets/widgets.o: /usr/include/gtk/gtktipsquery.h
widgets/widgets.o: /usr/include/gtk/gtktoolbar.h
widgets/widgets.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
widgets/widgets.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
widgets/widgets.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
widgets/widgets.o: /usr/include/gtk/gtkvscale.h
widgets/widgets.o: /usr/include/gtk/gtkvseparator.h
widgets/widgets.o: /usr/include/glade/glade.h /usr/include/glade/glade-xml.h
widgets/buttonctrl.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
widgets/buttonctrl.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
widgets/buttonctrl.o: /usr/lib/glib/include/glibconfig.h
widgets/buttonctrl.o: /usr/include/limits.h /usr/include/features.h
widgets/buttonctrl.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
widgets/buttonctrl.o: /usr/include/bits/posix1_lim.h
widgets/buttonctrl.o: /usr/include/bits/local_lim.h
widgets/buttonctrl.o: /usr/include/linux/limits.h
widgets/buttonctrl.o: /usr/include/bits/posix2_lim.h
widgets/buttonctrl.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
widgets/buttonctrl.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
widgets/buttonctrl.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
widgets/buttonctrl.o: /usr/include/gtk/gtkaccelgroup.h
widgets/buttonctrl.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
widgets/buttonctrl.o: /usr/include/gtk/gtktypeutils.h
widgets/buttonctrl.o: /usr/include/gtk/gtktypebuiltins.h
widgets/buttonctrl.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
widgets/buttonctrl.o: /usr/include/gtk/gtkaccellabel.h
widgets/buttonctrl.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
widgets/buttonctrl.o: /usr/include/gtk/gtkwidget.h
widgets/buttonctrl.o: /usr/include/gtk/gtkadjustment.h
widgets/buttonctrl.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
widgets/buttonctrl.o: /usr/include/gtk/gtkalignment.h
widgets/buttonctrl.o: /usr/include/gtk/gtkbin.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcontainer.h
widgets/buttonctrl.o: /usr/include/gtk/gtkaspectframe.h
widgets/buttonctrl.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
widgets/buttonctrl.o: /usr/include/gtk/gtkbindings.h
widgets/buttonctrl.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
widgets/buttonctrl.o: /usr/include/gtk/gtkbutton.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcalendar.h
widgets/buttonctrl.o: /usr/include/gtk/gtksignal.h
widgets/buttonctrl.o: /usr/include/gtk/gtkmarshal.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcheckbutton.h
widgets/buttonctrl.o: /usr/include/gtk/gtktogglebutton.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcheckmenuitem.h
widgets/buttonctrl.o: /usr/include/gtk/gtkmenuitem.h
widgets/buttonctrl.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhscrollbar.h
widgets/buttonctrl.o: /usr/include/gtk/gtkscrollbar.h
widgets/buttonctrl.o: /usr/include/gtk/gtkrange.h
widgets/buttonctrl.o: /usr/include/gtk/gtkvscrollbar.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcolorsel.h
widgets/buttonctrl.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
widgets/buttonctrl.o: /usr/include/gtk/gtkpreview.h
widgets/buttonctrl.o: /usr/include/gtk/gtkentry.h
widgets/buttonctrl.o: /usr/include/gtk/gtkeditable.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
widgets/buttonctrl.o: /usr/include/gtk/gtkscale.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhscale.h
widgets/buttonctrl.o: /usr/include/gtk/gtktable.h
widgets/buttonctrl.o: /usr/include/gtk/gtkeventbox.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcombo.h
widgets/buttonctrl.o: /usr/include/gtk/gtkcompat.h
widgets/buttonctrl.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
widgets/buttonctrl.o: /usr/include/gtk/gtkdrawingarea.h
widgets/buttonctrl.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
widgets/buttonctrl.o: /usr/include/gtk/gtkselection.h
widgets/buttonctrl.o: /usr/include/gtk/gtkfeatures.h
widgets/buttonctrl.o: /usr/include/gtk/gtkfilesel.h
widgets/buttonctrl.o: /usr/include/gtk/gtkfixed.h
widgets/buttonctrl.o: /usr/include/gtk/gtkfontsel.h
widgets/buttonctrl.o: /usr/include/gtk/gtknotebook.h
widgets/buttonctrl.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhandlebox.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhbbox.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhpaned.h
widgets/buttonctrl.o: /usr/include/gtk/gtkpaned.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhruler.h
widgets/buttonctrl.o: /usr/include/gtk/gtkruler.h
widgets/buttonctrl.o: /usr/include/gtk/gtkhseparator.h
widgets/buttonctrl.o: /usr/include/gtk/gtkseparator.h
widgets/buttonctrl.o: /usr/include/gtk/gtkimage.h
widgets/buttonctrl.o: /usr/include/gtk/gtkinputdialog.h
widgets/buttonctrl.o: /usr/include/gtk/gtkitemfactory.h
widgets/buttonctrl.o: /usr/include/gtk/gtkmenufactory.h
widgets/buttonctrl.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
widgets/buttonctrl.o: /usr/include/gtk/gtklistitem.h
widgets/buttonctrl.o: /usr/include/gtk/gtkmenu.h
widgets/buttonctrl.o: /usr/include/gtk/gtkmenushell.h
widgets/buttonctrl.o: /usr/include/gtk/gtkmenubar.h
widgets/buttonctrl.o: /usr/include/gtk/gtkoptionmenu.h
widgets/buttonctrl.o: /usr/include/gtk/gtkpacker.h
widgets/buttonctrl.o: /usr/include/gtk/gtkpixmap.h /usr/include/gtk/gtkplug.h
widgets/buttonctrl.o: /usr/include/gtk/gtkprogress.h
widgets/buttonctrl.o: /usr/include/gtk/gtkprogressbar.h
widgets/buttonctrl.o: /usr/include/gtk/gtkradiobutton.h
widgets/buttonctrl.o: /usr/include/gtk/gtkradiomenuitem.h
widgets/buttonctrl.o: /usr/include/gtk/gtkrc.h
widgets/buttonctrl.o: /usr/include/gtk/gtkscrolledwindow.h
widgets/buttonctrl.o: /usr/include/gtk/gtkviewport.h
widgets/buttonctrl.o: /usr/include/gtk/gtksocket.h
widgets/buttonctrl.o: /usr/include/gtk/gtkspinbutton.h
widgets/buttonctrl.o: /usr/include/gtk/gtkstatusbar.h
widgets/buttonctrl.o: /usr/include/gtk/gtktearoffmenuitem.h
widgets/buttonctrl.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
widgets/buttonctrl.o: /usr/include/gtk/gtktipsquery.h
widgets/buttonctrl.o: /usr/include/gtk/gtktoolbar.h
widgets/buttonctrl.o: /usr/include/gtk/gtktooltips.h
widgets/buttonctrl.o: /usr/include/gtk/gtktree.h
widgets/buttonctrl.o: /usr/include/gtk/gtktreeitem.h
widgets/buttonctrl.o: /usr/include/gtk/gtkvbbox.h
widgets/buttonctrl.o: /usr/include/gtk/gtkvpaned.h
widgets/buttonctrl.o: /usr/include/gtk/gtkvruler.h
widgets/buttonctrl.o: /usr/include/gtk/gtkvscale.h
widgets/buttonctrl.o: /usr/include/gtk/gtkvseparator.h widgets/buttonctrl.h
widgets/right_vbox.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
widgets/right_vbox.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
widgets/right_vbox.o: /usr/lib/glib/include/glibconfig.h
widgets/right_vbox.o: /usr/include/limits.h /usr/include/features.h
widgets/right_vbox.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
widgets/right_vbox.o: /usr/include/bits/posix1_lim.h
widgets/right_vbox.o: /usr/include/bits/local_lim.h
widgets/right_vbox.o: /usr/include/linux/limits.h
widgets/right_vbox.o: /usr/include/bits/posix2_lim.h
widgets/right_vbox.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
widgets/right_vbox.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
widgets/right_vbox.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
widgets/right_vbox.o: /usr/include/gtk/gtkaccelgroup.h
widgets/right_vbox.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
widgets/right_vbox.o: /usr/include/gtk/gtktypeutils.h
widgets/right_vbox.o: /usr/include/gtk/gtktypebuiltins.h
widgets/right_vbox.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
widgets/right_vbox.o: /usr/include/gtk/gtkaccellabel.h
widgets/right_vbox.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
widgets/right_vbox.o: /usr/include/gtk/gtkwidget.h
widgets/right_vbox.o: /usr/include/gtk/gtkadjustment.h
widgets/right_vbox.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
widgets/right_vbox.o: /usr/include/gtk/gtkalignment.h
widgets/right_vbox.o: /usr/include/gtk/gtkbin.h
widgets/right_vbox.o: /usr/include/gtk/gtkcontainer.h
widgets/right_vbox.o: /usr/include/gtk/gtkaspectframe.h
widgets/right_vbox.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
widgets/right_vbox.o: /usr/include/gtk/gtkbindings.h
widgets/right_vbox.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
widgets/right_vbox.o: /usr/include/gtk/gtkbutton.h
widgets/right_vbox.o: /usr/include/gtk/gtkcalendar.h
widgets/right_vbox.o: /usr/include/gtk/gtksignal.h
widgets/right_vbox.o: /usr/include/gtk/gtkmarshal.h
widgets/right_vbox.o: /usr/include/gtk/gtkcheckbutton.h
widgets/right_vbox.o: /usr/include/gtk/gtktogglebutton.h
widgets/right_vbox.o: /usr/include/gtk/gtkcheckmenuitem.h
widgets/right_vbox.o: /usr/include/gtk/gtkmenuitem.h
widgets/right_vbox.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
widgets/right_vbox.o: /usr/include/gtk/gtkhscrollbar.h
widgets/right_vbox.o: /usr/include/gtk/gtkscrollbar.h
widgets/right_vbox.o: /usr/include/gtk/gtkrange.h
widgets/right_vbox.o: /usr/include/gtk/gtkvscrollbar.h
widgets/right_vbox.o: /usr/include/gtk/gtkcolorsel.h
widgets/right_vbox.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
widgets/right_vbox.o: /usr/include/gtk/gtkpreview.h
widgets/right_vbox.o: /usr/include/gtk/gtkentry.h
widgets/right_vbox.o: /usr/include/gtk/gtkeditable.h
widgets/right_vbox.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
widgets/right_vbox.o: /usr/include/gtk/gtkscale.h
widgets/right_vbox.o: /usr/include/gtk/gtkhscale.h
widgets/right_vbox.o: /usr/include/gtk/gtktable.h
widgets/right_vbox.o: /usr/include/gtk/gtkeventbox.h
widgets/right_vbox.o: /usr/include/gtk/gtkcombo.h
widgets/right_vbox.o: /usr/include/gtk/gtkcompat.h
widgets/right_vbox.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
widgets/right_vbox.o: /usr/include/gtk/gtkdrawingarea.h
widgets/right_vbox.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
widgets/right_vbox.o: /usr/include/gtk/gtkselection.h
widgets/right_vbox.o: /usr/include/gtk/gtkfeatures.h
widgets/right_vbox.o: /usr/include/gtk/gtkfilesel.h
widgets/right_vbox.o: /usr/include/gtk/gtkfixed.h
widgets/right_vbox.o: /usr/include/gtk/gtkfontsel.h
widgets/right_vbox.o: /usr/include/gtk/gtknotebook.h
widgets/right_vbox.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
widgets/right_vbox.o: /usr/include/gtk/gtkhandlebox.h
widgets/right_vbox.o: /usr/include/gtk/gtkhbbox.h
widgets/right_vbox.o: /usr/include/gtk/gtkhpaned.h
widgets/right_vbox.o: /usr/include/gtk/gtkpaned.h
widgets/right_vbox.o: /usr/include/gtk/gtkhruler.h
widgets/right_vbox.o: /usr/include/gtk/gtkruler.h
widgets/right_vbox.o: /usr/include/gtk/gtkhseparator.h
widgets/right_vbox.o: /usr/include/gtk/gtkseparator.h
widgets/right_vbox.o: /usr/include/gtk/gtkimage.h
widgets/right_vbox.o: /usr/include/gtk/gtkinputdialog.h
widgets/right_vbox.o: /usr/include/gtk/gtkitemfactory.h
widgets/right_vbox.o: /usr/include/gtk/gtkmenufactory.h
widgets/right_vbox.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
widgets/right_vbox.o: /usr/include/gtk/gtklistitem.h
widgets/right_vbox.o: /usr/include/gtk/gtkmenu.h
widgets/right_vbox.o: /usr/include/gtk/gtkmenushell.h
widgets/right_vbox.o: /usr/include/gtk/gtkmenubar.h
widgets/right_vbox.o: /usr/include/gtk/gtkoptionmenu.h
widgets/right_vbox.o: /usr/include/gtk/gtkpacker.h
widgets/right_vbox.o: /usr/include/gtk/gtkpixmap.h /usr/include/gtk/gtkplug.h
widgets/right_vbox.o: /usr/include/gtk/gtkprogress.h
widgets/right_vbox.o: /usr/include/gtk/gtkprogressbar.h
widgets/right_vbox.o: /usr/include/gtk/gtkradiobutton.h
widgets/right_vbox.o: /usr/include/gtk/gtkradiomenuitem.h
widgets/right_vbox.o: /usr/include/gtk/gtkrc.h
widgets/right_vbox.o: /usr/include/gtk/gtkscrolledwindow.h
widgets/right_vbox.o: /usr/include/gtk/gtkviewport.h
widgets/right_vbox.o: /usr/include/gtk/gtksocket.h
widgets/right_vbox.o: /usr/include/gtk/gtkspinbutton.h
widgets/right_vbox.o: /usr/include/gtk/gtkstatusbar.h
widgets/right_vbox.o: /usr/include/gtk/gtktearoffmenuitem.h
widgets/right_vbox.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
widgets/right_vbox.o: /usr/include/gtk/gtktipsquery.h
widgets/right_vbox.o: /usr/include/gtk/gtktoolbar.h
widgets/right_vbox.o: /usr/include/gtk/gtktooltips.h
widgets/right_vbox.o: /usr/include/gtk/gtktree.h
widgets/right_vbox.o: /usr/include/gtk/gtktreeitem.h
widgets/right_vbox.o: /usr/include/gtk/gtkvbbox.h
widgets/right_vbox.o: /usr/include/gtk/gtkvpaned.h
widgets/right_vbox.o: /usr/include/gtk/gtkvruler.h
widgets/right_vbox.o: /usr/include/gtk/gtkvscale.h
widgets/right_vbox.o: /usr/include/gtk/gtkvseparator.h
widgets/scale.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
widgets/scale.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
widgets/scale.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
widgets/scale.o: /usr/include/features.h /usr/include/sys/cdefs.h
widgets/scale.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
widgets/scale.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
widgets/scale.o: /usr/include/bits/posix2_lim.h
widgets/scale.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
widgets/scale.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
widgets/scale.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
widgets/scale.o: /usr/include/gtk/gtkaccelgroup.h
widgets/scale.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
widgets/scale.o: /usr/include/gtk/gtktypeutils.h
widgets/scale.o: /usr/include/gtk/gtktypebuiltins.h
widgets/scale.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
widgets/scale.o: /usr/include/gtk/gtkaccellabel.h /usr/include/gtk/gtklabel.h
widgets/scale.o: /usr/include/gtk/gtkmisc.h /usr/include/gtk/gtkwidget.h
widgets/scale.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
widgets/scale.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
widgets/scale.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
widgets/scale.o: /usr/include/gtk/gtkaspectframe.h
widgets/scale.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
widgets/scale.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
widgets/scale.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
widgets/scale.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
widgets/scale.o: /usr/include/gtk/gtkmarshal.h
widgets/scale.o: /usr/include/gtk/gtkcheckbutton.h
widgets/scale.o: /usr/include/gtk/gtktogglebutton.h
widgets/scale.o: /usr/include/gtk/gtkcheckmenuitem.h
widgets/scale.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
widgets/scale.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
widgets/scale.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
widgets/scale.o: /usr/include/gtk/gtkvscrollbar.h
widgets/scale.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
widgets/scale.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
widgets/scale.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
widgets/scale.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
widgets/scale.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
widgets/scale.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
widgets/scale.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
widgets/scale.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
widgets/scale.o: /usr/include/gtk/gtkdrawingarea.h
widgets/scale.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
widgets/scale.o: /usr/include/gtk/gtkselection.h
widgets/scale.o: /usr/include/gtk/gtkfeatures.h /usr/include/gtk/gtkfilesel.h
widgets/scale.o: /usr/include/gtk/gtkfixed.h /usr/include/gtk/gtkfontsel.h
widgets/scale.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
widgets/scale.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
widgets/scale.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
widgets/scale.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
widgets/scale.o: /usr/include/gtk/gtkruler.h /usr/include/gtk/gtkhseparator.h
widgets/scale.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
widgets/scale.o: /usr/include/gtk/gtkinputdialog.h
widgets/scale.o: /usr/include/gtk/gtkitemfactory.h
widgets/scale.o: /usr/include/gtk/gtkmenufactory.h
widgets/scale.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
widgets/scale.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
widgets/scale.o: /usr/include/gtk/gtkmenushell.h
widgets/scale.o: /usr/include/gtk/gtkmenubar.h
widgets/scale.o: /usr/include/gtk/gtkoptionmenu.h
widgets/scale.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
widgets/scale.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
widgets/scale.o: /usr/include/gtk/gtkprogressbar.h
widgets/scale.o: /usr/include/gtk/gtkradiobutton.h
widgets/scale.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
widgets/scale.o: /usr/include/gtk/gtkscrolledwindow.h
widgets/scale.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
widgets/scale.o: /usr/include/gtk/gtkspinbutton.h
widgets/scale.o: /usr/include/gtk/gtkstatusbar.h
widgets/scale.o: /usr/include/gtk/gtktearoffmenuitem.h
widgets/scale.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
widgets/scale.o: /usr/include/gtk/gtktipsquery.h
widgets/scale.o: /usr/include/gtk/gtktoolbar.h /usr/include/gtk/gtktooltips.h
widgets/scale.o: /usr/include/gtk/gtktree.h /usr/include/gtk/gtktreeitem.h
widgets/scale.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
widgets/scale.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
widgets/scale.o: /usr/include/gtk/gtkvseparator.h widgets/scale.h gl/gl.h
widgets/scale.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
widgets/scale.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h
widgets/scale.o: gl/aw.h misc/dynarray.h math/matrix.h gl/particle.h
widgets/scale.o: math/matrix.h
widgets/status.o: /usr/include/stdio.h /usr/include/features.h
widgets/status.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
widgets/status.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
widgets/status.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
widgets/status.o: /usr/include/bits/types.h /usr/include/libio.h
widgets/status.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
widgets/status.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
widgets/status.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
widgets/status.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
widgets/status.o: /usr/include/bits/posix1_lim.h
widgets/status.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
widgets/status.o: /usr/include/bits/posix2_lim.h
widgets/status.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
widgets/status.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
widgets/status.o: /usr/include/gtk/gtkaccelgroup.h
widgets/status.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
widgets/status.o: /usr/include/gtk/gtktypeutils.h
widgets/status.o: /usr/include/gtk/gtktypebuiltins.h
widgets/status.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
widgets/status.o: /usr/include/gtk/gtkaccellabel.h
widgets/status.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
widgets/status.o: /usr/include/gtk/gtkwidget.h
widgets/status.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
widgets/status.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
widgets/status.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
widgets/status.o: /usr/include/gtk/gtkaspectframe.h
widgets/status.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
widgets/status.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
widgets/status.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
widgets/status.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
widgets/status.o: /usr/include/gtk/gtkmarshal.h
widgets/status.o: /usr/include/gtk/gtkcheckbutton.h
widgets/status.o: /usr/include/gtk/gtktogglebutton.h
widgets/status.o: /usr/include/gtk/gtkcheckmenuitem.h
widgets/status.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
widgets/status.o: /usr/include/gtk/gtkclist.h
widgets/status.o: /usr/include/gtk/gtkhscrollbar.h
widgets/status.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
widgets/status.o: /usr/include/gtk/gtkvscrollbar.h
widgets/status.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
widgets/status.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
widgets/status.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
widgets/status.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
widgets/status.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
widgets/status.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
widgets/status.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
widgets/status.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
widgets/status.o: /usr/include/gtk/gtkdrawingarea.h
widgets/status.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
widgets/status.o: /usr/include/gtk/gtkselection.h
widgets/status.o: /usr/include/gtk/gtkfeatures.h
widgets/status.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
widgets/status.o: /usr/include/gtk/gtkfontsel.h
widgets/status.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
widgets/status.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
widgets/status.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
widgets/status.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
widgets/status.o: /usr/include/gtk/gtkruler.h
widgets/status.o: /usr/include/gtk/gtkhseparator.h
widgets/status.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
widgets/status.o: /usr/include/gtk/gtkinputdialog.h
widgets/status.o: /usr/include/gtk/gtkitemfactory.h
widgets/status.o: /usr/include/gtk/gtkmenufactory.h
widgets/status.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
widgets/status.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
widgets/status.o: /usr/include/gtk/gtkmenushell.h
widgets/status.o: /usr/include/gtk/gtkmenubar.h
widgets/status.o: /usr/include/gtk/gtkoptionmenu.h
widgets/status.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
widgets/status.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
widgets/status.o: /usr/include/gtk/gtkprogressbar.h
widgets/status.o: /usr/include/gtk/gtkradiobutton.h
widgets/status.o: /usr/include/gtk/gtkradiomenuitem.h
widgets/status.o: /usr/include/gtk/gtkrc.h
widgets/status.o: /usr/include/gtk/gtkscrolledwindow.h
widgets/status.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
widgets/status.o: /usr/include/gtk/gtkspinbutton.h
widgets/status.o: /usr/include/gtk/gtkstatusbar.h
widgets/status.o: /usr/include/gtk/gtktearoffmenuitem.h
widgets/status.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
widgets/status.o: /usr/include/gtk/gtktipsquery.h
widgets/status.o: /usr/include/gtk/gtktoolbar.h
widgets/status.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
widgets/status.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
widgets/status.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
widgets/status.o: /usr/include/gtk/gtkvscale.h
widgets/status.o: /usr/include/gtk/gtkvseparator.h widgets/status.h gtkmisc.h
gl/gl.o: /usr/include/stdlib.h /usr/include/features.h
gl/gl.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/gl.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gl/gl.o: /usr/include/sys/types.h /usr/include/bits/types.h
gl/gl.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
gl/gl.o: /usr/include/sys/select.h /usr/include/bits/select.h
gl/gl.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
gl/gl.o: /usr/include/alloca.h /usr/X11R6/include/GL/gl.h
gl/gl.o: /usr/X11R6/include/GL/glext.h /usr/local/include/GL/glu.h
gl/gl.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gdk/gdk.h
gl/gl.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gl/gl.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/gl.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/gl.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/gl.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/gl.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/gl.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/gl.o: /usr/include/gtkgl/gdkgl.h /usr/include/gtk/gtkdrawingarea.h
gl/gl.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkaccelgroup.h
gl/gl.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
gl/gl.o: /usr/include/gtk/gtktypeutils.h /usr/include/gtk/gtktypebuiltins.h
gl/gl.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
gl/gl.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
gl/gl.o: /usr/include/gtk/gtkstyle.h /usr/include/math.h
gl/gl.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
gl/gl.o: /usr/include/bits/mathcalls.h math/matrix.h ./math/_math.h gl/gl.h
gl/gl.o: gl/aw.h misc/dynarray.h math/matrix.h gl/particle.h ./options.h
gl/gl.o: /usr/include/gtk/gtk.h /usr/include/gtk/gtkaccellabel.h
gl/gl.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
gl/gl.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
gl/gl.o: /usr/include/gtk/gtkcontainer.h /usr/include/gtk/gtkaspectframe.h
gl/gl.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
gl/gl.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
gl/gl.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
gl/gl.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
gl/gl.o: /usr/include/gtk/gtkmarshal.h /usr/include/gtk/gtkcheckbutton.h
gl/gl.o: /usr/include/gtk/gtktogglebutton.h
gl/gl.o: /usr/include/gtk/gtkcheckmenuitem.h /usr/include/gtk/gtkmenuitem.h
gl/gl.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
gl/gl.o: /usr/include/gtk/gtkhscrollbar.h /usr/include/gtk/gtkscrollbar.h
gl/gl.o: /usr/include/gtk/gtkrange.h /usr/include/gtk/gtkvscrollbar.h
gl/gl.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
gl/gl.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
gl/gl.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
gl/gl.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
gl/gl.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
gl/gl.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
gl/gl.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
gl/gl.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
gl/gl.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
gl/gl.o: /usr/include/gtk/gtkselection.h /usr/include/gtk/gtkfeatures.h
gl/gl.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
gl/gl.o: /usr/include/gtk/gtkfontsel.h /usr/include/gtk/gtknotebook.h
gl/gl.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
gl/gl.o: /usr/include/gtk/gtkhandlebox.h /usr/include/gtk/gtkhbbox.h
gl/gl.o: /usr/include/gtk/gtkhpaned.h /usr/include/gtk/gtkpaned.h
gl/gl.o: /usr/include/gtk/gtkhruler.h /usr/include/gtk/gtkruler.h
gl/gl.o: /usr/include/gtk/gtkhseparator.h /usr/include/gtk/gtkseparator.h
gl/gl.o: /usr/include/gtk/gtkimage.h /usr/include/gtk/gtkinputdialog.h
gl/gl.o: /usr/include/gtk/gtkitemfactory.h /usr/include/gtk/gtkmenufactory.h
gl/gl.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
gl/gl.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
gl/gl.o: /usr/include/gtk/gtkmenushell.h /usr/include/gtk/gtkmenubar.h
gl/gl.o: /usr/include/gtk/gtkoptionmenu.h /usr/include/gtk/gtkpacker.h
gl/gl.o: /usr/include/gtk/gtkpixmap.h /usr/include/gtk/gtkplug.h
gl/gl.o: /usr/include/gtk/gtkprogress.h /usr/include/gtk/gtkprogressbar.h
gl/gl.o: /usr/include/gtk/gtkradiobutton.h
gl/gl.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/gl.o: /usr/include/gtk/gtkscrolledwindow.h /usr/include/gtk/gtkviewport.h
gl/gl.o: /usr/include/gtk/gtksocket.h /usr/include/gtk/gtkspinbutton.h
gl/gl.o: /usr/include/gtk/gtkstatusbar.h
gl/gl.o: /usr/include/gtk/gtktearoffmenuitem.h /usr/include/gtk/gtktext.h
gl/gl.o: /usr/include/gtk/gtkthemes.h /usr/include/gtk/gtktipsquery.h
gl/gl.o: /usr/include/gtk/gtktoolbar.h /usr/include/gtk/gtktooltips.h
gl/gl.o: /usr/include/gtk/gtktree.h /usr/include/gtk/gtktreeitem.h
gl/gl.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
gl/gl.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
gl/gl.o: /usr/include/gtk/gtkvseparator.h ./misc/_time.h
gl/gl.o: ./widgets/buttonctrl.h ./widgets/scale.h ./widgets/status.h
gl/gl.o: ./widgets/window.h gl/trackball/trackball.h gl/selection.h gtkmisc.h
gl/aw.o: /usr/include/stdio.h /usr/include/features.h
gl/aw.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/aw.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gl/aw.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/aw.o: /usr/include/bits/types.h /usr/include/libio.h
gl/aw.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
gl/aw.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
gl/aw.o: /usr/include/endian.h /usr/include/bits/endian.h
gl/aw.o: /usr/include/sys/select.h /usr/include/bits/select.h
gl/aw.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
gl/aw.o: /usr/include/alloca.h /usr/include/glib.h
gl/aw.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/aw.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/aw.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/aw.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/aw.o: /usr/include/sys/stat.h /usr/include/bits/stat.h ./misc/_string.h
gl/aw.o: ./misc/io.h ./misc/dynarray.h ./widgets/status.h
gl/aw.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
gl/aw.o: /usr/include/gdk/gdktypes.h /usr/include/gdk/gdkcursors.h
gl/aw.o: /usr/include/gdk/gdkrgb.h /usr/include/gtk/gtkaccelgroup.h
gl/aw.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
gl/aw.o: /usr/include/gtk/gtktypeutils.h /usr/include/gtk/gtktypebuiltins.h
gl/aw.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
gl/aw.o: /usr/include/gtk/gtkaccellabel.h /usr/include/gtk/gtklabel.h
gl/aw.o: /usr/include/gtk/gtkmisc.h /usr/include/gtk/gtkwidget.h
gl/aw.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
gl/aw.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
gl/aw.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
gl/aw.o: /usr/include/gtk/gtkaspectframe.h /usr/include/gtk/gtkframe.h
gl/aw.o: /usr/include/gtk/gtkarrow.h /usr/include/gtk/gtkbindings.h
gl/aw.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
gl/aw.o: /usr/include/gtk/gtkbutton.h /usr/include/gtk/gtkcalendar.h
gl/aw.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
gl/aw.o: /usr/include/gtk/gtkcheckbutton.h /usr/include/gtk/gtktogglebutton.h
gl/aw.o: /usr/include/gtk/gtkcheckmenuitem.h /usr/include/gtk/gtkmenuitem.h
gl/aw.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
gl/aw.o: /usr/include/gtk/gtkhscrollbar.h /usr/include/gtk/gtkscrollbar.h
gl/aw.o: /usr/include/gtk/gtkrange.h /usr/include/gtk/gtkvscrollbar.h
gl/aw.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
gl/aw.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
gl/aw.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
gl/aw.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
gl/aw.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
gl/aw.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
gl/aw.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
gl/aw.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
gl/aw.o: /usr/include/gtk/gtkdrawingarea.h /usr/include/gtk/gtkdialog.h
gl/aw.o: /usr/include/gtk/gtkdnd.h /usr/include/gtk/gtkselection.h
gl/aw.o: /usr/include/gtk/gtkfeatures.h /usr/include/gtk/gtkfilesel.h
gl/aw.o: /usr/include/gtk/gtkfixed.h /usr/include/gtk/gtkfontsel.h
gl/aw.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
gl/aw.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
gl/aw.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
gl/aw.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
gl/aw.o: /usr/include/gtk/gtkruler.h /usr/include/gtk/gtkhseparator.h
gl/aw.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
gl/aw.o: /usr/include/gtk/gtkinputdialog.h /usr/include/gtk/gtkitemfactory.h
gl/aw.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
gl/aw.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
gl/aw.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
gl/aw.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
gl/aw.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
gl/aw.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
gl/aw.o: /usr/include/gtk/gtkprogressbar.h /usr/include/gtk/gtkradiobutton.h
gl/aw.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/aw.o: /usr/include/gtk/gtkscrolledwindow.h /usr/include/gtk/gtkviewport.h
gl/aw.o: /usr/include/gtk/gtksocket.h /usr/include/gtk/gtkspinbutton.h
gl/aw.o: /usr/include/gtk/gtkstatusbar.h
gl/aw.o: /usr/include/gtk/gtktearoffmenuitem.h /usr/include/gtk/gtktext.h
gl/aw.o: /usr/include/gtk/gtkthemes.h /usr/include/gtk/gtktipsquery.h
gl/aw.o: /usr/include/gtk/gtktoolbar.h /usr/include/gtk/gtktooltips.h
gl/aw.o: /usr/include/gtk/gtktree.h /usr/include/gtk/gtktreeitem.h
gl/aw.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
gl/aw.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
gl/aw.o: /usr/include/gtk/gtkvseparator.h gl/textures.h gl/aw.h
gl/aw.o: misc/dynarray.h math/matrix.h
gl/selection.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
gl/selection.o: /usr/local/include/GL/glu.h /usr/include/gdk/gdk.h
gl/selection.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gl/selection.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/selection.o: /usr/include/features.h /usr/include/sys/cdefs.h
gl/selection.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
gl/selection.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
gl/selection.o: /usr/include/bits/posix2_lim.h
gl/selection.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/selection.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/selection.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/selection.o: /usr/include/gtk/gtk.h /usr/include/gtk/gtkaccelgroup.h
gl/selection.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
gl/selection.o: /usr/include/gtk/gtktypeutils.h
gl/selection.o: /usr/include/gtk/gtktypebuiltins.h
gl/selection.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
gl/selection.o: /usr/include/gtk/gtkaccellabel.h /usr/include/gtk/gtklabel.h
gl/selection.o: /usr/include/gtk/gtkmisc.h /usr/include/gtk/gtkwidget.h
gl/selection.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
gl/selection.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
gl/selection.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
gl/selection.o: /usr/include/gtk/gtkaspectframe.h /usr/include/gtk/gtkframe.h
gl/selection.o: /usr/include/gtk/gtkarrow.h /usr/include/gtk/gtkbindings.h
gl/selection.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
gl/selection.o: /usr/include/gtk/gtkbutton.h /usr/include/gtk/gtkcalendar.h
gl/selection.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
gl/selection.o: /usr/include/gtk/gtkcheckbutton.h
gl/selection.o: /usr/include/gtk/gtktogglebutton.h
gl/selection.o: /usr/include/gtk/gtkcheckmenuitem.h
gl/selection.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
gl/selection.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
gl/selection.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
gl/selection.o: /usr/include/gtk/gtkvscrollbar.h
gl/selection.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
gl/selection.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
gl/selection.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
gl/selection.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
gl/selection.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
gl/selection.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
gl/selection.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
gl/selection.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
gl/selection.o: /usr/include/gtk/gtkdrawingarea.h
gl/selection.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
gl/selection.o: /usr/include/gtk/gtkselection.h
gl/selection.o: /usr/include/gtk/gtkfeatures.h /usr/include/gtk/gtkfilesel.h
gl/selection.o: /usr/include/gtk/gtkfixed.h /usr/include/gtk/gtkfontsel.h
gl/selection.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
gl/selection.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
gl/selection.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
gl/selection.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
gl/selection.o: /usr/include/gtk/gtkruler.h /usr/include/gtk/gtkhseparator.h
gl/selection.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
gl/selection.o: /usr/include/gtk/gtkinputdialog.h
gl/selection.o: /usr/include/gtk/gtkitemfactory.h
gl/selection.o: /usr/include/gtk/gtkmenufactory.h
gl/selection.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
gl/selection.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
gl/selection.o: /usr/include/gtk/gtkmenushell.h /usr/include/gtk/gtkmenubar.h
gl/selection.o: /usr/include/gtk/gtkoptionmenu.h /usr/include/gtk/gtkpacker.h
gl/selection.o: /usr/include/gtk/gtkpixmap.h /usr/include/gtk/gtkplug.h
gl/selection.o: /usr/include/gtk/gtkprogress.h
gl/selection.o: /usr/include/gtk/gtkprogressbar.h
gl/selection.o: /usr/include/gtk/gtkradiobutton.h
gl/selection.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/selection.o: /usr/include/gtk/gtkscrolledwindow.h
gl/selection.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
gl/selection.o: /usr/include/gtk/gtkspinbutton.h
gl/selection.o: /usr/include/gtk/gtkstatusbar.h
gl/selection.o: /usr/include/gtk/gtktearoffmenuitem.h
gl/selection.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
gl/selection.o: /usr/include/gtk/gtktipsquery.h /usr/include/gtk/gtktoolbar.h
gl/selection.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
gl/selection.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
gl/selection.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
gl/selection.o: /usr/include/gtk/gtkvscale.h /usr/include/gtk/gtkvseparator.h
gl/selection.o: gl/selection.h gl/gl.h /usr/include/gtkgl/gtkglarea.h
gl/selection.o: /usr/include/gtkgl/gdkgl.h gl/aw.h misc/dynarray.h
gl/selection.o: math/matrix.h gl/particle.h math/matrix.h widgets/window.h
gl/selection.o: menus/objectmenu.h
gl/textures.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
gl/textures.o: /usr/include/gdk/gdk.h /usr/include/gdk/gdktypes.h
gl/textures.o: /usr/include/glib.h /usr/lib/glib/include/glibconfig.h
gl/textures.o: /usr/include/limits.h /usr/include/features.h
gl/textures.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/textures.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/textures.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/textures.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/textures.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/textures.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/textures.o: /usr/include/gdk_imlib.h /usr/include/gdk_imlib_types.h
gl/textures.o: gl/textures.h
gl/normals.o: math/matrix.h ./widgets/status.h /usr/include/gtk/gtk.h
gl/normals.o: /usr/include/gdk/gdk.h /usr/include/gdk/gdktypes.h
gl/normals.o: /usr/include/glib.h /usr/lib/glib/include/glibconfig.h
gl/normals.o: /usr/include/limits.h /usr/include/features.h
gl/normals.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/normals.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/normals.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/normals.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/normals.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/normals.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/normals.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkobject.h
gl/normals.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
gl/normals.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
gl/normals.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkaccellabel.h
gl/normals.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
gl/normals.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkadjustment.h
gl/normals.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
gl/normals.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
gl/normals.o: /usr/include/gtk/gtkcontainer.h
gl/normals.o: /usr/include/gtk/gtkaspectframe.h /usr/include/gtk/gtkframe.h
gl/normals.o: /usr/include/gtk/gtkarrow.h /usr/include/gtk/gtkbindings.h
gl/normals.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
gl/normals.o: /usr/include/gtk/gtkbutton.h /usr/include/gtk/gtkcalendar.h
gl/normals.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
gl/normals.o: /usr/include/gtk/gtkcheckbutton.h
gl/normals.o: /usr/include/gtk/gtktogglebutton.h
gl/normals.o: /usr/include/gtk/gtkcheckmenuitem.h
gl/normals.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
gl/normals.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
gl/normals.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
gl/normals.o: /usr/include/gtk/gtkvscrollbar.h /usr/include/gtk/gtkcolorsel.h
gl/normals.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
gl/normals.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
gl/normals.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
gl/normals.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
gl/normals.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
gl/normals.o: /usr/include/gtk/gtkeventbox.h /usr/include/gtk/gtkcombo.h
gl/normals.o: /usr/include/gtk/gtkcompat.h /usr/include/gtk/gtkctree.h
gl/normals.o: /usr/include/gtk/gtkcurve.h /usr/include/gtk/gtkdrawingarea.h
gl/normals.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
gl/normals.o: /usr/include/gtk/gtkselection.h /usr/include/gtk/gtkfeatures.h
gl/normals.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
gl/normals.o: /usr/include/gtk/gtkfontsel.h /usr/include/gtk/gtknotebook.h
gl/normals.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
gl/normals.o: /usr/include/gtk/gtkhandlebox.h /usr/include/gtk/gtkhbbox.h
gl/normals.o: /usr/include/gtk/gtkhpaned.h /usr/include/gtk/gtkpaned.h
gl/normals.o: /usr/include/gtk/gtkhruler.h /usr/include/gtk/gtkruler.h
gl/normals.o: /usr/include/gtk/gtkhseparator.h
gl/normals.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
gl/normals.o: /usr/include/gtk/gtkinputdialog.h
gl/normals.o: /usr/include/gtk/gtkitemfactory.h
gl/normals.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
gl/normals.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
gl/normals.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
gl/normals.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
gl/normals.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
gl/normals.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
gl/normals.o: /usr/include/gtk/gtkprogressbar.h
gl/normals.o: /usr/include/gtk/gtkradiobutton.h
gl/normals.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/normals.o: /usr/include/gtk/gtkscrolledwindow.h
gl/normals.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
gl/normals.o: /usr/include/gtk/gtkspinbutton.h
gl/normals.o: /usr/include/gtk/gtkstatusbar.h
gl/normals.o: /usr/include/gtk/gtktearoffmenuitem.h
gl/normals.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
gl/normals.o: /usr/include/gtk/gtktipsquery.h /usr/include/gtk/gtktoolbar.h
gl/normals.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
gl/normals.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
gl/normals.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
gl/normals.o: /usr/include/gtk/gtkvscale.h /usr/include/gtk/gtkvseparator.h
gl/normals.o: gl/normals.h gl/aw.h misc/dynarray.h math/matrix.h
gl/splines.o: /usr/include/math.h /usr/include/features.h
gl/splines.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/splines.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
gl/splines.o: /usr/include/bits/mathcalls.h
gl/splines.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/splines.o: math/matrix.h ./math/_math.h gl/splines.h gl/ctlpt.h
gl/splines.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
gl/splines.o: gl/gl.h /usr/include/gtkgl/gtkglarea.h /usr/include/gdk/gdk.h
gl/splines.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gl/splines.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/splines.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/splines.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/splines.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/splines.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/splines.o: /usr/include/gtkgl/gdkgl.h /usr/include/gtk/gtkdrawingarea.h
gl/splines.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkaccelgroup.h
gl/splines.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
gl/splines.o: /usr/include/gtk/gtktypeutils.h
gl/splines.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
gl/splines.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkadjustment.h
gl/splines.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h gl/aw.h
gl/splines.o: misc/dynarray.h math/matrix.h gl/particle.h gl/normals.h
gl/trackball/trackball.o: /usr/include/math.h /usr/include/features.h
gl/trackball/trackball.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/trackball/trackball.o: /usr/include/bits/huge_val.h
gl/trackball/trackball.o: /usr/include/bits/mathdef.h
gl/trackball/trackball.o: /usr/include/bits/mathcalls.h
gl/trackball/trackball.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/trackball/trackball.o: gl/trackball/trackball.h
gl/glarea2.o: /usr/include/stdio.h /usr/include/features.h
gl/glarea2.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/glarea2.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gl/glarea2.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/glarea2.o: /usr/include/bits/types.h /usr/include/libio.h
gl/glarea2.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
gl/glarea2.o: /usr/include/stdlib.h /usr/include/sys/types.h
gl/glarea2.o: /usr/include/time.h /usr/include/endian.h
gl/glarea2.o: /usr/include/bits/endian.h /usr/include/sys/select.h
gl/glarea2.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
gl/glarea2.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
gl/glarea2.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
gl/glarea2.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gl/glarea2.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/glarea2.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/glarea2.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/glarea2.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/glarea2.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/glarea2.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkobject.h
gl/glarea2.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
gl/glarea2.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
gl/glarea2.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkaccellabel.h
gl/glarea2.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
gl/glarea2.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkadjustment.h
gl/glarea2.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
gl/glarea2.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
gl/glarea2.o: /usr/include/gtk/gtkcontainer.h
gl/glarea2.o: /usr/include/gtk/gtkaspectframe.h /usr/include/gtk/gtkframe.h
gl/glarea2.o: /usr/include/gtk/gtkarrow.h /usr/include/gtk/gtkbindings.h
gl/glarea2.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
gl/glarea2.o: /usr/include/gtk/gtkbutton.h /usr/include/gtk/gtkcalendar.h
gl/glarea2.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
gl/glarea2.o: /usr/include/gtk/gtkcheckbutton.h
gl/glarea2.o: /usr/include/gtk/gtktogglebutton.h
gl/glarea2.o: /usr/include/gtk/gtkcheckmenuitem.h
gl/glarea2.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
gl/glarea2.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
gl/glarea2.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
gl/glarea2.o: /usr/include/gtk/gtkvscrollbar.h /usr/include/gtk/gtkcolorsel.h
gl/glarea2.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
gl/glarea2.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
gl/glarea2.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
gl/glarea2.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
gl/glarea2.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
gl/glarea2.o: /usr/include/gtk/gtkeventbox.h /usr/include/gtk/gtkcombo.h
gl/glarea2.o: /usr/include/gtk/gtkcompat.h /usr/include/gtk/gtkctree.h
gl/glarea2.o: /usr/include/gtk/gtkcurve.h /usr/include/gtk/gtkdrawingarea.h
gl/glarea2.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
gl/glarea2.o: /usr/include/gtk/gtkselection.h /usr/include/gtk/gtkfeatures.h
gl/glarea2.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
gl/glarea2.o: /usr/include/gtk/gtkfontsel.h /usr/include/gtk/gtknotebook.h
gl/glarea2.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
gl/glarea2.o: /usr/include/gtk/gtkhandlebox.h /usr/include/gtk/gtkhbbox.h
gl/glarea2.o: /usr/include/gtk/gtkhpaned.h /usr/include/gtk/gtkpaned.h
gl/glarea2.o: /usr/include/gtk/gtkhruler.h /usr/include/gtk/gtkruler.h
gl/glarea2.o: /usr/include/gtk/gtkhseparator.h
gl/glarea2.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
gl/glarea2.o: /usr/include/gtk/gtkinputdialog.h
gl/glarea2.o: /usr/include/gtk/gtkitemfactory.h
gl/glarea2.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
gl/glarea2.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
gl/glarea2.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
gl/glarea2.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
gl/glarea2.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
gl/glarea2.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
gl/glarea2.o: /usr/include/gtk/gtkprogressbar.h
gl/glarea2.o: /usr/include/gtk/gtkradiobutton.h
gl/glarea2.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/glarea2.o: /usr/include/gtk/gtkscrolledwindow.h
gl/glarea2.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
gl/glarea2.o: /usr/include/gtk/gtkspinbutton.h
gl/glarea2.o: /usr/include/gtk/gtkstatusbar.h
gl/glarea2.o: /usr/include/gtk/gtktearoffmenuitem.h
gl/glarea2.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
gl/glarea2.o: /usr/include/gtk/gtktipsquery.h /usr/include/gtk/gtktoolbar.h
gl/glarea2.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
gl/glarea2.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
gl/glarea2.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
gl/glarea2.o: /usr/include/gtk/gtkvscale.h /usr/include/gtk/gtkvseparator.h
gl/glarea2.o: /usr/include/gdk/gdkkeysyms.h /usr/include/gtkgl/gtkglarea.h
gl/glarea2.o: /usr/include/gtkgl/gdkgl.h /usr/X11R6/include/GL/gl.h
gl/glarea2.o: /usr/X11R6/include/GL/glext.h /usr/local/include/GL/glu.h
gl/glarea2.o: /usr/include/math.h /usr/include/bits/huge_val.h
gl/glarea2.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
gl/glarea2.o: gl/ctlpt.h gl/splines.h
gl/ctlpt.o: /usr/include/stdio.h /usr/include/features.h
gl/ctlpt.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/ctlpt.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gl/ctlpt.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/ctlpt.o: /usr/include/bits/types.h /usr/include/libio.h
gl/ctlpt.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
gl/ctlpt.o: /usr/include/stdlib.h /usr/include/sys/types.h
gl/ctlpt.o: /usr/include/time.h /usr/include/endian.h
gl/ctlpt.o: /usr/include/bits/endian.h /usr/include/sys/select.h
gl/ctlpt.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
gl/ctlpt.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
gl/ctlpt.o: /usr/include/math.h /usr/include/bits/huge_val.h
gl/ctlpt.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
gl/ctlpt.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/ctlpt.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
gl/ctlpt.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gl/ctlpt.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/ctlpt.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/ctlpt.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/ctlpt.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/ctlpt.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkobject.h
gl/ctlpt.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
gl/ctlpt.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
gl/ctlpt.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkaccellabel.h
gl/ctlpt.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
gl/ctlpt.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkadjustment.h
gl/ctlpt.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
gl/ctlpt.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
gl/ctlpt.o: /usr/include/gtk/gtkcontainer.h /usr/include/gtk/gtkaspectframe.h
gl/ctlpt.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
gl/ctlpt.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
gl/ctlpt.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
gl/ctlpt.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
gl/ctlpt.o: /usr/include/gtk/gtkmarshal.h /usr/include/gtk/gtkcheckbutton.h
gl/ctlpt.o: /usr/include/gtk/gtktogglebutton.h
gl/ctlpt.o: /usr/include/gtk/gtkcheckmenuitem.h
gl/ctlpt.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
gl/ctlpt.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
gl/ctlpt.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
gl/ctlpt.o: /usr/include/gtk/gtkvscrollbar.h /usr/include/gtk/gtkcolorsel.h
gl/ctlpt.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
gl/ctlpt.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
gl/ctlpt.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
gl/ctlpt.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
gl/ctlpt.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
gl/ctlpt.o: /usr/include/gtk/gtkeventbox.h /usr/include/gtk/gtkcombo.h
gl/ctlpt.o: /usr/include/gtk/gtkcompat.h /usr/include/gtk/gtkctree.h
gl/ctlpt.o: /usr/include/gtk/gtkcurve.h /usr/include/gtk/gtkdrawingarea.h
gl/ctlpt.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
gl/ctlpt.o: /usr/include/gtk/gtkselection.h /usr/include/gtk/gtkfeatures.h
gl/ctlpt.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
gl/ctlpt.o: /usr/include/gtk/gtkfontsel.h /usr/include/gtk/gtknotebook.h
gl/ctlpt.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
gl/ctlpt.o: /usr/include/gtk/gtkhandlebox.h /usr/include/gtk/gtkhbbox.h
gl/ctlpt.o: /usr/include/gtk/gtkhpaned.h /usr/include/gtk/gtkpaned.h
gl/ctlpt.o: /usr/include/gtk/gtkhruler.h /usr/include/gtk/gtkruler.h
gl/ctlpt.o: /usr/include/gtk/gtkhseparator.h /usr/include/gtk/gtkseparator.h
gl/ctlpt.o: /usr/include/gtk/gtkimage.h /usr/include/gtk/gtkinputdialog.h
gl/ctlpt.o: /usr/include/gtk/gtkitemfactory.h
gl/ctlpt.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
gl/ctlpt.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
gl/ctlpt.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
gl/ctlpt.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
gl/ctlpt.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
gl/ctlpt.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
gl/ctlpt.o: /usr/include/gtk/gtkprogressbar.h
gl/ctlpt.o: /usr/include/gtk/gtkradiobutton.h
gl/ctlpt.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/ctlpt.o: /usr/include/gtk/gtkscrolledwindow.h
gl/ctlpt.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
gl/ctlpt.o: /usr/include/gtk/gtkspinbutton.h /usr/include/gtk/gtkstatusbar.h
gl/ctlpt.o: /usr/include/gtk/gtktearoffmenuitem.h /usr/include/gtk/gtktext.h
gl/ctlpt.o: /usr/include/gtk/gtkthemes.h /usr/include/gtk/gtktipsquery.h
gl/ctlpt.o: /usr/include/gtk/gtktoolbar.h /usr/include/gtk/gtktooltips.h
gl/ctlpt.o: /usr/include/gtk/gtktree.h /usr/include/gtk/gtktreeitem.h
gl/ctlpt.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
gl/ctlpt.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
gl/ctlpt.o: /usr/include/gtk/gtkvseparator.h /usr/include/gdk/gdkkeysyms.h
gl/ctlpt.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h
gl/ctlpt.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
gl/ctlpt.o: /usr/local/include/GL/glu.h gl/ctlpt.h
gl/particle.o: /usr/include/stdio.h /usr/include/features.h
gl/particle.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
gl/particle.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
gl/particle.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
gl/particle.o: /usr/include/bits/types.h /usr/include/libio.h
gl/particle.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
gl/particle.o: /usr/include/stdlib.h /usr/include/sys/types.h
gl/particle.o: /usr/include/time.h /usr/include/endian.h
gl/particle.o: /usr/include/bits/endian.h /usr/include/sys/select.h
gl/particle.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
gl/particle.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
gl/particle.o: /usr/include/math.h /usr/include/bits/huge_val.h
gl/particle.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
gl/particle.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
gl/particle.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
gl/particle.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
gl/particle.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
gl/particle.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
gl/particle.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
gl/particle.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
gl/particle.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkobject.h
gl/particle.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
gl/particle.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
gl/particle.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkaccellabel.h
gl/particle.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
gl/particle.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkadjustment.h
gl/particle.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
gl/particle.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
gl/particle.o: /usr/include/gtk/gtkcontainer.h
gl/particle.o: /usr/include/gtk/gtkaspectframe.h /usr/include/gtk/gtkframe.h
gl/particle.o: /usr/include/gtk/gtkarrow.h /usr/include/gtk/gtkbindings.h
gl/particle.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
gl/particle.o: /usr/include/gtk/gtkbutton.h /usr/include/gtk/gtkcalendar.h
gl/particle.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
gl/particle.o: /usr/include/gtk/gtkcheckbutton.h
gl/particle.o: /usr/include/gtk/gtktogglebutton.h
gl/particle.o: /usr/include/gtk/gtkcheckmenuitem.h
gl/particle.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
gl/particle.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
gl/particle.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
gl/particle.o: /usr/include/gtk/gtkvscrollbar.h
gl/particle.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
gl/particle.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
gl/particle.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
gl/particle.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
gl/particle.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
gl/particle.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
gl/particle.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
gl/particle.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
gl/particle.o: /usr/include/gtk/gtkdrawingarea.h /usr/include/gtk/gtkdialog.h
gl/particle.o: /usr/include/gtk/gtkdnd.h /usr/include/gtk/gtkselection.h
gl/particle.o: /usr/include/gtk/gtkfeatures.h /usr/include/gtk/gtkfilesel.h
gl/particle.o: /usr/include/gtk/gtkfixed.h /usr/include/gtk/gtkfontsel.h
gl/particle.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
gl/particle.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
gl/particle.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
gl/particle.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
gl/particle.o: /usr/include/gtk/gtkruler.h /usr/include/gtk/gtkhseparator.h
gl/particle.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
gl/particle.o: /usr/include/gtk/gtkinputdialog.h
gl/particle.o: /usr/include/gtk/gtkitemfactory.h
gl/particle.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
gl/particle.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
gl/particle.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
gl/particle.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
gl/particle.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
gl/particle.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
gl/particle.o: /usr/include/gtk/gtkprogressbar.h
gl/particle.o: /usr/include/gtk/gtkradiobutton.h
gl/particle.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
gl/particle.o: /usr/include/gtk/gtkscrolledwindow.h
gl/particle.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
gl/particle.o: /usr/include/gtk/gtkspinbutton.h
gl/particle.o: /usr/include/gtk/gtkstatusbar.h
gl/particle.o: /usr/include/gtk/gtktearoffmenuitem.h
gl/particle.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
gl/particle.o: /usr/include/gtk/gtktipsquery.h /usr/include/gtk/gtktoolbar.h
gl/particle.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
gl/particle.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
gl/particle.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
gl/particle.o: /usr/include/gtk/gtkvscale.h /usr/include/gtk/gtkvseparator.h
gl/particle.o: /usr/include/gdk/gdkkeysyms.h /usr/include/gtkgl/gtkglarea.h
gl/particle.o: /usr/include/gtkgl/gdkgl.h /usr/X11R6/include/GL/gl.h
gl/particle.o: /usr/X11R6/include/GL/glext.h /usr/local/include/GL/glu.h
gl/particle.o: gl/particle.h math/matrix.h
misc/_string.o: /usr/include/ctype.h /usr/include/features.h
misc/_string.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
misc/_string.o: /usr/include/bits/types.h
misc/_string.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
misc/_string.o: /usr/include/endian.h /usr/include/bits/endian.h
misc/_string.o: /usr/include/string.h
misc/io.o: /usr/include/stdio.h /usr/include/features.h
misc/io.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
misc/io.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
misc/io.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
misc/io.o: /usr/include/bits/types.h /usr/include/libio.h
misc/io.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
misc/io.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
misc/io.o: /usr/include/endian.h /usr/include/bits/endian.h
misc/io.o: /usr/include/sys/select.h /usr/include/bits/select.h
misc/io.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
misc/io.o: /usr/include/alloca.h misc/io.h
misc/dynarray.o: /usr/include/glib.h /usr/lib/glib/include/glibconfig.h
misc/dynarray.o: /usr/include/limits.h /usr/include/features.h
misc/dynarray.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
misc/dynarray.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
misc/dynarray.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
misc/dynarray.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
misc/dynarray.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
misc/dynarray.o: misc/dynarray.h
misc/_time.o: misc/_time.h /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
misc/_time.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
misc/_time.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
misc/_time.o: /usr/include/features.h /usr/include/sys/cdefs.h
misc/_time.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
misc/_time.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
misc/_time.o: /usr/include/bits/posix2_lim.h
misc/_time.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
misc/_time.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
misc/_time.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
misc/_time.o: /usr/include/gtk/gtkaccelgroup.h /usr/include/gtk/gtkobject.h
misc/_time.o: /usr/include/gtk/gtkarg.h /usr/include/gtk/gtktypeutils.h
misc/_time.o: /usr/include/gtk/gtktypebuiltins.h /usr/include/gtk/gtkenums.h
misc/_time.o: /usr/include/gtk/gtkdebug.h /usr/include/gtk/gtkaccellabel.h
misc/_time.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
misc/_time.o: /usr/include/gtk/gtkwidget.h /usr/include/gtk/gtkadjustment.h
misc/_time.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
misc/_time.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
misc/_time.o: /usr/include/gtk/gtkcontainer.h
misc/_time.o: /usr/include/gtk/gtkaspectframe.h /usr/include/gtk/gtkframe.h
misc/_time.o: /usr/include/gtk/gtkarrow.h /usr/include/gtk/gtkbindings.h
misc/_time.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
misc/_time.o: /usr/include/gtk/gtkbutton.h /usr/include/gtk/gtkcalendar.h
misc/_time.o: /usr/include/gtk/gtksignal.h /usr/include/gtk/gtkmarshal.h
misc/_time.o: /usr/include/gtk/gtkcheckbutton.h
misc/_time.o: /usr/include/gtk/gtktogglebutton.h
misc/_time.o: /usr/include/gtk/gtkcheckmenuitem.h
misc/_time.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
misc/_time.o: /usr/include/gtk/gtkclist.h /usr/include/gtk/gtkhscrollbar.h
misc/_time.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
misc/_time.o: /usr/include/gtk/gtkvscrollbar.h /usr/include/gtk/gtkcolorsel.h
misc/_time.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
misc/_time.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
misc/_time.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
misc/_time.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
misc/_time.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
misc/_time.o: /usr/include/gtk/gtkeventbox.h /usr/include/gtk/gtkcombo.h
misc/_time.o: /usr/include/gtk/gtkcompat.h /usr/include/gtk/gtkctree.h
misc/_time.o: /usr/include/gtk/gtkcurve.h /usr/include/gtk/gtkdrawingarea.h
misc/_time.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
misc/_time.o: /usr/include/gtk/gtkselection.h /usr/include/gtk/gtkfeatures.h
misc/_time.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
misc/_time.o: /usr/include/gtk/gtkfontsel.h /usr/include/gtk/gtknotebook.h
misc/_time.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
misc/_time.o: /usr/include/gtk/gtkhandlebox.h /usr/include/gtk/gtkhbbox.h
misc/_time.o: /usr/include/gtk/gtkhpaned.h /usr/include/gtk/gtkpaned.h
misc/_time.o: /usr/include/gtk/gtkhruler.h /usr/include/gtk/gtkruler.h
misc/_time.o: /usr/include/gtk/gtkhseparator.h
misc/_time.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
misc/_time.o: /usr/include/gtk/gtkinputdialog.h
misc/_time.o: /usr/include/gtk/gtkitemfactory.h
misc/_time.o: /usr/include/gtk/gtkmenufactory.h /usr/include/gtk/gtklayout.h
misc/_time.o: /usr/include/gtk/gtklist.h /usr/include/gtk/gtklistitem.h
misc/_time.o: /usr/include/gtk/gtkmenu.h /usr/include/gtk/gtkmenushell.h
misc/_time.o: /usr/include/gtk/gtkmenubar.h /usr/include/gtk/gtkoptionmenu.h
misc/_time.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
misc/_time.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
misc/_time.o: /usr/include/gtk/gtkprogressbar.h
misc/_time.o: /usr/include/gtk/gtkradiobutton.h
misc/_time.o: /usr/include/gtk/gtkradiomenuitem.h /usr/include/gtk/gtkrc.h
misc/_time.o: /usr/include/gtk/gtkscrolledwindow.h
misc/_time.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
misc/_time.o: /usr/include/gtk/gtkspinbutton.h
misc/_time.o: /usr/include/gtk/gtkstatusbar.h
misc/_time.o: /usr/include/gtk/gtktearoffmenuitem.h
misc/_time.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
misc/_time.o: /usr/include/gtk/gtktipsquery.h /usr/include/gtk/gtktoolbar.h
misc/_time.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
misc/_time.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
misc/_time.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
misc/_time.o: /usr/include/gtk/gtkvscale.h /usr/include/gtk/gtkvseparator.h
misc/geometry.o: /usr/include/glib.h /usr/lib/glib/include/glibconfig.h
misc/geometry.o: /usr/include/limits.h /usr/include/features.h
misc/geometry.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
misc/geometry.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
misc/geometry.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
misc/geometry.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
misc/geometry.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
misc/geometry.o: /usr/include/math.h /usr/include/bits/huge_val.h
misc/geometry.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
misc/geometry.o: misc/dynarray.h math/matrix.h gl/aw.h misc/dynarray.h
misc/geometry.o: math/matrix.h
menus/filemenu.o: /usr/include/stdlib.h /usr/include/features.h
menus/filemenu.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
menus/filemenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
menus/filemenu.o: /usr/include/sys/types.h /usr/include/bits/types.h
menus/filemenu.o: /usr/include/time.h /usr/include/endian.h
menus/filemenu.o: /usr/include/bits/endian.h /usr/include/sys/select.h
menus/filemenu.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
menus/filemenu.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
menus/filemenu.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
menus/filemenu.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
menus/filemenu.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
menus/filemenu.o: /usr/include/bits/posix1_lim.h
menus/filemenu.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
menus/filemenu.o: /usr/include/bits/posix2_lim.h
menus/filemenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
menus/filemenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
menus/filemenu.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
menus/filemenu.o: /usr/include/gtk/gtkaccelgroup.h
menus/filemenu.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
menus/filemenu.o: /usr/include/gtk/gtktypeutils.h
menus/filemenu.o: /usr/include/gtk/gtktypebuiltins.h
menus/filemenu.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
menus/filemenu.o: /usr/include/gtk/gtkaccellabel.h
menus/filemenu.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
menus/filemenu.o: /usr/include/gtk/gtkwidget.h
menus/filemenu.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
menus/filemenu.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
menus/filemenu.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
menus/filemenu.o: /usr/include/gtk/gtkaspectframe.h
menus/filemenu.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
menus/filemenu.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
menus/filemenu.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
menus/filemenu.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
menus/filemenu.o: /usr/include/gtk/gtkmarshal.h
menus/filemenu.o: /usr/include/gtk/gtkcheckbutton.h
menus/filemenu.o: /usr/include/gtk/gtktogglebutton.h
menus/filemenu.o: /usr/include/gtk/gtkcheckmenuitem.h
menus/filemenu.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
menus/filemenu.o: /usr/include/gtk/gtkclist.h
menus/filemenu.o: /usr/include/gtk/gtkhscrollbar.h
menus/filemenu.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
menus/filemenu.o: /usr/include/gtk/gtkvscrollbar.h
menus/filemenu.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
menus/filemenu.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
menus/filemenu.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
menus/filemenu.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
menus/filemenu.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
menus/filemenu.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
menus/filemenu.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
menus/filemenu.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
menus/filemenu.o: /usr/include/gtk/gtkdrawingarea.h
menus/filemenu.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
menus/filemenu.o: /usr/include/gtk/gtkselection.h
menus/filemenu.o: /usr/include/gtk/gtkfeatures.h
menus/filemenu.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
menus/filemenu.o: /usr/include/gtk/gtkfontsel.h
menus/filemenu.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
menus/filemenu.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
menus/filemenu.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
menus/filemenu.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
menus/filemenu.o: /usr/include/gtk/gtkruler.h
menus/filemenu.o: /usr/include/gtk/gtkhseparator.h
menus/filemenu.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
menus/filemenu.o: /usr/include/gtk/gtkinputdialog.h
menus/filemenu.o: /usr/include/gtk/gtkitemfactory.h
menus/filemenu.o: /usr/include/gtk/gtkmenufactory.h
menus/filemenu.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
menus/filemenu.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
menus/filemenu.o: /usr/include/gtk/gtkmenushell.h
menus/filemenu.o: /usr/include/gtk/gtkmenubar.h
menus/filemenu.o: /usr/include/gtk/gtkoptionmenu.h
menus/filemenu.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
menus/filemenu.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
menus/filemenu.o: /usr/include/gtk/gtkprogressbar.h
menus/filemenu.o: /usr/include/gtk/gtkradiobutton.h
menus/filemenu.o: /usr/include/gtk/gtkradiomenuitem.h
menus/filemenu.o: /usr/include/gtk/gtkrc.h
menus/filemenu.o: /usr/include/gtk/gtkscrolledwindow.h
menus/filemenu.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
menus/filemenu.o: /usr/include/gtk/gtkspinbutton.h
menus/filemenu.o: /usr/include/gtk/gtkstatusbar.h
menus/filemenu.o: /usr/include/gtk/gtktearoffmenuitem.h
menus/filemenu.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
menus/filemenu.o: /usr/include/gtk/gtktipsquery.h
menus/filemenu.o: /usr/include/gtk/gtktoolbar.h
menus/filemenu.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
menus/filemenu.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
menus/filemenu.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
menus/filemenu.o: /usr/include/gtk/gtkvscale.h
menus/filemenu.o: /usr/include/gtk/gtkvseparator.h gl/aw.h misc/dynarray.h
menus/filemenu.o: math/matrix.h gl/gl.h /usr/X11R6/include/GL/gl.h
menus/filemenu.o: /usr/X11R6/include/GL/glext.h
menus/filemenu.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h
menus/filemenu.o: gl/aw.h gl/particle.h math/matrix.h gl/normals.h
menus/filemenu.o: widgets/status.h widgets/window.h gtkmisc.h
menus/editmenu.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
menus/editmenu.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
menus/editmenu.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
menus/editmenu.o: /usr/include/features.h /usr/include/sys/cdefs.h
menus/editmenu.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
menus/editmenu.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
menus/editmenu.o: /usr/include/bits/posix2_lim.h
menus/editmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
menus/editmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
menus/editmenu.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
menus/editmenu.o: /usr/include/gtk/gtkaccelgroup.h
menus/editmenu.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
menus/editmenu.o: /usr/include/gtk/gtktypeutils.h
menus/editmenu.o: /usr/include/gtk/gtktypebuiltins.h
menus/editmenu.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
menus/editmenu.o: /usr/include/gtk/gtkaccellabel.h
menus/editmenu.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
menus/editmenu.o: /usr/include/gtk/gtkwidget.h
menus/editmenu.o: /usr/include/gtk/gtkadjustment.h /usr/include/gtk/gtkdata.h
menus/editmenu.o: /usr/include/gtk/gtkstyle.h /usr/include/gtk/gtkalignment.h
menus/editmenu.o: /usr/include/gtk/gtkbin.h /usr/include/gtk/gtkcontainer.h
menus/editmenu.o: /usr/include/gtk/gtkaspectframe.h
menus/editmenu.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
menus/editmenu.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
menus/editmenu.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
menus/editmenu.o: /usr/include/gtk/gtkcalendar.h /usr/include/gtk/gtksignal.h
menus/editmenu.o: /usr/include/gtk/gtkmarshal.h
menus/editmenu.o: /usr/include/gtk/gtkcheckbutton.h
menus/editmenu.o: /usr/include/gtk/gtktogglebutton.h
menus/editmenu.o: /usr/include/gtk/gtkcheckmenuitem.h
menus/editmenu.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
menus/editmenu.o: /usr/include/gtk/gtkclist.h
menus/editmenu.o: /usr/include/gtk/gtkhscrollbar.h
menus/editmenu.o: /usr/include/gtk/gtkscrollbar.h /usr/include/gtk/gtkrange.h
menus/editmenu.o: /usr/include/gtk/gtkvscrollbar.h
menus/editmenu.o: /usr/include/gtk/gtkcolorsel.h /usr/include/gtk/gtkwindow.h
menus/editmenu.o: /usr/include/gtk/gtkvbox.h /usr/include/gtk/gtkpreview.h
menus/editmenu.o: /usr/include/gtk/gtkentry.h /usr/include/gtk/gtkeditable.h
menus/editmenu.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
menus/editmenu.o: /usr/include/gtk/gtkscale.h /usr/include/gtk/gtkhscale.h
menus/editmenu.o: /usr/include/gtk/gtktable.h /usr/include/gtk/gtkeventbox.h
menus/editmenu.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
menus/editmenu.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
menus/editmenu.o: /usr/include/gtk/gtkdrawingarea.h
menus/editmenu.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
menus/editmenu.o: /usr/include/gtk/gtkselection.h
menus/editmenu.o: /usr/include/gtk/gtkfeatures.h
menus/editmenu.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
menus/editmenu.o: /usr/include/gtk/gtkfontsel.h
menus/editmenu.o: /usr/include/gtk/gtknotebook.h /usr/include/gtk/gtkgamma.h
menus/editmenu.o: /usr/include/gtk/gtkgc.h /usr/include/gtk/gtkhandlebox.h
menus/editmenu.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
menus/editmenu.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
menus/editmenu.o: /usr/include/gtk/gtkruler.h
menus/editmenu.o: /usr/include/gtk/gtkhseparator.h
menus/editmenu.o: /usr/include/gtk/gtkseparator.h /usr/include/gtk/gtkimage.h
menus/editmenu.o: /usr/include/gtk/gtkinputdialog.h
menus/editmenu.o: /usr/include/gtk/gtkitemfactory.h
menus/editmenu.o: /usr/include/gtk/gtkmenufactory.h
menus/editmenu.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
menus/editmenu.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
menus/editmenu.o: /usr/include/gtk/gtkmenushell.h
menus/editmenu.o: /usr/include/gtk/gtkmenubar.h
menus/editmenu.o: /usr/include/gtk/gtkoptionmenu.h
menus/editmenu.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
menus/editmenu.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
menus/editmenu.o: /usr/include/gtk/gtkprogressbar.h
menus/editmenu.o: /usr/include/gtk/gtkradiobutton.h
menus/editmenu.o: /usr/include/gtk/gtkradiomenuitem.h
menus/editmenu.o: /usr/include/gtk/gtkrc.h
menus/editmenu.o: /usr/include/gtk/gtkscrolledwindow.h
menus/editmenu.o: /usr/include/gtk/gtkviewport.h /usr/include/gtk/gtksocket.h
menus/editmenu.o: /usr/include/gtk/gtkspinbutton.h
menus/editmenu.o: /usr/include/gtk/gtkstatusbar.h
menus/editmenu.o: /usr/include/gtk/gtktearoffmenuitem.h
menus/editmenu.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
menus/editmenu.o: /usr/include/gtk/gtktipsquery.h
menus/editmenu.o: /usr/include/gtk/gtktoolbar.h
menus/editmenu.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
menus/editmenu.o: /usr/include/gtk/gtktreeitem.h /usr/include/gtk/gtkvbbox.h
menus/editmenu.o: /usr/include/gtk/gtkvpaned.h /usr/include/gtk/gtkvruler.h
menus/editmenu.o: /usr/include/gtk/gtkvscale.h
menus/editmenu.o: /usr/include/gtk/gtkvseparator.h
menus/optionmenu.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
menus/optionmenu.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
menus/optionmenu.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
menus/optionmenu.o: /usr/include/features.h /usr/include/sys/cdefs.h
menus/optionmenu.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
menus/optionmenu.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
menus/optionmenu.o: /usr/include/bits/posix2_lim.h
menus/optionmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
menus/optionmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
menus/optionmenu.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
menus/optionmenu.o: /usr/include/gtk/gtkaccelgroup.h
menus/optionmenu.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
menus/optionmenu.o: /usr/include/gtk/gtktypeutils.h
menus/optionmenu.o: /usr/include/gtk/gtktypebuiltins.h
menus/optionmenu.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
menus/optionmenu.o: /usr/include/gtk/gtkaccellabel.h
menus/optionmenu.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
menus/optionmenu.o: /usr/include/gtk/gtkwidget.h
menus/optionmenu.o: /usr/include/gtk/gtkadjustment.h
menus/optionmenu.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
menus/optionmenu.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
menus/optionmenu.o: /usr/include/gtk/gtkcontainer.h
menus/optionmenu.o: /usr/include/gtk/gtkaspectframe.h
menus/optionmenu.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
menus/optionmenu.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
menus/optionmenu.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
menus/optionmenu.o: /usr/include/gtk/gtkcalendar.h
menus/optionmenu.o: /usr/include/gtk/gtksignal.h
menus/optionmenu.o: /usr/include/gtk/gtkmarshal.h
menus/optionmenu.o: /usr/include/gtk/gtkcheckbutton.h
menus/optionmenu.o: /usr/include/gtk/gtktogglebutton.h
menus/optionmenu.o: /usr/include/gtk/gtkcheckmenuitem.h
menus/optionmenu.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
menus/optionmenu.o: /usr/include/gtk/gtkclist.h
menus/optionmenu.o: /usr/include/gtk/gtkhscrollbar.h
menus/optionmenu.o: /usr/include/gtk/gtkscrollbar.h
menus/optionmenu.o: /usr/include/gtk/gtkrange.h
menus/optionmenu.o: /usr/include/gtk/gtkvscrollbar.h
menus/optionmenu.o: /usr/include/gtk/gtkcolorsel.h
menus/optionmenu.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
menus/optionmenu.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
menus/optionmenu.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
menus/optionmenu.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
menus/optionmenu.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
menus/optionmenu.o: /usr/include/gtk/gtkeventbox.h
menus/optionmenu.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
menus/optionmenu.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
menus/optionmenu.o: /usr/include/gtk/gtkdrawingarea.h
menus/optionmenu.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
menus/optionmenu.o: /usr/include/gtk/gtkselection.h
menus/optionmenu.o: /usr/include/gtk/gtkfeatures.h
menus/optionmenu.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
menus/optionmenu.o: /usr/include/gtk/gtkfontsel.h
menus/optionmenu.o: /usr/include/gtk/gtknotebook.h
menus/optionmenu.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
menus/optionmenu.o: /usr/include/gtk/gtkhandlebox.h
menus/optionmenu.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
menus/optionmenu.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
menus/optionmenu.o: /usr/include/gtk/gtkruler.h
menus/optionmenu.o: /usr/include/gtk/gtkhseparator.h
menus/optionmenu.o: /usr/include/gtk/gtkseparator.h
menus/optionmenu.o: /usr/include/gtk/gtkimage.h
menus/optionmenu.o: /usr/include/gtk/gtkinputdialog.h
menus/optionmenu.o: /usr/include/gtk/gtkitemfactory.h
menus/optionmenu.o: /usr/include/gtk/gtkmenufactory.h
menus/optionmenu.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
menus/optionmenu.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
menus/optionmenu.o: /usr/include/gtk/gtkmenushell.h
menus/optionmenu.o: /usr/include/gtk/gtkmenubar.h
menus/optionmenu.o: /usr/include/gtk/gtkoptionmenu.h
menus/optionmenu.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
menus/optionmenu.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
menus/optionmenu.o: /usr/include/gtk/gtkprogressbar.h
menus/optionmenu.o: /usr/include/gtk/gtkradiobutton.h
menus/optionmenu.o: /usr/include/gtk/gtkradiomenuitem.h
menus/optionmenu.o: /usr/include/gtk/gtkrc.h
menus/optionmenu.o: /usr/include/gtk/gtkscrolledwindow.h
menus/optionmenu.o: /usr/include/gtk/gtkviewport.h
menus/optionmenu.o: /usr/include/gtk/gtksocket.h
menus/optionmenu.o: /usr/include/gtk/gtkspinbutton.h
menus/optionmenu.o: /usr/include/gtk/gtkstatusbar.h
menus/optionmenu.o: /usr/include/gtk/gtktearoffmenuitem.h
menus/optionmenu.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
menus/optionmenu.o: /usr/include/gtk/gtktipsquery.h
menus/optionmenu.o: /usr/include/gtk/gtktoolbar.h
menus/optionmenu.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
menus/optionmenu.o: /usr/include/gtk/gtktreeitem.h
menus/optionmenu.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
menus/optionmenu.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
menus/optionmenu.o: /usr/include/gtk/gtkvseparator.h options.h gl/gl.h
menus/optionmenu.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
menus/optionmenu.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h
menus/optionmenu.o: gl/aw.h misc/dynarray.h math/matrix.h gl/particle.h
menus/optionmenu.o: math/matrix.h
menus/objectmenu.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
menus/objectmenu.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
menus/objectmenu.o: /usr/lib/glib/include/glibconfig.h /usr/include/limits.h
menus/objectmenu.o: /usr/include/features.h /usr/include/sys/cdefs.h
menus/objectmenu.o: /usr/include/gnu/stubs.h /usr/include/bits/posix1_lim.h
menus/objectmenu.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
menus/objectmenu.o: /usr/include/bits/posix2_lim.h
menus/objectmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
menus/objectmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
menus/objectmenu.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
menus/objectmenu.o: /usr/include/gtk/gtkaccelgroup.h
menus/objectmenu.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
menus/objectmenu.o: /usr/include/gtk/gtktypeutils.h
menus/objectmenu.o: /usr/include/gtk/gtktypebuiltins.h
menus/objectmenu.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
menus/objectmenu.o: /usr/include/gtk/gtkaccellabel.h
menus/objectmenu.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
menus/objectmenu.o: /usr/include/gtk/gtkwidget.h
menus/objectmenu.o: /usr/include/gtk/gtkadjustment.h
menus/objectmenu.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
menus/objectmenu.o: /usr/include/gtk/gtkalignment.h /usr/include/gtk/gtkbin.h
menus/objectmenu.o: /usr/include/gtk/gtkcontainer.h
menus/objectmenu.o: /usr/include/gtk/gtkaspectframe.h
menus/objectmenu.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
menus/objectmenu.o: /usr/include/gtk/gtkbindings.h /usr/include/gtk/gtkbox.h
menus/objectmenu.o: /usr/include/gtk/gtkbbox.h /usr/include/gtk/gtkbutton.h
menus/objectmenu.o: /usr/include/gtk/gtkcalendar.h
menus/objectmenu.o: /usr/include/gtk/gtksignal.h
menus/objectmenu.o: /usr/include/gtk/gtkmarshal.h
menus/objectmenu.o: /usr/include/gtk/gtkcheckbutton.h
menus/objectmenu.o: /usr/include/gtk/gtktogglebutton.h
menus/objectmenu.o: /usr/include/gtk/gtkcheckmenuitem.h
menus/objectmenu.o: /usr/include/gtk/gtkmenuitem.h /usr/include/gtk/gtkitem.h
menus/objectmenu.o: /usr/include/gtk/gtkclist.h
menus/objectmenu.o: /usr/include/gtk/gtkhscrollbar.h
menus/objectmenu.o: /usr/include/gtk/gtkscrollbar.h
menus/objectmenu.o: /usr/include/gtk/gtkrange.h
menus/objectmenu.o: /usr/include/gtk/gtkvscrollbar.h
menus/objectmenu.o: /usr/include/gtk/gtkcolorsel.h
menus/objectmenu.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
menus/objectmenu.o: /usr/include/gtk/gtkpreview.h /usr/include/gtk/gtkentry.h
menus/objectmenu.o: /usr/include/gtk/gtkeditable.h /usr/include/gtk/gtkhbox.h
menus/objectmenu.o: /usr/include/gtk/gtkmain.h /usr/include/gtk/gtkscale.h
menus/objectmenu.o: /usr/include/gtk/gtkhscale.h /usr/include/gtk/gtktable.h
menus/objectmenu.o: /usr/include/gtk/gtkeventbox.h
menus/objectmenu.o: /usr/include/gtk/gtkcombo.h /usr/include/gtk/gtkcompat.h
menus/objectmenu.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
menus/objectmenu.o: /usr/include/gtk/gtkdrawingarea.h
menus/objectmenu.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
menus/objectmenu.o: /usr/include/gtk/gtkselection.h
menus/objectmenu.o: /usr/include/gtk/gtkfeatures.h
menus/objectmenu.o: /usr/include/gtk/gtkfilesel.h /usr/include/gtk/gtkfixed.h
menus/objectmenu.o: /usr/include/gtk/gtkfontsel.h
menus/objectmenu.o: /usr/include/gtk/gtknotebook.h
menus/objectmenu.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
menus/objectmenu.o: /usr/include/gtk/gtkhandlebox.h
menus/objectmenu.o: /usr/include/gtk/gtkhbbox.h /usr/include/gtk/gtkhpaned.h
menus/objectmenu.o: /usr/include/gtk/gtkpaned.h /usr/include/gtk/gtkhruler.h
menus/objectmenu.o: /usr/include/gtk/gtkruler.h
menus/objectmenu.o: /usr/include/gtk/gtkhseparator.h
menus/objectmenu.o: /usr/include/gtk/gtkseparator.h
menus/objectmenu.o: /usr/include/gtk/gtkimage.h
menus/objectmenu.o: /usr/include/gtk/gtkinputdialog.h
menus/objectmenu.o: /usr/include/gtk/gtkitemfactory.h
menus/objectmenu.o: /usr/include/gtk/gtkmenufactory.h
menus/objectmenu.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
menus/objectmenu.o: /usr/include/gtk/gtklistitem.h /usr/include/gtk/gtkmenu.h
menus/objectmenu.o: /usr/include/gtk/gtkmenushell.h
menus/objectmenu.o: /usr/include/gtk/gtkmenubar.h
menus/objectmenu.o: /usr/include/gtk/gtkoptionmenu.h
menus/objectmenu.o: /usr/include/gtk/gtkpacker.h /usr/include/gtk/gtkpixmap.h
menus/objectmenu.o: /usr/include/gtk/gtkplug.h /usr/include/gtk/gtkprogress.h
menus/objectmenu.o: /usr/include/gtk/gtkprogressbar.h
menus/objectmenu.o: /usr/include/gtk/gtkradiobutton.h
menus/objectmenu.o: /usr/include/gtk/gtkradiomenuitem.h
menus/objectmenu.o: /usr/include/gtk/gtkrc.h
menus/objectmenu.o: /usr/include/gtk/gtkscrolledwindow.h
menus/objectmenu.o: /usr/include/gtk/gtkviewport.h
menus/objectmenu.o: /usr/include/gtk/gtksocket.h
menus/objectmenu.o: /usr/include/gtk/gtkspinbutton.h
menus/objectmenu.o: /usr/include/gtk/gtkstatusbar.h
menus/objectmenu.o: /usr/include/gtk/gtktearoffmenuitem.h
menus/objectmenu.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
menus/objectmenu.o: /usr/include/gtk/gtktipsquery.h
menus/objectmenu.o: /usr/include/gtk/gtktoolbar.h
menus/objectmenu.o: /usr/include/gtk/gtktooltips.h /usr/include/gtk/gtktree.h
menus/objectmenu.o: /usr/include/gtk/gtktreeitem.h
menus/objectmenu.o: /usr/include/gtk/gtkvbbox.h /usr/include/gtk/gtkvpaned.h
menus/objectmenu.o: /usr/include/gtk/gtkvruler.h /usr/include/gtk/gtkvscale.h
menus/objectmenu.o: /usr/include/gtk/gtkvseparator.h
menus/objectmenu.o: /usr/X11R6/include/GL/gl.h /usr/X11R6/include/GL/glext.h
menus/objectmenu.o: /usr/include/sys/stat.h /usr/include/bits/types.h
menus/objectmenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
menus/objectmenu.o: /usr/include/bits/stat.h gl/gl.h
menus/objectmenu.o: /usr/include/gtkgl/gtkglarea.h /usr/include/gtkgl/gdkgl.h
menus/objectmenu.o: gl/aw.h misc/dynarray.h math/matrix.h gl/particle.h
menus/objectmenu.o: math/matrix.h gtkmisc.h menus/objectmenu.h gl/textures.h
menus/objectmenu.o: gl/glarea2.h
menus/particlemenu.o: /usr/include/stdlib.h /usr/include/features.h
menus/particlemenu.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
menus/particlemenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
menus/particlemenu.o: /usr/include/sys/types.h /usr/include/bits/types.h
menus/particlemenu.o: /usr/include/time.h /usr/include/endian.h
menus/particlemenu.o: /usr/include/bits/endian.h /usr/include/sys/select.h
menus/particlemenu.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
menus/particlemenu.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
menus/particlemenu.o: /usr/include/gtk/gtk.h /usr/include/gdk/gdk.h
menus/particlemenu.o: /usr/include/gdk/gdktypes.h /usr/include/glib.h
menus/particlemenu.o: /usr/lib/glib/include/glibconfig.h
menus/particlemenu.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
menus/particlemenu.o: /usr/include/bits/local_lim.h
menus/particlemenu.o: /usr/include/linux/limits.h
menus/particlemenu.o: /usr/include/bits/posix2_lim.h
menus/particlemenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
menus/particlemenu.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stdarg.h
menus/particlemenu.o: /usr/include/gdk/gdkcursors.h /usr/include/gdk/gdkrgb.h
menus/particlemenu.o: /usr/include/gtk/gtkaccelgroup.h
menus/particlemenu.o: /usr/include/gtk/gtkobject.h /usr/include/gtk/gtkarg.h
menus/particlemenu.o: /usr/include/gtk/gtktypeutils.h
menus/particlemenu.o: /usr/include/gtk/gtktypebuiltins.h
menus/particlemenu.o: /usr/include/gtk/gtkenums.h /usr/include/gtk/gtkdebug.h
menus/particlemenu.o: /usr/include/gtk/gtkaccellabel.h
menus/particlemenu.o: /usr/include/gtk/gtklabel.h /usr/include/gtk/gtkmisc.h
menus/particlemenu.o: /usr/include/gtk/gtkwidget.h
menus/particlemenu.o: /usr/include/gtk/gtkadjustment.h
menus/particlemenu.o: /usr/include/gtk/gtkdata.h /usr/include/gtk/gtkstyle.h
menus/particlemenu.o: /usr/include/gtk/gtkalignment.h
menus/particlemenu.o: /usr/include/gtk/gtkbin.h
menus/particlemenu.o: /usr/include/gtk/gtkcontainer.h
menus/particlemenu.o: /usr/include/gtk/gtkaspectframe.h
menus/particlemenu.o: /usr/include/gtk/gtkframe.h /usr/include/gtk/gtkarrow.h
menus/particlemenu.o: /usr/include/gtk/gtkbindings.h
menus/particlemenu.o: /usr/include/gtk/gtkbox.h /usr/include/gtk/gtkbbox.h
menus/particlemenu.o: /usr/include/gtk/gtkbutton.h
menus/particlemenu.o: /usr/include/gtk/gtkcalendar.h
menus/particlemenu.o: /usr/include/gtk/gtksignal.h
menus/particlemenu.o: /usr/include/gtk/gtkmarshal.h
menus/particlemenu.o: /usr/include/gtk/gtkcheckbutton.h
menus/particlemenu.o: /usr/include/gtk/gtktogglebutton.h
menus/particlemenu.o: /usr/include/gtk/gtkcheckmenuitem.h
menus/particlemenu.o: /usr/include/gtk/gtkmenuitem.h
menus/particlemenu.o: /usr/include/gtk/gtkitem.h /usr/include/gtk/gtkclist.h
menus/particlemenu.o: /usr/include/gtk/gtkhscrollbar.h
menus/particlemenu.o: /usr/include/gtk/gtkscrollbar.h
menus/particlemenu.o: /usr/include/gtk/gtkrange.h
menus/particlemenu.o: /usr/include/gtk/gtkvscrollbar.h
menus/particlemenu.o: /usr/include/gtk/gtkcolorsel.h
menus/particlemenu.o: /usr/include/gtk/gtkwindow.h /usr/include/gtk/gtkvbox.h
menus/particlemenu.o: /usr/include/gtk/gtkpreview.h
menus/particlemenu.o: /usr/include/gtk/gtkentry.h
menus/particlemenu.o: /usr/include/gtk/gtkeditable.h
menus/particlemenu.o: /usr/include/gtk/gtkhbox.h /usr/include/gtk/gtkmain.h
menus/particlemenu.o: /usr/include/gtk/gtkscale.h
menus/particlemenu.o: /usr/include/gtk/gtkhscale.h
menus/particlemenu.o: /usr/include/gtk/gtktable.h
menus/particlemenu.o: /usr/include/gtk/gtkeventbox.h
menus/particlemenu.o: /usr/include/gtk/gtkcombo.h
menus/particlemenu.o: /usr/include/gtk/gtkcompat.h
menus/particlemenu.o: /usr/include/gtk/gtkctree.h /usr/include/gtk/gtkcurve.h
menus/particlemenu.o: /usr/include/gtk/gtkdrawingarea.h
menus/particlemenu.o: /usr/include/gtk/gtkdialog.h /usr/include/gtk/gtkdnd.h
menus/particlemenu.o: /usr/include/gtk/gtkselection.h
menus/particlemenu.o: /usr/include/gtk/gtkfeatures.h
menus/particlemenu.o: /usr/include/gtk/gtkfilesel.h
menus/particlemenu.o: /usr/include/gtk/gtkfixed.h
menus/particlemenu.o: /usr/include/gtk/gtkfontsel.h
menus/particlemenu.o: /usr/include/gtk/gtknotebook.h
menus/particlemenu.o: /usr/include/gtk/gtkgamma.h /usr/include/gtk/gtkgc.h
menus/particlemenu.o: /usr/include/gtk/gtkhandlebox.h
menus/particlemenu.o: /usr/include/gtk/gtkhbbox.h
menus/particlemenu.o: /usr/include/gtk/gtkhpaned.h
menus/particlemenu.o: /usr/include/gtk/gtkpaned.h
menus/particlemenu.o: /usr/include/gtk/gtkhruler.h
menus/particlemenu.o: /usr/include/gtk/gtkruler.h
menus/particlemenu.o: /usr/include/gtk/gtkhseparator.h
menus/particlemenu.o: /usr/include/gtk/gtkseparator.h
menus/particlemenu.o: /usr/include/gtk/gtkimage.h
menus/particlemenu.o: /usr/include/gtk/gtkinputdialog.h
menus/particlemenu.o: /usr/include/gtk/gtkitemfactory.h
menus/particlemenu.o: /usr/include/gtk/gtkmenufactory.h
menus/particlemenu.o: /usr/include/gtk/gtklayout.h /usr/include/gtk/gtklist.h
menus/particlemenu.o: /usr/include/gtk/gtklistitem.h
menus/particlemenu.o: /usr/include/gtk/gtkmenu.h
menus/particlemenu.o: /usr/include/gtk/gtkmenushell.h
menus/particlemenu.o: /usr/include/gtk/gtkmenubar.h
menus/particlemenu.o: /usr/include/gtk/gtkoptionmenu.h
menus/particlemenu.o: /usr/include/gtk/gtkpacker.h
menus/particlemenu.o: /usr/include/gtk/gtkpixmap.h /usr/include/gtk/gtkplug.h
menus/particlemenu.o: /usr/include/gtk/gtkprogress.h
menus/particlemenu.o: /usr/include/gtk/gtkprogressbar.h
menus/particlemenu.o: /usr/include/gtk/gtkradiobutton.h
menus/particlemenu.o: /usr/include/gtk/gtkradiomenuitem.h
menus/particlemenu.o: /usr/include/gtk/gtkrc.h
menus/particlemenu.o: /usr/include/gtk/gtkscrolledwindow.h
menus/particlemenu.o: /usr/include/gtk/gtkviewport.h
menus/particlemenu.o: /usr/include/gtk/gtksocket.h
menus/particlemenu.o: /usr/include/gtk/gtkspinbutton.h
menus/particlemenu.o: /usr/include/gtk/gtkstatusbar.h
menus/particlemenu.o: /usr/include/gtk/gtktearoffmenuitem.h
menus/particlemenu.o: /usr/include/gtk/gtktext.h /usr/include/gtk/gtkthemes.h
menus/particlemenu.o: /usr/include/gtk/gtktipsquery.h
menus/particlemenu.o: /usr/include/gtk/gtktoolbar.h
menus/particlemenu.o: /usr/include/gtk/gtktooltips.h
menus/particlemenu.o: /usr/include/gtk/gtktree.h
menus/particlemenu.o: /usr/include/gtk/gtktreeitem.h
menus/particlemenu.o: /usr/include/gtk/gtkvbbox.h
menus/particlemenu.o: /usr/include/gtk/gtkvpaned.h
menus/particlemenu.o: /usr/include/gtk/gtkvruler.h
menus/particlemenu.o: /usr/include/gtk/gtkvscale.h
menus/particlemenu.o: /usr/include/gtk/gtkvseparator.h widgets/widgets.h
menus/particlemenu.o: /usr/include/glade/glade.h
menus/particlemenu.o: /usr/include/glade/glade-xml.h ./misc/_time.h
menus/particlemenu.o: gl/particle.h /usr/X11R6/include/GL/gl.h
menus/particlemenu.o: /usr/X11R6/include/GL/glext.h math/matrix.h gl/gl.h
menus/particlemenu.o: /usr/include/gtkgl/gtkglarea.h
menus/particlemenu.o: /usr/include/gtkgl/gdkgl.h gl/aw.h misc/dynarray.h
menus/particlemenu.o: math/matrix.h gl/particle.h gl/trackball/trackball.h
menus/particlemenu.o: misc/geometry.h gl/aw.h /usr/include/math.h
menus/particlemenu.o: /usr/include/bits/huge_val.h
menus/particlemenu.o: /usr/include/bits/mathdef.h
menus/particlemenu.o: /usr/include/bits/mathcalls.h
math/matrix.o: /usr/include/stdlib.h /usr/include/features.h
math/matrix.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
math/matrix.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/stddef.h
math/matrix.o: /usr/include/sys/types.h /usr/include/bits/types.h
math/matrix.o: /usr/include/time.h /usr/include/endian.h
math/matrix.o: /usr/include/bits/endian.h /usr/include/sys/select.h
math/matrix.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
math/matrix.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
math/matrix.o: /usr/include/string.h /usr/include/math.h
math/matrix.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
math/matrix.o: /usr/include/bits/mathcalls.h
math/matrix.o: /usr/lib/gcc-lib/i586-mandrake-linux/2.95.3/include/float.h
math/matrix.o: math/matrix.h
