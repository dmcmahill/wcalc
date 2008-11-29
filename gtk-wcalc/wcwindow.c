/* $Id: wcwindow.c,v 1.2 2002/06/12 11:30:20 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001 Dan McMahill
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
