/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2006, 2008, 2009, 2020, 2021 Dan McMahill
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

#include <gtk/gtk.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <winreg.h>
#endif

#include "icon_bitmap"
#include "alert.h"
#include "menus.h"

/* the individual models */
#include "air_coil_gui.h"
#include "bars_gui.h"
#include "coax_gui.h"
#include "coplanar_gui.h"
#include "coupled_microstrip_gui.h"
#include "coupled_stripline_gui.h"
#include "ic_microstrip_gui.h"
#include "microstrip_gui.h"
#include "parallel_rc_gui.h"
#include "parallel_rl_gui.h"
#include "rods_gui.h"
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

  /* on windows we use a registry entry to find WCALC_RCDIR */
#ifdef WIN32

  /*
   * code for reading from the windows registry was suggested by
   * Goran Rakic on the gtk-app-devel-list mailing list.  Many
   * thanks for the help!  Note, I haven't quite gotten this going.
   * Maybe I don't have the registry written out correctly?
   */

#ifdef notdef
  /*  Fallback value: data in working dir */
  char dbpath[MAX_PATH]="data";

  HKEY hkResult;
  DWORD DataType;
  DWORD DataCount = MAX_PATH;

  g_print("g_win32_get_package_installation_directory gave \"%s\"\n", rcdir);

  /* Open registry key */
  if (RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                    "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\wcalc.exe",
                    0 ,
                    KEY_QUERY_VALUE,
                    &hkResult ) == ERROR_SUCCESS )
    {
      g_print("Opened windows registry key\n");
      /* Write registry key value to dbpath */
      RegQueryValueEx(hkResult, "dbpath", NULL, NULL, (unsigned  char*) dbpath, &DataCount );
      g_print("Wrote windows registry key value\n");
      g_print("Read dbpath=\"%s\" from the windows registry!\n", dbpath);
    } else {
      g_print("Failed to open windows registry key\n");
    }
  RegCloseKey(hkResult);

#else /* !notdef */

  char * tmps;

  tmps = g_win32_get_package_installation_directory(PACKAGE "-" VERSION, NULL);
#define REST_OF_PATH G_DIR_SEPARATOR_S "share" G_DIR_SEPARATOR_S PACKAGE "-" VERSION
  rcdir = (char *) malloc(strlen(tmps) +
			  strlen(REST_OF_PATH) +
			  1);
  sprintf(rcdir, "%s%s", tmps, REST_OF_PATH);
  free(tmps);
#undef REST_OF_PATH

#endif

#else /* !WIN32 */
  rcdir = getenv("WCALC_RCDIR");
  if ( rcdir == NULL )
    rcdir = WCALC_RCDIR;

#endif

  setlocale (LC_ALL, "");
  /* This lets you set a directory for the locale stuff for pre-install testing */
  if( (localedir=getenv("WCALC_LOCALEDIR")) == NULL ) {
    bindtextdomain (PACKAGE, LOCALEDIR);
  } else {
    /* g_print("binding to localedir=\"%s\"\n",localedir); */
    bindtextdomain (PACKAGE, localedir);
  }
  textdomain (PACKAGE);

  /* FIXME -- do a better job with WCALC_RCDIR and HOME.  Look to pcb or gerbv */
  homedir = getenv("HOME");

  gtk_init (&argc, &argv);

  /* set up the list of available models for the program */
  global_model_init();

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

  /* ************************ Air Coil ******************** */
  global_model_names = g_list_append(global_model_names,"Air Core Inductor");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Air Core Inductor");
  global_model_new = g_list_append(global_model_new, (gpointer) air_coil_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_AIR_COIL);

  /* ************************  Coax ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Coaxial Transmission Line");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Coax");
  global_model_new = g_list_append(global_model_new, (gpointer) coax_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_COAX);

  /* ************************  Coplanar ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Coplanar Waveguide");
  global_model_menus = g_list_append(global_model_menus,"/File/New/C_oplanar Waveguide");
  global_model_new = g_list_append(global_model_new, (gpointer) coplanar_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_COPLANAR);

  /* ************************ Coupled Microstrip ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Coupled Microstrip");
  global_model_menus = g_list_append(global_model_menus,
				     "/File/New/Co_upled Microstrip");
  global_model_new = g_list_append(global_model_new,
				   (gpointer) coupled_microstrip_gui_new);
  global_model_defaults = g_list_append(global_model_defaults,
					FILE_COUPLED_MICROSTRIP);

  /* ************************ Coupled Stripline ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Coupled Stripline");
  global_model_menus = g_list_append(global_model_menus,
				     "/File/New/Coupled S_tripline");
  global_model_new = g_list_append(global_model_new,
				   (gpointer) coupled_stripline_gui_new);
  global_model_defaults = g_list_append(global_model_defaults,
					FILE_COUPLED_STRIPLINE);

  /* ************************ IC Microstrip ******************** */
  global_model_names = g_list_append(global_model_names,"I.C. Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_I.C. Microstrip");
  global_model_new = g_list_append(global_model_new, (gpointer) ic_microstrip_gui_new);
  global_model_defaults = g_list_append(global_model_defaults,
					FILE_IC_MICROSTRIP);

  /* ************************ Microstrip ******************** */
  global_model_names = g_list_append(global_model_names,"Microstrip");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Microstrip");
  global_model_new = g_list_append(global_model_new, (gpointer) microstrip_gui_new);
  global_model_defaults = g_list_append(global_model_defaults,
					FILE_MICROSTRIP);

  /* ************************  Rods ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Parallel Circular Wires");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Rods");
  global_model_new = g_list_append(global_model_new, (gpointer) rods_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_RODS);

  /* ************************  Bars ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Parallel Rectangular Bars");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_Bars");
  global_model_new = g_list_append(global_model_new, (gpointer) bars_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_BARS);

  /* ************************  Parallel RC ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Series/Parallel RC");
  global_model_menus = g_list_append(global_model_menus,"/File/New/_RC");
  global_model_new = g_list_append(global_model_new, (gpointer) parallel_rc_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_PARALLEL_RC);


  /* ************************  Parallel RL ******************** */
  global_model_names = g_list_append(global_model_names,
				     "Series/Parallel RL");
  global_model_menus = g_list_append(global_model_menus,"/File/New/R_L");
  global_model_new = g_list_append(global_model_new, (gpointer) parallel_rl_gui_new);
  global_model_defaults = g_list_append(global_model_defaults, FILE_PARALLEL_RL);


  /* ************************ Stripline ******************** */
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
  GdkPixbuf *pixbuf;
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

    case MODEL_BARS:
      new_cmd = (void *) bars_gui_new;
      break;

    case MODEL_COAX:
      new_cmd = (void *) coax_gui_new;
      break;

    case MODEL_COPLANAR:
      new_cmd = (void *) coplanar_gui_new;
      break;

    case MODEL_COUPLED_MICROSTRIP:
      new_cmd = (void *) coupled_microstrip_gui_new;
      break;

    case MODEL_COUPLED_STRIPLINE:
      new_cmd = (void *) coupled_stripline_gui_new;
      break;

    case MODEL_IC_MICROSTRIP:
      new_cmd = (void *) ic_microstrip_gui_new;
      break;

    case MODEL_MICROSTRIP:
      new_cmd = (void *) microstrip_gui_new;
      break;

    case MODEL_PARALLEL_RC:
      new_cmd = (void *) parallel_rc_gui_new;
      break;

    case MODEL_PARALLEL_RL:
      new_cmd = (void *) parallel_rl_gui_new;
      break;

    case MODEL_RODS:
      new_cmd = (void *) rods_gui_new;
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
  g_print("wcalc_setup():  Created new wcalc = %p and set wcalc->window = %p\n",
	  wcalc, wcalc->window);
#endif
  if ( fname != NULL ) {
    wcalc->file_fullname = g_strdup(fname);
#ifdef DEBUG
    g_print("wcalc_setup():  fname = \"%s\" != NULL so set wcalc->file_fullname\n",
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
  pixbuf = gdk_pixbuf_new_from_data(icon_bitmap_bits,
                                        GDK_COLORSPACE_RGB,
                                        FALSE,
                                        8,
                                        icon_bitmap_width,
                                        icon_bitmap_height,
                                        icon_bitmap_width * 3,
                                        NULL,
                                        NULL
                                        );
  gtk_window_set_icon(GTK_WINDOW(wcalc->window), pixbuf);


  /*
   * Setup main window callbacks
   */

  /* Window Manager "delete" */
  g_signal_connect( G_OBJECT( wcalc->window ), "delete_event",
                    G_CALLBACK(wcalc_delete_event), NULL);

  /* Window Manager "destroy" */
  g_signal_connect( G_OBJECT( wcalc->window ), "destroy_event",
                    G_CALLBACK(wcalc_destroy_event), NULL);


  /* File->Close */
  g_signal_connect (G_OBJECT (wcalc->window), "destroy",
                    G_CALLBACK (wcalc_destroy_sig),
                    wcalc);

  /*
   * Create the main window layout
   */

  /* create the main vbox */
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 1);
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

  Wcalc_init(new);

#ifdef DEBUG
  g_print("Wcalc_new():  New pointer is %p\n",new);
#endif

  return(new);
}

void Wcalc_init(Wcalc *wcalc)
{
  wcalc->init_done=0;

  wcalc->init = NULL;
  wcalc->load = NULL;
  wcalc->save = NULL;
  wcalc->analyze = NULL;
  wcalc->synthesize = NULL;
  wcalc->display = NULL;
  wcalc->dump_values = NULL;

  wcalc->file_fullname=NULL;
  wcalc->file_dirname=NULL;
  wcalc->file_filename=NULL;
  wcalc->file_basename=NULL;
  wcalc->pdf_filename = NULL;

  wcalc->model_name=NULL;
  wcalc->model_version=NULL;

  wcalc->window_title=NULL;
  wcalc->save_needed=NULL;
  wcalc->units_menu_list = NULL;

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
  glong flen, i;

  /*
   * If we know the file name (i.e. it is not a new window which
   * should be called Untitled)
   */
  if (wcalc->file_fullname != NULL) {

    /* file name without directory portion */
    wcalc->file_filename = g_path_get_basename(wcalc->file_fullname);

    /* the directory name */
    wcalc->file_dirname = g_path_get_dirname(wcalc->file_fullname);

    /* basename (no extension) */
    flen = g_utf8_strlen(wcalc->file_filename, FILENAME_MAX) - 1;
    i = flen;
    while(i > 0 && wcalc->file_filename[i] != '.') i--;
    if(i == 0) i = flen;
    wcalc->file_basename = g_utf8_substring(wcalc->file_filename, 0, i);

#ifdef DEBUG
    g_print("%s:  wcalc->file_fullname = %s\n", __FUNCTION__, wcalc->file_fullname);
    g_print("%s:  wcalc->file_dirname = %s\n", __FUNCTION__, wcalc->file_dirname);
    g_print("%s:  wcalc->file_filename = %s\n", __FUNCTION__, wcalc->file_filename);
    g_print("%s:  wcalc->file_basename = %s\n", __FUNCTION__, wcalc->file_basename);
#endif
  } else {
    /* file_fullname was NULL */
    wcalc->file_basename = "Untitled";
#ifdef DEBUG
    g_print("%s:  wcalc->file_fullname = %p\n", __FUNCTION__, wcalc->file_fullname);
    g_print("%s:  wcalc->file_dirname = %p\n", __FUNCTION__, wcalc->file_dirname);
    g_print("%s:  wcalc->file_filename = %p\n", __FUNCTION__, wcalc->file_filename);
    g_print("%s:  wcalc->file_basename = %s\n", __FUNCTION__, wcalc->file_basename);
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
    g_print("%s:  wcalc->window_title = \"%s\" (%p) %ld len\n",
	    __FUNCTION__, wcalc->window_title, wcalc->window_title, (long) strlen(wcalc->window_title));
    g_print("%s:  wcalc->file_saveneeded = \"%c\" (%p)\n",
	    __FUNCTION__, *(wcalc->save_needed), wcalc->save_needed);
#endif
  gtk_window_set_title (GTK_WINDOW (wcalc->window), wcalc->window_title);

}

void vals_changedCB(GtkWidget *widget, gpointer data )
{
  Wcalc *gui;

  gui = WC_WCALC(data);

  if(gui->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), _("Values Out Of Sync"));

  gui->values_in_sync = FALSE;
}


/* report how many open wcalc windows we have */
int wcalc_num_windows(void)
{
  return (g_slist_length(window_list));
}


