/* $Id: coax_gui.c,v 1.6 2002/01/03 03:54:47 dan Exp $ */

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
#include "gtk-units.h"

#include "misc.h"

#include "coax.h"
#include "coax_gui.h"
#include "coax_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

static void print_ps(Wcalc *wcalc,FILE *fp);

static void abct_units_changed(GtkWidget *w, gpointer data );
static void abct_units_update(coax_gui *gui,int which);
static void len_units_changed(GtkWidget *w, gpointer data );
static void len_units_update(coax_gui *gui,int which);
static void freq_units_changed(GtkWidget *w, gpointer data );
static void freq_units_update(coax_gui *gui,int which);
static void rho_units_ohm_changed(GtkWidget *w, gpointer data );
static void rho_units_ohm_update(coax_gui *gui,int which);
static void rho_units_m_changed(GtkWidget *w, gpointer data );
static void rho_units_m_update(coax_gui *gui,int which);
static void rho_units_update(coax_gui *gui); 

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


#define ENTRYLENGTH  10
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

  new_gui->rho_units = resistivity_units_new();
  new_gui->L_units = inc_inductance_units_new();
  new_gui->R_units = inc_resistance_units_new();
  new_gui->C_units = inc_capacitance_units_new();
  new_gui->G_units = inc_conductance_units_new();

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

  gtk_widget_show_all (wcalc->window);

  wcalc->init_done=1;

  update_display(gui);
}

/*
 * Private Functions
 */

static void values_init(coax_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *hbox;
  GtkWidget *table;
  GtkWidget *text;
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (8, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  


  /* Analyze button */
  button = gtk_button_new_with_label ("Analyze->");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 6, 7, 0,
		   GTK_EXPAND|GTK_FILL,XPAD,YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       "Calculate electrical characteristics "
		       "from physical parameters",
		       NULL);

  
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
  gtk_tooltips_set_tip(tips, button, 
		       "Synthesize 'a' and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length.",
		       NULL);

  /* b */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_b), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 1, 2, 0,0,XPAD,YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       "Synthesize 'b' and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length.",
		       NULL);

  /* c */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_c), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 3, 0,0,XPAD,YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       "Synthesize 'c' and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length.",
		       NULL);

  /* Len */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_len), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 3, 4, 0,
		   GTK_EXPAND|GTK_FILL,XPAD,YPAD);
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
  gtk_tooltips_set_tip(tips, button, 
		       "Synthesize relative dielectric constant "
		       "and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length.",
		       NULL);



  /* Text labels */
  text = gtk_label_new( "a" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  hbox=gtk_hbox_new(FALSE,0);
  gtk_table_attach(GTK_TABLE(table), hbox,
		   2, 3, 0, 1, GTK_EXPAND|GTK_FILL, 0,XPAD,YPAD);
  gui->menu_abct_units =
    units_menu_new(length_units,0,WC_WCALC(gui),abct_units_changed);
  gtk_box_pack_start (GTK_BOX (hbox),gui->menu_abct_units,FALSE,FALSE,0);

  text = gtk_label_new( "b" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  gui->units_b = gtk_label_new("");
  gtk_table_attach(GTK_TABLE(table), gui->units_b, 2, 3, 1, 2, 
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(gui->units_b),0,0);


  /*
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), 
		      (gpointer) gui);
  gtk_signal_connect (GTK_OBJECT(GTK_COMBO(combo)->entry),
		      "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), 
		      (gpointer) gui);

  */

  text = gtk_label_new( "c" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  gui->units_c = gtk_label_new("");
  gtk_table_attach(GTK_TABLE(table), gui->units_c, 2, 3, 2, 3, 
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(gui->units_c),0,0);


  text = gtk_label_new( "Length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  hbox=gtk_hbox_new(FALSE,0);
  gtk_table_attach(GTK_TABLE(table), hbox,
		   2, 3, 3, 4, GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gui->menu_len_units =
    units_menu_new(length_units,0,WC_WCALC(gui),len_units_changed);
  gtk_box_pack_start (GTK_BOX (hbox),gui->menu_len_units,FALSE,FALSE,0);


  text = gtk_label_new( "Er" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Tand" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Emax" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 6, 7, 0,0,XPAD,YPAD);

  text = gtk_label_new( "V/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 6, 7, 
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(text),0,0);


  text = gtk_label_new( "t" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 7, 8, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  gui->units_t = gtk_label_new("");
  gtk_table_attach(GTK_TABLE(table), gui->units_t, 2, 3, 7, 8, 
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(gui->units_t),0,0);


  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 0, 1, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "ohms" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 0, 1,
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(text),0,0);

  text = gtk_label_new( "Elen" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 1, 2,
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);

  text = gtk_label_new( "degrees" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 1, 2,
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(text),0,0);

  text = gtk_label_new( "TE10 Cutoff" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 2, 3, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  gui->units_fc = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), gui->units_fc, 
		   7, 8, 2, 3, GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gtk_misc_set_alignment(GTK_MISC(gui->units_fc),0,0);

  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text,
		   5, 6, 3, 4, 0,0,XPAD,YPAD);

  hbox=gtk_hbox_new(FALSE,0);
  gtk_table_attach(GTK_TABLE(table), hbox,
		   7, 8, 3, 4, GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gui->menu_freq_units = 
    units_menu_new(frequency_units,0,WC_WCALC(gui),freq_units_changed);
  gtk_box_pack_start (GTK_BOX (hbox),gui->menu_freq_units,FALSE,FALSE,0);


  text = gtk_label_new( "RHO_a" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 4, 5, 0,0,XPAD,YPAD);
  gtk_widget_show(text);

  hbox=gtk_hbox_new(FALSE,0);
  gtk_table_attach(GTK_TABLE(table), hbox, 7, 8, 4, 5,
		   GTK_EXPAND|GTK_FILL,0,XPAD,YPAD);
  gui->menu_rho_units_ohm =
    units_menu_new(resistance_units,0,WC_WCALC(gui),rho_units_ohm_changed);
  gtk_box_pack_start (GTK_BOX (hbox),gui->menu_rho_units_ohm,FALSE,FALSE,0);
  text = gtk_label_new( "-" );
  gtk_box_pack_start (GTK_BOX (hbox),text,FALSE,FALSE,0);
  gui->menu_rho_units_m =
    units_menu_new(length_units,0,WC_WCALC(gui),rho_units_m_changed);
  gtk_box_pack_start (GTK_BOX (hbox),gui->menu_rho_units_m,FALSE,FALSE,0);

  text = gtk_label_new( "RHO_b" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 5, 6, 0,0,XPAD,YPAD);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 5, 6, 0,0,XPAD,YPAD);


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

  gui->units_delay = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), gui->units_delay, 
		   2, 3, 0, 1, 0,0,XPAD,YPAD);

  text = gtk_label_new( "loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,XPAD,YPAD);

  text = gtk_label_new( "dB" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2, 0,0,XPAD,YPAD);

  text = gtk_label_new( "loss/length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,XPAD,YPAD);

  text = gtk_label_new( "dB/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 2, 3, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Conductor loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,XPAD,YPAD);

  text = gtk_label_new( "dB/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 3, 4, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Dielectric loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,XPAD,YPAD);

  text = gtk_label_new( "dB/m" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 4, 5, 0,0,XPAD,YPAD);

  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Henries/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 0,0,XPAD,YPAD);

  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Ohms/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 1, 2, 0,0,XPAD,YPAD);

  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 2, 3, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Farads/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 2, 3, 0,0,XPAD,YPAD);

  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 3, 4, 0,0,XPAD,YPAD);

  text = gtk_label_new( "Siemens/m" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 3, 4, 0,0,XPAD,YPAD);


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

static void abct_units_changed( GtkWidget *w, gpointer data)
{
  int which;
  coax_gui *gui;

  gui = WC_COAX_GUI(data);

  which = GPOINTER_TO_INT(gtk_object_get_user_data(GTK_OBJECT(w)));
#ifdef DEBUG
  g_print("abct_units_changed():  gui  =%p\n",gui);
  g_print("abct_units_changed():  which=%d\n",which);
#endif
  abct_units_update(gui,which);
}

static void abct_units_update( coax_gui *gui,int which)
{
  gtk_option_menu_set_history(GTK_OPTION_MENU(gui->menu_abct_units), which);

  gtk_label_set_text(GTK_LABEL(gui->units_b),length_units[which].name);
  gtk_label_set_text(GTK_LABEL(gui->units_c),length_units[which].name);
  gtk_label_set_text(GTK_LABEL(gui->units_t),length_units[which].name);
  gui->line->a_sf = length_units[which].sf;
  gui->line->b_sf = length_units[which].sf;
  gui->line->c_sf = length_units[which].sf;
  gui->line->tshield_sf = length_units[which].sf;

  gui->line->a_units = length_units[which].name;
  gui->line->b_units = length_units[which].name;
  gui->line->c_units = length_units[which].name;
  gui->line->tshield_units = length_units[which].name;

}

static void len_units_changed( GtkWidget *w, gpointer data )
{
  int which;
  coax_gui *gui;

  gui=WC_COAX_GUI(data);

  which = GPOINTER_TO_INT(gtk_object_get_user_data(GTK_OBJECT(w)));
#ifdef DEBUG
  g_print("len_units_changed():  gui  =%p\n",gui);
  g_print("len_units_changed():  which=%d\n",which);
#endif
  len_units_update(gui,which);

}

static void len_units_update( coax_gui *gui,int which)
{
  gtk_option_menu_set_history(GTK_OPTION_MENU(gui->menu_len_units), which);

  gui->line->len_sf = length_units[which].sf;
  gui->line->len_units = length_units[which].name;
}

static void freq_units_changed( GtkWidget *w, gpointer data )
{
  int which;
  coax_gui *gui;

  gui = WC_COAX_GUI(data);

  which = GPOINTER_TO_INT(gtk_object_get_user_data(GTK_OBJECT(w)));
#ifdef DEBUG
  g_print("freq_units_changed():  gui  =%p\n",gui);
  g_print("freq_units_changed():  which=%d\n",which);
#endif
  freq_units_update(gui,which);
}

static void freq_units_update( coax_gui *gui,int which)
{
#ifdef DEBUG
  g_print("freq_units_update():  gui  =%p\n",gui);
  g_print("freq_units_update():  which=%d\n",which);
#endif

  gtk_option_menu_set_history(GTK_OPTION_MENU(gui->menu_freq_units), which);
  gui->line->freq_sf = frequency_units[which].sf;
  gui->line->freq_units = frequency_units[which].name;
}

static void rho_units_ohm_changed( GtkWidget *w, gpointer data )
{
  int which;
  coax_gui *gui;

  gui = WC_COAX_GUI(data);

  which = GPOINTER_TO_INT(gtk_object_get_user_data(GTK_OBJECT(w)));
#ifdef DEBUG
  g_print("rho_units_ohm_changed():  gui  =%p\n",gui);
  g_print("rho_units_ohm_changed():  which=%d\n",which);
#endif
  rho_units_ohm_update(gui,which);
}

static void rho_units_ohm_update( coax_gui *gui,int which)
{
#ifdef DEBUG
  g_print("rho_units_ohm_update():  gui  =%p\n",gui);
  g_print("rho_units_ohm_update():  which=%d\n",which);
#endif

  gtk_option_menu_set_history(GTK_OPTION_MENU(gui->menu_rho_units_ohm), which);
  gui->rho_units->numi[0] = which;
  rho_units_update(gui);

}

static void rho_units_m_changed( GtkWidget *w, gpointer data )
{
  int which;
  coax_gui *gui;

  gui = WC_COAX_GUI(data);

  which = GPOINTER_TO_INT(gtk_object_get_user_data(GTK_OBJECT(w)));
#ifdef DEBUG
  g_print("rho_units_m_changed():  gui  =%p\n",gui);
  g_print("rho_units_m_changed():  which=%d\n",which);
#endif
  rho_units_m_update(gui,which);
}

static void rho_units_m_update( coax_gui *gui,int which)
{
#ifdef DEBUG
  g_print("rho_units_m_update():  gui  =%p\n",gui);
  g_print("rho_units_m_update():  which=%d\n",which);
#endif

  gtk_option_menu_set_history(GTK_OPTION_MENU(gui->menu_rho_units_m), which);
  gui->rho_units->numi[1] = which;

  rho_units_update(gui);
}

static void rho_units_update(coax_gui *gui) 
{
  units_update(gui->rho_units,&gui->line->rho_a_sf,&gui->line->rho_a_units);
  units_update(gui->rho_units,&gui->line->rho_b_sf,&gui->line->rho_b_units);
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


  /* XXX should use an enum and switch... */
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
  int i;

  i = units_get_index(length_units,gui->line->a_sf);
  abct_units_update(gui,i);

  sprintf(str,"%.4g",gui->line->a/gui->line->a_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_a), str );

  sprintf(str,"%.4g",gui->line->b/gui->line->b_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_b), str );

  sprintf(str,"%.4g",gui->line->c/gui->line->c_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_c), str );

  i = units_get_index(length_units,gui->line->len_sf);
  len_units_update(gui,i);

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

  /* autoscale the TE10 cutoff frequency units */
  i=0;
  while ( (frequency_units[i].name != NULL) &&
	  (gui->line->fc / frequency_units[i].sf >= 1000.0) )
    {
      i++;
    }
  if (frequency_units[i].name == NULL)
    i--;
  
  gui->line->fc_sf = frequency_units[i].sf;
  gui->line->fc_units = frequency_units[i].name;
  sprintf(str,"%.4g",gui->line->fc/gui->line->fc_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_fc), str );
  gtk_label_set_text( GTK_LABEL(gui->units_fc), 
		      frequency_units[i].name );


  i = units_get_index(frequency_units,gui->line->freq_sf);
  freq_units_update(gui,i);

  sprintf(str,"%.4g",gui->line->freq/gui->line->freq_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );

  /* update the resistivity units menus */
  i = gui->line->units_rhoa->numi[0];
  rho_units_ohm_update(gui,i);

  i = gui->line->units_rhoa->numi[1];
  rho_units_m_update(gui,i);

  sprintf(str,"%.4g",gui->line->rho_a/gui->line->rho_a_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho_a), str );

  sprintf(str,"%.4g",gui->line->rho_b/gui->line->rho_b_sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho_b), str );


  /* autoscale the delay units */
  i=0;
  while ( (time_units[i].name != NULL) &&
	  (gui->line->delay / time_units[i].sf >= 1000.0) ) {
    i++;
  }
  if (time_units[i].name == NULL)
    i--;
    
  gui->line->delay_sf = time_units[i].sf;
  gui->line->delay_units = time_units[i].name;
  gtk_label_set_text( GTK_LABEL(gui->units_delay), 
		      time_units[i].name );
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

  fprintf(fp,"(a) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->a/gui->line->a_sf,
	  gui->line->a_units);
  fprintf(fp,"(2a) show tab1 (=) show tab2 (%g %s) show newline\n",
	  2*gui->line->a/gui->line->a_sf,
	  gui->line->a_units);
  fprintf(fp,"(b) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->b/gui->line->b_sf,
	  gui->line->b_units);
  fprintf(fp,"(2b) show tab1 (=) show tab2 (%g %s) show newline\n",
	  2*gui->line->b/gui->line->b_sf,
	  gui->line->b_units);
  fprintf(fp,"(c) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->c/gui->line->c_sf,
	  gui->line->c_units);
  fprintf(fp,"(t) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->tshield/gui->line->tshield_sf,
	  gui->line->tshield_units);
  fprintf(fp,"(length) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->len/gui->line->len_sf,
	  gui->line->len_units);
  fprintf(fp,"(e) symbolshow (r) subshow tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->er);
  fprintf(fp,"(tan) show (d) symbolshow tab1 (=) show tab2 (%g) show newline\n",
	  gui->line->tand);
  fprintf(fp,"(r) symbolshow (a) subshow tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->rho_a/gui->line->rho_a_sf,
	  gui->line->rho_a_units);
  fprintf(fp,"(r) symbolshow (b) subshow tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->rho_b/gui->line->rho_b_sf,
	  gui->line->rho_b_units);
  fprintf(fp,"(frequency) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->freq/gui->line->freq_sf,
	  gui->line->freq_units);
  fprintf(fp,"newline\n");

  /* Second column of the output */
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");


  fprintf(fp,"(Z0) show tab1 (=) show tab2 (%g ) show (W) symbolshow newline\n",
	  gui->line->z0);
  fprintf(fp,"(elen) show tab1 (=) show tab2 (%g deg) show newline\n",
	  gui->line->elen);
  fprintf(fp,"(delay) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->delay/gui->line->delay_sf,
	  gui->line->delay_units);
  fprintf(fp,"(TE) show (10) subshow newlineclose (  cutoff) show "
	  "tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->fc/gui->line->fc_sf,
	  gui->line->fc_units);
  fprintf(fp,"(Loss) show tab1 (=) show tab2 (%g dB) show newline\n",
	  gui->line->loss);
  fprintf(fp,"(Conductor) show newlineclose (  loss) show "
	  "tab1 (=) show tab2 (%g dB/m) show newline\n",
	  gui->line->alpha_c);
  fprintf(fp,"(Dielectric) show newlineclose (  loss) show "
	  "tab1 (=) show tab2 (%g dB/m) show newline\n",
	  gui->line->alpha_d);

  fprintf(fp,"(Loss/Len) show tab1 (=) show tab2 (%g dB/meter) show newline\n",
	  gui->line->losslen);
  fprintf(fp,"newline\n");
  fprintf(fp,"(L) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->L/gui->line->L_sf,
	  gui->line->L_units);
  fprintf(fp,"(R) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->R/gui->line->R_sf,
	  gui->line->R_units);
  fprintf(fp,"(C) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->C/gui->line->C_sf,
	  gui->line->C_units);
  fprintf(fp,"(G) show tab1 (=) show tab2 (%g %s) show newline\n",
	  gui->line->G/gui->line->G_sf,
	  gui->line->G_units);

  fprintf(fp,"newline\n");

}

