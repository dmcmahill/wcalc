/*
 * Copyright (C) 2001, 2002, 2004, 2005, 2006, 2007, 2008, 2009, 2020 Dan McMahill
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
"Wcalc is copyright (C) 1999-2020 Dan McMahill."


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
  guint len;
  const char *name;
  int foundit=0;
  void *new_cmd;

  /* lookup the selected model in the list of models */
  name = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo_model)->entry));

  /* how many model entries might there be? */
  len = g_list_length(global_model_names);

  /* figure out which one we've selected */
  for(ind=0; ind<len; ind++){
    if(strcmp(g_list_nth_data(global_model_names,ind),name) == 0){
#ifdef DEBUG
      g_print("new_pressed():  Found entry at index %d\n",ind);
#endif
      foundit = 1;
      break;
    }
  }

  if(!foundit){
    g_print("start.c:new_pressed():  could not find \"%s\" in the model list\n",
	    name);
  }

#ifdef DEBUG
  g_print("start.c:new_pressed():  length of global_model_names is %d\n",
	  g_list_length(global_model_names));
  g_print("start.c:new_pressed():  Model \"Microstrip\" is entry # %d\n",
	  g_list_index(global_model_names,"Microstrip"));
  g_print("start.c:new_pressed():  Model \"%s\" is entry # %d\n",
	  name, ind);
#endif

  /* extract the _new function for our selected model */
  new_cmd = (void *) g_list_nth_data(global_model_new,ind);

  if(new_cmd == NULL){
    g_print("start.c:new_pressed():  Sorry, I don't know how to create \"%s\"\n",
	    name);
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

 
void start_popup(void)
{
  GtkWidget *window;
  GtkWidget *main_vbox;

  GtkWidget *button;
  GtkWidget *label;

  GtkWidget *separator;
  GtkWidget *action_area;

  /* stuff for the picture */
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;    
  
  /* create the initial window */
  /* XXX this was for gtk-1.2 */
  /* window = gtk_window_new(GTK_WINDOW_DIALOG); */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  /* made it modal */
  gtk_grab_add(window);

  /* don't let the user grow or shrink this window */
  gtk_window_set_policy(GTK_WINDOW(window),FALSE,FALSE,TRUE);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW(window), "Wcalc");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);

  /* Window Manager "delete" */
  gtk_signal_connect (GTK_OBJECT (window), "delete_event",
		      GTK_SIGNAL_FUNC (destroy_event),
		      NULL);
  
  /* Window Manager "destroy" */
  gtk_signal_connect (GTK_OBJECT (window), "destroy_event",
		      GTK_SIGNAL_FUNC (destroy_event),
		      NULL);
  

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
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(open_pressed),
		     GTK_OBJECT(window));
  
  gtk_box_pack_start (GTK_BOX (action_area),
		      button, TRUE, FALSE, 0);
  gtk_widget_show (button);

  /* Add the "New" button and set its action */
  button = gtk_button_new_with_label (_("New"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(new_pressed),
		     GTK_OBJECT(window));
  
  gtk_box_pack_start (GTK_BOX (action_area),
		      button, TRUE, FALSE, 0);
  gtk_window_set_focus(GTK_WINDOW(window),button);
  gtk_widget_show (button);


  combo_model =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo_model), global_model_names);
  gtk_combo_set_use_arrows( GTK_COMBO(combo_model), 1);
  gtk_box_pack_start (GTK_BOX (action_area), combo_model, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo_model)->entry), FALSE);
  gtk_widget_show( combo_model );

  /* Add the "Quit" button and set its action */
  button = gtk_button_new_with_label (_("Quit"));
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(quit_pressed),
		     GTK_OBJECT(window));
  
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

  style = gtk_widget_get_style( window );
  
  pixmap = gdk_pixmap_create_from_xpm_d( window->window, 
					 &mask,
					 &style->bg[GTK_STATE_NORMAL],
					 (gchar **) splash);
    					
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (main_vbox),
		      pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    


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






