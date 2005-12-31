/* $Id: alert.c,v 1.7 2005/12/31 18:05:48 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2003, 2005 Dan McMahill
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
 *        This product includes software developed by Dan McMahill
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
"WaveCalc is copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005\n" \
"Dan McMahill."


/* 
 * *********************************************************
 * File Globals
 * *********************************************************
 */

static GtkWidget *alert_window = NULL;

#if GTK_CHECK_VERSION(2,0,0)
static GtkWidget *alert_view;
static GtkWidget *alert_scroll;
#endif

/*
 * *********************************************************
 * Prototypes
 * *********************************************************
 */


/*
 * The top level delete/destroy signal and event callbacks
 */
static gint alert_delete_event( GtkWidget *widget,
				GdkEvent *event,
				gpointer data )
{

  g_print("alert_delete_event():  widget = %p\n", widget);
  g_print("                       event  = %p\n", event);
  g_print("                       data   = %p\n", (void *) data);

  return TRUE;
}

static gint alert_destroy_event (GtkWidget *widget, 
				 GdkEvent *event,
				 gpointer data)
{

  g_print("alert_destroy_event():  widget = %p\n", widget);
  g_print("                        event  = %p\n", event);
  g_print("                        data   = %p\n", (void *) data);
  
  return TRUE;
}

static void ok_pressed (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  /* gtk_grab_remove(window); */

  /* blow away the window */
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
#else
  GtkWidget *text, *vscrollbar;
  GtkWidget *label;
#endif

  /* create the dialog box */
  alert_window = gtk_dialog_new();
  
  /* made it modal */
  /* gtk_grab_add(alert_window); */
  
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
  hbox = gtk_hbox_new(FALSE,10);
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
    
    
  /* add the text to the window */
  alert_scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (alert_scroll), 10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (alert_scroll),
											 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  
  gtk_box_pack_start (GTK_BOX (hbox), alert_scroll, TRUE, TRUE, 0);
  
#if GTK_CHECK_VERSION(2,0,0)
  alert_view = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(alert_view) , FALSE);
    
  buffer = gtk_text_buffer_new(NULL);
  gtk_text_view_set_buffer(GTK_TEXT_VIEW(alert_view), buffer);

  //  gtk_scrolled_window_add_with_viewport(
  //													 GTK_SCROLLED_WINDOW (alert_scroll), alert_view);
  gtk_container_add(GTK_CONTAINER(alert_scroll), alert_view);

  gtk_widget_show_all(alert_scroll);

#else
  text = gtk_text_new( NULL, NULL );
  vscrollbar = gtk_vscrollbar_new (GTK_TEXT(text)->vadj);
  gtk_box_pack_start(GTK_BOX(hbox), vscrollbar, FALSE, FALSE, 0);
  gtk_text_set_editable(text, FALSE);
  gtk_text_set_word_wrap(text, TRUE);
    
  /* 
	* gtk_text_insert(GtkText  *text,
	*                 GdkFont  *font,
	*                 GdkColor *fore,
	*                 GdkColor *back,
	*                 const char *chars,
	*                 gint length)
	*/
  gtk_text_insert(text, NULL, NULL, NULL, msg, -1);
  gtk_widget_show (text);
  gtk_widget_show (vscrollbar);
#endif


#ifdef notdef
  label = gtk_label_new (msg);
  gtk_box_pack_start (GTK_BOX (hbox),
							 label, TRUE, TRUE, 0);
  gtk_widget_show (label);
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
  gint lines;
  static GtkTextMark *mark = NULL;
#endif


  /* count of how many alerts we've had */
  static int cnt = 0;
  char tmps[32];

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

#if GTK_CHECK_VERSION(2,0,0)
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(alert_view));
  cnt++;
  sprintf(tmps, "[%3d]: ", cnt);

  gtk_text_buffer_get_end_iter(buffer, &iter);

  if(cnt > 1) {
	 gtk_text_buffer_insert(buffer, &iter, "\n\n", -1);
  }
  gtk_text_buffer_insert(buffer, &iter, tmps, -1);
  gtk_text_buffer_insert(buffer, &iter, msg, -1);

  /* Prune down the buffer */
#define MAX_LINES 20
  lines = gtk_text_buffer_get_line_count(buffer);
  if(lines > MAX_LINES) {
	 gtk_text_buffer_get_start_iter(buffer, &iter);
	 gtk_text_buffer_get_iter_at_line(buffer, &iter2, lines - MAX_LINES);
	 gtk_text_buffer_delete(buffer, &iter, &iter2);
	 lines = gtk_text_buffer_get_line_count(buffer);
  }
#undef MAX_LINES

  if (!mark) {
	 gtk_text_buffer_get_end_iter(buffer, &iter);
	 mark = gtk_text_buffer_create_mark(buffer, NULL, &iter, FALSE);
  }
  //  gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(alert_view), &iter, 0.05, TRUE, 0.0, 1.0);
  gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(alert_view), mark, 0.05, TRUE, 0.0, 1.0);

  /* scroll down to the bottom */
  //adj = gtk_scrolled_window_get_vadjustment(alert_scroll);
  //gtk_adjustment_set_value(adj, adj->upper);
  //gtk_adjustment_changed(adj);

#endif
  
  
  gtk_widget_show_all (alert_window);

  /* and BEEEP */
  gdk_beep();
}




