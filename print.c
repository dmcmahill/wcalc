/* $Id: print.c,v 1.3 2001/09/17 14:31:38 dan Exp $ */

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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <gtk/gtk.h>

#include "print.h"


static void print_browse_file(void);
static void browse_ok_sel (GtkWidget *w, GtkWidget *fs);
static void browse_cancel_sel (GtkWidget *w, GtkWidget *window);

static void save_state(void);

static int print_ps_header(FILE *fp);
static void do_print(void);


/* the print command */
static GtkWidget *text_cmd=NULL;

/* the print to file filename */
static GtkWidget *text_fname=NULL;

/* the "browse" button */
static GtkWidget *browse_button=NULL;

/* where to print */
#define PRINT_TO_PRINTER  0
#define PRINT_TO_FILE     1
static int where_print;

static int stop_sig;

/* 
 * remember the last print command and last print filename so the next
 * time we're asked to print, we can get back the sane info.
 */
static GString *print_cmd;
static GString *print_file;

static void to_printer_pressed (GtkWidget *w, gpointer data)
{
  if (stop_sig)
    return;

  gtk_widget_set_sensitive (text_cmd, TRUE);
  gtk_widget_set_sensitive (text_fname, FALSE);
  where_print = PRINT_TO_PRINTER;
  gtk_widget_set_sensitive (browse_button, FALSE);
}

static void to_file_pressed (GtkWidget *w, gpointer data)
{
  if (stop_sig)
    return;

  gtk_widget_set_sensitive (text_cmd, FALSE);
  gtk_widget_set_sensitive (text_fname, TRUE);
  where_print = PRINT_TO_FILE;
  gtk_widget_set_sensitive (browse_button, TRUE);
}

static void ok_pressed (GtkWidget *w, GtkWidget *window)
{
  char *str;

  if (stop_sig)
    return;

  save_state();

  str = gtk_entry_get_text( GTK_ENTRY(text_cmd) ); 
  printf("ok_pressed():  print command = \"%s\"\n",str);

  str = gtk_entry_get_text( GTK_ENTRY(text_fname) ); 
  printf("ok_pressed():  print file = \"%s\"\n",str);

  if (where_print == PRINT_TO_PRINTER)
    printf("ok_pressed():  print to printer\n");
  else if(where_print == PRINT_TO_FILE)
    printf("ok_pressed():  print to file\n");
  else
    printf("ok_pressed():  print to UNKNOWN\n");

  
  do_print();

  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

}

static void cancel_pressed (GtkWidget *w, GtkWidget *window)
{
  if (stop_sig)
    return;

  save_state();

  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);

}

static void save_state()
{
  char *str;

  str = gtk_entry_get_text( GTK_ENTRY(text_cmd) ); 
  g_string_assign(print_cmd,str);

  str = gtk_entry_get_text( GTK_ENTRY(text_fname) ); 
  g_string_assign(print_file,str);
}
 
void print_popup(void)
{
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *window;

  GtkWidget *table;

  GtkWidget *my_hbox;

  /* the "File/Printer" radio button group */
  GSList *where_group;

  static int first_time=1;
  
  /* block signals */
  stop_sig=1;

  if (first_time){
    where_print=PRINT_TO_PRINTER;

    print_cmd = g_string_new("lpr");
    print_file = g_string_new("wcalc.ps");
    first_time = 0;

  }

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
  /* also put the focus on the print button */
  gtk_window_set_focus(GTK_WINDOW(window),button);
  

  /* Add the "Cancel" button and set its action */
  button = gtk_button_new_with_label ("Cancel");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(cancel_pressed),
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
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(to_printer_pressed),
		     NULL);
  gtk_table_attach_defaults (GTK_TABLE(table), button, 1, 2, 0, 1);
  if (where_print == PRINT_TO_PRINTER)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  gtk_widget_show (button);

  where_group = gtk_radio_button_group (GTK_RADIO_BUTTON (button));
  button = gtk_radio_button_new_with_label(where_group, "File");
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(to_file_pressed),
		     NULL);
  gtk_table_attach_defaults (GTK_TABLE(table), button, 2, 3, 0, 1);
  if (where_print == PRINT_TO_FILE)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
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
  gtk_entry_set_text(GTK_ENTRY(text_cmd),print_cmd->str);
  gtk_table_attach_defaults (GTK_TABLE(table), text_cmd, 1, 5, 1, 2);
  if (where_print == PRINT_TO_PRINTER)
    gtk_widget_set_sensitive (text_cmd, TRUE);
  else
    gtk_widget_set_sensitive (text_cmd, FALSE);
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
  gtk_entry_set_text(GTK_ENTRY(text_fname),print_file->str);
  if (where_print == PRINT_TO_FILE)
    gtk_widget_set_sensitive (text_fname, TRUE);
  else
    gtk_widget_set_sensitive (text_fname, FALSE);
  gtk_table_attach_defaults (GTK_TABLE(table), text_fname, 1, 4, 2, 3);
  gtk_widget_show(text_fname);


  browse_button = gtk_button_new_with_label ("Browse...");
  gtk_signal_connect(GTK_OBJECT(browse_button), "clicked",
		     GTK_SIGNAL_FUNC(print_browse_file), 
		     GTK_OBJECT(window));
  //gtk_table_attach_defaults (GTK_TABLE(table), button, 4, 5, 2, 3);
  gtk_table_attach (GTK_TABLE(table), browse_button, 4, 5, 2, 3, 0,0,0,0);
  if (where_print == PRINT_TO_FILE)
    gtk_widget_set_sensitive (browse_button, TRUE);
  else
    gtk_widget_set_sensitive (browse_button, FALSE);
  gtk_widget_show (browse_button);

  /*
   * show the action area table
   */
  gtk_widget_show (table);

  /* allow signals */
  stop_sig=0;


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
		      "clicked", (GtkSignalFunc) browse_ok_sel,
		      GTK_OBJECT (filew) );
    
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
static void browse_ok_sel (GtkWidget *w, GtkWidget *fs)
{
    g_print ("%s\n", 
	     gtk_file_selection_get_filename (GTK_FILE_SELECTION
					      (fs)));

  gtk_entry_set_text(GTK_ENTRY(text_fname),
	     gtk_file_selection_get_filename (GTK_FILE_SELECTION
					      (fs)));

  /* unmake it modal */
  gtk_grab_remove(fs);
  
  /* blow away the window */
  gtk_widget_destroy(fs);
  
}

static void browse_cancel_sel (GtkWidget *w, GtkWidget *window)
{
  /* unmake it modal */
  gtk_grab_remove(window);

  /* blow away the window */
  gtk_widget_destroy(window);
}


static void do_print()
{
  FILE *fp;
  char *str;

  switch(where_print){
  case PRINT_TO_FILE:
    str = gtk_entry_get_text( GTK_ENTRY(text_fname) ); 
    if ( (fp = fopen(str,"w")) == NULL){
      fprintf(stderr,"do_print():  fopen of \"%s\"failed\n\n",str);
      return;
    }
    print_ps_header(fp);
    fclose(fp);
    break;

  case PRINT_TO_PRINTER:
    str = gtk_entry_get_text( GTK_ENTRY(text_cmd) ); 
    if ( (fp = popen(str,"w")) == NULL){
      fprintf(stderr,"do_print():  popen of \"%s\"failed\n\n",str);
      return;
    }
    print_ps_header(fp);
    pclose(fp);
    break;

    
  default:
    fprintf(stderr,"do_print():  unknown destination (%d)\n",where_print);
    return;
    break;
  }
  

}


static int print_ps_header(FILE *fp)
{
  double fontsize=12;
  double leftmargin=1.0;
  double topmargin=1.5;
  double bottommargin=1.5;
  double paperheight=11;
  double paperwidth=8.5;
  double tab1=0.75;
  double tab2=0.2;
  double tab3=0.6;
  double tab4=0.5;

  time_t now;

  int bbox_llx, bbox_lly, bbox_urx, bbox_ury;

  tab2 += tab1;
  tab3 += tab2;
  tab4 += tab3;
  
  now = time(NULL);

  bbox_llx=101;
  bbox_lly=664;
  bbox_urx=309;
  bbox_ury=775;

#include "ps_header.c"

  return 0;
}
