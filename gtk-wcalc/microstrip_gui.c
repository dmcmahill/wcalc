/* $Id: microstrip_gui.c,v 1.7 2002/06/12 11:30:15 dan Exp $ */

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

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "alert.h"
#include "epscat.h"
#include "menus.h"
#include "microstrip.h"
#include "microstrip_gui.h"
#include "microstrip_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);

static void mscalc_analyze( GtkWidget *w, gpointer data );
static void mscalc_synthesize( GtkWidget *w, gpointer data );
static void mscalc( microstrip_gui *gui, GtkWidget *w, gpointer data );

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void units_init(microstrip_gui *gui, GtkWidget *parent);
static void values_init(microstrip_gui *gui, GtkWidget *parent);
static void outputs_init(microstrip_gui *gui, GtkWidget *parent);
static void substrate_init(microstrip_gui *gui, GtkWidget *parent);
static void picture_init(microstrip_gui *gui, GtkWidget *window, GtkWidget *parent);
static void tooltip_init(microstrip_gui *gui);

static void cb_punits_menu_select( GtkWidget *item,
				   gpointer data);


#define ENTRYLENGTH  8


static char *name="Microstrip Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

microstrip_gui *microstrip_gui_new(void)
{
  microstrip_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (microstrip_gui *) malloc(sizeof(microstrip_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"microstrip_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = microstrip_gui_init;
  wcalc->print_ps = print_ps;
  wcalc->load = NULL;
  wcalc->save = gui_save;
  wcalc->analyze = NULL;
  wcalc->synthesize = NULL;
  wcalc->display = NULL;

  wcalc->file_name=NULL;
  wcalc->file_basename=NULL;

  wcalc->model_name=name;
  wcalc->model_version=version;

  wcalc->window_title=NULL;
  wcalc->save_needed=NULL;

  /*
   * Initialize the model dependent portions
   */

  /* create the microstrip line which will be used */
  new_gui->line = microstrip_line_new();
  new_gui->phys_units_text = NULL;

#ifdef DEBUG
  g_print("Wcalc_new():  New pointer is %p\n",new);
  g_print("              wcalc->line = %p\n",new->line);
  g_print("              wcalc->line->subs = %p\n",new->line->subs);
#endif

  return new_gui;
}

void microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp)
{
  GtkWidget *units_vbox;
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *substrate_vbox;
  GtkWidget *picture_vbox;

  microstrip_gui *gui;

  gui = WC_MICROSTRIP_GUI(wcalc);

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

  units_init(gui,units_vbox);
  values_init(gui,values_vbox);
  outputs_init(gui,outputs_vbox);
  substrate_init(gui,substrate_vbox);
  picture_init(gui,wcalc->window,picture_vbox);

  tooltip_init(gui);

  gtk_widget_show (wcalc->window);

  wcalc->init_done=1;
}

/*
 * Private Functions
 */

static void units_init(microstrip_gui *gui,GtkWidget *parent)
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


  gui->text_freq = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_box_pack_start (GTK_BOX (my_hbox), gui->text_freq, FALSE, FALSE, 0);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),60,0);
  gtk_widget_show(gui->text_freq);

  
  glist = g_list_append(glist,"kHz");
  glist = g_list_append(glist,"MHz");
  glist = g_list_append(glist,"GHz");

  gui->combo_funits =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(gui->combo_funits), glist);
  gtk_combo_set_use_arrows( GTK_COMBO(gui->combo_funits), 1);
  gtk_box_pack_start (GTK_BOX (my_hbox), gui->combo_funits, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(gui->combo_funits)->entry), FALSE);
  //gdk_string_width()
  gtk_widget_set_usize(GTK_WIDGET( gui->combo_funits),60,0);
  gtk_widget_show( gui->combo_funits );


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


  gui->combo_punits =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(gui->combo_punits), glist2);
  gtk_combo_set_use_arrows( GTK_COMBO(gui->combo_punits), 1);
  gtk_box_pack_end (GTK_BOX (my_hbox), gui->combo_punits, FALSE, FALSE, 0);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(gui->combo_punits)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET( gui->combo_punits),100,0);
  gtk_signal_connect (GTK_OBJECT (GTK_COMBO(gui->combo_punits)->entry), "changed",
		      GTK_SIGNAL_FUNC (cb_punits_menu_select),
		      (gpointer) gui);
  gtk_widget_show( gui->combo_punits );
  

  text = gtk_label_new( "Physical Units:" );
  gtk_box_pack_end (GTK_BOX (my_hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);



  gtk_entry_set_text( GTK_ENTRY(gui->text_freq),"1.88" );
  gtk_entry_set_text( GTK_ENTRY(GTK_COMBO(gui->combo_funits)->entry),"GHz" );

}

static void values_init(microstrip_gui *gui, GtkWidget *parent)
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
		      GTK_SIGNAL_FUNC (mscalc_analyze), (gpointer) gui);
  gtk_table_attach_defaults (GTK_TABLE(table), button, 3, 4, 0, 1);
  gtk_widget_show (button);
  
  /* Synthesize button */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (mscalc_synthesize), (gpointer) gui);
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
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2, 3, 1, 2);
  gtk_widget_show(text);
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

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
  gui->text_W = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_W),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_W, 1, 2, 0, 1,0,0,0,0);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_W),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_W), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_W);

  gui->text_L = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_L, 1, 2, 1, 2,0,0,0,0);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_L),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_L), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_L);

  gui->text_Z0 = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_Z0, 5, 6, 0, 1);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_Z0),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_Z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_Z0);

  gui->text_elen = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_elen, 5, 6, 1, 2);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);


  gtk_entry_set_text( GTK_ENTRY(gui->text_W),"100" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_L),"1000" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_Z0),"50" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen),"90" );

  gtk_widget_show (table);
}

static void outputs_init(microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  GtkWidget *box;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (3, 6, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */
  /* Keff */
  text = gtk_label_new( "Keff" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0, 1, 0, 1);
  gtk_widget_show(text);

  box = gtk_hbox_new(FALSE,0);
  gtk_table_attach_defaults (GTK_TABLE(table), box, 2,3,0,1);
  gtk_widget_show(box);
  text = gtk_label_new( "--" );
  gtk_box_pack_start (GTK_BOX (box), text, FALSE, FALSE, 0);
  gtk_widget_show(text);

  /* Loss */
  text = gtk_label_new( "Loss" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,1,2);
  gtk_widget_show(text);

  box = gtk_hbox_new(FALSE,0);
  gtk_table_attach_defaults (GTK_TABLE(table), box, 2,3,1,2);
  gtk_widget_show(box);
  text = gtk_label_new( "dB" );
  gtk_box_pack_start (GTK_BOX (box), text, FALSE, FALSE, 0);
  gtk_widget_show(text);

  /* Loss/length */
  text = gtk_label_new( "Loss/Len" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,2,3);
  gtk_widget_show(text);

  box = gtk_hbox_new(FALSE,0);
  gtk_table_attach_defaults (GTK_TABLE(table), box, 2,3,2,3);
  gtk_widget_show(box);
  text = gtk_label_new( "dB/" );
  gtk_box_pack_start (GTK_BOX (box), text, FALSE, FALSE, 0);
  gtk_widget_show(text);
  text = gtk_label_new( MILSTR );
  gtk_box_pack_start (GTK_BOX (box), text, FALSE, FALSE, 0);
  gtk_widget_show(text);
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

  /* Skin Depth */
  text = gtk_label_new( "Skin Depth" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 0, 1);
  gtk_widget_show(text);

  box = gtk_hbox_new(FALSE,0);
  gtk_table_attach_defaults (GTK_TABLE(table), box, 2,3,2,3);
  gtk_widget_show(box);
  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,0,1);
  gtk_widget_show(text);
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

#define OUTPUT_TEXT "     "
  gui->label_keff = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->label_keff, 1,2,0,1);
  gtk_widget_show(gui->label_keff);

  gui->label_loss = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->label_loss, 1,2,1,2);
  gtk_widget_show(gui->label_loss);

  gui->label_losslen = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->label_losslen, 1,2,2,3);
  gtk_widget_show(gui->label_losslen);

  gui->label_skindepth = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->label_skindepth, 4,5,0,1);
  gtk_widget_show(gui->label_skindepth);

#undef OUTPUT_TEXT

  gtk_widget_show(table);

}


static void substrate_init(microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  GtkWidget *box;

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

  gui->text_H = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_H, 1,2,0,1);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_H),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_H), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_H);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,0,1);
  gtk_widget_show(text);
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

  text = gtk_label_new( "Er" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,1,2);
  gtk_widget_show(text);

  gui->text_er = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_er, 1,2,1,2);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_er),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_er);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,1,2);
  gtk_widget_show(text);

  text = gtk_label_new( "TanD" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 0,1,2,3);
  gtk_widget_show(text);

  gui->text_tand = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_tand, 1,2,2,3);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tand),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tand);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 2,3,2,3);
  gtk_widget_show(text);

  text = gtk_label_new( "Tmet" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 0, 1);
  gtk_widget_show(text);

  gui->text_tmet = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_tmet, 4,5,0,1);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tmet),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tmet);

  text = gtk_label_new( MILSTR );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,0,1);
  gtk_widget_show(text);
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

  text = gtk_label_new( "Rho" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4, 1, 2);
  gtk_widget_show(text);

  gui->text_rho = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_rho, 4,5,1,2);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho);

  text = gtk_label_new( "--" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 5,6,1,2);
  gtk_widget_show(text);

  text = gtk_label_new( "Rough" );
  gtk_table_attach_defaults (GTK_TABLE(table), text, 3,4,2,3);
  gtk_widget_show(text);

  gui->text_rough = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach_defaults (GTK_TABLE(table), gui->text_rough, 4,5,2,3);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rough),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rough);

  box = gtk_hbox_new(FALSE,0);
  gtk_table_attach_defaults (GTK_TABLE(table), box, 5,6,2,3);
  gtk_widget_show(box);
  text = gtk_label_new( "-rms" );
  gtk_box_pack_end (GTK_BOX (box), text, FALSE, FALSE, 0);
  gtk_widget_show(text);
  text = gtk_label_new( MILSTR );
  gtk_box_pack_end (GTK_BOX (box), text, FALSE, FALSE, 0);
  gtk_widget_show(text);
  gui->phys_units_text = g_list_append(gui->phys_units_text, text);

  gtk_entry_set_text( GTK_ENTRY(gui->text_H),"62" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_er),"4.8" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_tand),"0.01" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_tmet),"1.4" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho),"1" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_rough),"0.055" );



  gtk_widget_show(table);

}




#include "pixmaps/microstrip.xpm"

static void picture_init(microstrip_gui *gui, GtkWidget *window,GtkWidget *parent)
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

  gui->text_status = gtk_label_new( "Values Out Of Sync" );
  gtk_box_pack_start (GTK_BOX (my_hbox), gui->text_status, FALSE, FALSE, 0);
  gtk_widget_show (gui->text_status);
  

}

static void mscalc_analyze( GtkWidget *w, gpointer data )
{
  mscalc(WC_MICROSTRIP_GUI(data), w, "analyze");
}

static void mscalc_synthesize( GtkWidget *w, gpointer data )
{
  mscalc(WC_MICROSTRIP_GUI(data), w, "synthesize");
}

static void mscalc( microstrip_gui *gui, GtkWidget *w, gpointer data )
{
  double freq;
  char str[80];
  char *vstr;
  double sf=1.0;


  /* get the physical units */
  vstr = gtk_entry_get_text( GTK_ENTRY(GTK_COMBO(gui->combo_punits)->entry) ); 
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
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_H) ); 
  gui->line->subs->h=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) ); 
  gui->line->subs->er=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) ); 
  gui->line->subs->tand=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) ); 
  gui->line->subs->tmet=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) ); 
  gui->line->subs->rough=sf*atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->line->subs->rho=atof(vstr);



  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  freq=atof(vstr);

  vstr = gtk_entry_get_text( GTK_ENTRY(GTK_COMBO(gui->combo_funits)->entry) ); 
  if (strcmp(vstr,"kHz")==0)
    freq = 1.0e3*freq;
  else if (strcmp(vstr,"MHz")==0)
    freq = 1.0e6*freq;
  else if (strcmp(vstr,"GHz")==0)
    freq = 1.0e9*freq;


  /* if "analyze" get W and L */
  if( strcmp(data,"analyze")==0)
    {
      vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_W) ); 
      gui->line->w=sf*atof(vstr);
      
      vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_L) ); 
      gui->line->l=sf*atof(vstr);

      if (microstrip_calc(gui->line,freq) != 0)
      	alert("Analysis failed for an unknown reason.\n"
	      "Please save your inputs and send the file\n"
	      "to the program author.\n");

      sprintf(str,"%8f",gui->line->z0);
      gtk_entry_set_text( GTK_ENTRY(gui->text_Z0), str );
			  
      sprintf(str,"%8f",gui->line->len);
      gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );


    }
  else if( strcmp(data,"synthesize")==0)
    {
      /* if "synthesize" get z0 and len */
      vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Z0) ); 
      gui->line->z0 = atof(vstr);
      gui->line->Ro = gui->line->z0;

      vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
      gui->line->len = atof(vstr);

      if (microstrip_syn(gui->line,freq,MLISYN_W) != 0)
	alert("Synthesis failed\n");

 
      sprintf(str,"%8f",gui->line->w/sf);
      gtk_entry_set_text( GTK_ENTRY(gui->text_W), str );
			  
      sprintf(str,"%8f",gui->line->l/sf);
      gtk_entry_set_text( GTK_ENTRY(gui->text_L), str );

    }
  else
    {
      g_print("error in mstrip callback");
    }

  sprintf(str,"%8f",gui->line->keff);
  gtk_label_set_text( GTK_LABEL(gui->label_keff), str );
  
  sprintf(str,"%8f",gui->line->loss);
  gtk_label_set_text( GTK_LABEL(gui->label_loss), str );
  
  sprintf(str,"%8f",gui->line->losslen*sf);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen), str );
  
  sprintf(str,"%8f",gui->line->skindepth/sf);
  gtk_label_set_text( GTK_LABEL(gui->label_skindepth), str );
  
  gtk_label_set_text(GTK_LABEL(gui->text_status), "");

}


static void tooltip_init(microstrip_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);

  gtk_tooltips_set_tip(tips, gui->text_W, "Width of the microstrip line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_L, "Length of the microstrip line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_Z0, "Characteristic impedance of the microstrip line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, "Electrical length of the microstrip line", NULL);

  gtk_tooltips_set_tip(tips, gui->text_H, "Thickness of the substrate dielectric", NULL);
  gtk_tooltips_set_tip(tips, gui->text_er, "Relative permitivity of the dielectric", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tand, "Loss tangent of the dielectric", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tmet, "Thickness of the metalization", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho, "Resistance of the metalization relative to copper", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rough, "RMS deviation of the metal surface from a plane" 
		       "(surface roughness)", NULL);

  
}

static void change_units_text(void * , char *);

static void cb_punits_menu_select( GtkWidget *item,
				   gpointer data)
{
  char *units;
  microstrip_gui *gui;

  gui = WC_MICROSTRIP_GUI(data);

  units = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(gui->combo_punits)->entry));

  /* Set the value position on both scale widgets */
  /*
  g_sf = sf; 
  g_print("set global physical scale factor to %g\n",sf);
  g_print("set global physical units to \"%s\"\n",punit);
  g_print("cb_punits_menu_select():  \n");
  */

  g_list_foreach( gui->phys_units_text,
		  (GFunc) change_units_text,
		  units );
}


static void change_units_text(void * text, char * label)
{
  gtk_label_set_text(GTK_LABEL(text),label);
}



static void vals_changedCB(GtkWidget *widget, gpointer data )
{
  microstrip_gui *gui;

  gui = WC_MICROSTRIP_GUI(data);

  if(WC_WCALC(gui)->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "Values Out Of Sync");
}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("microstrip_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  microstrip_save(WC_MICROSTRIP_GUI(wcalc)->line,fp,name);
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  microstrip_gui *gui;
  char *file;

  gui = WC_MICROSTRIP_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("microstrip.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "microstrip.eps");
  eps_cat(file,fp);

  /* print the data */

  fprintf(fp,"%% spit out the numbers\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"/col1x currentpoint pop def\n");
  fprintf(fp,"/col2x %g 2 div inch def\n",global_print_config->paperwidth);
  fprintf(fp,"/coly currentpoint exch pop def\n");
  fprintf(fp,"/linespace 1.5 def\n");
  fprintf(fp,"\n");
  fprintf(fp,"col1x coly moveto\n");
  fprintf(fp,"/leftcol col1x  def\n");

  fprintf(fp,"(W) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->w,gui->line->units_lwht->name);
  fprintf(fp,"(H) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->subs->h,gui->line->units_lwht->name);
  fprintf(fp,"(L) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->l,gui->line->units_lwht->name);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Tmet) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->subs->tmet,gui->line->subs->tmet_units);
  fprintf(fp,"(Rho) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->subs->rho,gui->line->subs->rho_units);
  fprintf(fp,"(Rough) show tab1 (=) show tab2 (%g %s-rms) show newline\n",
	  gui->line->subs->rough,gui->line->subs->rough_units);
  fprintf(fp,"(e) symbolshow (r) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->subs->er);
  fprintf(fp,"(tan) show (d) symbolshow tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->subs->tand);

  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(Z0) show tab1 (=) show tab2 (%g ) show (W) symbolshow newline\n",
	  gui->line->z0);
  fprintf(fp,"(keff) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->keff);
  fprintf(fp,"(elen) show tab1 (=) show tab2 (%g deg) show newline\n",
	  gui->line->len);
  fprintf(fp,"(Loss) show tab1 (=) show tab2 (%g dB) show newline\n",
	  gui->line->loss);
  fprintf(fp,"(Loss/Len) show tab1 (=) show tab2 (%g dB/mil) show newline\n",
	  gui->line->losslen);
  fprintf(fp,"(skin depth) show tab1 (=) show tab2 (%g mil) show newline\n",
	  gui->line->skindepth);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Ls) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->Ls, gui->line->units_L->name);
  fprintf(fp,"(Rs) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->Rs, gui->line->units_R->name);
  fprintf(fp,"(Cs) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->Cs, gui->line->units_C->name);
  fprintf(fp,"(Gs) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->Gs, gui->line->units_G->name);
  
}

