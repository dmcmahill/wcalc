/* $Id: wcalc.c,v 1.18 2001/09/22 03:50:18 dan Exp $ */

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

//#define DEBUG

#include "config.h"

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "icon_bitmap"
#include "menus.h"

/* the individual models */
#include "air_coil_gui.h"
#include "microstrip_gui.h"
//#include "stripline_gui.h"


#include "physconst.h"

#include "about.h"
#include "start.h"
#include "wcalc.h"


static gint wcalc_delete_event( GtkWidget *widget,
				GdkEvent *event,
				gpointer data );

static gint wcalc_destroy_event (GtkWidget *widget, 
				 GdkEvent *event,
				 gpointer data);

static void wcalc_destroy_sig( GtkWidget *widget,
			       gpointer   data );

static void global_model_init(void);

/*
 * globals (yuck!)
 */


static GSList *window_list=NULL;



/*
 * The top level delete/destroy signal and event callbacks
 */
static gint wcalc_delete_event( GtkWidget *widget,
				GdkEvent *event,
				gpointer data )
{

#ifdef DEBUG
  g_print("wcalc_delete_event():  widget = %p\n",widget);
  g_print("                       event  = %p\n",event);
  g_print("                       data   = %p\n",(void *) data);
#endif

  /* we haven't handled this event */
  return FALSE;
}

static gint wcalc_destroy_event (GtkWidget *widget, 
				 GdkEvent *event,
				 gpointer data)
{

#ifdef DEBUG
  g_print("wcalc_destroy_event():  widget = %p\n",widget);
  g_print("                        event  = %p\n",event);
  g_print("                        data   = %p\n",(void *) data);
#endif

  gtk_main_quit ();
  
  /* we have indeed handled this event */
  return TRUE;
}

static void wcalc_destroy_sig( GtkWidget *widget,
			       gpointer   data )
{
  GtkWidget *window;
  
#ifdef DEBUG
  g_print("wcalc_destroy_sig():  widget = %p\n",widget);
  g_print("                      data   = %p\n",(void *) data);
#endif

  window = ( (Wcalc *) data)->window;
  window_list = g_slist_remove(window_list,window);
  
  /* if this was the last window, then quit */
  if (g_slist_length(window_list) == 0)
    gtk_main_quit ();
}

/*
 * End of the top level delete/destroy callbacks
 */


int main( int   argc,
          char *argv[] )
{
  gtk_init (&argc, &argv);

  /* set up the list of available models for the program */
  global_model_init();
  global_printer_init();

  /*
   * the splash screen and also ask for what type of model for a new
   * calculator or if we should open an existing design 
   */
  start_popup();

  gtk_main ();

  return 0;
}

/* When a new model is added, this function must be updated. */
static void global_model_init()
{
  global_model_names = NULL;
  global_model_menus = NULL;
  global_model_new = NULL;

  global_model_names = g_list_append(global_model_names,"Air Core Inductor");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Air Core Inductor");
  global_model_new = g_list_append(global_model_new,air_coil_gui_new);

  /*
  global_model_names = g_list_append(global_model_names,"Coupled Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Coupled Microstrip");
  global_model_new = g_list_append(global_model_new,NULL);

  global_model_names = g_list_append(global_model_names,"I.C. Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_I.C. Microstrip");
  global_model_new = g_list_append(global_model_new,NULL);
  */
  global_model_names = g_list_append(global_model_names,"Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Microstrip");
  global_model_new = g_list_append(global_model_new,microstrip_gui_new);

  /*
  global_model_names = g_list_append(global_model_names,"Stripline");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Stripline");
  global_model_new = g_list_append(global_model_new,stripline_gui_new);
  */
}

void wcalc_setup (gpointer data,
		  guint action,
		  GtkWidget *widget)
{
  Wcalc *wcalc;

  GtkWidget *main_vbox;
  GtkWidget *menubar;
  GdkBitmap *icon_bitmap;
  void * (* new_cmd)(void);  /* the function which will create a new
			      * *_gui 
			      */ 
  char * model_name; /* the name of the model we will analyze */

  /* extract the _new function for our selected model */
  new_cmd = (void *) g_list_nth_data(global_model_new,action);

  /* extract the name of this model */
  model_name = (char *) g_list_nth_data(global_model_names,action);

#ifdef DEBUG
  g_print("wcalc_setup():  model = \"%s\" (# %d)\n",
	  (char *) g_list_nth_data(global_model_names,action),
	  action);
  g_print("                new_cmd = %p\n",
	  (void *) g_list_nth_data(global_model_new,action));
#endif

  if(new_cmd == NULL){
    g_print("wcalc.c:wcalc_setup():  Sorry, I don't know how to create \"%s\"\n",
	    model_name);
    return ;
  }

  /*
   * create our new GUI of the appropriate type.  Note that every GUI
   * _must_ have the first element be a Wcalc so that we can cast to
   * (Wcalc *) here and get uniform behaviour
   */
  wcalc = (Wcalc *) new_cmd();

  /* Create a new window */
  wcalc->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  /* add this window to our global list */
  window_list = g_slist_append(window_list, wcalc->window);

#ifdef DEBUG
  g_print("wcalc_setup():  Just set wcalc->window = %p\n",wcalc->window);
#endif

  /* Setup main window properties */
  gtk_window_set_title (GTK_WINDOW (wcalc->window), model_name);
  gtk_container_set_border_width (GTK_CONTAINER (wcalc->window), 0);

  /*
   * XXX need a better way to set this size.  We want to pick the
   * right size, but then not have it change while running.
   * Unfortunately, we don't know the correct size at compile time. 
   */
  //gtk_widget_set_usize (GTK_WIDGET(wcalc->window), 600, 550);

  /* don't let the user grow or shrink this window */
  //gtk_window_set_policy(GTK_WINDOW(wcalc->window),FALSE,FALSE,TRUE);

  /* Setup pixmap for the icon */
  gtk_widget_realize(wcalc->window);
  icon_bitmap = gdk_bitmap_create_from_data(wcalc->window->window,
					    icon_bitmap_bits, 
					    icon_bitmap_width, 
					    icon_bitmap_height);
  gdk_window_set_icon(wcalc->window->window, NULL, icon_bitmap, NULL);



  /* 
   * Setup main window callbacks 
   */

  /* Window Manager "delete" */
  gtk_signal_connect (GTK_OBJECT (wcalc->window), "delete_event",
		      GTK_SIGNAL_FUNC (wcalc_delete_event),
		      wcalc);
  
  /* Window Manager "destroy" */
  gtk_signal_connect (GTK_OBJECT (wcalc->window), "destroy_event",
		      GTK_SIGNAL_FUNC (wcalc_destroy_event),
		      wcalc);
  
  /* File->Close */
  gtk_signal_connect (GTK_OBJECT (wcalc->window), "destroy", 
		      GTK_SIGNAL_FUNC (wcalc_destroy_sig), 
		      wcalc);

  /*
   * Create the main window layout
   */

  /* create the main vbox */
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (main_vbox), 1);
  gtk_container_add (GTK_CONTAINER (wcalc->window), main_vbox);
  gtk_widget_show (main_vbox);

  /* set up the menu bar */
  get_main_menu (wcalc,wcalc->window, &menubar);
  gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
  gtk_widget_show (menubar);

  
  /* call the MD initialization */
  wcalc->init(wcalc,main_vbox);
}


Wcalc *Wcalc_new(void)
{
  Wcalc *new;

  new = (Wcalc *) malloc(sizeof(Wcalc));
  if (new == NULL)
    {
      fprintf(stderr,"Wcalac_new():  couldn't allocate memory\n");
      exit(1);
    }

  new->init_done=0;

#ifdef DEBUG
  g_print("Wcalc_new():  New pointer is %p\n",new);
#endif

  return(new);
}
