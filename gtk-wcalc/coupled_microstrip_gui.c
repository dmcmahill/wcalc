/* $Id: coupled_microstrip_gui.c,v 1.6 2004/07/31 03:56:58 dan Exp $ */

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

#include "coupled_microstrip.h"
#include "coupled_microstrip_gui.h"
#include "coupled_microstrip_loadsave.h"
#include "physconst.h"
#include "units.h"

#include "gtk-units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void use_z0k_pressed(GtkWidget *widget, gpointer data );
static void use_z0ez0o_pressed(GtkWidget *widget, gpointer data );

static void print_ps(Wcalc *wcalc,FILE *fp);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize( GtkWidget *w, gpointer data );
static void calculate( coupled_microstrip_gui *gui, GtkWidget *w, gpointer data );
static void update_display( coupled_microstrip_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void values_init(coupled_microstrip_gui *gui, GtkWidget *parent);
static void outputs_init(coupled_microstrip_gui *gui, GtkWidget *parent);
static void picture_init(coupled_microstrip_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(coupled_microstrip_gui *gui);

static char *name="Coupled Microstrip Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

coupled_microstrip_gui *coupled_microstrip_gui_new(void)
{
  coupled_microstrip_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (coupled_microstrip_gui *) malloc(sizeof(coupled_microstrip_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"coupled_microstrip_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = coupled_microstrip_gui_init;
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

  /* create the coupled_microstrip line which will be used */
  new_gui->line = coupled_microstrip_line_new();

  return new_gui;
}

void coupled_microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  coupled_microstrip_gui *gui;

  gui = WC_COUPLED_MICROSTRIP_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("coupled_microstrip_gui.c:coupled_microstrip_gui_init():  calling coupled_microstrip_load\n");
#endif
    if (coupled_microstrip_load(gui->line,fp) != 0) {
      alert("ERROR:  Could not load the coupled_microstrip\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("coupled_microstrip_gui.c:coupled_microstrip_gui_init():  new using defaults\n");
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

static void values_init(coupled_microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *lwht;
  GtkWidget *button;
  GtkWidget *frame;
  wc_units_gui *ug;

  /* the "z0/k vs z0e/z0o" radio button group */
  GSList *z0k_group;

  int y=0;
  int x=0;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (4, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  

  /* Synthesize button */
  button = gtk_button_new_with_label ("Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 4, 
		   0, GTK_EXPAND|GTK_FILL, WC_XPAD, WC_YPAD);
  gtk_widget_show (button);

  /* Analyze button */
  button = gtk_button_new_with_label ("Analyze");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 0, 4, 
		   0, GTK_EXPAND|GTK_FILL, WC_XPAD, WC_YPAD);
  gtk_widget_show (button);
  

  /* ---------------- Width  -------------- */

  text = gtk_label_new( "Width (W)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_w = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_w),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_w, x+1, x+2, y, y+1, 
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_w),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_w);

  text = wc_units_menu_new(gui->line->units_lwst, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+2, x+3, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  y++;

  /* ---------------- Width  -------------- */

  text = gtk_label_new( "Spacing (S)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_s = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_s),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_s, x+1, x+2, y, y+1, 
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_s),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_s), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_s), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_s);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);

  y++;

  /* ---------------- Length  -------------- */
  text = gtk_label_new( "Length (L)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_l = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_l, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_l),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_l), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_l), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_l);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);

  y++;

  /* ---------------- Height  -------------- */

  text = gtk_label_new( "Height (H)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_h = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_h, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_h),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_h), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_h), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_h);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);

  y++;

  /* ---------------- Dielectric Constant -------------- */

  text = gtk_label_new( "Er" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_er = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_er, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_er),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_er);

  y++;

  /* ---------------- Loss Tangent -------------- */

  text = gtk_label_new( "Tand" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_tand = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tand, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tand),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tand);

  y++;

  /* ---------------- Metal thickness -------------- */

  text = gtk_label_new( "Tmet" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_tmet = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tmet, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tmet),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tmet), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tmet);

  lwht = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), lwht, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(lwht), 0, 0);
  gtk_widget_show(lwht);
  wc_units_attach_units_label(ug, lwht);

  y++;

  /* ---------------- Resistivity -------------- */

  text = gtk_label_new( "Rho" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_rho  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho);

  text = wc_units_menu_new(gui->line->units_rho, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  y++;

  /* ---------------- Surface roughness -------------- */

  text = gtk_label_new( "Rough" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_rough  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rough, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rough),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rough);

  text = wc_units_menu_new(gui->line->units_rough, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  y++;

  /* ----------------  COLUMN #2 -------------- */
  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  y = 0;
  x = 5;

  /* ----------------  Impedance -------------- */

  button = gtk_radio_button_new_with_label (NULL, _("Z0"));
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(use_z0k_pressed),
		     gui);
  gui->button_z0k = button;
  gtk_widget_show (button);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);

  z0k_group = gtk_radio_button_group (GTK_RADIO_BUTTON (button));


  /*
  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);
  */

  gui->text_z0 = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0);

  y++;

  /* ----------------  Coupling -------------- */

  text = gtk_label_new( "K" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_k = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_k, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_k),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_k), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_k), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_k);

  y++;

  /* ----------------  Even mode impedance -------------- */

  button = gtk_radio_button_new_with_label(z0k_group, "Z0e");
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(use_z0ez0o_pressed),
		     gui);
  gui->button_z0ez0o = button;
  gtk_widget_show (button);
  
  /*
  text = gtk_label_new( "Z0e" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);
  */

  gui->text_z0e = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0e, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0e),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0e), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0e), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0e);

  y++;

  /* ----------------  Odd mode impedance -------------- */

  text = gtk_label_new( "Z0o" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_z0o = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0o, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0o),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0o), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0o), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0o);


  if(gui->line->use_z0k){
    gtk_widget_set_sensitive (gui->text_z0e, FALSE);
    gtk_widget_set_sensitive (gui->text_z0o, FALSE);
    gtk_widget_set_sensitive (gui->text_z0, TRUE);
    gtk_widget_set_sensitive (gui->text_k, TRUE);
  }
  else{
    gtk_widget_set_sensitive (gui->text_z0e, TRUE);
    gtk_widget_set_sensitive (gui->text_z0o, TRUE);
    gtk_widget_set_sensitive (gui->text_z0, FALSE);
    gtk_widget_set_sensitive (gui->text_k, FALSE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  }

  y++;

  /* ----------------  Electrical length -------------- */

  text = gtk_label_new( "Elec. Len." );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_elen = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_elen, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);

  y++;


  /* ---------------- Frequency -------------- */

  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_freq  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);

  text = wc_units_menu_new(gui->line->units_freq, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  y++;

  /* ----------------  - -------------- */
  
  gtk_widget_show (table);
}

static void outputs_init(coupled_microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  int x=0;
  int y=0;
  wc_units_gui *ug;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (5, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* ---------------- Keff-e -------------- */
  text = gtk_label_new( "Keff-e" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_keffe = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_keffe, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_keffe);

  y++;

  /* ---------------- Keff-o -------------- */
  text = gtk_label_new( "Keff-o" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_keffo = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_keffo, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_keffo);

  y++;

  /* ---------------- Even mode Loss -------------- */
  text = gtk_label_new( "Even Mode Loss" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_loss_ev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss_ev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_loss_ev);

  text = wc_units_menu_new(gui->line->units_loss, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_loss_ev, &(gui->line->loss_ev), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- Odd mode Loss -------------- */
  text = gtk_label_new( "Odd Mode Loss" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_loss_od = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss_od, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_loss_od);

  wc_units_attach_label(ug, gui->label_loss_od, &(gui->line->loss_odd), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  gtk_widget_show(text);
  wc_units_attach_units_label(ug, text);

  y++;

  /* ----------------  Even Mode Loss/Length -------------- */
  text = gtk_label_new( "Even Mode Loss/Length" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_losslen_ev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen_ev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_losslen_ev);

  text = wc_units_menu_new(gui->line->units_losslen, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_losslen_ev, &(gui->line->losslen_ev), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ----------------  Odd Mode Loss/Length -------------- */
  text = gtk_label_new( "Odd Mode Loss/Length" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_losslen_od = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen_od, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_losslen_od);

  wc_units_attach_label(ug, gui->label_losslen_od, &(gui->line->losslen_odd), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  gtk_widget_show(text);
  wc_units_attach_units_label(ug, text);

  y++;

  /* ----------------  Skin Depth -------------- */
  text = gtk_label_new( "Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_depth = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_depth, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_depth);

  text = wc_units_menu_new(gui->line->units_depth, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_depth, &(gui->line->skindepth), 
			NULL, NULL, WC_FMT_G, 1);
  
  y++;

  /* ----------------  COLUMN #2 -------------- */
  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  y = 0;
  x = 5;

  /* ----------------  Even mode End correction -------------- */
  text = gtk_label_new( "Even Mode Delta L" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_deltal_ev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_deltal_ev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_deltal_ev);

  text = wc_units_menu_new(gui->line->units_deltal, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_deltal_ev, &(gui->line->deltale), 
			NULL, NULL, WC_FMT_G, 1);
  y++;

  /* ----------------  Odd mode End correction -------------- */
  text = gtk_label_new( "Odd Mode Delta L" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_deltal_od = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_deltal_od, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_deltal_od);

  wc_units_attach_label(ug, gui->label_deltal_od, &(gui->line->deltalo), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  gtk_widget_show(text);
  wc_units_attach_units_label(ug, text);

  y++;

  
  /* XXX */
#ifdef notdef
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
#endif

  /* spacer */

  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  gtk_widget_show(table);

}


#include "pixmaps/coupled_microstrip.xpm"

static void picture_init(coupled_microstrip_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) coupled_microstrip);
					
  
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
  calculate(WC_COUPLED_MICROSTRIP_GUI(data), w, "analyze");
}

static void synthesize( GtkWidget *w, gpointer data )
{
  calculate(WC_COUPLED_MICROSTRIP_GUI(data), w, "synthesize");
}

static void calculate( coupled_microstrip_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_w) ); 
  gui->line->w=atof(vstr)*gui->line->units_lwst->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  w = %g %s\n",
	  gui->line->w/gui->units_lwst->sf, gui->units_lwst->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_s) ); 
  gui->line->s = atof(vstr)*gui->line->units_lwst->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  s = %g %s\n", 
	  gui->line->s/gui->units_lwst->sf, gui->units_lwst->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l) ); 
  gui->line->l=atof(vstr)*gui->line->units_lwst->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  l = %g %s\n", 
	  gui->line->l/gui->units_lwst->sf, gui->units_lwst->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_h) ); 
  gui->line->subs->h=atof(vstr)*gui->line->units_lwst->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  h = %g %s\n",
	  gui->line->subs->h/gui->units_lwst->sf, gui->units_lwst->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) ); 
  gui->line->subs->er=atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  er = %g\n", 
	  gui->line->subs->er);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) ); 
  gui->line->subs->tand=atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  tand = %g\n", 
	  gui->line->subs->tand);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) ); 
  gui->line->subs->tmet=atof(vstr)*gui->line->units_lwst->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  tmet = %g %s\n",
	  gui->line->subs->tmet/gui->units_lwst->sf, gui->units_lwst->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->line->subs->rho=atof(vstr)*gui->line->units_rho->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  rho = %g %s\n",
	  gui->line->subs->rho/gui->units_rho->sf, gui->units_rho->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) ); 
  gui->line->subs->rough=atof(vstr)*gui->line->units_rough->sf;
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  rough = %g %s\n",
	  gui->line->subs->rough/gui->units_rough->sf, gui->units_rough->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0) ); 
  gui->line->z0 = atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  z0 = %g\n", 
	  gui->line->z0);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_k) ); 
  gui->line->k = atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  k = %g\n", 
	  gui->line->k);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0e) ); 
  gui->line->z0e = atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  z0e = %g\n", 
	  gui->line->z0e);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0o) ); 
  gui->line->z0o = atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  z0o = %g\n",
	  gui->line->z0o);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
  gui->line->len=atof(vstr);
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  elen = %g\n",
	  gui->line->len);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->line->freq=atof(vstr)*gui->line->units_freq->sf;

#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  freq = %g %s\n",
	  gui->line->freq/gui->units_freq->sf, gui->units_freq->name);
#endif

#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  calculation with \"%s\"\n",
	  (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt=coupled_microstrip_calc(gui->line, gui->line->freq);
  }
  else if( strcmp(data,"synthesize")==0) {
    rslt=coupled_microstrip_syn(gui->line, gui->line->freq);
  } else{
    fprintf(stderr,"coupled_microstrip_gui.c:  error in coupled_microstrip callback\n"
	    "Please report this bug and how you triggered it\n");
    exit(1);
  }
  
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:calculate():  finished calculation\n");
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

static void update_display(coupled_microstrip_gui *gui)
{
  char str[80];

  /* the entries */

  /* ---------------- w -------------- */
  sprintf(str,WC_FMT_G,gui->line->w/gui->line->units_lwst->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_w), str );

  /* ---------------- s -------------- */
  sprintf(str,WC_FMT_G,gui->line->s/gui->line->units_lwst->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_s), str );

  /* ---------------- l -------------- */
  sprintf(str,WC_FMT_G,gui->line->l/gui->line->units_lwst->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_l), str );

  /* ---------------- h -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->h/gui->line->units_lwst->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_h), str );

  /* ---------------- er -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->er);
  gtk_entry_set_text( GTK_ENTRY(gui->text_er), str );

  /* ---------------- tand -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->tand);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tand), str );

  /* ---------------- tmet -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->tmet/gui->line->units_lwst->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tmet), str );
  
  /* ---------------- rho -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->rho/gui->line->units_rho->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );
  
  /* ---------------- rough -------------- */
  sprintf(str,WC_FMT_G,gui->line->subs->rough/gui->line->units_rough->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rough), str );
  
  /* ---------------- z0 -------------- */
  sprintf(str,WC_FMT_G,gui->line->z0);
  gtk_entry_set_text( GTK_ENTRY(gui->text_z0), str );

  /* ---------------- k -------------- */
  sprintf(str,WC_FMT_G,gui->line->k);
  gtk_entry_set_text( GTK_ENTRY(gui->text_k), str );

  /* ---------------- z0e -------------- */
  sprintf(str,WC_FMT_G,gui->line->z0e);
  gtk_entry_set_text( GTK_ENTRY(gui->text_z0e), str );

  /* ---------------- z0oo -------------- */
  sprintf(str,WC_FMT_G,gui->line->z0o);
  gtk_entry_set_text( GTK_ENTRY(gui->text_z0o), str );

  /* ---------------- elen -------------- */
  sprintf(str,WC_FMT_G,gui->line->len);
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );

  /* ---------------- freq -------------- */
  sprintf(str,WC_FMT_G,gui->line->freq/gui->line->units_freq->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );
  

  /* the labels */
  /* ---------------- Keff-e  -------------- */
  sprintf(str, WC_FMT_G, gui->line->kev);
  gtk_label_set_text( GTK_LABEL(gui->label_keffe), str );

  /* ---------------- Keff-o  -------------- */
  sprintf(str, WC_FMT_G, gui->line->kodd);
  gtk_label_set_text( GTK_LABEL(gui->label_keffo), str );

  /* ---------------- Even Mode Loss -------------- */
  sprintf(str, WC_FMT_G,gui->line->loss_ev/gui->line->units_loss->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_loss_ev), str );

  /* ---------------- Odd Mode Loss -------------- */
  sprintf(str, WC_FMT_G,gui->line->loss_odd/gui->line->units_loss->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_loss_od), str );

  /* ---------------- Even Mode Loss/Length  -------------- */
  sprintf(str, WC_FMT_G,gui->line->losslen_ev/gui->line->units_losslen->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen_ev), str );

  /* ---------------- Odd Mode Loss/Length  -------------- */
  sprintf(str, WC_FMT_G,gui->line->losslen_odd/gui->line->units_losslen->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen_od), str );

  /* ---------------- depth -------------- */
  sprintf(str, WC_FMT_G, gui->line->skindepth/gui->line->units_depth->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_depth), str );
    
  /* ---------------- Even Mode end correction -------------- */
  sprintf(str, WC_FMT_G,gui->line->deltale/gui->line->units_deltal->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_deltal_ev), str );

  /* ---------------- Odd Mode end correction -------------- */
  sprintf(str, WC_FMT_G,gui->line->deltalo/gui->line->units_deltal->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_deltal_od), str );

  /* XXX */
#ifdef notdef
  /* ---------------- delay -------------- */
  sprintf(str,WC_FMT_G, gui->line->delay/gui->line->units_delay->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );
  
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
#endif

}


static void tooltip_init(coupled_microstrip_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_w, "Width of coupled_microstrip", NULL);
  gtk_tooltips_set_tip(tips, gui->text_l, "Length of coupled_microstrip", NULL);
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
  coupled_microstrip_gui *gui;

  gui = WC_COUPLED_MICROSTRIP_GUI(data);

  if(WC_WCALC(gui)->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "Values Out Of Sync");
}


static void use_z0k_pressed(GtkWidget *widget, gpointer data )
{
  coupled_microstrip_gui *gui;

  gui = WC_COUPLED_MICROSTRIP_GUI(data);
  gtk_widget_set_sensitive (gui->text_z0, TRUE);
  gtk_widget_set_sensitive (gui->text_k, TRUE);
  gtk_widget_set_sensitive (gui->text_z0e, FALSE);
  gtk_widget_set_sensitive (gui->text_z0o, FALSE);
  gui->line->use_z0k = 1;
}

static void use_z0ez0o_pressed(GtkWidget *widget, gpointer data )
{
  coupled_microstrip_gui *gui;

  gui = WC_COUPLED_MICROSTRIP_GUI(data);
  gtk_widget_set_sensitive (gui->text_z0, FALSE);
  gtk_widget_set_sensitive (gui->text_k, FALSE);
  gtk_widget_set_sensitive (gui->text_z0e, TRUE);
  gtk_widget_set_sensitive (gui->text_z0o, TRUE);
  gui->line->use_z0k = 0;
}


static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("coupled_microstrip_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  coupled_microstrip_save(WC_COUPLED_MICROSTRIP_GUI(wcalc)->line,fp,name);
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  coupled_microstrip_gui *gui;
  char *file;

  gui = WC_COUPLED_MICROSTRIP_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("coupled_microstrip.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "coupled_microstrip.eps");
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
	  gui->line->w/gui->line->units_lwst->sf,
	  gui->line->units_lwst->name);
  fprintf(fp,"(S) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->s/gui->line->units_lwst->sf,
	  gui->line->units_lwst->name);
  fprintf(fp,"(H) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->subs->h/gui->line->units_lwst->sf,
	  gui->line->units_lwst->name);
  fprintf(fp,"(L) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->l/gui->line->units_lwst->sf,
	  gui->line->units_lwst->name);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Tmet) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->subs->tmet/gui->line->units_lwst->sf,
	  gui->line->units_lwst->name);
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
  fprintf(fp,"newline\n");

  fprintf(fp,"(Keff-e) show tab1 (=) show tab2 ("
	  WC_FMT_G ") show newline\n",
	  gui->line->kev);
  fprintf(fp,"(Keff-o) show tab1 (=) show tab2 ("
	  WC_FMT_G ") show newline\n",
	  gui->line->kodd);

  fprintf(fp,"newline\n");
  fprintf(fp,"(freq) show tab1 (=) show tab2 ("
	  WC_FMT_G " %s) show newline\n",
	  gui->line->freq/gui->line->units_freq->sf,
	  gui->line->units_freq->name);

  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(Z0) show tab1 (=) show tab2 (" WC_FMT_G " ) show (W) symbolshow newline\n",
	  gui->line->z0);
  fprintf(fp,"(k) show tab1 (=) show tab2 (" WC_FMT_G " ) show newline\n",
	  gui->line->k);
  fprintf(fp,"(Z0e) show tab1 (=) show tab2 (" WC_FMT_G " ) show (W) symbolshow newline\n",
	  gui->line->z0e);
  fprintf(fp,"(Z0o) show tab1 (=) show tab2 (" WC_FMT_G " ) show (W) symbolshow newline\n",
	  gui->line->z0o);
  fprintf(fp,"(elen) show tab1 (=) show tab2 (" WC_FMT_G " deg) show newline\n",
	  gui->line->len);

  fprintf(fp,"(Even Mode) show newlineclose "
	  "(Loss) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->loss_ev/gui->line->units_loss->sf,
	  gui->line->units_loss->name);
  fprintf(fp,"(Odd Mode) show newlineclose "
	  "(Loss) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->loss_odd/gui->line->units_loss->sf,
	  gui->line->units_loss->name);

  fprintf(fp,"(Even Mode) show newlineclose "
	  "(Loss/Len) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->losslen_ev/gui->line->units_losslen->sf,
	  gui->line->units_losslen->name);
  fprintf(fp,"(Odd Mode) show newlineclose "
	  "(Loss/Len) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->losslen_odd/gui->line->units_losslen->sf,
	  gui->line->units_losslen->name);

  fprintf(fp,"(skin depth) show tab1 (=) show tab2 ("
	  WC_FMT_G " %s) show newline\n",
	  gui->line->skindepth/gui->line->units_depth->sf,
	  gui->line->units_depth->name);


  fprintf(fp,"(Even Mode ) show newlineclose "
	  "(D) symbolshow (l) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->deltale/gui->line->units_deltal->sf,
	  gui->line->units_deltal->name);
  fprintf(fp,"(Odd Mode ) show newlineclose "
	  "(D) symbolshow (l) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->deltalo/gui->line->units_deltal->sf,
	  gui->line->units_deltal->name);


  /* XXX */
#ifdef notdef
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
#endif

}

