/* $Id: ic_microstrip_gui.c,v 1.1 2001/11/12 04:02:47 dan Exp $ */

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

#include "ic_microstrip.h"
#include "ic_microstrip_gui.h"
#include "ic_microstrip_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

static void print_ps(Wcalc *wcalc,FILE *fp);

static void dia_units_changed(GtkWidget *w, gpointer data );
static void len_units_changed(GtkWidget *widget, gpointer data );
static void use_len_pressed(GtkWidget *widget, gpointer data );
static void use_fill_pressed(GtkWidget *widget, gpointer data );
static void L_units_changed(GtkWidget *widget, gpointer data );
static void freq_units_changed(GtkWidget *widget, gpointer data );

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_w( GtkWidget *w, gpointer data );
static void synthesize_h( GtkWidget *w, gpointer data );
static void synthesize_tox( GtkWidget *w, gpointer data );
static void calculate( ic_microstrip_gui *gui, GtkWidget *w, gpointer data );
static void update_display( ic_microstrip_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void values_init(ic_microstrip_gui *gui, GtkWidget *parent);
static void outputs_init(ic_microstrip_gui *gui, GtkWidget *parent);
static void picture_init(ic_microstrip_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(ic_microstrip_gui *gui);


#define ENTRYLENGTH  8
#define XPAD 2
#define YPAD 0

static char *name="Ic_Microstrip Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

ic_microstrip_gui *ic_microstrip_gui_new(void)
{
  ic_microstrip_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (ic_microstrip_gui *) malloc(sizeof(ic_microstrip_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"ic_microstrip_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = ic_microstrip_gui_init;
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

  /* create the ic_microstrip line which will be used */
  new_gui->line = ic_microstrip_line_new();
  new_gui->phys_units_text = NULL;

  return new_gui;
}

void ic_microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  ic_microstrip_gui *gui;

  gui = WC_IC_MICROSTRIP_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("ic_microstrip_gui.c:ic_microstrip_gui_init():  calling ic_microstrip_load\n");
#endif
    if (ic_microstrip_load(gui->line,fp) != 0) {
      alert("ERROR:  Could not load the ic_microstrip\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("ic_microstrip_gui.c:ic_microstrip_gui_init():  new using defaults\n");
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

static void values_init(ic_microstrip_gui *gui, GtkWidget *parent)
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
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 2, 5, 0,GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_widget_show (button);
  
  /* 
   * Synthesize buttons 
   */

  /* width */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_w), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show (button);

  /* height */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_h), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show (button);

  /* oxide thickness */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_tox), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 5, 6, 0, GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_widget_show (button);



  /* Text labels */
  text = gtk_label_new( "Width (W)" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  /* Length*/
  text = gtk_label_new( "Length (L)" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
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

  /* Height */
  text = gtk_label_new( "Substrate Height (H)" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Es" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "sigmas" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Tox" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Eox" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 6, 7, 0,0,XPAD,YPAD);
  gtk_widget_show(text);


  text = gtk_label_new( "Ro" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Elec. Len." );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Tmet" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Rho" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Rough" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 5, 6, 0,0,XPAD,YPAD);
  gtk_widget_show(text);



  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   XPAD,YPAD);
  gtk_widget_show(text);

  
#define WIDTH 60
  gui->text_w = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_w),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_w, 1, 2, 0, 1,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_w),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_w);

  gui->text_l = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_l, 1, 2, 1, 2,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_l),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_l), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_l), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_l);

  gui->text_h = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_h, 1, 2, 2, 3,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_h),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_h), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_h), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_h);

  gui->text_es = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_es, 1, 2, 3, 4,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_es),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_es), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_es), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_es);

  gui->text_sigmas = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_sigmas, 1, 2, 4, 5,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_sigmas),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_sigmas), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_sigmas), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_sigmas);

  gui->text_tox = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tox, 1, 2, 5, 6,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tox),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tox), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tox), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tox);

  gui->text_eox = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_eox, 1, 2, 6, 7,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_eox),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_eox), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_eox), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_eox);

  gui->text_Ro = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_Ro, 7, 8, 0, 1,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_Ro),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_Ro), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_Ro), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_Ro);

  gui->text_elen = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_elen, 7, 8, 1, 2,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);

  gui->text_tmet = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tmet, 7, 8, 2, 3,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tmet),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tmet);


  gui->text_rho  = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho, 7, 8, 3, 4,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho);

  gui->text_rough  = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rough, 7, 8, 4, 5,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rough),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rough);

  gui->text_freq  = gtk_entry_new_with_max_length( ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq, 7, 8, 5, 6,0,0,XPAD,YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);

  gtk_widget_show (table);
}

static void outputs_init(ic_microstrip_gui *gui, GtkWidget *parent)
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

  table = gtk_table_new (5, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */
  text = gtk_label_new( "Delay" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Loss/Length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Metal Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Substrate Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Keff" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 4, 5, 0,0,XPAD,YPAD);
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

  gui->label_met_depth = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_met_depth, 
		    1,2,3,4, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_met_depth);

  gui->label_subs_depth = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_subs_depth, 
		    1,2,4,5, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_subs_depth);

  gui->label_keff = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_keff, 
		    1,2,5,6, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_keff);

  gui->label_Lmis = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lmis, 
		    5,6,0,1, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Lmis);

  gui->label_Rmis = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Rmis, 
		    5,6,1,2, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Rmis);

  gui->label_Cmis = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Cmis, 
		    5,6,2,3, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Cmis);

  gui->label_Gmis = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Gmis, 
		    5,6,3,4, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_Gmis);

  gui->label_z0 = gtk_label_new( OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_z0, 
		    5,6,4,5, 0,0,XPAD,YPAD);
  gtk_widget_show(gui->label_z0);

#undef OUTPUT_TEXT

  gtk_widget_show(table);

}




#include "pixmaps/ic_microstrip.xpm"

static void picture_init(ic_microstrip_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) ic_microstrip);
					
  
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
  calculate(WC_IC_MICROSTRIP_GUI(data), w, "analyze");
}

static void synthesize_w( GtkWidget *w, gpointer data )
{
  calculate(WC_IC_MICROSTRIP_GUI(data), w, "synthesize_w");
}

static void synthesize_h( GtkWidget *w, gpointer data )
{
  calculate(WC_IC_MICROSTRIP_GUI(data), w, "synthesize_h");
}

static void synthesize_tox( GtkWidget *w, gpointer data )
{
  calculate(WC_IC_MICROSTRIP_GUI(data), w, "synthesize_tox");
}


static void calculate( ic_microstrip_gui *gui, GtkWidget *w, gpointer data )
{
  char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_w) ); 
  gui->line->w=atof(vstr)*gui->line->w_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  w = %g\n",gui->line->w);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l) ); 
  gui->line->l=atof(vstr)*gui->line->l_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  l = %g\n",gui->line->l);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_h) ); 
  gui->line->subs->h=atof(vstr)*gui->line->subs->h_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  h = %g\n",gui->line->subs->h);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tox) ); 
  gui->line->subs->tox=atof(vstr)*gui->line->subs->tox_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  tox = %g\n",gui->line->subs->tox);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_es) ); 
  gui->line->subs->es=atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  es = %g\n",gui->line->subs->es);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_es) ); 
  gui->line->subs->es=atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  es = %g\n",gui->line->subs->es);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_sigmas) ); 
  gui->line->subs->sigmas=atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  tand = %g\n",gui->line->subs->sigmas);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Ro) ); 
  gui->line->Ro=atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  z0 = %g\n",gui->line->Ro);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
  gui->line->len=atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  elen = %g\n",gui->line->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) ); 
  gui->line->subs->tmet=atof(vstr)*gui->line->subs->tmet_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  tmet = %g\n",gui->line->subs->tmet);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->line->subs->rho=atof(vstr)*gui->line->subs->rho_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  rho = %g\n",gui->line->subs->rho);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) ); 
  gui->line->subs->rough=atof(vstr)*gui->line->subs->rough_sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  rough = %g\n",gui->line->subs->rough);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->line->freq=atof(vstr)*gui->line->freq_sf;

#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  freq = %g\n",gui->line->freq);
#endif

#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  calculation with \"%s\"\n",
	  data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt=ic_microstrip_calc(gui->line,gui->line->freq);
  }
  else if( strcmp(data,"synthesize_w")==0) {
    rslt=ic_microstrip_syn(gui->line,gui->line->freq,IC_MLISYN_W);
  }
  else if( strcmp(data,"synthesize_h")==0) {
    rslt=ic_microstrip_syn(gui->line,gui->line->freq,IC_MLISYN_H);
  }
  else if( strcmp(data,"synthesize_tox")==0) {
    rslt=ic_microstrip_syn(gui->line,gui->line->freq,IC_MLISYN_TOX);
  }
  else{
    fprintf(stderr,"ic_microstrip_gui.c:  error in ic_microstrip callback\n"
	    "Please report this bug and how you triggered it\n");
    exit(1);
  }
  
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  finished calculation\n");
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

static void update_display(ic_microstrip_gui *gui)
{
  char str[80];
  char *vstr;
  double sf;

  /* the entries */

  sprintf(str,"%.4g",gui->line->w/gui->line->w_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_w), str );

  sprintf(str,"%.4g",gui->line->l/gui->line->l_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_l), str );

  sprintf(str,"%.4g",gui->line->subs->h/gui->line->subs->h_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_h), str );

  sprintf(str,"%.4g",gui->line->subs->es);
  gtk_entry_set_text( GTK_ENTRY(gui->text_es), str );

  sprintf(str,"%.4g",gui->line->subs->sigmas/gui->line->subs->sigmas_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_sigmas), str );

  sprintf(str,"%.4g",gui->line->subs->tox/gui->line->subs->tox_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tox), str );

  sprintf(str,"%.4g",gui->line->subs->eox);
  gtk_entry_set_text( GTK_ENTRY(gui->text_eox), str );

  sprintf(str,"%.4g",gui->line->Ro);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Ro), str );

  sprintf(str,"%.4g",gui->line->len);
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );

  sprintf(str,"%.4g",gui->line->subs->tmet/gui->line->subs->tmet_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tmet), str );
  
  sprintf(str,"%.4g",gui->line->subs->rho);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );
  
  sprintf(str,"%.4g",gui->line->subs->rough/gui->line->subs->rough_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rough), str );
  
  sprintf(str,"%.4g",gui->line->freq/gui->line->freq_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );
  

  /* the labels */

  sprintf(str,"%.4g",gui->line->len/gui->line->freq);
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );
  
  sprintf(str,"%8.4g",gui->line->loss);
  gtk_label_set_text( GTK_LABEL(gui->label_loss), str );
    
  sprintf(str,"%8.4g",gui->line->losslen);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen), str );
    
  sprintf(str,"%8.4g",gui->line->met_skindepth);
  gtk_label_set_text( GTK_LABEL(gui->label_met_depth), str );
    
  sprintf(str,"%8.4g",gui->line->subs_skindepth);
  gtk_label_set_text( GTK_LABEL(gui->label_subs_depth), str );
    
  sprintf(str,"%8.4g",gui->line->Lmis);
  gtk_label_set_text( GTK_LABEL(gui->label_Lmis), str );
    
  sprintf(str,"%8.4g",gui->line->Rmis);
  gtk_label_set_text( GTK_LABEL(gui->label_Rmis), str );
    
  sprintf(str,"%8.4g",gui->line->Cmis);
  gtk_label_set_text( GTK_LABEL(gui->label_Cmis), str );
    
  sprintf(str,"%8.4g",gui->line->Gmis);
  gtk_label_set_text( GTK_LABEL(gui->label_Gmis), str );
    
  sprintf(str,"%8.4g + j%8.4g",gui->line->Ro,gui->line->Xo);
  gtk_label_set_text( GTK_LABEL(gui->label_z0), str );
    

}


static void tooltip_init(ic_microstrip_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_w, "Width of ic_microstrip", NULL);
  gtk_tooltips_set_tip(tips, gui->text_l, "Length of ic_microstrip", NULL);
  gtk_tooltips_set_tip(tips, gui->text_h, "Substrate thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_es, "Substrate relative"
		       " dielectric constant",NULL);
  gtk_tooltips_set_tip(tips, gui->text_sigmas, "Substrate loss tangent", NULL);
  gtk_tooltips_set_tip(tips, gui->text_eox, "Oxide relative"
		       " dielectric constant",NULL);
  gtk_tooltips_set_tip(tips, gui->text_tox, "Oxide thickness", NULL);

  gtk_tooltips_set_tip(tips, gui->text_Ro, "Characteristic impedance", NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, "Electrical length", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tmet, "Line metal thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho, "Metal resistivity", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rough, "Metal surface roughness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);
  
}

static void vals_changedCB(GtkWidget *widget, gpointer data )
{
  ic_microstrip_gui *gui;

  gui = WC_IC_MICROSTRIP_GUI(data);

  if(WC_WCALC(gui)->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "Values Out Of Sync");
}

/*
static void len_units_changed(GtkWidget *widget, gpointer data )
{
  ic_microstrip_gui *gui;
  char *vstr;

  gui = WC_IC_MICROSTRIP_GUI(data);

  if(WC_WCALC(data)->init_done){
    vstr = gtk_entry_get_text( GTK_ENTRY(widget) ); 
    gui->line->len_units = vstr;
    gui->line->len_sf = phys_units_get_sf(vstr);
#ifdef DEBUG
    g_print("ic_microstrip_gui.c:len_units_changed:  units=\"%s\", sf=%g\n",
	    gui->line->len_units, gui->line->len_sf);
#endif
  }
}
*/

static void freq_units_changed(GtkWidget *widget, gpointer data )
{
  ic_microstrip_gui *gui;
  char *vstr;

  gui = WC_IC_MICROSTRIP_GUI(data);

  if(WC_WCALC(data)->init_done){
    vstr = gtk_entry_get_text( GTK_ENTRY(widget) ); 
    gui->line->freq_units = vstr;
    gui->line->freq_sf = freq_units_get_sf(vstr);
  }
}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  ic_microstrip_save(WC_IC_MICROSTRIP_GUI(wcalc)->line,fp,name);
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  ic_microstrip_gui *gui;
  char *file;

  gui = WC_IC_MICROSTRIP_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("ic_microstrip.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "ic_microstrip.eps");
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
	  gui->line->w,gui->line->w_units);
  fprintf(fp,"(H) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->subs->h,gui->line->subs->h_units);
  fprintf(fp,"(L) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->l,gui->line->l_units);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Tmet) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->subs->tmet,gui->line->subs->tmet_units);
  fprintf(fp,"(Rho) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->subs->rho,gui->line->subs->rho_units);
  fprintf(fp,"(Rough) show tab1 (=) show tab2 (%g %s-rms) show newline\n",
	  gui->line->subs->rough,gui->line->subs->rough_units);
  fprintf(fp,"(e) symbolshow (r) show tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->subs->es);
  fprintf(fp,"(tan) show (d) symbolshow tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->subs->sigmas);
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(Z0) show tab1 (=) show tab2 (%g + j %g ) show (W) symbolshow newline\n",
	  gui->line->Ro,gui->line->Xo);
  fprintf(fp,"(elen) show tab1 (=) show tab2 (%g deg) show newline\n",
	  gui->line->len);
  fprintf(fp,"(Loss) show tab1 (=) show tab2 (%g dB) show newline\n",
	  gui->line->loss);
  fprintf(fp,"(Loss/Len) show tab1 (=) show tab2 (%g dB/mil) show newline\n",
	  gui->line->losslen);
  fprintf(fp,"(subs. skin depth) show tab1 (=) show tab2 (%g mil) show newline\n",
	  gui->line->subs_skindepth);
  fprintf(fp,"(metal skin depth) show tab1 (=) show tab2 (%g mil) show newline\n",
	  gui->line->met_skindepth);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Ls) show tab1 (=) show tab2 (%g nH/mil) show newline\n",
	  gui->line->Lmis);
  fprintf(fp,"(Rs) show tab1 (=) show tab2 (%g ) show (W) symbolshow (/mil) show newline\n",
	  gui->line->Rmis);
  fprintf(fp,"(Cs) show tab1 (=) show tab2 (%g pF/mil) show newline\n",
	  gui->line->Cmis);
  fprintf(fp,"(Gs) show tab1 (=) show tab2 (%g 1/) show (W) symbolshow (-mil) show newline\n",
	  gui->line->Gmis);
  
}

