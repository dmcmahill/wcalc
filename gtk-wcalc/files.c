/* $Id: files.c,v 1.9 2004/08/05 13:20:12 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002 Dan McMahill
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

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

/* for stat(2) */
#include <sys/types.h>
#include <sys/stat.h>

/* for open(2) */
#include <fcntl.h>

#include "files.h"
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





