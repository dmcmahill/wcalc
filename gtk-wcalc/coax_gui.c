/* $Id: coax_gui.c,v 1.4 2001/12/16 17:59:40 dan Exp $ */

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

#define DEBUG

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
#include "misc.h"

#include "coax.h"
#include "coax_gui.h"
#include "coax_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

static void print_ps(Wcalc *wcalc,FILE *fp);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_a( GtkWidget *w, gpointer data );
static void synthesize_b( GtkWidget *w, gpointer data );
static void synthesize_c( GtkWidget *w, gpointer data );
static void synthesize_er( GtkWidget *w, gpointer data );
static void synthesize_len( GtkWidget *w, gpointer data );
static void calculate( coax_gui *gui, GtkWidget *w, gpointer data );
static void update_display( coax_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void values_init(coax_gui *gui, GtkWidget *parent);
static void outputs_init(coax_gui *gui, GtkWidget *parent);
static void picture_init(coax_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(coax_gui *gui);


#define ENTRYLENGTH  8
#define XPAD 2
#define YPAD 0

static char *name="Coaxial Transmission Line Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

coax_gui *coax_gui_new(void)
{
  coax_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (coax_gui *) malloc(sizeof(coax_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"coax_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = coax_gui_init;
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

  /* create the coax line which will be used */
  new_gui->line = coax_new();
  new_gui->phys_units_text = NULL;

  return new_gui;
}

void coax_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  coax_gui *gui;

  gui = WC_COAX_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("coax_gui.c:coax_gui_init():  calling coax_load\n");
#endif
    if (coax_load(gui->line,fp) != 0) {
      alert("ERROR:  Could not load the coax line\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("coax_gui.c:coax_gui_init():  new using defaults\n");
#endif
  }

  /* create the other vbox's and pack them into the main vbox */
  values_vbox = gtk_vbox_new (FALSE, 1);
  outputs_vbox = gtk_vbox_new (FALSE, 1);
  picture_vbox = gtk_vbox_new (FALSE, 1);

  gtk_container_set_border_width (GTK_CONTAINER (values_vbox), 5); 
  gtk_container_set_border_width (GTK_CONTAINER (outputs_vbox), 5); 
  gtk_container_set_border_width (GTK_CONTAINER (picture_vbox), 5); 

  gtk_box_pack_start (GTK_BOX (main_vbox), values_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), outputs_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), picture_vbox, FALSE, TRUE, 0);

  gtk_widget_show (values_vbox);
  gtk_widget_show (outputs_vbox);
  gtk_widget_show (picture_vbox);

  values_init(gui,values_vbox);
  outputs_init(gui,outputs_vbox);
  picture_init(gui,wcalc->window,picture_vbox);

  tooltip_init(gui);

  gtk_widget_show (wcalc->window);

  wcalc->init_done=1;

  update_display(gui);
}

/*
 * Private Functions
 */

static void values_init(coax_gui *gui, GtkWidget *parent)
{
  GtkWidget *hbox;
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *combo;
  GList *PhysUnits=NULL;
  GList *IndUnits=NULL;
  GList *FreqUnits=NULL;

  /* the "Len/Fill" radio button group */
  GSList *len_group;


  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (8, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  


  IndUnits = g_list_append(IndUnits,"pH");
  IndUnits = g_list_append(IndUnits,"nH");
  IndUnits = g_list_append(IndUnits,"uH");
  IndUnits = g_list_append(IndUnits,"mH");
  IndUnits = g_list_append(IndUnits,"H");

  PhysUnits = g_list_append(PhysUnits,"um");
  PhysUnits = g_list_append(PhysUnits,"mm");
  PhysUnits = g_list_append(PhysUnits,"cm");
  PhysUnits = g_list_append(PhysUnits,"m");
  PhysUnits = g_list_append(PhysUnits,"mil");
  PhysUnits = g_list_append(PhysUnits,"inches");

  FreqUnits = g_list_append(FreqUnits,"Hz");
  FreqUnits = g_list_append(FreqUnits,"kHz");
  FreqUnits = g_list_append(FreqUnits,"MHz");
  FreqUnits = g_list_append(FreqUnits,"GHz");

  /* Analyze button */
  button = gtk_button_new_with_label ("Analyze->");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 6, 7, 0,GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_widget_show (button);
  
  /* 
   * Synthesize buttons 
   */

  /* a */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_a), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show (button);

  /* b */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_b), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show (button);

  /* c */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_c), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show (button);

  /* Len */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_len), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 3, 4, 0, GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_widget_set_sensitive (button, FALSE);
  gtk_widget_show (button);

  /* er */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_er), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show (button);



  /* Text labels */
  text = gtk_label_new( "a" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "b" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  /*
  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), PhysUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_table_attach(GTK_TABLE(table), combo, 2, 3, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->line->dia_units);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (dia_units_changed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);
  gtk_widget_show(combo);
  */

  text = gtk_label_new( "c" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "Length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "Er" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Tand" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Emax" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 6, 7, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "V/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 6, 7, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "t" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 7, 8, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 7, 8, 0,0,XPAD,YPAD);
  gtk_widget_show(text);



  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "ohms" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Elen" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "degrees" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "TE10 Cutoff" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "MHz" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "MHz" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "RHO_a" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "ohm-m" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "RHO_b" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 5, 6, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "ohm-m" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 5, 6, 0,0,XPAD,YPAD);
  gtk_widget_show(text);



  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   XPAD,YPAD);
  gtk_widget_show(text);

  
#define WIDTH 60
  gui->text_a = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_a),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_a, 1, 2, 0, 1,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_a),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_a), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_a), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_a);

  gui->text_b = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_b, 1, 2, 1, 2,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_b),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_b), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_b), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_b);

  gui->text_c = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_c, 1, 2, 2, 3,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_c),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_c), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_c), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_c);

  gui->text_len = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_len, 1, 2, 3, 4,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_len),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_len);

  gui->text_er = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_er, 1, 2, 4, 5,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_er),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_er);

  gui->text_tand = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tand, 1, 2, 5, 6,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tand),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tand);

  gui->text_emax = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_emax, 1, 2, 6, 7,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_emax),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_emax), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_emax), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_set_sensitive(gui->text_emax,FALSE);
  gtk_widget_show(gui->text_emax);

  gui->text_tshield = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tshield, 1, 2, 7, 8,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tshield),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tshield), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tshield), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tshield);

  gui->text_z0 = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0, 6, 7, 0, 1,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0);

  gui->text_elen = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_elen, 6, 7, 1, 2,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);

  gui->text_fc = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_fc, 6, 7, 2, 3,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_fc),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_fc), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_fc), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_fc);

  gui->text_freq = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq, 6, 7, 3, 4,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);

  gui->text_rho_a = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho_a, 6, 7, 4, 5,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho_a),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho_a), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho_a), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho_a);

  gui->text_rho_b = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho_b, 6, 7, 5, 6,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho_b),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho_b), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho_b), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho_b);

  gtk_widget_show (table);
}

static void outputs_init(coax_gui *gui, GtkWidget *parent)
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

  table = gtk_table_new (5, 7, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */
  text = gtk_label_new( "delay" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "s" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "dB" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "loss/length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "dB/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Conductor loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "dB/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Dielectric loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "dB/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Henries/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Ohms/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Farads/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Siemens/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  /* spacer */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   XPAD,YPAD);
  gtk_widget_show(text);



#define OUTPUT_TEXT "     "
  gui->label_delay = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_delay,
		    1,2,0,1, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_delay);

  gui->label_loss = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss,
		    1,2,1,2, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_loss);

  gui->label_losslen = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen, 
		    1,2,2,3, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_losslen);

  gui->label_closs = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_closs,
		    1,2,3,4, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_closs);

  gui->label_dloss = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_dloss,
		    1,2,4,5, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_dloss);


  gui->label_L = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_L, 5,6,0,1, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_L);

  gui->label_R = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_R, 5,6,1,2, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_R);

  gui->label_C = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_C, 5,6,2,3, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_C);

  gui->label_G = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_G, 5,6,3,4, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_G);

#undef OUTPUT_TEXT

  gtk_widget_show(table);

}




#include "pixmaps/coax.xpm"

static void picture_init(coax_gui *gui, GtkWidget *window,GtkWidget *parent)
{
  GtkWidget *my_hbox;
  GtkWidget *pixmapwid;
  GdkPixmap *pixmap;
  GdkBitmap *mask;
  GtkStyle *style;    
  GtkWidget *frame;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
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
					 (gchar **) coax);
					
  
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (my_hbox), pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    

  gui->text_status = gtk_label_new( "Values Out Of Sync" );
  gtk_box_pack_start (GTK_BOX (my_hbox), gui->text_status, FALSE, FALSE, 0);
  gtk_widget_show (gui->text_status);
  

}

static void analyze( GtkWidget *w, gpointer data )
{
  calculate(WC_COAX_GUI(data), w, "analyze");
}

static void synthesize_a( GtkWidget *w, gpointer data )
{
  calculate(WC_COAX_GUI(data), w, "synthesize_a");
}

static void synthesize_b( GtkWidget *w, gpointer data )
{
  calculate(WC_COAX_GUI(data), w, "synthesize_b");
}

static void synthesize_c( GtkWidget *w, gpointer data )
{
  calculate(WC_COAX_GUI(data), w, "synthesize_c");
}

static void synthesize_er( GtkWidget *w, gpointer data )
{
  calculate(WC_COAX_GUI(data), w, "synthesize_er");
}

static void synthesize_len( GtkWidget *w, gpointer data )
{
  calculate(WC_COAX_GUI(data), w, "synthesize_len");
}


static void calculate( coax_gui *gui, GtkWidget *w, gpointer data )
{
  char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_a) ); 
  gui->line->a=atof(vstr)*gui->line->a_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  a = %g\n",gui->line->a);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_b) ); 
  gui->line->b=atof(vstr)*gui->line->b_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  b = %g\n",gui->line->b);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_c) ); 
  gui->line->c=atof(vstr)*gui->line->c_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  c = %g\n",gui->line->c);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_len) ); 
  gui->line->len=atof(vstr)*gui->line->len_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  len = %g\n",gui->line->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) ); 
  gui->line->er=atof(vstr);
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  er = %g\n",gui->line->er);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) ); 
  gui->line->tand=atof(vstr);
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  tand = %g\n",gui->line->tand);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_emax) ); 
  gui->line->emax=atof(vstr)*gui->line->emax_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  emax = %g\n",gui->line->emax);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tshield) ); 
  gui->line->tshield=atof(vstr)*gui->line->tshield_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  tshield = %g\n",gui->line->tshield);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0) ); 
  gui->line->z0=atof(vstr);
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  z0 = %g\n",gui->line->z0);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
  gui->line->elen=atof(vstr);
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  elen = %g\n",gui->line->elen);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->line->freq=atof(vstr)*gui->line->freq_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  freq = %g\n",gui->line->freq);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_fc) ); 
  gui->line->fc=atof(vstr)*gui->line->fc_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  fc = %g\n",gui->line->fc);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho_a) ); 
  gui->line->rho_a=atof(vstr)*gui->line->rho_a_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  rho_a = %g\n",gui->line->rho_a);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho_b) ); 
  gui->line->rho_b=atof(vstr)*gui->line->rho_b_sf;
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  rho_b = %g\n",gui->line->rho_b);
#endif


  // XXX should use an enum and switch...
  if( strcmp(data,"analyze")==0) {
    rslt=coax_calc(gui->line,gui->line->freq);
  }
  else if( strcmp(data,"synthesize_a")==0) {
    rslt=coax_syn(gui->line,gui->line->freq,COAXSYN_A);
  }
  else if( strcmp(data,"synthesize_b")==0) {
    rslt=coax_syn(gui->line,gui->line->freq,COAXSYN_B);
  }
  else if( strcmp(data,"synthesize_c")==0) {
    rslt=coax_syn(gui->line,gui->line->freq,COAXSYN_C);
  }
  else if( strcmp(data,"synthesize_er")==0) {
    rslt=coax_syn(gui->line,gui->line->freq,COAXSYN_ER);
  }
  else{
    g_print("error in coax callback.  data=\"%s\"",(char *)data);
    exit(1);
  }
  
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  finished calculation\n");
#endif

  update_display(gui);

  /*
   * if the calculation completed with no errors, then clear the
   * "values out of sync" field
   */
  if (rslt==0)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "");
  else
    gtk_label_set_text(GTK_LABEL(gui->text_status), "Values out of\nrange.");

}

static void update_display(coax_gui *gui)
{
  char str[80];
  char *vstr;
  double sf;

  sprintf(str,"%.4g",gui->line->a/gui->line->a_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_a), str );

  sprintf(str,"%.4g",gui->line->b/gui->line->b_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_b), str );

  sprintf(str,"%.4g",gui->line->c/gui->line->c_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_c), str );

  sprintf(str,"%.4g",gui->line->len/gui->line->len_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_len), str );

  sprintf(str,"%.4g",gui->line->er);
  gtk_entry_set_text( GTK_ENTRY(gui->text_er), str );

  sprintf(str,"%.4g",gui->line->tand);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tand), str );

  sprintf(str,"%.4g",gui->line->emax/gui->line->emax_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_emax), str );

  sprintf(str,"%.4g",gui->line->tshield/gui->line->tshield_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tshield), str );

  sprintf(str,"%.4g",gui->line->z0);
  gtk_entry_set_text( GTK_ENTRY(gui->text_z0), str );

  sprintf(str,"%.4g",gui->line->elen);
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );

  sprintf(str,"%.4g",gui->line->fc/gui->line->fc_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_fc), str );

  sprintf(str,"%.4g",gui->line->freq/gui->line->freq_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );

  sprintf(str,"%.4g",gui->line->rho_a/gui->line->rho_a_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho_a), str );

  sprintf(str,"%.4g",gui->line->rho_b/gui->line->rho_b_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho_b), str );


  sprintf(str,"%8.4g",gui->line->delay/gui->line->delay_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );
  
  sprintf(str,"%8.4g",gui->line->loss/gui->line->loss_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_loss), str );
  
  sprintf(str,"%8.4g",gui->line->losslen/gui->line->losslen_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen), str );

  sprintf(str,"%8.4g",gui->line->alpha_c/gui->line->loss_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_closs), str );
  
  sprintf(str,"%8.4g",gui->line->alpha_d/gui->line->loss_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_dloss), str );
    
  sprintf(str,"%8.4g",gui->line->L/gui->line->L_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_L), str );
  
  sprintf(str,"%8.4g",gui->line->R/gui->line->R_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_R), str );
  
  sprintf(str,"%8.4g",gui->line->C/gui->line->C_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_C), str );
  
  sprintf(str,"%8.4g",gui->line->G/gui->line->G_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_G), str );
  
}


static void tooltip_init(coax_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_a, "Radius of inner conductor", NULL);
  gtk_tooltips_set_tip(tips, gui->text_b, "Inner radius of outer conductor", NULL);
  gtk_tooltips_set_tip(tips, gui->text_c, "Offset of inner conductor", NULL);
  gtk_tooltips_set_tip(tips, gui->text_len, "Physical length of line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_er, "Dielectric relative permitivitty", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tand, "Dielectric loss tangent", NULL);
  gtk_tooltips_set_tip(tips, gui->text_emax, "Breakdown field strength "
		       "in the dielectric", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tshield, "Thickness of outer conductor", NULL);

  gtk_tooltips_set_tip(tips, gui->text_z0, "Characteristic impedance", NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, "Electrical length of line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);
  gtk_tooltips_set_tip(tips, gui->text_fc, "Cutoff frequency for"
		       " TE10 mode", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho_a, "Resistivity of inner conductor", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho_b, "Resistivity of outer conductor", NULL);

  
}

static void vals_changedCB(GtkWidget *widget, gpointer data )
{
  coax_gui *gui;

  gui = WC_COAX_GUI(data);

  if(WC_WCALC(gui)->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "Values Out Of Sync");
}


static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("coax_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  coax_save(WC_COAX_GUI(wcalc)->line,fp,name);
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  coax_gui *gui;
  char *file;

  gui = WC_COAX_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("coax.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "coax.eps");
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
#ifdef notdef
  /* XXX finish this */
  fprintf(fp,"(N) show tab1 (=) show tab2 (%g turns) show newline\n",
	  gui->line->Nf);
  fprintf(fp,"(I.D.) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->dia/gui->line->dia_sf,
	  gui->line->dia_units);
  fprintf(fp,"(Len.) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->len/gui->line->len_sf,
	  gui->line->len_units);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Wire Size) show tab1 (=) show tab2 (%g AWG) show newline\n",
	  gui->line->AWGf);
  fprintf(fp,"(Rho) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->rho);
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(L) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->L/gui->line->L_sf,
	  gui->line->L_units);
  fprintf(fp,"(Q) show tab1 (=) show tab2 (%g at %g %s) show newline\n",
	  gui->line->Q,gui->line->freq/gui->line->freq_sf,
	  gui->line->freq_units);
  fprintf(fp,"(SRF) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->SRF/gui->line->freq_sf,
	  gui->line->freq_units);
  fprintf(fp,"(Closewound) show\n");
  fprintf(fp,"linespace /linespace 1.0 def newline /linespace exch def \n");
  fprintf(fp,"(inductance) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->Lmax/gui->line->L_sf,
	  gui->line->L_units);
  fprintf(fp,"(fill) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->fill);
  fprintf(fp,"newline\n");
#endif

}

