/* $Id: wcwindow.c,v 1.1 2001/10/25 00:49:54 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001 Dan McMahill
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

#include <gtk/gtk.h>

#include "icon_bitmap"
#include "menus.h"
#include "wcwindow.h"

static void wc_window_class_init (WcWindowClass *klass);
static void wc_window_init       (WcWindow      *dialog);


GtkType
wc_window_get_type (void)
{
  static GtkType wc_window_type = 0;

  if (!wc_window_type)
    {
      static const GtkTypeInfo wc_window_info =
      {
        "WcWindow",
        sizeof (WcWindow),
        sizeof (WcWindowClass),
        (GtkClassInitFunc) wc_window_class_init,
        (GtkObjectInitFunc) wc_window_init,
        /* reserved_1 */ NULL,
        /* reserved_2 */ NULL,
        (GtkClassInitFunc) NULL,
      };

      wc_window_type = gtk_type_unique (GTK_TYPE_WINDOW, &wc_window_info);
    }

  return wc_window_type;
}

static void
wc_window_class_init (WcWindowClass *class)
{
}

static void
wc_window_init (WcWindow *wcwindow)
{
  GtkWidget *menubar;
  GdkBitmap *icon_bitmap;


  /* Setup main window properties */
  gtk_window_set_title (GTK_WINDOW (wcwindow), "WaveCalc");
  gtk_container_set_border_width (GTK_CONTAINER (wcwindow), 0);
  gtk_widget_set_usize (GTK_WIDGET(wcwindow), 600, 525);


  /* Setup pixmap for the icon */
  //gtk_widget_realize(GTK_WINDOW(wcwindow));
  icon_bitmap = gdk_bitmap_create_from_data(GTK_WINDOW(wcwindow),
					    icon_bitmap_bits, 
					    icon_bitmap_width, 
					    icon_bitmap_height);
  gdk_window_set_icon(GTK_WINDOW(wcwindow), NULL, icon_bitmap, NULL);




  /* Setup main window callbacks */
  /* XXX probably need some sort of better way to catch this.
     in fact, i probably need to keep a global list of windows
     and have this event take windows out of the global list
     then quit when they're all gone....  yep.  otherwise 
     we never exit the program....
  */
  gtk_signal_connect (GTK_OBJECT (wcwindow), "delete_event",
		      GTK_SIGNAL_FUNC (delete_event),
		      NULL);
  
  gtk_signal_connect (GTK_OBJECT (wcwindow), "destroy", 
		      GTK_SIGNAL_FUNC (delete_event), 
		      NULL);


  /*create the main vbox */
  wcwindow->vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (wcwindow->vbox), 1);
  gtk_container_add (GTK_CONTAINER (wcwindow), wcwindow->vbox);
  gtk_widget_show (wcwindow->vbox);




  /* set up the menu bar */
  /* XXX the NULL used to be a (Wcalc *) */
  get_main_menu (NULL,wcwindow, &menubar);
  gtk_box_pack_start (GTK_BOX (wcwindow->vbox), menubar, FALSE, TRUE, 0);
  gtk_widget_show (menubar);

}

GtkWidget*
wc_window_new (void)
{
  return GTK_WIDGET (gtk_type_new (WC_TYPE_WINDOW));
}
