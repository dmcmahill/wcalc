/* $Id: microstrip_gui.c,v 1.3 2001/09/15 14:39:25 dan Exp $ */

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


#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "icon_bitmap"
#include "menus.h"

#include "microstrip.h"
#include "physconst.h"

#include "wcalc.h"

#define ENTRYLENGTH 8

GList *pys_units_text;

microstrip_gui *microstrip_gui_new(void)
{
  microstrip_gui *new_gui;
  
  /* allocate memory */
  new_gui = (microstrip_gui *) malloc(sizeof(microstrip_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"microstrip_gui_new: malloc() failed\n");
      exit(1);
    }


  /* initialize */
  new_gui->load = &microstrip_gui_load;
  new_gui->save = &microstrip_gui_save;
  new_gui->analyze = &microstrip_gui_analyze;
  new_gui->synthesize = &microstrip_gui_synthesize;
  new_gui->display = &microstrip_gui_display;

  new_gui->line = microstrip_line_new();

}


static void vals_changedCB( GtkWidget *widget, gpointer data );

static void units_init(GtkWidget *parent);
static void values_init(GtkWidget *parent);
static void outputs_init(GtkWidget *parent);
static void substrate_init(GtkWidget *parent);
static void picture_init(GtkWidget *window, GtkWidget *parent);
static void tooltip_init(void);


static GtkWidget *make_menu_item( gchar *name,
				  GtkSignalFunc callback,
				  gpointer data );

static void cb_punits_menu_select( GtkWidget *item,
				   gpointer data);

/*
 * globals (yuck!)
 */

Wcalc *wcalc;

int init_done=0;
GtkWidget *text_freq;
GtkWidget *label_keff,*label_loss,*label_losslen,*label_skindepth;
GtkWidget *text_W,*text_L,*text_Z0,*text_elen;
GtkWidget *text_H,*text_er,*text_rho,*text_rough,*text_tmet,*text_tand;

GtkWidget *text_status;

GtkWidget *combo_funits;
GtkWidget *combo_punits;
GtkWidget *combo_model;




static gint button_press (GtkWidget *widget, GdkEvent *event)
{

  if (event->type == GDK_BUTTON_PRESS) {
    GdkEventButton *bevent = (GdkEventButton *) event; 
    gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
		    bevent->button, bevent->time);
    /* Tell calling code that we have handled this event; the buck
     * stops here. */
    return TRUE;
  }

  /* Tell calling code that we have not handled this event; pass it on. */
  return FALSE;
}

/* Print a string when a menu item is selected */

static void menuitem_response (gchar *string)
{
  //gtk_label_set_text( GTK_LABEL(label_funits), string);
  printf ("%s\n", string);
}






static void vals_changedCB( GtkWidget *widget, gpointer data )
{
  if(init_done)
    gtk_label_set_text(GTK_LABEL(text_status), "Values Out Of Sync");
}


/* Our callback.
 * The data passed to this function is printed to stdout */
void callback( GtkWidget *widget,
               gpointer   data )
{
  g_print ("Hello again - %s was pressed\n", (char *) data);
}



/* This callback quits the program */
void delete_event( GtkWidget *widget,
                   GdkEvent  *event,
		   gpointer   data )
{
  gtk_main_quit ();
}

void destroy (GtkWidget *widget, gpointer data)
{
  gtk_main_quit ();
}



int main( int   argc,
          char *argv[] )
{
  gtk_init (&argc, &argv);

  wcalc_setup ();

  init_done = 1;
  gtk_main ();

  return 0;
}

void wcalc_setup (void)
{
  GtkWidget *window;
  GtkWidget *main_vbox;
  GtkWidget *menubar;
  GdkBitmap *icon_bitmap;

  GtkWidget *units_vbox;
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *substrate_vbox;
  GtkWidget *picture_vbox;

  
  /* Create a new window */
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  /* Setup main window properties */
  gtk_window_set_title (GTK_WINDOW (window), "WaveCalc");
  gtk_container_set_border_width (GTK_CONTAINER (window), 0);
  gtk_widget_set_usize (GTK_WIDGET(window), 600, 525);


  /* Setup pixmap for the icon */
  gtk_widget_realize(window);
  icon_bitmap = gdk_bitmap_create_from_data(window->window,
					    icon_bitmap_bits, 
					    icon_bitmap_width, 
					    icon_bitmap_height);
  gdk_window_set_icon(window->window, NULL, icon_bitmap, NULL);




  /* Setup main window callbacks */
  gtk_signal_connect (GTK_OBJECT (window), "delete_event",
		      GTK_SIGNAL_FUNC (delete_event), NULL);
  
  gtk_signal_connect (GTK_OBJECT (window), "destroy", 
		      GTK_SIGNAL_FUNC (gtk_main_quit), 
		      "WM destroy");



  /*create the main vbox */
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (main_vbox), 1);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show (main_vbox);




  /* set up the menu bar */
  get_main_menu (window, &menubar);
  gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
  gtk_widget_show (menubar);

  /* create the other vbox's and pack them into the main vbox */
  units_vbox = gtk_vbox_new (FALSE, 1);
  values_vbox = gtk_vbox_new (FALSE, 1);
  outputs_vbox = gtk_vbox_new (FALSE, 1);
  substrate_vbox = gtk_vbox_new (FALSE, 1);
  picture_vbox = gtk_vbox_new (FALSE, 1);

  gtk_container_set_border_width (GTK_CONTAINER (units_vbox), 5); 
  gtk_container_set_border_width (GTK_CONTAINER (values_vbox), 5); 
  gtk_container_set_border_width (GTK_CONTAINER (outputs_vbox), 5); 
  gtk_container_set_border_width (GTK_CONTAINER (substrate_vbox), 5); 
  gtk_container_set_border_width (GTK_CONTAINER (picture_vbox), 5); 

  gtk_box_pack_start (GTK_BOX (main_vbox), units_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), values_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), outputs_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), substrate_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), picture_vbox, FALSE, TRUE, 0);

  gtk_widget_show (units_vbox);
  gtk_widget_show (values_vbox);
  gtk_widget_show (outputs_vbox);
  gtk_widget_show (substrate_vbox);
  gtk_widget_show (picture_vbox);

  phys_units_text = NULL;

  units_init(units_vbox);
  values_init(values_vbox);
  outputs_init(outputs_vbox);
  substrate_init(substrate_vbox);
  picture_init(window,picture_vbox);

  tooltip_init();

  gtk_widget_show (window);

}


static void units_init(GtkWidget *parent)
{
  GtkWidget *text;
  GtkWidget *my_hbox;
  GtkWidget *my_vbox;
  GtkWidget *menu_model,*menu,*menu_items;
  GtkWidget *menu_punits,*opt,*item;
  GList *glist=NULL;
  GList *glist1=NULL;
  GList *glist2=NULL;
  GtkWidget *frame;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  my_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_vbox), 1);
  gtk_container_add (GTK_CONTAINER (frame), my_vbox);
  gtk_widget_show (my_vbox);


  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_box_pack_start (GTK_BOX (my_vbox), my_hbox, TRUE, FALSE, 1);
  gtk_widget_show (my_hbox);


  text = gtk_label_new( "Frequency:" );
  gtk_box_pack_start (GTK_BOX (my_hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);


  text_freq = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_box_pack_start (GTK_BOX (my_hbox), text_freq, FALSE, FALSE, 0);
  gtk_widget_set_usize(GTK_WIDGET(text_freq),60,0);
  gtk_widget_show(text_freq);

  
  glist = g_list_append(glist,"kHz");
  glist = g_list_append(glist,"MHz");
  glist = g_list_append(glist,"GHz");

  combo_funits =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo_funits), glist);
  gtk_combo_set_use_arrows( GTK_COMBO(combo_funits), 1);
  gtk_box_pack_start (GTK_BOX (my_hbox), combo_funits, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo_funits)->entry), FALSE);
  //gdk_string_width()
  gtk_widget_set_usize(GTK_WIDGET( combo_funits),60,0);
  gtk_widget_show( combo_funits );

  //g_list_free(glist);

  /*  
      my_hbox = gtk_hbox_new (FALSE, 1);
      gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
      gtk_box_pack_start (GTK_BOX (my_vbox), my_hbox, TRUE, FALSE, 1);
      gtk_widget_show (my_hbox);
  */

  glist1 = g_list_append(glist1,"Microstrip");
  /* glist1 = g_list_append(glist1,"Stripline"); */
  combo_model =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo_model), glist1);
  gtk_combo_set_use_arrows( GTK_COMBO(combo_model), 1);
  gtk_box_pack_start (GTK_BOX (my_hbox), combo_model, FALSE, FALSE, 30);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo_model)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET( combo_model),100,0);
  gtk_widget_show( combo_model );

  //#define NEWPUNITS
#ifdef NEWPUNITS

  menu = gtk_menu_new ();
  menu_items = gtk_menu_item_new_with_label ("Microstrip");
  gtk_menu_append (GTK_MENU (menu), menu_items);
  /*gtk_signal_connect_object (GTK_OBJECT (menu_items), "activate",
   *		     GTK_SIGNAL_FUNC (menuitem_response), 
   *		     (gpointer) g_strdup (buf));
   */
  gtk_widget_show (menu_items);

  menu_model = gtk_option_menu_new ();
  gtk_option_menu_set_menu (GTK_OPTION_MENU (menu_model), menu);  
  gtk_box_pack_start (GTK_BOX (my_hbox), menu_model, 
		      FALSE, FALSE, 30);
  gtk_widget_show (menu_model);

  opt = gtk_option_menu_new();
  menu_punits = gtk_menu_new();
  item = make_menu_item ("mil",
			 GTK_SIGNAL_FUNC(cb_punits_menu_select),
			 GINT_TO_POINTER (GTK_POS_TOP));
  gtk_menu_append (GTK_MENU (menu_punits), item);
  
  gtk_option_menu_set_menu (GTK_OPTION_MENU (opt), menu_punits);
  gtk_box_pack_end (GTK_BOX (my_hbox), opt, TRUE, TRUE, 0);
  gtk_widget_show (opt);
  
#else


  /* 
   * Note, these strings are the same length so the window layout
   * won't change as we select different physical units.
   */
  
  glist2 = NULL;
  glist2 = g_list_append(glist2,MILSTR);
  glist2 = g_list_append(glist2,UMSTR);
  glist2 = g_list_append(glist2,MMSTR);
  glist2 = g_list_append(glist2,CMSTR);
  glist2 = g_list_append(glist2,MSTR);


  combo_punits =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo_punits), glist2);
  gtk_combo_set_use_arrows( GTK_COMBO(combo_punits), 1);
  gtk_box_pack_end (GTK_BOX (my_hbox), combo_punits, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo_punits)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET( combo_punits),100,0);
  gtk_signal_connect (GTK_OBJECT (GTK_COMBO(combo_punits)->entry), "changed",
		      GTK_SIGNAL_FUNC (cb_punits_menu_select), GTK_ENTRY(GTK_COMBO(combo_punits)->entry));
  gtk_widget_show( combo_punits );
  

#endif


  text = gtk_label_new( "Physical Units:" );
  gtk_box_pack_end (GTK_BOX (my_hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);



  gtk_entry_set_text( GTK_ENTRY(text_freq),"1.88" );
  gtk_entry_set_text( GTK_ENTRY(GTK_COMBO(combo_funits)->entry),"GHz" );

}
static void values_init(GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *button;
  GtkWidget *frame;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (2, 7, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  
  /* Analyze button */
  button = gtk_button_new_with_label ("Analyze->");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (mscalc), (gpointer) "analyze");
  gtk_table_attach_defaults (GTK_TABLE(table), button, 3, 4, 0, 1);
  gtk_widget_show (button);
  
  /* Synthesize button */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (mscalc), (gpointer) "synthesize");
  /* Insert button 2 into the upper right quadrant of the table */
  gtk_table_attach_defaults (GTK_TABLE(table), button, 3, 4, 1, 2);
  
  gtk_widget_show (button);
  

  /* Text labels */
  text = gtk_label_new( "Width" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0, 1, 0, 1);
  gtk_widget_show(text);
  text = gtk_label_new( "Length" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0, 1, 1, 2);
  gtk_widget_show(text);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2, 3, 0, 1);
  gtk_widget_show(text);
  phys_units_text = g_list_append(phys_units_text, text);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2, 3, 1, 2);
  gtk_widget_show(text);
  phys_units_text = g_list_append(phys_units_text, text);

  text = gtk_label_new( "Z0" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 4, 5, 0, 1);
  gtk_widget_show(text);
  text = gtk_label_new( "E. Len." );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 4, 5, 1, 2);
  gtk_widget_show(text);
  text = gtk_label_new( "Ohms" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 6, 7, 0, 1);
  gtk_widget_show(text);
  text = gtk_label_new( "Degrees" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 6, 7, 1, 2);
  gtk_widget_show(text);
  
#define WIDTH 60
  text_W = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(text_W),"      ");
  gtk_table_attach (GTK_TABLE(table), text_W, 1, 2, 0, 1,0,0,0,0);
  gtk_widget_set_usize(GTK_WIDGET(text_W),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_W), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_W);

  text_L = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), text_L, 1, 2, 1, 2,0,0,0,0);
  gtk_widget_set_usize(GTK_WIDGET(text_L),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_L), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_L);

  text_Z0 = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_Z0, 5, 6, 0, 1);
  gtk_widget_set_usize(GTK_WIDGET(text_Z0),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_Z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_Z0);

  text_elen = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_elen, 5, 6, 1, 2);
  gtk_widget_set_usize(GTK_WIDGET(text_elen),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_elen);


  gtk_entry_set_text( GTK_ENTRY(text_W),"100" );
  gtk_entry_set_text( GTK_ENTRY(text_L),"1000" );
  gtk_entry_set_text( GTK_ENTRY(text_Z0),"50" );
  gtk_entry_set_text( GTK_ENTRY(text_elen),"90" );

  gtk_widget_show (table);
}

static void outputs_init(GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (3, 6, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */
  text = gtk_label_new( "Keff" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0, 1, 0, 1);
  gtk_widget_show(text);
  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,0,1);
  gtk_widget_show(text);
  text = gtk_label_new( "Loss" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,1,2);
  gtk_widget_show(text);
  text = gtk_label_new( "dB" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,1,2);
  gtk_widget_show(text);
  text = gtk_label_new( "Loss/Len" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,2,3);
  gtk_widget_show(text);
  text = gtk_label_new( "dB/mil" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,2,3);
  gtk_widget_show(text);
  text = gtk_label_new( "Skin Depth" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 0, 1);
  gtk_widget_show(text);
  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,0,1);
  gtk_widget_show(text);
  phys_units_text = g_list_append(phys_units_text, text);

#define OUTPUT_TEXT "     "
  label_keff = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), label_keff, 1,2,0,1);
  gtk_widget_show(label_keff);

  label_loss = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), label_loss, 1,2,1,2);
  gtk_widget_show(label_loss);

  label_losslen = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), label_losslen, 1,2,2,3);
  gtk_widget_show(label_losslen);

  label_skindepth = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), label_skindepth, 4,5,0,1);
  gtk_widget_show(label_skindepth);

#undef OUTPUT_TEXT

  gtk_widget_show(table);

}


static void substrate_init(GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Substrate Parameters" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  /* Setup the values_vbox contents */
  table = gtk_table_new (3, 6, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */
  text = gtk_label_new( "Height" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0, 1, 0, 1);
  gtk_widget_show(text);

  text_H = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_H, 1,2,0,1);
  gtk_widget_set_usize(GTK_WIDGET(text_H),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_H), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_H);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,0,1);
  gtk_widget_show(text);
  phys_units_text = g_list_append(phys_units_text, text);

  text = gtk_label_new( "Er" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,1,2);
  gtk_widget_show(text);

  text_er = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_er, 1,2,1,2);
  gtk_widget_set_usize(GTK_WIDGET(text_er),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_er);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,1,2);
  gtk_widget_show(text);

  text = gtk_label_new( "TanD" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,2,3);
  gtk_widget_show(text);

  text_tand = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_tand, 1,2,2,3);
  gtk_widget_set_usize(GTK_WIDGET(text_tand),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_tand);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,2,3);
  gtk_widget_show(text);

  text = gtk_label_new( "Tmet" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 0, 1);
  gtk_widget_show(text);

  text_tmet = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_tmet, 4,5,0,1);
  gtk_widget_set_usize(GTK_WIDGET(text_tmet),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_tmet), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_tmet);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,0,1);
  gtk_widget_show(text);
  phys_units_text = g_list_append(phys_units_text, text);

  text = gtk_label_new( "Rho" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 1, 2);
  gtk_widget_show(text);

  text_rho = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_rho, 4,5,1,2);
  gtk_widget_set_usize(GTK_WIDGET(text_rho),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_rho);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,1,2);
  gtk_widget_show(text);

  text = gtk_label_new( "Rough" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4,2,3);
  gtk_widget_show(text);

  text_rough = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), text_rough, 4,5,2,3);
  gtk_widget_set_usize(GTK_WIDGET(text_rough),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), NULL);
  gtk_widget_show(text_rough);

  text = gtk_label_new( "mil-rms" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,2,3);
  gtk_widget_show(text);

  gtk_entry_set_text( GTK_ENTRY(text_H),"62" );
  gtk_entry_set_text( GTK_ENTRY(text_er),"4.8" );
  gtk_entry_set_text( GTK_ENTRY(text_tand),"0.01" );
  gtk_entry_set_text( GTK_ENTRY(text_tmet),"1.4" );
  gtk_entry_set_text( GTK_ENTRY(text_rho),"1" );
  gtk_entry_set_text( GTK_ENTRY(text_rough),"0.055" );



  gtk_widget_show(table);

}




#include "pixmaps/microstrip.xpm"

static void picture_init(GtkWidget *window,GtkWidget *parent)
{
  GtkWidget *my_hbox;
  GtkWidget *button;
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;    
  GtkWidget *frame;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  //  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_container_add (GTK_CONTAINER (frame), my_hbox);
  gtk_widget_show (my_hbox);



  /* now for the pixmap from gdk */
  style = gtk_widget_get_style( window );
  pixmap = gdk_pixmap_create_from_xpm_d( window->window, 
					 &mask,
					 &style->bg[GTK_STATE_NORMAL],
					 (gchar **) microstrip);
					
  
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (my_hbox), pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    

  /* Create "Quit" button */
  /*  button = gtk_button_new_with_label ("Quit");
      gtk_signal_connect (GTK_OBJECT (button), "clicked",
      GTK_SIGNAL_FUNC (delete_event), NULL);
      gtk_box_pack_start (GTK_BOX (my_hbox), button, FALSE, FALSE, 0);
      gtk_widget_show (button);
  */

  text_status = gtk_label_new( "Values Out Of Sync" );
  gtk_box_pack_start (GTK_BOX (my_hbox), text_status, FALSE, FALSE, 0);
  gtk_widget_show (text_status);
  

}


void mscalc( GtkWidget *w, gpointer data )
{

  microstrip_subs subs;
  microstrip_line mstr;
  double freq;
  char str[80];
  char *vstr;
  double sf=1.0;

  /* get the physical units */
  vstr = gtk_entry_get_text( GTK_ENTRY(GTK_COMBO(combo_punits)->entry) ); 
  if (strcmp(vstr,MILSTR)==0)
    sf = 1.0;
  else if (strcmp(vstr,UMSTR)==0)
    sf = MICRON2MIL(1.0);
  else if (strcmp(vstr,MMSTR)==0)
    sf = MM2MIL(1.0);
  else if (strcmp(vstr,CMSTR)==0)
    sf = CM2MIL(1.0);
  else if (strcmp(vstr,MSTR)==0)
    sf = M2MIL(1.0);
  else
    {
      fprintf(stderr,"bad phyusical units.  contact the author\n");
      exit(1);
    }

  /* get the substrate parameters */
  vstr = gtk_entry_get_text( GTK_ENTRY(text_H) ); 
  subs.h=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(text_er) ); 
  subs.er=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(text_tand) ); 
  subs.tand=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(text_tmet) ); 
  subs.tmet=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(text_rough) ); 
  subs.rough=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(text_rho) ); 
  subs.rho=atof(vstr);


  mstr.subs=&subs;

  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(text_freq) ); 
  freq=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(GTK_COMBO(combo_funits)->entry) ); 
  if (strcmp(vstr,"kHz")==0)
    freq = 1.0e3*freq;
  else if (strcmp(vstr,"MHz")==0)
    freq = 1.0e6*freq;
  else if (strcmp(vstr,"GHz")==0)
    freq = 1.0e9*freq;


  /* if "analyze" get W and L */
  if( strcmp(data,"analyze")==0)
    {
      vstr = gtk_entry_get_text( GTK_ENTRY(text_W) ); 
      mstr.w=sf*atof(vstr);
      
      vstr = gtk_entry_get_text( GTK_ENTRY(text_L) ); 
      mstr.l=sf*atof(vstr);


      mstr.z0=microstrip_calc(&mstr,freq);
      
      sprintf(str,"%8f",mstr.z0);
      gtk_entry_set_text( GTK_ENTRY(text_Z0), str );
			  
      sprintf(str,"%8f",mstr.len);
      gtk_entry_set_text( GTK_ENTRY(text_elen), str );


    }
  else if( strcmp(data,"synthesize")==0)
    {
      /* if "synthesize" get z0 and len */
      vstr = gtk_entry_get_text( GTK_ENTRY(text_Z0) ); 
      mstr.z0 = atof(vstr);

      vstr = gtk_entry_get_text( GTK_ENTRY(text_elen) ); 
      mstr.len = atof(vstr);

      microstrip_syn(&mstr,freq,MLISYN_W);

 
      sprintf(str,"%8f",mstr.w/sf);
      gtk_entry_set_text( GTK_ENTRY(text_W), str );
			  
      sprintf(str,"%8f",mstr.l/sf);
      gtk_entry_set_text( GTK_ENTRY(text_L), str );

    }
  else
    {
      g_print("error in mstrip callback");
    }

  sprintf(str,"%8f",mstr.keff);
  gtk_label_set_text( GTK_LABEL(label_keff), str );
  
  sprintf(str,"%8f",mstr.loss);
  gtk_label_set_text( GTK_LABEL(label_loss), str );
  
  sprintf(str,"%8f",mstr.losslen*sf);
  gtk_label_set_text( GTK_LABEL(label_losslen), str );
  
  sprintf(str,"%8f",mstr.skindepth/sf);
  gtk_label_set_text( GTK_LABEL(label_skindepth), str );
  
  gtk_label_set_text(GTK_LABEL(text_status), "");

}


static void tooltip_init(void)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, text_freq, "Frequency of operation", NULL);

  gtk_tooltips_set_tip(tips, text_W, "Width of the microstrip line", NULL);
  gtk_tooltips_set_tip(tips, text_L, "Length of the microstrip line", NULL);
  gtk_tooltips_set_tip(tips, text_Z0, "Characteristic impedance of the microstrip line", NULL);
  gtk_tooltips_set_tip(tips, text_elen, "Electrical length of the microstrip line", NULL);

  gtk_tooltips_set_tip(tips, text_H, "Thickness of the substrate dielectric", NULL);
  gtk_tooltips_set_tip(tips, text_er, "Relative permitivity of the dielectric", NULL);
  gtk_tooltips_set_tip(tips, text_tand, "Loss tangent of the dielectric", NULL);
  gtk_tooltips_set_tip(tips, text_tmet, "Thickness of the metalization", NULL);
  gtk_tooltips_set_tip(tips, text_rho, "Resistance of the metalization relative to copper", NULL);
  gtk_tooltips_set_tip(tips, text_rough, "RMS deviation of the metal surface from a plane" 
		       "(surface roughness)", NULL);

  
}

static void change_units_text(void * , char *);

static void cb_punits_menu_select( GtkWidget *item,
                                   gpointer data)
{
  char *units;

  units = gtk_entry_get_text(GTK_ENTRY(data));
  /* Set the value position on both scale widgets */
  /*
  g_sf = sf; 
  g_print("set global physical scale factor to %g\n",sf);
  g_print("set global physical units to \"%s\"\n",punit);
  g_print("cb_punits_menu_select():  \n");
  */

  g_list_foreach( phys_units_text,change_units_text, units );
}


static void change_units_text(void * text, char * label)
{
  gtk_label_set_text(GTK_LABEL(text),label);
}

/* Convenience functions */

static GtkWidget *make_menu_item( gchar *name,
				  GtkSignalFunc callback,
				  gpointer data )
{
  GtkWidget *item;
  
  item = gtk_menu_item_new_with_label (name);
  gtk_signal_connect (GTK_OBJECT (item), "activate",
		      callback, data);
  gtk_widget_show (item);
  
  return(item);
}

static Wcalc *Wcalc_new(void)
{
  Wcalc *new;
  static int flag=0;

  if (flag)
    {
      fprintf(stderr,"Wcalc_new():  This routine may only be used once\n");
      fprintf(stderr,"              in the program.\n\n");
      exit(1);
    }

  new = (Wcalc *) malloc(sizeof(Wcalc));
  if (new == NULL)
    {
      fprintf(stderr,"Wcalac_new():  couldn't allocate memory\n");
      exit(1);
    }

  return(new);
}
