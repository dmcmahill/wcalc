/* $Id: wcalc.c,v 1.27 2005/10/30 17:57:08 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2004, 2005 Dan McMahill
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

/* #define DEBUG */

#include "config.h"

#include <gtk/gtk.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "icon_bitmap"
#include "alert.h"
#include "menus.h"

/* the individual models */
#include "air_coil_gui.h"
#include "coax_gui.h"
#include "coupled_microstrip_gui.h"
#include "ic_microstrip_gui.h"
#include "microstrip_gui.h"
#include "stripline_gui.h"

/* libwcalc related */
#include "misc.h"
#include "physconst.h"

/* GTK-wcalc related */
#include "about.h"
#include "start.h"
#include "wcalc.h"
#include "wcalc_loadsave.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

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

static char *rcdir = NULL;
static char *homedir = NULL;

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
  char *localedir;
  int i;

  setlocale (LC_ALL, "");
  /* This lets you set a directory for the locale stuff for pre-install testing */
  if( (localedir=getenv("WCALC_LOCALEDIR")) == NULL ) {
    bindtextdomain (PACKAGE, LOCALEDIR);
  } else {
    /* g_print("binding to localedir=\"%s\"\n",localedir); */
    bindtextdomain (PACKAGE, localedir);
  }
  textdomain (PACKAGE);

  rcdir = getenv("WCALC_RCDIR");
  if ( rcdir == NULL )
    rcdir = WCALC_RCDIR;

  homedir = getenv("HOME");

  gtk_init (&argc, &argv);

  /* set up the list of available models for the program */
  global_model_init();
  global_printer_init();

  /* handle command line arguments */

  for(i = 1; i < argc; i++) {
   wcalc_setup(argv[i], -1, NULL);
  }
  /*
   * the splash screen and also ask for what type of model for a new
   * calculator or if we should open an existing design.  If we already
   * opened a design from the command line, then skip the splash screen.
   */
  if( wcalc_num_windows() == 0 ) start_popup();

  gtk_main ();

  return 0;
}

/* When a new model is added, this function must be updated. */
static void global_model_init()
{
  global_model_names = NULL;
  global_model_menus = NULL;
  global_model_new = NULL;
  global_model_defaults = NULL;

  global_model_names = g_list_append(global_model_names,"Air Core Inductor");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Air Core Inductor");
  global_model_new = g_list_append(global_model_new, (gpointer) air_coil_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_AIR_COIL);

  global_model_names = g_list_append(global_model_names,
				     "Coaxial Transmission Line");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Coax");
  global_model_new = g_list_append(global_model_new, (gpointer) coax_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_COAX);

  global_model_names = g_list_append(global_model_names,
				     "Coupled Microstrip");
  global_model_menus = g_list_append(global_model_menus,
				     "/File/New/_Coupled Microstrip");
  global_model_new = g_list_append(global_model_new,
				   (gpointer) coupled_microstrip_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, 
					FILE_COUPLED_MICROSTRIP);

  global_model_names = g_list_append(global_model_names,"I.C. Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_I.C. Microstrip");
  global_model_new = g_list_append(global_model_new, (gpointer) ic_microstrip_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, 
					FILE_IC_MICROSTRIP);

  global_model_names = g_list_append(global_model_names,"Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Microstrip");
  global_model_new = g_list_append(global_model_new, (gpointer) microstrip_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, 
					FILE_MICROSTRIP);

  global_model_names = g_list_append(global_model_names,"Stripline");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Stripline");
  global_model_new = g_list_append(global_model_new, (gpointer) stripline_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, 
					FILE_STRIPLINE);
}

void wcalc_setup_cb (gpointer data,
		     guint action,
		     GtkWidget *widget)
{

  wcalc_setup(NULL, action, widget);
}

void wcalc_setup (gpointer data,
		  guint action,
		  GtkWidget *widget)
{
  Wcalc *wcalc;

  GtkWidget *main_vbox;
  GtkWidget *menubar;
  GdkBitmap *icon_bitmap;
  char tmps[FILENAME_MAX];

  void * (* new_cmd)(void);  /* the function which will create a new
			      * *_gui 
			      */ 
  char * model_name; /* the name of the model we will analyze */
  
  char *fname=NULL;
  FILE *fp=NULL;
  int type;

#ifdef DEBUG
  g_print("wcalc.c:wcalc_setup(): action = %d\n", action);
#endif
  
  if (action != -1) {
    /* 
     * This is a "new" command so lets look around for a default
     * settings file for this type of model.
     */

    /* extract file name */
    model_name = (char *) g_list_nth_data(global_model_defaults, action);

#ifdef DEBUG
    g_print("wcalc.c:wcalc_setup(): looking for defaults file \"%s.wc\"\n",
	    model_name);
#endif
    
    /* check current directory */
    sprintf(tmps, "%s.wc", model_name);
#ifdef DEBUG
    g_print("\ttrying \"%s\"\n", tmps);
#endif
    if ( (fp = fopen(tmps, "r")) != NULL ) {
#ifdef DEBUG
      g_print("Loading defaults from \"%s\"\n", tmps);
#endif
    } 

    if( (fp == NULL) && (homedir != NULL) ) {
      /* check $HOME/.wcalc/ */
      sprintf(tmps, "%s%s.wcalc%s%s.wc", 
	      homedir, G_DIR_SEPARATOR_S, G_DIR_SEPARATOR_S, model_name);
#ifdef DEBUG
      g_print("\ttrying \"%s\"\n", tmps);
#endif
      if ( (fp = fopen(tmps, "r")) != NULL ) {
#ifdef DEBUG
	g_print("Loading defaults from \"%s\"\n", tmps);
#endif
      }
    } 

    if( (fp == NULL) && (rcdir != NULL) ) {
      /* check $WCALC_RCDIR/ */
      sprintf(tmps, "%s%s%s.wc", rcdir, G_DIR_SEPARATOR_S, model_name);
#ifdef DEBUG
      g_print("\ttrying \"%s\"\n", tmps);
#endif
      if ( (fp = fopen(tmps, "r")) != NULL ) {
#ifdef DEBUG
	g_print("Loading defaults from \"%s\"\n", tmps);
#endif
      }

      if( fp == NULL ) {
#ifdef DEBUG
	g_print("\tNone found\n");
#endif
      }
    }


  } /* action != -1 */


  /* 
   * in the above code for action != -1, we searched for a file
   * containing defaults for the model in question.  If we found such
   * a file, we opened it so fp != NULL
   */
  if ( (action == -1) || (fp != NULL) ) {
    /* 
     * this is a file->open command or a file->new command
     * We can tell because for file->open data is the name of the file
     * for file->open.
     */
    fname = (char *) data;
#ifdef DEBUG
    g_print("wcalc_setup():  file open requested (%p)\n", data);
    if (fname != NULL )
      g_print("                file = \"%s\"\n", fname);
#endif

    /*
     * we are either reading a default file from fp or trying to load
     * the file named in fname
     */
    if ( fp == NULL ) {
      if( fname != NULL ) {
#ifdef DEBUG
	g_print("wcalc_setup():  Trying to open \"%s\"\n", fname);
#endif
	if ( (fp = fopen(data, "r")) == NULL ) {
	  alert("Could not open the file\n"
		"\"%s\"\n"
		"for reading.", fname);
	  if(wcalc_num_windows() == 0) start_popup();
	  return;
	}
      } else {
	alert("wcalc_setup():  fp == NULL and fname == NULL\n"
	      "Please report this bug\n");
	if(wcalc_num_windows() == 0) start_popup();
	return ;
      }
    }
    
    /* Now it is just a normal file open */
    /* extract the _new function for our selected model */
#ifdef DEBUG
    g_print("wcalc_setup():  extracting model type\n");
#endif
    type = wcalc_load(fp);
    if( type == -1 ) {
      alert("The file \n"
	    "\"%s\"\n"
	    "appears to be corrupted\n", fname);
      if(wcalc_num_windows() == 0) start_popup();
      return ;
    }
#ifdef DEBUG
    g_print("wcalc_setup():  model type = %d\n", type);
#endif
    switch (type) {
    case MODEL_AIR_COIL:
      new_cmd = (void *) air_coil_gui_new;
      break;

    case MODEL_COAX:
      new_cmd = (void *) coax_gui_new;
      break;
      
    case MODEL_COUPLED_MICROSTRIP:
      new_cmd = (void *) coupled_microstrip_gui_new;
      break;

    case MODEL_IC_MICROSTRIP:
      new_cmd = (void *) ic_microstrip_gui_new;
      break;

    case MODEL_MICROSTRIP:
      new_cmd = (void *) microstrip_gui_new;
      break;

    case MODEL_STRIPLINE:
      new_cmd = (void *) stripline_gui_new;
      break;

    case -1:
      alert( "Unable to load file \"%s\"\n", fname);
      if(wcalc_num_windows() == 0) start_popup();
      return;

    default:
      alert("Bad model type in \"%s\"", fname);
      if(wcalc_num_windows() == 0) start_popup();
      return;
    }

    /* XXX this should just be done with the _new function */
    model_name = NULL;

  } else {

    /* extract the name of this model */
    model_name = (char *) g_list_nth_data(global_model_names, action);
    
    /* extract the _new function for our selected model */
    new_cmd = (void *) g_list_nth_data(global_model_new, action);

#ifdef DEBUG
    g_print("wcalc_setup():  model = \"%s\" (# %d)\n", model_name, action);
    g_print("                new_cmd = %p\n", new_cmd);
#endif
  }
  
  if(new_cmd == NULL){
    g_print("wcalc.c:wcalc_setup():  Sorry, I don't know how to create \"%s\"\n",
	    model_name);
    if(wcalc_num_windows() == 0) start_popup();
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
  g_print("wcalc_setup():  Just set wcalc->window = %p\n", wcalc->window);
#endif
  if ( fname != NULL ) {
    wcalc->file_name = fname;
#ifdef DEBUG
    g_print("wcalc_setup():  fname = \"%s\" != NULL so set wcalc->file_name\n",
	    fname);
#endif
  }

  /* Setup main window properties */
  wcalc_set_title(wcalc);

  gtk_container_set_border_width (GTK_CONTAINER (wcalc->window), 0);

  /*
   * XXX need a better way to set this size.  We want to pick the
   * right size, but then not have it change while running.
   * Unfortunately, we don't know the correct size at compile time. 
   */
  /* gtk_widget_set_usize (GTK_WIDGET(wcalc->window), 600, 550); */

  /* don't let the user grow or shrink this window */
  /* gtk_window_set_policy(GTK_WINDOW(wcalc->window),FALSE,FALSE,TRUE); */

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
  wcalc->init(wcalc, main_vbox, fp);
}


Wcalc *Wcalc_new(void)
{
  Wcalc *new;

  new = (Wcalc *) malloc(sizeof(Wcalc));
  if (new == NULL)
    {
      fprintf(stderr,"Wcalc_new():  couldn't allocate memory\n");
      exit(1);
    }

  new->file_name = NULL;
  new->file_basename = "Untitled";

  new->init_done=0;

  new->units_menu_list = NULL;

#ifdef DEBUG
  g_print("Wcalc_new():  New pointer is %p\n",new);
#endif

  return(new);
}

/* call back used all over all the _gui's */
void wcalc_save_needed(GtkWidget *widget, gpointer data )
{
  if(WC_WCALC(data)->save_needed != NULL){
    *(WC_WCALC(data)->save_needed) = '*';
    gtk_window_set_title (GTK_WINDOW (WC_WCALC(data)->window),
			  WC_WCALC(data)->window_title);
  }
}


void wcalc_set_title(Wcalc * wcalc)
{
  size_t len;

  /*
   * If we know the file name (i.e. it is not a new window which
   * should be called Untitled)
   */
  if (wcalc->file_name != NULL) {
    /* extract the basefile name */
    /* 
     * filename = 3, len=4 means [3,4,5,6] has characters and 7 has
     * the \0 termination.  file_name + strlen() = 7.
     */
    wcalc->file_basename = wcalc->file_name + strlen(wcalc->file_name);
    while(--wcalc->file_basename >= wcalc->file_name) {
      if(*wcalc->file_basename == G_DIR_SEPARATOR)
	break;
    }
    /*
     * if we ended on a directory separator, then increment 1 to get
     * the filename string.
     * otherwise, we ended up 1 before the start of the path and
     * incrementing by 1 gets us back to the start of the string
     */
    wcalc->file_basename++;
#ifdef DEBUG
    g_print("wcalc.c:wcalc_set_title():  wcalc->file_name = %s\n", wcalc->file_name);
    g_print("wcalc.c:wcalc_set_title():  wcalc->file_basename = %s\n", wcalc->file_basename);
#endif
  } else {
    /* file_name was NULL */
    wcalc->file_basename = "Untitled";
#ifdef DEBUG
    g_print("wcalc.c:wcalc_set_title():  wcalc->file_name = NULL\n");
    g_print("wcalc.c:wcalc_set_title():  wcalc->file_basename = %s\n", wcalc->file_basename);
#endif
  }
    
  if (wcalc->window_title != NULL)
    free(wcalc->window_title);
  
  len  = strlen("Wcalc: ");
  len += strlen(wcalc->model_name);
  len += strlen(": ");
  len += strlen(wcalc->file_basename);
  len++;  /* for the '*' when file->save is needed */
  len++;  /* for the '\0' */

  wcalc->window_title=g_malloc(len*sizeof(char));
  sprintf(wcalc->window_title,"Wcalc: %s: %s ",
	  wcalc->model_name,
	  wcalc->file_basename);
  /* 
   * pointer to the last character of the window title which is either
   * ' ' or '*' if no save is needed or a save is needed.
   */
  wcalc->save_needed = wcalc->window_title + strlen(wcalc->window_title) - 1;
  
#ifdef DEBUG
    g_print("wcalc.c:wcalc_set_title():  wcalc->window_title = \"%s\" (%p) %d len\n",
	    wcalc->window_title, wcalc->window_title, strlen(wcalc->window_title);
    g_print("wcalc.c:wcalc_set_title():  wcalc->file_saveneeded = \"%c\" (%p)\n", 
	    *(wcalc->save_needed), wcalc->save_needed);
#endif
  gtk_window_set_title (GTK_WINDOW (wcalc->window), wcalc->window_title);

}

void vals_changedCB(GtkWidget *widget, gpointer data )
{
  Wcalc *gui;

  gui = WC_WCALC(data);

  if(gui->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), _("Values Out Of Sync"));
}


/* report how many open wcalc windows we have */
int wcalc_num_windows(void)
{
  return (g_slist_length(window_list));
}

