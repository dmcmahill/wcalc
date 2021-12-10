/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2021 Dan McMahill
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
#include <glib.h>
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


/*
 * the "Save As..." dialog box
 *
 * The data will point to the  (Wcalc *wcalc)
 */

void wcalc_save_as(GtkAction *action, gpointer data)
{
  Wcalc *wcalc;
  GtkWidget *dialog;
  struct stat sb;
  FILE *fp;

  wcalc = WC_WCALC(data);
  dialog = gtk_file_chooser_dialog_new ("Save File",
                                        GTK_WINDOW(wcalc->window),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                        NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

  if (wcalc->file_fullname == NULL)
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "Untitled document");
  else
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dialog), wcalc->file_fullname);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
      char *filename;

      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      //save_to_file (filename);
      g_print("%s:  save to %s\n", __FUNCTION__, filename);

      /*
       * FIXME should use open(2) with O_EXCL to avoid a race condition
       * where we check for the file, but then it gets created before we
       * open it.
       */
      /* see if the selected file already exists */
      if( stat(filename, &sb)==0 ){
        g_print("%s:  warning: \"%s\" already exists\n", __FUNCTION__, filename);
      }

      /* open the file */
      if ( (fp = fopen(filename, "w")) == NULL){
        g_print("%s:  could not open \"%s\"\n", __FUNCTION__, filename);
        return;
      }

      /* store the filename in the wcalc */
      wcalc->file_fullname = g_strdup(filename);

      /* actually do the save (model dependent) */
      if (wcalc->save != NULL){
        wcalc->save(wcalc, fp, wcalc->file_fullname);
      }
      else{
        g_print("files.c:file_ok_sel():  no ->save function available for"
                "this model");
      }

      /* close the file */
      fclose(fp);

      /* update the window title */
      wcalc_set_title(wcalc);

      g_free (filename);
    }

  gtk_widget_destroy (dialog);
  
}

void wcalc_open(void)
{
  GtkWidget *dialog;
  char *filename;
  
  /* Create a new file selection widget */
  dialog = gtk_file_chooser_dialog_new (_("Open File"),
                                        NULL,
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                        NULL);

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    wcalc_setup(filename, -1, NULL);
    g_free (filename);
  }

  gtk_widget_destroy (dialog);
  
}

void wcalc_save(GtkAction *action, gpointer data)
{
  FILE *fp;
  Wcalc *wcalc;

  wcalc = WC_WCALC(data);

  /* if there is no filename stored, then do "Save As..." instead */
  if(wcalc->file_fullname == NULL){
    wcalc_save_as(action, data);
    return;
  }

  /* open the file */
  if ( (fp = fopen(wcalc->file_fullname,"w")) == NULL){
    g_print("files.c:wcalc_save():  could not open \"%s\"\n",
	    wcalc->file_fullname);
    return;
  }

  /* actually do the save (model dependent) */
  if (wcalc->save != NULL){
    wcalc->save(wcalc, fp, wcalc->file_fullname);
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





