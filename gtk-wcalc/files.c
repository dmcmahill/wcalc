/* $Id: files.c,v 1.11 2005/10/25 21:26:00 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002 Dan McMahill
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

/* for stat(2) */
#include <sys/types.h>
#include <sys/stat.h>

/* for open(2) */
#include <fcntl.h>

#include "files.h"
#include "start.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void file_ok_sel (GtkWidget *w, gpointer data[])
{
  const char *fname;
  Wcalc *wcalc;
  GtkFileSelection *fs;
  struct stat sb;
  FILE *fp;
  wcalc = data[0];
  fs = data[1];

#ifdef DEBUG
  g_print("files.c:file_ok_sel():  got cbdata[0] (wcalc) = %p\n",data[0]);
  g_print("                        got cbdata[1] (filew) = %p\n",data[1]);
#endif

  fname = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));

#ifdef DEBUG
  g_print ("files.c:file_ok_sel():  \"%s\" selected\n", fname);
#endif

  /*
   * XXX should use open(2) with O_EXCL to avoid a race condition
   * where we check for the file, but then it gets created before we
   * open it.
   */
  /* see if the selected file already exists */
  if( stat(fname, &sb)==0 ){
    g_print("files.c:file_ok_sel():  warning: \"%s\" already exists\n", fname);
  }

  /* open the file */
  if ( (fp = fopen(fname,"w")) == NULL){
    g_print("files.c:file_ok_sel():  could not open \"%s\"\n", fname);
    return;
  }

  /* store the filename in the wcalc */
  wcalc->file_name = strdup(fname);

  /* actually do the save (model dependent) */
  if (wcalc->save != NULL){
    wcalc->save(wcalc,fp,wcalc->file_name);
  }
  else{
    g_print("files.c:file_ok_sel():  no ->save function available for"
	    "this model");
  }

  /* close the file */
  fclose(fp);

  /* update the window title */
  wcalc_set_title(wcalc);

  /* close the print window */
  gtk_grab_remove(GTK_WIDGET(fs));
  gtk_widget_destroy(GTK_WIDGET(fs));
}

static void file_open_ok_sel (GtkWidget *w, gpointer data)
{
  char *fname;
  const char *ret;
  GtkFileSelection *fs;

  fs = data;
  
  ret = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));
  fname=strdup(ret);

#ifdef DEBUG
  g_print ("files.c:file_open_ok_sel():  \"%s\" selected\n", fname);
#endif

  /* close the print window */
  gtk_grab_remove(GTK_WIDGET(fs));
  gtk_widget_destroy(GTK_WIDGET(fs));

  /* request a new wcalc window */
#ifdef DEBUG
  g_print ("files.c:file_open_ok_sel():  calling wcalc_setup(%p,-1,NULL)\n",
	   fname);
#endif
  wcalc_setup(fname,-1,NULL);
}

static  void destroy (GtkWidget *widget, gpointer data)
{
  /* gtk_main_quit (); */
}

static void file_cancel_sel (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

  /* make sure we have the start window if nothing else exists */
  if( wcalc_num_windows() == 0 ) start_popup();
}


/* 
 * the "Save As..." dialog box 
 *
 * The data will point to the  (Wcalc *wcalc)
 */

void wcalc_save_as(gpointer data,
		   guint action,
		   GtkWidget *widget)
{
  GtkWidget *filew;
  Wcalc *wcalc;
  static gpointer cbdata[2];
  
  wcalc = WC_WCALC(data);
     
  /* Create a new file selection widget */
  filew = gtk_file_selection_new ("Save As...");

  /* made it modal */
  gtk_grab_add(filew);

  cbdata[0] = data;
  cbdata[1] = filew;

#ifdef DEBUG
  g_print("files.c:wcalc_save_as():  set cbdata[0] (wcalc) = %p\n",cbdata[0]);
  g_print("                          set cbdata[1] (filew) = %p\n",cbdata[1]);
#endif

  /* Connect the ok_button to file_ok_sel function */
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked",
		      (GtkSignalFunc) file_ok_sel, 
		      cbdata );
    
  /* Connect the cancel_button to destroy the widget */
  gtk_signal_connect(GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
		     "clicked", 
		     (GtkSignalFunc) file_cancel_sel,
		     filew);
    
  /* 
   * choose default filename.  XXX this should be extracted from
   * the wcalc->filename.
   */
  gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), 
				   "wcalc.wc");
  
  gtk_widget_show(filew);
}

void wcalc_open(void)
{
  GtkWidget *filew;
     
  /* Create a new file selection widget */
  filew = gtk_file_selection_new ("Open");

  /* made it modal */
  gtk_grab_add(filew);

  gtk_signal_connect (GTK_OBJECT (filew), "destroy",
		      GTK_SIGNAL_FUNC (destroy),
		      &filew);

  /* Connect the ok_button to file_ok_sel function */
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked", (GtkSignalFunc) file_open_ok_sel, filew );
    
  /* Connect the cancel_button to destroy the widget */
  gtk_signal_connect(GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
		     "clicked", 
		     (GtkSignalFunc) file_cancel_sel,
		     GTK_OBJECT (filew));
    
  /* Lets set the filename, as if this were a save dialog, and we are giving
     a default filename */
  gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), 
				   "wcalc.wc");
  
  gtk_widget_show(filew);

}

void wcalc_save(gpointer data,
		guint action,
		GtkWidget *widget)
{
  FILE *fp;
  Wcalc *wcalc;

  wcalc = WC_WCALC(data);

  /* if there is no filename stored, then do "Save As..." instead */
  if(wcalc->file_name == NULL){
    wcalc_save_as(data,action,widget);
    return;
  }

  /* open the file */
  if ( (fp = fopen(wcalc->file_name,"w")) == NULL){
    g_print("files.c:wcalc_save():  could not open \"%s\"\n", 
	    wcalc->file_name);
    return;
  }

  /* actually do the save (model dependent) */
  if (wcalc->save != NULL){
    wcalc->save(wcalc,fp,wcalc->file_name);
  }
  else{
    g_print("files.c:wcalc_save():  no ->save function available for"
	    "this model");
  }

  /* close the file */
  fclose(fp);

  /* update the window title */
  *(wcalc->save_needed) = ' ';
  gtk_window_set_title (GTK_WINDOW (wcalc->window), wcalc->window_title);

}





