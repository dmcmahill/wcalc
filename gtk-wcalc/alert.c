/*
 * Copyright (C) 2001, 2002, 2003, 2005, 2006, 2021 Dan McMahill
 * All rights reserved.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
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

#include "gettext.h"

#ifndef _
#define _(String) gettext (String)
#endif
#ifndef gettext_noop
#define gettext_noop(String) (String)
#endif
#ifndef N_
#define N_(String) gettext_noop (String)
#endif

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#define ABOUT_TEXT \
"Wcalc\n" \
"\n" \
"Version " \
VER \
"\n"\
"A transmission line calculator.\n" \
"\n" \
"Written by,\n" \
"Dan McMahill, <mcmahill@alum.mit.edu>\n" \
"\n" \
"Wcalc is copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006\n" \
"2007, 2008, 2009 Dan McMahill."


/*
 * *********************************************************
 * File Globals
 * *********************************************************
 */

static GtkWidget *alert_window = NULL;
static GtkWidget *alert_view;
static GtkWidget *alert_scroll;

/*
 * *********************************************************
 * Prototypes
 * *********************************************************
 */


/* max number of lines in alert window */
#define MAX_LINES 20

/*
 * The top level delete/destroy signal and event callbacks
 */
static gint alert_delete_event( GtkWidget *widget,
				GdkEvent *event,
				gpointer data )
{

  gtk_widget_hide(alert_window);
  return TRUE;
}

static gint alert_destroy_event (GtkWidget *widget,
				 GdkEvent *event,
				 gpointer data)
{

  gtk_widget_hide(alert_window);
  return TRUE;
}

static void ok_pressed (GtkWidget *w, GtkWidget *window)
{

  /* hide the window */
  gtk_widget_hide(window);
}

#define MAX_MSG 8192

static gint alert_window_create()
{
  /* widgets for the dialog box */
  GtkWidget *button;
  GtkWidget *hbox;

  /* stuff for the picture */
  GdkPixbuf *pixbuf;
  GtkWidget *image;

  GtkTextBuffer *buffer;

  /* create the dialog box */
  alert_window = gtk_dialog_new();

  /* Window Manager "delete" */
  g_signal_connect( G_OBJECT( alert_window ), "delete_event",
                    G_CALLBACK(alert_delete_event),
                    NULL);

  /* Window Manager  "destroy" */
  g_signal_connect( G_OBJECT( alert_window ), "destroy_event",
                    G_CALLBACK(alert_destroy_event),
                    NULL);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (alert_window), _("Wcalc:  Warning!"));
  gtk_container_set_border_width(GTK_CONTAINER(alert_window),10);

  /*
   * The Top 1/2 of the window
   */

  /* Make a hbox */
  hbox = gtk_hbox_new(FALSE, 10);
  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_content_area(GTK_DIALOG (alert_window))),
		      hbox, TRUE, TRUE, 0);
  gtk_widget_show(hbox);

  /* add the picture */
  pixbuf = gdk_pixbuf_new_from_xpm_data( (const char **) alert_fig);
  image = gtk_image_new_from_pixbuf(pixbuf);
  gtk_box_pack_start (GTK_BOX (hbox),
		      image, FALSE, FALSE, 0);


  /* add the text to the window */
  alert_scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (alert_scroll), 10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (alert_scroll),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_box_pack_start (GTK_BOX (hbox), alert_scroll, TRUE, TRUE, 0);

  alert_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(alert_view) , FALSE);

  buffer = gtk_text_buffer_new(NULL);
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(alert_view), buffer);

  gtk_container_add(GTK_CONTAINER(alert_scroll), alert_view);

  gtk_widget_show_all(alert_scroll);

  /*
   * The Action Area
   */

  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label (_("Ok"));
  g_signal_connect( G_OBJECT( button ), "clicked",
                    G_CALLBACK(ok_pressed),
                    GTK_OBJECT(alert_window) );

  gtk_box_pack_start (GTK_BOX (gtk_dialog_get_action_area(GTK_DIALOG (alert_window))),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);
  gtk_window_set_focus(GTK_WINDOW(alert_window),button);

  gtk_window_set_default_size(GTK_WINDOW(alert_window), 600, 120);

  return 0;
}

void alert(const char *fmt,...)
{

  GtkTextIter iter, iter2;
  GtkTextBuffer *buffer;
  static GtkTextMark *mark = NULL;
  gint lines;

  /* count of how many alerts we've had */
  static int cnt = 0;
  char prompt[32];

  /* stuff for the message */
  char msg[MAX_MSG+1];
  va_list ap;


  /* extract the warning string */
  va_start(ap, fmt);
#ifdef HAVE_VSNPRINTF
  vsnprintf(msg, MAX_MSG, fmt, ap);
#else
  vsprintf(msg, fmt, ap);
#endif
  va_end(ap);

  if( alert_window == NULL ) {
	 alert_window_create();
  }

  /* show it */
  gtk_widget_show_all (alert_window);

  cnt++;
  sprintf(prompt, "[%3d]: ", cnt);

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(alert_view));

  gtk_text_buffer_get_end_iter(buffer, &iter);

  if(cnt > 1) {
	 gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
  }
  gtk_text_buffer_insert(buffer, &iter, prompt, -1);
  gtk_text_buffer_insert(buffer, &iter, msg, -1);

  /* Prune down the buffer */
  lines = gtk_text_buffer_get_line_count(buffer);
  if(lines > MAX_LINES) {
	 gtk_text_buffer_get_start_iter(buffer, &iter);
	 gtk_text_buffer_get_iter_at_line(buffer, &iter2, lines - MAX_LINES);
	 gtk_text_buffer_delete(buffer, &iter, &iter2);
	 lines = gtk_text_buffer_get_line_count(buffer);
  }

  if (!mark) {
	 gtk_text_buffer_get_end_iter(buffer, &iter);
	 mark = gtk_text_buffer_create_mark(buffer, NULL, &iter, FALSE);
  }
  gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(alert_view), mark, 0.05, TRUE, 0.0, 1.0);


  gtk_widget_show_all (alert_window);

  /* bring the window to the top  --  these messages are important */
  gtk_window_present( GTK_WINDOW(alert_window) );

  /* and BEEEP */
  gdk_beep();
}
