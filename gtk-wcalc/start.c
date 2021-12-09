/*
 * Copyright (C) 2001, 2002, 2004, 2005, 2006, 2007, 2008, 2009, 2020, 2021 Dan McMahill
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

/* #define DEBUG */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "pixmaps/splash.xpm"

#include "files.h"
#include "start.h"
#include "wcalc.h"

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
"Wcalc is copyright (C) 1999-2021 Dan McMahill."


/*
 * This is ok to have file scope becasue we will only have 1 splash
 * window open at a time ever
 */
static GtkWidget *combo_model;

/*
 * The window delete/destroy event callbacks
 */

static gint destroy_event (GtkWidget *widget,
			   GdkEvent *event,
			   gpointer data)
{
  gtk_main_quit ();

  /* we have indeed handled this event */
  return TRUE;
}

static void new_pressed (GtkWidget *w, GtkWidget *window)
{
  guint ind;
  void *new_cmd;

  /* lookup the selected model in the list of models */
  ind = gtk_combo_box_get_active( GTK_COMBO_BOX(combo_model));


  /* extract the _new function for our selected model */
  new_cmd = (void *) g_list_nth_data(global_model_new,ind);

  if(new_cmd == NULL){
    g_print("%s():  Sorry, I don't know how to create \"%s\"\n",
	    __FUNCTION__, gtk_combo_box_text_get_active_text( GTK_COMBO_BOX_TEXT(combo_model)));
    return ;
  }

  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

  wcalc_setup (NULL, ind, NULL);

}

static void open_pressed (GtkWidget *w, GtkWidget *window)
{

#ifdef DEBUG
  g_print("start.c:open_pressed():\n");
#endif

  wcalc_open();

  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

}

static void quit_pressed (GtkWidget *w, GtkWidget *window)
{

#ifdef DEBUG
  g_print("start.c:quit_pressed():\n");
#endif

  gtk_main_quit();
}

static void add_model_to_splash(gpointer data, gpointer user_data)
{
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT (user_data), data);
}

void start_popup(void)
{
  GtkWidget *window;
  GtkWidget *main_vbox;

  GtkWidget *button;
  GtkWidget *label;

  GtkWidget *separator;
  GtkWidget *action_area;

  /* stuff for the picture */
  GdkPixbuf *pixbuf;
  GtkWidget *image;

  /* create the initial window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  /* made it modal */
  gtk_grab_add(window);

  /* don't let the user grow or shrink this window */
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW(window), "Wcalc");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Window Manager "delete" */
  g_signal_connect( G_OBJECT( window ), "delete_event",
                    G_CALLBACK(destroy_event), NULL);
  /* Window Manager "destroy" */
  g_signal_connect( G_OBJECT( window ), "destroy_event",
                    G_CALLBACK(destroy_event), NULL);


  gtk_widget_realize(window);


  main_vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show(main_vbox);


  /*
   * The Action Area
   */

  action_area = gtk_hbox_new (FALSE, 5);
  gtk_container_set_border_width (GTK_CONTAINER (action_area), 10);
  gtk_box_pack_end (GTK_BOX (main_vbox), action_area, FALSE, TRUE, 0);
  gtk_widget_show(action_area);

  separator = gtk_hseparator_new ();
  gtk_box_pack_end (GTK_BOX (main_vbox), separator, FALSE, TRUE, 0);
  gtk_widget_show (separator);

  /* Add the "Open..." button and set its action */
  button = gtk_button_new_with_label (_("Open..."));
  g_signal_connect( G_OBJECT( button ), "clicked",
                    G_CALLBACK(open_pressed), window);

  gtk_box_pack_start (GTK_BOX (action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);

  /* Add the "New" button and set its action */
  button = gtk_button_new_with_label (_("New"));
  g_signal_connect( G_OBJECT( button ), "clicked",
                    G_CALLBACK(new_pressed), window);

  gtk_box_pack_start (GTK_BOX (action_area),
		      button, TRUE, FALSE, 0);
  gtk_window_set_focus(GTK_WINDOW(window),button);
  gtk_widget_show (button);

  combo_model = gtk_combo_box_text_new();
  g_list_foreach( global_model_names, add_model_to_splash, combo_model);
  gtk_box_pack_start (GTK_BOX (action_area), combo_model, FALSE, FALSE, 0);
  gtk_widget_show( combo_model );
  gtk_combo_box_set_active (GTK_COMBO_BOX (combo_model), 0);

  /* Add the "Quit" button and set its action */
  button = gtk_button_new_with_label (_("Quit"));
  g_signal_connect( G_OBJECT( button ), "clicked",
                    G_CALLBACK(quit_pressed), window);
  gtk_box_pack_end (GTK_BOX (action_area),
		      button, TRUE, FALSE, 0);
  gtk_window_set_focus(GTK_WINDOW(window),button);
  gtk_widget_show (button);


  /*
   * The info Area
   */

  /*  my_vbox = gtk_vbox_new (FALSE, 1); */
  /*  gtk_box_pack_start (GTK_BOX (main_vbox), my_vbox, FALSE, TRUE, 0); */

#ifdef DEVEL_VERSION
  label = gtk_label_new (_("**** WARNING ****\nThis is a development snapshot\nand it may crash and/or give totally wrong results.\nUnless you're a developer you should not be using\nthis."));
  gtk_box_pack_start (GTK_BOX (main_vbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);
#endif

  /* now for the pixmap from gdk */
  pixbuf = gdk_pixbuf_new_from_xpm_data( (const char **) splash);
  image = gtk_image_new_from_pixbuf(pixbuf);
  gtk_box_pack_start (GTK_BOX (main_vbox), image, FALSE, FALSE, 0);
  gtk_widget_show( image );


  /* add the text to the window */
  label = gtk_label_new (ABOUT_TEXT);
  gtk_box_pack_start (GTK_BOX (main_vbox),
		      label, TRUE, TRUE, 0);
  gtk_widget_show (label);

  /* show it */
  gtk_widget_show (window);

  /* unmake it modal */
  gtk_grab_remove(window);
}






