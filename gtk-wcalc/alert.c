/* $Id: alert.c,v 1.1 2001/10/05 00:50:22 dan Exp $ */

/*
 * Copyright (c) 2001, 2002 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "config.h"

#include <gtk/gtk.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "alert.h"
#include "pixmaps/alert_fig.xpm"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ABOUT_TEXT \
"WaveCalc\n" \
"\n" \
"Version " \
VER \
"\n"\
"A transmission line calculator.\n" \
"\n" \
"Written by,\n" \
"Dan McMahill, <mcmahill@alum.mit.edu>\n" \
"\n" \
"WaveCalc is copyright (C) 1999, 2000, 2001\n" \
"Dan McMahill."


static void ok_pressed (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);
}

#define MAX_MSG 511

void alert(const char *fmt,...)
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *window;
  GtkWidget *hbox;

  /* stuff for the message */
  char msg[MAX_MSG+1];
  va_list ap;

  /* stuff for the picture */
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;    

  /* extract the warning string */
  va_start(ap,fmt);
#ifdef HAVE_VSNPRINTF
  vsnprintf(msg,MAX_MSG,fmt,ap);
#else
  vsprintf(msg,fmt,ap);
#endif
  va_end(ap);

  /* create the dialog box */
  window = gtk_dialog_new();
  
  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), "Wcalc:  Warning!");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /*
   * The Top 1/2 of the window
   */

  /* Make a hbox */
  hbox = gtk_hbox_new(FALSE,10);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox),
		      hbox, FALSE, FALSE, 0);
  gtk_widget_show(hbox);

  /* add the picture */
  gtk_widget_realize(window);
  style = gtk_widget_get_style( window );
  pixmap = gdk_pixmap_create_from_xpm_d( window->window, 
					 &mask,
					 &style->bg[GTK_STATE_NORMAL],
					 (gchar **) alert_fig);
    					
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (hbox),
		      pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    

  /* add the text to the window */
  label = gtk_label_new (msg);
  gtk_box_pack_start (GTK_BOX (hbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  /*
   * The Action Area
   */

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label ("Ok");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));
  
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);
  gtk_window_set_focus(GTK_WINDOW(window),button);

  /* show it */
  gtk_widget_show (window);

  /* and BEEEP */
  gdk_beep();
}




