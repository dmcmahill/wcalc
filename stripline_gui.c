/* $Id$ */

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

#include "menus.h"

#include "stripline.h"
#include "stripline_gui.h"
#include "physconst.h"

#include "wcalc.h"


static void mscalc_analyze( GtkWidget *w, gpointer data );
static void mscalc_synthesize( GtkWidget *w, gpointer data );
static void mscalc( Wcalc *wcalc, GtkWidget *w, gpointer data );

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void units_init(Wcalc *wcalc, GtkWidget *parent);
static void values_init(Wcalc *wcalc, GtkWidget *parent);
static void outputs_init(Wcalc *wcalc, GtkWidget *parent);
static void substrate_init(Wcalc *wcalc, GtkWidget *parent);
static void picture_init(Wcalc *wcalc, GtkWidget *window, GtkWidget *parent);
static void tooltip_init(Wcalc *wcalc);

static void cb_punits_menu_select( GtkWidget *item,
				   gpointer data);


#define ENTRYLENGTH  8


/*
 * Public Functions
 */

stripline_gui *stripline_gui_new(void)
{
  stripline_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (stripline_gui *) malloc(sizeof(stripline_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"stripline_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = stripline_gui_init;
  wcalc->load = NULL;
  wcalc->save = NULL;
  wcalc->analyze = NULL;
  wcalc->synthesize = NULL;
  wcalc->display = NULL;


  /* XXX these must go.  they should be MD */
  wcalc->line = microstrip_line_new();
  wcalc->phys_units_text = NULL;

  /*
   * create the stripline line which will be used
   */
  new_gui->line = stripline_line_new();

#ifdef DEBUG
  g_print("Wcalc_new():  New pointer is %p\n",new);
  g_print("              wcalc->line = %p\n",new->line);
  g_print("              wcalc->line->subs = %p\n",new->line->subs);
#endif


  return new_gui;
}

void stripline_gui_init(Wcalc *wcalc, GtkWidget *main_vbox)
{
  GtkWidget *units_vbox;
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *substrate_vbox;
  GtkWidget *picture_vbox;

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

  units_init(wcalc,units_vbox);
  values_init(wcalc,values_vbox);
  outputs_init(wcalc,outputs_vbox);
  substrate_init(wcalc,substrate_vbox);
  picture_init(wcalc,wcalc->window,picture_vbox);

  tooltip_init(wcalc);

  gtk_widget_show (wcalc->window);

  wcalc->init_done=1;
}


static void units_init(Wcalc *wcalc,GtkWidget *parent)
{
  GtkWidget *text;
  GtkWidget *my_hbox;
  GtkWidget *my_vbox;
  GList *glist=NULL;
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


  wcalc->text_freq = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_box_pack_start (GTK_BOX (my_hbox), wcalc->text_freq, FALSE, FALSE, 0);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_freq),60,0);
  gtk_widget_show(wcalc->text_freq);

  
  glist = g_list_append(glist,"kHz");
  glist = g_list_append(glist,"MHz");
  glist = g_list_append(glist,"GHz");

  wcalc->combo_funits =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(wcalc->combo_funits), glist);
  gtk_combo_set_use_arrows( GTK_COMBO(wcalc->combo_funits), 1);
  gtk_box_pack_start (GTK_BOX (my_hbox), wcalc->combo_funits, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(wcalc->combo_funits)->entry), FALSE);
  //gdk_string_width()
  gtk_widget_set_usize(GTK_WIDGET( wcalc->combo_funits),60,0);
  gtk_widget_show( wcalc->combo_funits );

  //g_list_free(glist);

  /*  
      my_hbox = gtk_hbox_new (FALSE, 1);
      gtk_container_border_width (GTK_CONTAINER (my_hbox), 1);
      gtk_box_pack_start (GTK_BOX (my_vbox), my_hbox, TRUE, FALSE, 1);
      gtk_widget_show (my_hbox);
  */
#ifdef notdef
  glist1 = g_list_append(glist1,"Microstrip");
  /* glist1 = g_list_append(glist1,"Stripline"); */
  wcalc->combo_model =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(wcalc->combo_model), glist1);
  gtk_combo_set_use_arrows( GTK_COMBO(wcalc->combo_model), 1);
  gtk_box_pack_start (GTK_BOX (my_hbox), wcalc->combo_model, FALSE, FALSE, 30);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(wcalc->combo_model)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET( wcalc->combo_model),100,0);
  gtk_widget_show( wcalc->combo_model );
#endif

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


  wcalc->combo_punits =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(wcalc->combo_punits), glist2);
  gtk_combo_set_use_arrows( GTK_COMBO(wcalc->combo_punits), 1);
  gtk_box_pack_end (GTK_BOX (my_hbox), wcalc->combo_punits, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(wcalc->combo_punits)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET( wcalc->combo_punits),100,0);
  gtk_signal_connect (GTK_OBJECT (GTK_COMBO(wcalc->combo_punits)->entry), "changed",
		      GTK_SIGNAL_FUNC (cb_punits_menu_select),
		      (gpointer) wcalc);
  gtk_widget_show( wcalc->combo_punits );
  

  text = gtk_label_new( "Physical Units:" );
  gtk_box_pack_end (GTK_BOX (my_hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);



  gtk_entry_set_text( GTK_ENTRY(wcalc->text_freq),"1.88" );
  gtk_entry_set_text( GTK_ENTRY(GTK_COMBO(wcalc->combo_funits)->entry),"GHz" );

}
static void values_init(Wcalc *wcalc, GtkWidget *parent)
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
		      GTK_SIGNAL_FUNC (mscalc_analyze), (gpointer) wcalc);
  gtk_table_attach_defaults (GTK_TABLE(table), button, 3, 4, 0, 1);
  gtk_widget_show (button);
  
  /* Synthesize button */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (mscalc_synthesize), (gpointer) wcalc);
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
  wcalc->phys_units_text = g_list_append(wcalc->phys_units_text, text);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2, 3, 1, 2);
  gtk_widget_show(text);
  wcalc->phys_units_text = g_list_append(wcalc->phys_units_text, text);

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
  wcalc->text_W = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(wcalc->text_W),"      ");
  gtk_table_attach (GTK_TABLE(table), wcalc->text_W, 1, 2, 0, 1,0,0,0,0);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_W),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_W), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_W);

  wcalc->text_L = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), wcalc->text_L, 1, 2, 1, 2,0,0,0,0);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_L),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_L), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_L);

  wcalc->text_Z0 = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_Z0, 5, 6, 0, 1);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_Z0),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_Z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_Z0);

  wcalc->text_elen = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_elen, 5, 6, 1, 2);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_elen),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_elen);


  gtk_entry_set_text( GTK_ENTRY(wcalc->text_W),"100" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_L),"1000" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_Z0),"50" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_elen),"90" );

  gtk_widget_show (table);
}

static void outputs_init(Wcalc *wcalc, GtkWidget *parent)
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
  wcalc->phys_units_text = g_list_append(wcalc->phys_units_text, text);

#define OUTPUT_TEXT "     "
  wcalc->label_keff = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->label_keff, 1,2,0,1);
  gtk_widget_show(wcalc->label_keff);

  wcalc->label_loss = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->label_loss, 1,2,1,2);
  gtk_widget_show(wcalc->label_loss);

  wcalc->label_losslen = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->label_losslen, 1,2,2,3);
  gtk_widget_show(wcalc->label_losslen);

  wcalc->label_skindepth = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->label_skindepth, 4,5,0,1);
  gtk_widget_show(wcalc->label_skindepth);

#undef OUTPUT_TEXT

  gtk_widget_show(table);

}


static void substrate_init(Wcalc *wcalc, GtkWidget *parent)
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

  wcalc->text_H = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_H, 1,2,0,1);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_H),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_H), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_H);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,0,1);
  gtk_widget_show(text);
  wcalc->phys_units_text = g_list_append(wcalc->phys_units_text, text);

  text = gtk_label_new( "Er" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,1,2);
  gtk_widget_show(text);

  wcalc->text_er = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_er, 1,2,1,2);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_er),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_er);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,1,2);
  gtk_widget_show(text);

  text = gtk_label_new( "TanD" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,2,3);
  gtk_widget_show(text);

  wcalc->text_tand = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_tand, 1,2,2,3);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_tand),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_tand);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,2,3);
  gtk_widget_show(text);

  text = gtk_label_new( "Tmet" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 0, 1);
  gtk_widget_show(text);

  wcalc->text_tmet = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_tmet, 4,5,0,1);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_tmet),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_tmet);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,0,1);
  gtk_widget_show(text);
  wcalc->phys_units_text = g_list_append(wcalc->phys_units_text, text);

  text = gtk_label_new( "Rho" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 1, 2);
  gtk_widget_show(text);

  wcalc->text_rho = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_rho, 4,5,1,2);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_rho),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_rho);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,1,2);
  gtk_widget_show(text);

  text = gtk_label_new( "Rough" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4,2,3);
  gtk_widget_show(text);

  wcalc->text_rough = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), wcalc->text_rough, 4,5,2,3);
  gtk_widget_set_usize(GTK_WIDGET(wcalc->text_rough),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (wcalc->text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), wcalc);
  gtk_widget_show(wcalc->text_rough);

  text = gtk_label_new( "mil-rms" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,2,3);
  gtk_widget_show(text);

  gtk_entry_set_text( GTK_ENTRY(wcalc->text_H),"62" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_er),"4.8" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_tand),"0.01" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_tmet),"1.4" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_rho),"1" );
  gtk_entry_set_text( GTK_ENTRY(wcalc->text_rough),"0.055" );



  gtk_widget_show(table);

}




#include "pixmaps/stripline.xpm"

static void picture_init(Wcalc *wcalc, GtkWidget *window,GtkWidget *parent)
{
  GtkWidget *my_hbox;
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
					 (gchar **) stripline);
					
  
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (my_hbox), pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    

  wcalc->text_status = gtk_label_new( "Values Out Of Sync" );
  gtk_box_pack_start (GTK_BOX (my_hbox), wcalc->text_status, FALSE, FALSE, 0);
  gtk_widget_show (wcalc->text_status);
  

}

static void mscalc_analyze( GtkWidget *w, gpointer data )
{
  mscalc((Wcalc *) data, w, "analyze");
}

static void mscalc_synthesize( GtkWidget *w, gpointer data )
{
  mscalc((Wcalc *) data, w, "synthesize");
}

static void mscalc( Wcalc *wcalc, GtkWidget *w, gpointer data )
{
  double freq;
  char str[80];
  char *vstr;
  double sf=1.0;


  /* get the physical units */
  vstr = gtk_entry_get_text( GTK_ENTRY(GTK_COMBO(wcalc->combo_punits)->entry) ); 
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
  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_H) ); 
  wcalc->line->subs->h=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_er) ); 
  wcalc->line->subs->er=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_tand) ); 
  wcalc->line->subs->tand=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_tmet) ); 
  wcalc->line->subs->tmet=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_rough) ); 
  wcalc->line->subs->rough=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_rho) ); 
  wcalc->line->subs->rho=atof(vstr);



  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_freq) ); 
  freq=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(GTK_COMBO(wcalc->combo_funits)->entry) ); 
  if (strcmp(vstr,"kHz")==0)
    freq = 1.0e3*freq;
  else if (strcmp(vstr,"MHz")==0)
    freq = 1.0e6*freq;
  else if (strcmp(vstr,"GHz")==0)
    freq = 1.0e9*freq;


  /* if "analyze" get W and L */
  if( strcmp(data,"analyze")==0)
    {
      vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_W) ); 
      wcalc->line->w=sf*atof(vstr);
      
      vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_L) ); 
      wcalc->line->l=sf*atof(vstr);


      wcalc->line->z0=microstrip_calc(wcalc->line,freq);
      
      sprintf(str,"%8f",wcalc->line->z0);
      gtk_entry_set_text( GTK_ENTRY(wcalc->text_Z0), str );
			  
      sprintf(str,"%8f",wcalc->line->len);
      gtk_entry_set_text( GTK_ENTRY(wcalc->text_elen), str );


    }
  else if( strcmp(data,"synthesize")==0)
    {
      /* if "synthesize" get z0 and len */
      vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_Z0) ); 
      wcalc->line->z0 = atof(vstr);
      wcalc->line->Ro = wcalc->line->z0;

      vstr = gtk_entry_get_text( GTK_ENTRY(wcalc->text_elen) ); 
      wcalc->line->len = atof(vstr);

      microstrip_syn(wcalc->line,freq,MLISYN_W);

 
      sprintf(str,"%8f",wcalc->line->w/sf);
      gtk_entry_set_text( GTK_ENTRY(wcalc->text_W), str );
			  
      sprintf(str,"%8f",wcalc->line->l/sf);
      gtk_entry_set_text( GTK_ENTRY(wcalc->text_L), str );

    }
  else
    {
      g_print("error in mstrip callback");
    }

  sprintf(str,"%8f",wcalc->line->keff);
  gtk_label_set_text( GTK_LABEL(wcalc->label_keff), str );
  
  sprintf(str,"%8f",wcalc->line->loss);
  gtk_label_set_text( GTK_LABEL(wcalc->label_loss), str );
  
  sprintf(str,"%8f",wcalc->line->losslen*sf);
  gtk_label_set_text( GTK_LABEL(wcalc->label_losslen), str );
  
  sprintf(str,"%8f",wcalc->line->skindepth/sf);
  gtk_label_set_text( GTK_LABEL(wcalc->label_skindepth), str );
  
  gtk_label_set_text(GTK_LABEL(wcalc->text_status), "");

}


static void tooltip_init(Wcalc *wcalc)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, wcalc->text_freq, "Frequency of operation", NULL);

  gtk_tooltips_set_tip(tips, wcalc->text_W, "Width of the stripline line", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_L, "Length of the stripline line", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_Z0, "Characteristic impedance of the stripline line", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_elen, "Electrical length of the stripline line", NULL);

  gtk_tooltips_set_tip(tips, wcalc->text_H, "Thickness of the substrate dielectric", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_er, "Relative permitivity of the dielectric", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_tand, "Loss tangent of the dielectric", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_tmet, "Thickness of the metalization", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_rho, "Resistance of the metalization relative to copper", NULL);
  gtk_tooltips_set_tip(tips, wcalc->text_rough, "RMS deviation of the metal surface from a plane" 
		       "(surface roughness)", NULL);

  
}

static void change_units_text(void * , char *);

static void cb_punits_menu_select( GtkWidget *item,
				   gpointer data)
{
  char *units;
  Wcalc *wcalc;

  wcalc = data;

  units = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(wcalc->combo_punits)->entry));

  /* Set the value position on both scale widgets */
  /*
  g_sf = sf; 
  g_print("set global physical scale factor to %g\n",sf);
  g_print("set global physical units to \"%s\"\n",punit);
  g_print("cb_punits_menu_select():  \n");
  */

  g_list_foreach( wcalc->phys_units_text,
		  (GFunc) change_units_text,
		  units );
}


static void change_units_text(void * text, char * label)
{
  gtk_label_set_text(GTK_LABEL(text),label);
}



static void vals_changedCB(GtkWidget *widget, gpointer data )
{
  Wcalc *wcalc;
  wcalc = (Wcalc *) data;

  if(wcalc->init_done)
    gtk_label_set_text(GTK_LABEL(wcalc->text_status), "Values Out Of Sync");
}