/* $Id: microstrip_gui.c,v 1.11 2004/07/29 02:37:58 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2004 Dan McMahill
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
#include "misc.h"

#include "microstrip.h"
#include "microstrip_gui.h"
#include "microstrip_loadsave.h"
#include "physconst.h"
#include "units.h"

#include "gtk-units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_w( GtkWidget *w, gpointer data );
static void synthesize_l( GtkWidget *w, gpointer data );
static void synthesize_h( GtkWidget *w, gpointer data );
static void synthesize_er( GtkWidget *w, gpointer data );
static void calculate( microstrip_gui *gui, GtkWidget *w, gpointer data );
static void update_display( microstrip_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void values_init(microstrip_gui *gui, GtkWidget *parent);
static void outputs_init(microstrip_gui *gui, GtkWidget *parent);
static void picture_init(microstrip_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(microstrip_gui *gui);

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
  wcalc->units_menu_list = NULL;

  /*
   * Initialize the model dependent portions
   */

  /* create the microstrip line which will be used */
  new_gui->line = microstrip_line_new();
  new_gui->phys_units_text = NULL;

  return new_gui;
}

void microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  microstrip_gui *gui;

  gui = WC_MICROSTRIP_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("microstrip_gui.c:microstrip_gui_init():  calling microstrip_load\n");
#endif
    if (microstrip_load(gui->line,fp) != 0) {
      alert("ERROR:  Could not load the microstrip\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("microstrip_gui.c:microstrip_gui_init():  new using defaults\n");
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

  wc_units_menu_init( wcalc );
  analyze(NULL, gui);
  wc_units_menu_init( wcalc );

}

/*
 * Private Functions
 */

static void values_init(microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *lwht;
  GtkWidget *button;
  GtkWidget *frame;
  wc_units_gui *ug;


  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (4, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  

  /* Analyze button */
  button = gtk_button_new_with_label ("Analyze");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 0, 4, 0,GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
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
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show (button);

  /* height */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_h), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show (button);

  /* dielectric constant */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_er), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 3, 4, 0, GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_show (button);




  /* ---------------- Width  -------------- */

  text = gtk_label_new( "Width (W)" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  text = wc_units_menu_new(gui->line->units_lwht, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   2, 3, 0, 1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);


  /* ---------------- Length  -------------- */
  text = gtk_label_new( "Length (L)" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, 2, 3, 1, 2, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);


  /* ---------------- Height  -------------- */

  text = gtk_label_new( "Height (H)" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, 2, 3, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);

  /* ---------------- Dielectric Constant -------------- */

  text = gtk_label_new( "Er" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  /* ---------------- Loss Tangent -------------- */

  text = gtk_label_new( "Tand" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  /* ----------------  Impedance -------------- */

  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  /* ----------------  Electrical length -------------- */

  text = gtk_label_new( "Elec. Len." );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  /* ---------------- Metal thickness -------------- */

  text = gtk_label_new( "Tmet" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, 8, 9, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);

  /* ---------------- Resistivity -------------- */

  text = gtk_label_new( "Rho" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 3, 4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  text = wc_units_menu_new(gui->line->units_rho, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   8, 9, 3, 4, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);


  /* ---------------- Surface roughness -------------- */

  text = gtk_label_new( "Rough" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 4, 5, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  text = wc_units_menu_new(gui->line->units_rough, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   8, 9, 4, 5, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ---------------- Frequency -------------- */

  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 5, 6, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  text = wc_units_menu_new(gui->line->units_freq, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   8, 9, 5, 6, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);



  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  
  gui->text_w = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_w),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_w, 1, 2, 0, 1,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_w),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_w);

  gui->text_l = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_l, 1, 2, 1, 2,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_l),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_l), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_l), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_l);

  gui->text_h = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_h, 1, 2, 2, 3,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_h),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_h), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_h), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_h);

  gui->text_er = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_er, 1, 2, 3, 4,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_er),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_er);

  gui->text_tand = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tand, 1, 2, 4, 5,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tand),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tand);

  gui->text_z0 = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0, 7, 8, 0, 1,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0);

  gui->text_elen = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_elen, 7, 8, 1, 2,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);

  gui->text_tmet = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tmet, 7, 8, 2, 3,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tmet),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tmet);


  gui->text_rho  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho, 7, 8, 3, 4,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho);

  gui->text_rough  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rough, 7, 8, 4, 5,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rough),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rough);

  gui->text_freq  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq, 7, 8, 5, 6,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);

  gtk_widget_show (table);
}

static void outputs_init(microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  wc_units_gui *ug;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (5, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* ---------------- Delay -------------- */
  text = gtk_label_new( "Delay" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_delay = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_delay, 
		    1,2,0,1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_delay);

  text = wc_units_menu_new(gui->line->units_delay, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   3, 4, 0, 1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_delay, &(gui->line->delay), 
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- Loss -------------- */
  text = gtk_label_new( "Loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_loss = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss, 
		    1,2,1,2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_loss);

  text = wc_units_menu_new(gui->line->units_loss, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   3, 4, 1, 2, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_loss, &(gui->line->loss), 
			NULL, NULL, WC_FMT_G, 1);

  /* ----------------  Loss/Length -------------- */
  text = gtk_label_new( "Loss/Length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_losslen = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen, 
		    1,2,2,3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_losslen);

  text = wc_units_menu_new(gui->line->units_losslen, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   3, 4, 2, 3, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_losslen, &(gui->line->losslen), 
			NULL, NULL, WC_FMT_G, 1);

  /* ----------------  Skin Depth -------------- */
  text = gtk_label_new( "Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_depth = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_depth, 
		    1,2,3,4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_depth);

  text = wc_units_menu_new(gui->line->units_depth, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   3, 4, 3, 4, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_depth, &(gui->line->skindepth), 
			NULL, NULL, WC_FMT_G, 1);


  /* ----------------  End correction -------------- */
  text = gtk_label_new( "Delta L" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_deltal = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_deltal, 
		    1,2,4,5, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_deltal);

  text = wc_units_menu_new(gui->line->units_deltal, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   3, 4, 4, 5, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_deltal, &(gui->line->deltal), 
			NULL, NULL, WC_FMT_G, 1);

  /* ----------------  Effective dielectric constant -------------- */
  text = gtk_label_new( "Keff" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_keff = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_keff, 
		    1, 2, 5, 6, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_keff);

  /* ---------------- L -------------- */
  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Ls = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Ls, 
		    5, 6, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Ls);

  text = wc_units_menu_new(gui->line->units_L, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Ls, &(gui->line->Ls), 
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- R -------------- */
  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Rs = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Rs, 
		    5, 6, 1, 2, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Rs);

  text = wc_units_menu_new(gui->line->units_R, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 1, 2, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Rs, &(gui->line->Rs), 
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- C -------------- */
  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Cp = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Cp, 
		    5, 6, 2, 3, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Cp);

  text = wc_units_menu_new(gui->line->units_C, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Cp, &(gui->line->Cs), 
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- G -------------- */
  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 3, 4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Gp = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Gp, 
		    5, 6, 3, 4, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Gp);

  text = wc_units_menu_new(gui->line->units_G, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 3, 4, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Gp, &(gui->line->Gs), 
			NULL, NULL, WC_FMT_G, 1);

  /* spacer */

  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


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
    

  gui->text_status = gtk_label_new( "Values Out Of Sync" );
  gtk_box_pack_start (GTK_BOX (my_hbox), gui->text_status, FALSE, FALSE, 0);
  gtk_widget_show (gui->text_status);
  

}

static void analyze( GtkWidget *w, gpointer data )
{
  calculate(WC_MICROSTRIP_GUI(data), w, "analyze");
}

static void synthesize_w( GtkWidget *w, gpointer data )
{
  calculate(WC_MICROSTRIP_GUI(data), w, "synthesize_w");
}

static void synthesize_l( GtkWidget *w, gpointer data )
{
  calculate(WC_MICROSTRIP_GUI(data), w, "synthesize_l");
}

static void synthesize_h( GtkWidget *w, gpointer data )
{
  calculate(WC_MICROSTRIP_GUI(data), w, "synthesize_h");
}

static void synthesize_er( GtkWidget *w, gpointer data )
{
  calculate(WC_MICROSTRIP_GUI(data), w, "synthesize_er");
}


static void calculate( microstrip_gui *gui, GtkWidget *w, gpointer data )
{
  char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_w) ); 
  gui->line->w=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  w = %g\n", gui->line->w);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l) ); 
  gui->line->l=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  l = %g\n", gui->line->l);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_h) ); 
  gui->line->subs->h=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  h = %g\n", gui->line->subs->h);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) ); 
  gui->line->subs->er=atof(vstr);
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  er = %g\n", gui->line->subs->er);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) ); 
  gui->line->subs->tand=atof(vstr);
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  tand = %g\n", gui->line->subs->tand);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0) ); 
  gui->line->Ro=atof(vstr);
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  z0 = %g\n", gui->line->z0);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
  gui->line->len=atof(vstr);
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  elen = %g\n", gui->line->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) ); 
  gui->line->subs->tmet=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  tmet = %g\n", gui->line->subs->tmet);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->line->subs->rho=atof(vstr)*gui->line->units_rho->sf;
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  rho = %g\n", gui->line->subs->rho);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) ); 
  gui->line->subs->rough=atof(vstr)*gui->line->units_rough->sf;
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  rough = %g\n",
	  gui->line->subs->rough);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->line->freq=atof(vstr)*gui->line->units_freq->sf;

#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  freq = %g\n",
	  gui->line->freq);
#endif

#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  calculation with \"%s\"\n",
	  (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt=microstrip_calc(gui->line, gui->line->freq);
  }
  else if( strcmp(data,"synthesize_w")==0) {
    rslt=microstrip_syn(gui->line, gui->line->freq, MLISYN_W);
  }
  else if( strcmp(data,"synthesize_l")==0) {
    rslt=microstrip_syn(gui->line, gui->line->freq, MLISYN_L);
  }
  else if( strcmp(data,"synthesize_h")==0) {
    rslt=microstrip_syn(gui->line, gui->line->freq, MLISYN_H);
  }
  else if( strcmp(data,"synthesize_er")==0) {
    rslt=microstrip_syn(gui->line, gui->line->freq, MLISYN_ES);
  }
  else{
    fprintf(stderr,"microstrip_gui.c:  error in microstrip callback\n"
	    "Please report this bug and how you triggered it\n");
    exit(1);
  }
  
#ifdef DEBUG
  g_print("microstrip_gui.c:calculate():  finished calculation\n");
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

static void update_display(microstrip_gui *gui)
{
  char str[80];

  /* the entries */

  /* ---------------- w -------------- */
  sprintf(str,WC_FMT_G,gui->line->w/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_w), str );

  /* ---------------- l -------------- */
  sprintf(str,WC_FMT_G,gui->line->l/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_l), str );

  /* ---------------- h -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->h/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_h), str );

  /* ---------------- er -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->er);
  gtk_entry_set_text( GTK_ENTRY(gui->text_er), str );

  /* ---------------- tand -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->tand);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tand), str );

  /* ---------------- z0 -------------- */
  sprintf(str,WC_FMT_G,gui->line->z0);
  gtk_entry_set_text( GTK_ENTRY(gui->text_z0), str );

  /* ---------------- elen -------------- */
  sprintf(str,WC_FMT_G,gui->line->len);
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );

  /* ---------------- tmet -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->tmet/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tmet), str );
  
  /* ---------------- rho -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->rho/gui->line->units_rho->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );
  
  /* ---------------- rough -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->rough/gui->line->units_rough->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rough), str );
  
  /* ---------------- freq -------------- */
  sprintf(str,WC_FMT_G,gui->line->freq/gui->line->units_freq->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );
  

  /* the labels */

  /* ---------------- delay -------------- */
  sprintf(str,WC_FMT_G, gui->line->delay/gui->line->units_delay->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );
  
  /* ---------------- loss -------------- */
  sprintf(str,WC_FMT_G, gui->line->loss/gui->line->units_loss->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_loss), str );
    
  /* ---------------- loss/len -------------- */
  sprintf(str,WC_FMT_G, gui->line->losslen/gui->line->units_losslen->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen), str );
    
  /* ---------------- depth -------------- */
  sprintf(str,WC_FMT_G, gui->line->skindepth/gui->line->units_depth->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_depth), str );
    
  /* ---------------- deltal -------------- */
  sprintf(str,WC_FMT_G, gui->line->deltal/gui->line->units_deltal->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_deltal), str );

  /* ---------------- keff -------------- */
  sprintf(str,WC_FMT_G, gui->line->keff);
  gtk_label_set_text( GTK_LABEL(gui->label_keff), str );
    
  /* ---------------- L -------------- */
  sprintf(str,WC_FMT_G, gui->line->Ls/gui->line->units_L->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Ls), str );
    
  /* ---------------- R -------------- */
  sprintf(str,WC_FMT_G, gui->line->Rs/gui->line->units_R->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Rs), str );
    
  /* ---------------- C -------------- */
  sprintf(str,WC_FMT_G, gui->line->Cs/gui->line->units_C->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Cp), str );
    
  /* ---------------- R -------------- */
  sprintf(str,WC_FMT_G, gui->line->Gs/gui->line->units_G->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Gp), str );
    

}


static void tooltip_init(microstrip_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_w, "Width of microstrip", NULL);
  gtk_tooltips_set_tip(tips, gui->text_l, "Length of microstrip", NULL);
  gtk_tooltips_set_tip(tips, gui->text_h, "Total substrate thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_er, "Substrate relative"
		       " dielectric constant",NULL);
  gtk_tooltips_set_tip(tips, gui->text_tand, "Substrate loss tangent", NULL);

  gtk_tooltips_set_tip(tips, gui->text_z0, "Characteristic impedance", NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, "Electrical length", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tmet, "Line metal thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho, "Metal resistivity", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rough, "Metal surface roughness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);
  
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

  fprintf(fp,"(W) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->w/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"(H) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->subs->h/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"(L) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->l/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Tmet) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->subs->tmet/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"(Rho) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->subs->rho/gui->line->units_rho->sf,
	  gui->line->units_rho->name);
  fprintf(fp,"(Rough) show tab1 (=) show tab2 (" WC_FMT_G " %s-rms) show newline\n",
	  gui->line->subs->rough/gui->line->units_rough->sf,
	  gui->line->units_rough->name);
  fprintf(fp,"(e) symbolshow (r) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->line->subs->er);
  fprintf(fp,"(tan) show (d) symbolshow tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->line->subs->tand);
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(Z0) show tab1 (=) show tab2 (" WC_FMT_G " ) show (W) symbolshow newline\n",
	  gui->line->z0);
  fprintf(fp,"(elen) show tab1 (=) show tab2 (" WC_FMT_G " deg) show newline\n",
	  gui->line->len);
  fprintf(fp,"(Delay) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->delay/gui->line->units_delay->sf,
	  gui->line->units_delay->name);
  fprintf(fp,"(Loss) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->loss/gui->line->units_loss->sf,
	  gui->line->units_loss->name);
  fprintf(fp,"(Loss/Len) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->losslen/gui->line->units_losslen->sf,
	  gui->line->units_losslen->name);
  fprintf(fp,"(skin depth) show tab1 (=) show tab2 (" WC_FMT_G 
	  " %s) show newline\n",
	  gui->line->skindepth/gui->line->units_depth->sf,
	  gui->line->units_depth->name);
  fprintf(fp,"(D) symbolshow (l) show tab1 (=) show tab2 (" WC_FMT_G 
	  " %s) show newline\n",
	  gui->line->deltal/gui->line->units_deltal->sf,
	  gui->line->units_deltal->name);
  fprintf(fp,"(Keff) show tab1 (=) show tab2 (" WC_FMT_G 
	  ") show newline\n",
	  gui->line->keff);

  fprintf(fp,"newline\n");
  fprintf(fp,"(Ls) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Ls/gui->line->units_L->sf,
	  gui->line->units_L->name);
  fprintf(fp,"(Rs) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Rs/gui->line->units_R->sf,
	  gui->line->units_R->name);
  fprintf(fp,"(Cs) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Cs/gui->line->units_C->sf,
	  gui->line->units_C->name);
  fprintf(fp,"(Gs) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Gs/gui->line->units_G->sf,
	  gui->line->units_G->name);
  
}

