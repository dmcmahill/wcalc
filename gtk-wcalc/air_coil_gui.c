/* $Id: air_coil_gui.c,v 1.5 2001/11/03 04:12:25 dan Exp $ */

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
#include "misc.h"

#include "air_coil.h"
#include "air_coil_gui.h"
#include "air_coil_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);

static void dia_units_changed(GtkWidget *w, gpointer data );
static void len_units_changed(GtkWidget *widget, gpointer data );
static void use_len_pressed(GtkWidget *widget, gpointer data );
static void use_fill_pressed(GtkWidget *widget, gpointer data );
static void L_units_changed(GtkWidget *widget, gpointer data );
static void freq_units_changed(GtkWidget *widget, gpointer data );

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_N( GtkWidget *w, gpointer data );
static void synthesize_dia( GtkWidget *w, gpointer data );
static void synthesize_len( GtkWidget *w, gpointer data );
static void calculate( air_coil_gui *gui, GtkWidget *w, gpointer data );
static void update_display( air_coil_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void values_init(air_coil_gui *gui, GtkWidget *parent);
static void outputs_init(air_coil_gui *gui, GtkWidget *parent);
static void picture_init(air_coil_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(air_coil_gui *gui);


#define ENTRYLENGTH  8
#define XPAD 2
#define YPAD 0

static char *name="Air Core Inductor Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

air_coil_gui *air_coil_gui_new(void)
{
  air_coil_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (air_coil_gui *) malloc(sizeof(air_coil_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"air_coil_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = air_coil_gui_init;
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

  /* create the air_coil line which will be used */
  new_gui->coil = air_coil_new();
  new_gui->phys_units_text = NULL;

  return new_gui;
}

void air_coil_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  air_coil_gui *gui;

  gui = WC_AIR_COIL_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("air_coil_gui.c:air_coil_gui_init():  calling air_coil_load\n");
#endif
    if (air_coil_load(gui->coil,fp) != 0) {
      alert("ERROR:  Could not load the air coil\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("air_coil_gui.c:air_coil_gui_init():  new using defaults\n");
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

static void values_init(air_coil_gui *gui, GtkWidget *parent)
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
  table = gtk_table_new (4, 8, FALSE);
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
  button = gtk_button_new_with_label ("Analyze");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 0, 4, 0,GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_widget_show (button);
  
  /* 
   * Synthesize buttons 
   */

  /* N */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_N), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show (button);

  /* I.D. */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_dia), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_set_sensitive (button, FALSE);
  gtk_widget_show (button);

  /* Len */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_len), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 4, 0, GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_widget_show (button);



  /* Text labels */
  text = gtk_label_new( "N" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  /* Coil diameter and units */
  text = gtk_label_new( "I.D." );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), PhysUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_table_attach(GTK_TABLE(table), combo, 2, 3, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->coil->dia_units);
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


  /* Wire Size */
  text = gtk_label_new( "Wire Dia." );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  hbox = gtk_hbox_new(FALSE,1);
  gtk_table_attach(GTK_TABLE(table), hbox, 7, 8, 0, 1, 
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_widget_show(hbox);
  text = gtk_label_new( "AWG" );
  gtk_box_pack_start (GTK_BOX (hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);

  text = gtk_label_new( "Resistivity" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  /* XXX  units for resistivity
  hbox = gtk_hbox_new(FALSE,1);
  gtk_table_attach(GTK_TABLE(table), hbox, 7, 8, 1, 2, 
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_widget_show(hbox);

  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), IndUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->coil->L_units);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (L_units_changed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);
  gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 0);
  gtk_widget_show(combo);

  text = gtk_label_new( "/" );
  gtk_box_pack_start (GTK_BOX (hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);

  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), IndUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->coil->L_units);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (L_units_changed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);
  gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 0);
  gtk_widget_show(combo);

  */

  text = gtk_label_new( "Inductance" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  hbox = gtk_hbox_new(FALSE,1);
  gtk_table_attach(GTK_TABLE(table), hbox, 7, 8, 2, 3,
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_widget_show(hbox);
  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), IndUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->coil->L_units);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (L_units_changed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);
  gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 0);
  gtk_widget_show(combo);


  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  hbox = gtk_hbox_new(FALSE,1);
  gtk_table_attach(GTK_TABLE(table), hbox, 7, 8, 3, 4,
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_widget_show(hbox);
  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), FreqUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->coil->freq_units);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (freq_units_changed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);
  gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 0);
  gtk_widget_show(combo);


  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   XPAD,YPAD);
  gtk_widget_show(text);

  
#define WIDTH 60
  gui->text_Nf = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_Nf),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_Nf, 1, 2, 0, 1,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_Nf),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_Nf), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_Nf), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_Nf);

  gui->text_dia = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_dia, 1, 2, 1, 2,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_dia),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_dia), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_dia), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_dia);

  gui->text_len = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_len, 1, 2, 2, 3,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_len),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_len);

  gui->text_fill = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_fill, 1, 2, 3, 4,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_fill),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_fill), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_fill), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_fill);

  gui->text_AWGf = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_AWGf, 6, 7, 0, 1,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_AWGf),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_AWGf), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_AWGf), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_AWGf);

  gui->text_rho = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho, 6, 7, 1, 2,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_set_sensitive(gui->text_rho,FALSE);
  gtk_widget_show(gui->text_rho);

  gui->text_L = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_L, 6, 7, 2, 3,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_L),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_L), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_L), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_L);

  gui->text_freq = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq, 6, 7, 3, 4,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);



  /* Coil length and units */
  
  button = gtk_radio_button_new_with_label (NULL, "Len.");
  gtk_table_attach(GTK_TABLE(table), button, 0, 1, 2, 3, 0,0,XPAD,YPAD);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(use_len_pressed),
		     gui);
  gui->len_button = button;
  gtk_widget_show (button);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);

  len_group = gtk_radio_button_group (GTK_RADIO_BUTTON (button));
  button = gtk_radio_button_new_with_label(len_group, "Fill");
  gtk_table_attach(GTK_TABLE(table), button, 0, 1, 3, 4, 0,0,XPAD,YPAD);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(use_fill_pressed),
		     gui);
  gui->fill_button = button;
  gtk_widget_show (button);

  if(gui->coil->use_fill){
    gtk_widget_set_sensitive (gui->text_len, FALSE);
    gtk_widget_set_sensitive (gui->text_fill, TRUE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  }
  else{
    gtk_widget_set_sensitive (gui->text_len, TRUE);
    gtk_widget_set_sensitive (gui->text_fill, FALSE);
  }


  combo =  gtk_combo_new();
  gtk_combo_set_popdown_strings( GTK_COMBO(combo), PhysUnits);
  gtk_combo_set_use_arrows( GTK_COMBO(combo), 1);
  gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);
  gtk_table_attach(GTK_TABLE(table), combo, 2, 3, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(combo),60,0);
  gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),
		     gui->coil->len_units);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (len_units_changed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);
  gtk_widget_show(combo);



  gtk_entry_set_text( GTK_ENTRY(gui->text_Nf),"10" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_dia),"0.25" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_len),"1.0" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_AWGf),"22" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho),"1" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_L),"50" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq),"10" );

  gtk_widget_show (table);
}

static void outputs_init(air_coil_gui *gui, GtkWidget *parent)
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

  table = gtk_table_new (2, 7, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */
  text = gtk_label_new( "Q" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Self Resonant Freq." );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Closewound Inductance" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Len/Closewound Len" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  gui->label_Qfreq = gtk_label_new( "@freq" );
  gtk_table_attach(GTK_TABLE(table), gui->label_Qfreq, 2, 3, 0, 1,
		   0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Qfreq);

  gui->label_SRF_units = gtk_label_new( "MHz" );
  gtk_table_attach(GTK_TABLE(table), gui->label_SRF_units, 2, 3, 1, 2,
		   0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_SRF_units);

  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  /* spacer */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   XPAD,YPAD);
  gtk_widget_show(text);



#define OUTPUT_TEXT "     "
  gui->label_Q = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Q, 
		    1,2,0,1, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Q);

  gui->label_SRF = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_SRF, 
		    1,2,1,2, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_SRF);

  gui->label_Lmax = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lmax, 
		    5,6,0,1, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Lmax);

  gui->label_Lmax_units = gtk_label_new( gui->coil->L_units );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lmax_units, 
		    6,7,0,1, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Lmax_units);


  gui->label_fill = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_fill, 5,6,1,2, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_fill);
#undef OUTPUT_TEXT

  gtk_widget_show(table);

}




#include "pixmaps/air_coil.xpm"

static void picture_init(air_coil_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) air_coil);
					
  
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
  calculate(WC_AIR_COIL_GUI(data), w, "analyze");
}

static void synthesize_N( GtkWidget *w, gpointer data )
{
  calculate(WC_AIR_COIL_GUI(data), w, "synthesize_N");
}

static void synthesize_dia( GtkWidget *w, gpointer data )
{
  calculate(WC_AIR_COIL_GUI(data), w, "synthesize_dia");
}

static void synthesize_len( GtkWidget *w, gpointer data )
{
  calculate(WC_AIR_COIL_GUI(data), w, "synthesize_len");
}


static void calculate( air_coil_gui *gui, GtkWidget *w, gpointer data )
{
  char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Nf) ); 
  gui->coil->Nf=atof(vstr);
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  Nf = %g\n",gui->coil->Nf);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_dia) ); 
  gui->coil->dia=atof(vstr)*gui->coil->dia_sf;
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  I.D. = %g\n",gui->coil->dia);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_len) ); 
  gui->coil->len=atof(vstr)*gui->coil->len_sf;
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  len = %g\n",gui->coil->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_fill) ); 
  gui->coil->fill=atof(vstr);
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  fill = %g\n",gui->coil->fill);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_AWGf) ); 
  gui->coil->AWGf=atof(vstr);
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  AWGf = %g\n",gui->coil->AWGf);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->coil->rho=atof(vstr);
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  rho = %g\n",gui->coil->rho);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_L) ); 
  gui->coil->L=atof(vstr)*gui->coil->L_sf;
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  L = %g\n",gui->coil->L);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->coil->freq=atof(vstr)*gui->coil->freq_sf;

#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  freq = %g\n",gui->coil->freq);
#endif

  // XXX should use an enum and switch...
  if( strcmp(data,"analyze")==0) {
    rslt=air_coil_calc(gui->coil,gui->coil->freq);
  }
  else if( strcmp(data,"synthesize_N")==0) {
    rslt=air_coil_syn(gui->coil,gui->coil->freq,AIRCOILSYN_NMIN);
  }
  else if( strcmp(data,"synthesize_len")==0) {
    rslt=air_coil_syn(gui->coil,gui->coil->freq,AIRCOILSYN_NFIX);
  }
  else{
    g_print("error in mstrip callback");
  }
  
#ifdef DEBUG
  g_print("air_coil_gui.c:calculate():  finished calculation\n");
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

static void update_display(air_coil_gui *gui)
{
  char str[80];
  char *vstr;
  double sf;

  sprintf(str,"%.4g",gui->coil->Nf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Nf), str );

  sprintf(str,"%.4g",gui->coil->dia/gui->coil->dia_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_dia), str );

  sprintf(str,"%.4g",gui->coil->len/gui->coil->len_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_len), str );

  sprintf(str,"%.4g",gui->coil->fill);
  gtk_entry_set_text( GTK_ENTRY(gui->text_fill), str );

  sprintf(str,"%.4g",gui->coil->L/gui->coil->L_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_L), str );
  
  sprintf(str,"%.4g",gui->coil->rho);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );
  
  sprintf(str,"%.4g",gui->coil->AWGf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_AWGf), str );
  
  sprintf(str,"%.4g",gui->coil->freq/gui->coil->freq_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );
  
  sprintf(str,"%8.4g",gui->coil->Q);
  gtk_label_set_text( GTK_LABEL(gui->label_Q), str );
    
  vstr = eng_units(gui->coil->freq,"Hz",&sf);
  sprintf(str,"@ %.4g %s",sf*gui->coil->freq,vstr);
  free(vstr);
  gtk_label_set_text(GTK_LABEL(gui->label_Qfreq),str);

  vstr = eng_units(gui->coil->SRF,"Hz",&sf);
  gtk_label_set_text(GTK_LABEL(gui->label_SRF_units),vstr);
  sprintf(str,"@ %.4g %s",sf*gui->coil->freq,vstr);
  free(vstr);
  sprintf(str,"%8.4g",gui->coil->SRF*sf);
  gtk_label_set_text( GTK_LABEL(gui->label_SRF), str );

  sprintf(str,"%8.4g",gui->coil->Lmax/gui->coil->L_sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Lmax), str );
  
  sprintf(str,"%8.4g",gui->coil->fill);
  gtk_label_set_text( GTK_LABEL(gui->label_fill), str );

}


static void tooltip_init(air_coil_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_Nf, "Number of turns", NULL);
  gtk_tooltips_set_tip(tips, gui->text_dia, "Inside diameter of the coil", NULL);
  gtk_tooltips_set_tip(tips, gui->text_len, "Length of the coil", NULL);
  gtk_tooltips_set_tip(tips, gui->text_fill, "Ratio of the length of the coil"
		       " to the length of a closewound coil",NULL);
  gtk_tooltips_set_tip(tips, gui->text_AWGf, "Wire diameter", NULL);

  gtk_tooltips_set_tip(tips, gui->text_rho, "Resistivity of the wire", NULL);
  gtk_tooltips_set_tip(tips, gui->text_L, "Inductance of the coil", NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);
  
}

static void vals_changedCB(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;

  gui = WC_AIR_COIL_GUI(data);

  if(WC_WCALC(gui)->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "Values Out Of Sync");
}

static void use_len_pressed(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;

  gui = WC_AIR_COIL_GUI(data);
  gtk_widget_set_sensitive (gui->text_len, TRUE);
  gtk_widget_set_sensitive (gui->text_fill, FALSE);
  gui->coil->use_fill=0;
}

static void use_fill_pressed(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;

  gui = WC_AIR_COIL_GUI(data);
  gtk_widget_set_sensitive (gui->text_len, FALSE);
  gtk_widget_set_sensitive (gui->text_fill, TRUE);
  gui->coil->use_fill=1;
}

static void dia_units_changed(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;
  char *vstr;

  gui = WC_AIR_COIL_GUI(data);

  if(WC_WCALC(data)->init_done){
    vstr = gtk_entry_get_text( GTK_ENTRY(widget) ); 
    gui->coil->dia_units = vstr;
    gui->coil->dia_sf = phys_units_get_sf(vstr);
#ifdef DEBUG
    g_print("air_coil_gui.c:dia_units_changed:  units=\"%s\", sf=%g\n",
	    gui->coil->dia_units, gui->coil->dia_sf);
#endif
  }
}

static void len_units_changed(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;
  char *vstr;

  gui = WC_AIR_COIL_GUI(data);

  if(WC_WCALC(data)->init_done){
    vstr = gtk_entry_get_text( GTK_ENTRY(widget) ); 
    gui->coil->len_units = vstr;
    gui->coil->len_sf = phys_units_get_sf(vstr);
#ifdef DEBUG
    g_print("air_coil_gui.c:len_units_changed:  units=\"%s\", sf=%g\n",
	    gui->coil->len_units, gui->coil->len_sf);
#endif
  }
}

static void L_units_changed(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;
  char *vstr;

  gui = WC_AIR_COIL_GUI(data);

  if(WC_WCALC(data)->init_done){
    vstr = gtk_entry_get_text( GTK_ENTRY(widget) ); 
    gui->coil->L_units = vstr;
    gui->coil->L_sf = induct_units_get_sf(vstr);
    gtk_label_set_text(GTK_LABEL(gui->label_Lmax_units),vstr);
#ifdef DEBUG
    g_print("air_coil_gui.c:L_units_changed:  units=\"%s\", sf=%g\n",
	    gui->coil->L_units, gui->coil->L_sf);
#endif
  }
}

static void freq_units_changed(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;
  char *vstr;

  gui = WC_AIR_COIL_GUI(data);

  if(WC_WCALC(data)->init_done){
    vstr = gtk_entry_get_text( GTK_ENTRY(widget) ); 
    gui->coil->freq_units = vstr;
    gui->coil->freq_sf = freq_units_get_sf(vstr);
  }
}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("air_coil_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  air_coil_save(WC_AIR_COIL_GUI(wcalc)->coil,fp,name);
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  air_coil_gui *gui;
  char *file;

  gui = WC_AIR_COIL_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("air_coil.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "air_coil.eps");
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
  fprintf(fp,"(N) show tab1 (=) show tab2 (%g turns) show newline\n",
	  gui->coil->Nf);
  fprintf(fp,"(I.D.) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->coil->dia/gui->coil->dia_sf,
	  gui->coil->dia_units);
  fprintf(fp,"(Len.) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->coil->len/gui->coil->len_sf,
	  gui->coil->len_units);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Wire Size) show tab1 (=) show tab2 (%g AWG) show newline\n",
	  gui->coil->AWGf);
  fprintf(fp,"(Rho) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->coil->rho);
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(L) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->coil->L/gui->coil->L_sf,
	  gui->coil->L_units);
  fprintf(fp,"(Q) show tab1 (=) show tab2 (%g at %g %s) show newline\n",
	  gui->coil->Q,gui->coil->freq/gui->coil->freq_sf,
	  gui->coil->freq_units);
  fprintf(fp,"(SRF) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->coil->SRF/gui->coil->freq_sf,
	  gui->coil->freq_units);
  fprintf(fp,"(Closewound) show\n");
  fprintf(fp,"linespace /linespace 1.0 def newline /linespace exch def \n");
  fprintf(fp,"(inductance) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->coil->Lmax/gui->coil->L_sf,
	  gui->coil->L_units);
  fprintf(fp,"(fill) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->coil->fill);
  fprintf(fp,"newline\n");
  
}

