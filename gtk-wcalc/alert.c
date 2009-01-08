/* $Id: alert.c,v 1.13 2008/11/29 20:41:40 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2003, 2005, 2006 Dan McMahill
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

  gtk_widget_hide_all(alert_window);
  return TRUE;
}

static gint alert_destroy_event (GtkWidget *widget, 
				 GdkEvent *event,
				 gpointer data)
{

  gtk_widget_hide_all(alert_window);
  return TRUE;
}

static void ok_pressed (GtkWidget *w, GtkWidget *window)
{

  /* hide the window */
  gtk_widget_hide_all(window);
}

#define MAX_MSG 511

static gint alert_window_create()
{
  /* widgets for the dialog box */
  GtkWidget *button;
  GtkWidget *hbox;
  
  /* stuff for the picture */
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;    
  
#if GTK_CHECK_VERSION(2,0,0)
  GtkTextBuffer *buffer;
#endif
  
  /* create the dialog box */
  alert_window = gtk_dialog_new();
  
  /* Window Manager "delete" */
  gtk_signal_connect (GTK_OBJECT (alert_window), "delete_event",
		      GTK_SIGNAL_FUNC (alert_delete_event),
		      NULL);
  
  /* Window Manager  "destroy" */
  gtk_signal_connect (GTK_OBJECT (alert_window), "destroy_event",
		      GTK_SIGNAL_FUNC (alert_destroy_event),
		      NULL);
  
  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (alert_window), _("Wcalc:  Warning!"));
  gtk_container_set_border_width(GTK_CONTAINER(alert_window),10);
  
  /*
   * The Top 1/2 of the window
   */
  
  /* Make a hbox */
  hbox = gtk_hbox_new(FALSE, 10);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alert_window)->vbox),
		      hbox, TRUE, TRUE, 0);
  gtk_widget_show(hbox);
  
  /* add the picture */
  gtk_widget_realize(alert_window);
  style = gtk_widget_get_style( alert_window );
  pixmap = gdk_pixmap_create_from_xpm_d( alert_window->window, 
					 &mask,
					 &style->bg[GTK_STATE_NORMAL],
					 (gchar **) alert_fig);
  
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (hbox),
		      pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
  
  
#if GTK_CHECK_VERSION(2,0,0)
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

#else /* GTK-1.2 */
  alert_view = gtk_text_new( NULL, NULL );
  alert_scroll = gtk_vscrollbar_new (GTK_TEXT(alert_view)->vadj);
  gtk_box_pack_start(GTK_BOX(hbox), alert_view, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), alert_scroll, FALSE, FALSE, 0);
  gtk_text_set_editable(GTK_TEXT(alert_view), FALSE);
  gtk_text_set_word_wrap(GTK_TEXT(alert_view), TRUE);
#endif
  
  /*
   * The Action Area
   */
  
  /* Add the "OK" button and set its action */
  button = gtk_button_new_with_label (_("Ok"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(alert_window));
  
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (alert_window)->action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);
  gtk_window_set_focus(GTK_WINDOW(alert_window),button);
  
  gtk_window_set_default_size(GTK_WINDOW(alert_window), 600, 120);
  
  return 0;
}

void alert(const char *fmt,...)
{

#if GTK_CHECK_VERSION(2,0,0)
  GtkTextIter iter, iter2;
  GtkTextBuffer *buffer;
  static GtkTextMark *mark = NULL;
#else
  GtkAdjustment *adj;
  int i;
#endif



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

#if GTK_CHECK_VERSION(2,0,0)
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

#else /* GTK-1.2 */
  /* 
   * gtk_text_insert(GtkText  *text,
   *                 GdkFont  *font,
   *                 GdkColor *fore,
   *                 GdkColor *back,
   *                 const char *chars,
   *                 gint length)
   */

  /* freeze the widget before making all of our updates */
  gtk_text_freeze(GTK_TEXT(alert_view));

  /* move our insertion point to the end */
  lines = gtk_text_get_length(GTK_TEXT(alert_view));
  gtk_text_set_point( GTK_TEXT(alert_view), lines);

  /* 
   * if this isn't the first alert(), then add a couple of new lines
   * after the last one 
   */
  if(cnt > 1) {
	 gtk_text_insert(GTK_TEXT(alert_view), NULL, NULL, NULL, "\n\n", -1);
  }

  /* add the prompt and the message */
  gtk_text_insert(GTK_TEXT(alert_view), NULL, NULL, NULL, prompt, -1);
  gtk_text_insert(GTK_TEXT(alert_view), NULL, NULL, NULL, msg, -1);


  /* see if we need to prune lines */
  lines = 0;
  i = gtk_text_get_length(GTK_TEXT(alert_view));
  while( (i > 0) && (lines < MAX_LINES) ) {
    if( GTK_TEXT_INDEX(GTK_TEXT(alert_view), i) == '\n' ) {
      lines++;
    }
    i--;
  }
  
  if( i > 0 ) {
    gtk_text_set_point( GTK_TEXT(alert_view), 0);
    gtk_text_forward_delete( GTK_TEXT(alert_view), i);
    gtk_text_set_point( GTK_TEXT(alert_view), gtk_text_get_length(GTK_TEXT(alert_view)));
  }

  /*
   * thaw the widget.  We do this before scrolling or else the
   * scrolling fails to do the right thing 
   */

  gtk_text_thaw(GTK_TEXT(alert_view));

  /* scroll down to the bottom */
  adj = gtk_range_get_adjustment(GTK_RANGE(alert_scroll));
  gtk_adjustment_set_value(adj, adj->upper);
  gtk_adjustment_changed(adj);


#endif
  
  
  gtk_widget_show_all (alert_window);

  /* bring the window to the top  --  these messages are important */
#if GTK_CHECK_VERSION(2,0,0)
  gtk_window_present( GTK_WINDOW(alert_window) );
#else
  gdk_window_raise(alert_window->window);
#endif

  /* and BEEEP */
  gdk_beep();
}
