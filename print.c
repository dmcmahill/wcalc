/* $Id$ */

/*
 * Copyright (c) 2001 Dan McMahill
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

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "print.h"


static void print_browse_file(void);
static void browse_ok_sel (GtkWidget *w, GtkFileSelection *fs);
static void browse_cancel_sel (GtkWidget *w, GtkWidget *window);

static void ok_pressed (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

}

 
void print_popup(void)
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *window;
  GSList *group;

  GtkWidget *table;
  GtkWidget *text_cmd;
  GtkWidget *text_fname;

  GtkWidget *my_hbox;

  /* create the "print" dialog */
  window = gtk_dialog_new();
  
  /* made it modal */
  gtk_grab_add(window);

  /* set other properties */
  gtk_window_set_title (GTK_WINDOW (window), "WaveCalc: Print");
  gtk_container_set_border_width(GTK_CONTAINER(window),10);



  /*
   * Make a hbox for the action area
   */

  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area),
		      my_hbox, TRUE, TRUE, 0);
  gtk_widget_show (my_hbox);


  /* Add the "Print" button and set its action */
  button = gtk_button_new_with_label ("Print");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));
  gtk_box_pack_start (GTK_BOX (my_hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);
  
  /* Add the "Cancel" button and set its action */
  button = gtk_button_new_with_label ("Cancel");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(ok_pressed),
		     GTK_OBJECT(window));
  gtk_box_pack_end (GTK_BOX (my_hbox), button, FALSE, FALSE, 0);
  gtk_widget_show (button);
  


  /* 
   * The table to contain everything in the top 1/2 of the dialog box
   */
  table = gtk_table_new (5, 3, FALSE);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG(window)->vbox), table);
  

  /* 
   * The "Print To:" line
   */
  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_table_attach (GTK_TABLE(table), my_hbox, 0, 1, 0, 1, GTK_FILL,0,0,0);
  gtk_widget_show (my_hbox);

  label = gtk_label_new ("Print To:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  //  gtk_table_attach (GTK_TABLE(table), label, 0, 1, 0, 1,
  //  GTK_FILL,0,0,0);
  gtk_box_pack_end (GTK_BOX (my_hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);



  button = gtk_radio_button_new_with_label (NULL, "Printer");
  gtk_table_attach_defaults (GTK_TABLE(table), button, 1, 2, 0, 1);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  gtk_widget_show (button);

  group = gtk_radio_button_group (GTK_RADIO_BUTTON (button));
  button = gtk_radio_button_new_with_label(group, "File");
  gtk_table_attach_defaults (GTK_TABLE(table), button, 2, 3, 0, 1);
  gtk_widget_show (button);


  /* 
   * The "Print Command:" line
   */
  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_table_attach (GTK_TABLE(table), my_hbox, 0, 1, 1, 2, GTK_FILL,0,0,0);
  gtk_widget_show (my_hbox);

  label = gtk_label_new ("Print Command:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  /*gtk_table_attach (GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL,0,0,0);*/
  gtk_box_pack_end (GTK_BOX (my_hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);


  text_cmd = gtk_entry_new_with_max_length( 80 );
  gtk_entry_set_text(GTK_ENTRY(text_cmd),"lpr -Ppastry ");
  gtk_table_attach_defaults (GTK_TABLE(table), text_cmd, 1, 5, 1, 2);
  gtk_widget_show(text_cmd);


  /* 
   * The "File Name:" line
   */
  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_table_attach (GTK_TABLE(table), my_hbox, 0, 1, 2, 3, GTK_FILL,0,0,0);
  gtk_widget_show (my_hbox);

  label = gtk_label_new ("File Name:");
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_RIGHT);
  //gtk_table_attach (GTK_TABLE(table), label, 0, 1, 2, 3, GTK_FILL,0,0,0);
  //gtk_table_attach_defaults (GTK_TABLE(table), label, 0, 1, 2, 3);
  gtk_box_pack_end (GTK_BOX (my_hbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);


  text_fname = gtk_entry_new_with_max_length( 80 );
  gtk_entry_set_text(GTK_ENTRY(text_fname),"wcalc.ps");
  gtk_entry_set_editable (GTK_ENTRY(text_fname), FALSE);
  gtk_table_attach_defaults (GTK_TABLE(table), text_fname, 1, 4, 2, 3);
  gtk_widget_show(text_fname);


  button = gtk_button_new_with_label ("Browse...");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(print_browse_file), 
		     GTK_OBJECT(window));
  //gtk_table_attach_defaults (GTK_TABLE(table), button, 4, 5, 2, 3);
  gtk_table_attach (GTK_TABLE(table), button, 4, 5, 2, 3, 0,0,0,0);
  gtk_widget_show (button);

  /*
   * show the action area table
   */
  gtk_widget_show (table);

  /* show the complete dialog box */
  gtk_widget_show (window);
}




static void print_browse_file(void)
{
  GtkWidget *filew;
    
     
  /* Create a new file selection widget */
  filew = gtk_file_selection_new ("Save Postscript As...");

  /* made it modal */
  gtk_grab_add(filew);

  /*  
  gtk_signal_connect (GTK_OBJECT (filew), "destroy",
		      (GtkSignalFunc) destroy, &filew);
		      */

  /* Connect the ok_button to file_ok_sel function */
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
		      "clicked", (GtkSignalFunc) browse_ok_sel, filew );
    
  /* Connect the cancel_button to destroy the widget */
  gtk_signal_connect(GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
		     "clicked", 
		     (GtkSignalFunc) browse_cancel_sel,
		     GTK_OBJECT (filew));

  /* Lets set the filename, as if this were a save dialog, and we are giving
     a default filename */
  gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), 
				   "wcalc.ps");
  
  gtk_widget_show(filew);

}

/* Get the selected filename and print it to the console */
static void browse_ok_sel (GtkWidget *w, GtkFileSelection *fs)
{
    g_print ("%s\n", 
	     gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));

}

static void browse_cancel_sel (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);
}


