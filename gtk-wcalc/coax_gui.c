/* $Id: coax_gui.c,v 1.33 2008/11/29 20:41:42 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
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
#include "gtk-units.h"

#include "misc.h"
#include "units.h"

#include "coax.h"
#include "coax_gui.h"
#include "coax_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

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

static void values_init(coax_gui *gui, GtkWidget *parent);
static void outputs_init(coax_gui *gui, GtkWidget *parent);
static void picture_init(coax_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(coax_gui *gui);


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
      fprintf(stderr,_("coax_gui_new: malloc() failed\n"));
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
  wcalc->units_menu_list = NULL;
  /*
   * Initialize the model dependent portions
   */

  /* create the coax line which will be used */
  new_gui->line = coax_new();

  return new_gui;
}

void coax_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  coax_gui *gui;

  gui = WC_COAX_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print(_("coax_gui.c:coax_gui_init():  calling coax_load\n"));
#endif
    if (coax_load(gui->line,fp) != 0) {
      alert(_("ERROR:  Could not load the coax line\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n"));
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print(_("coax_gui.c:coax_gui_init():  new using defaults\n"));
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
  

  /* run the analysis once since we've changed input units */
  wc_units_menu_init( wcalc );
  analyze(NULL, gui);
  wc_units_menu_init( wcalc );
}

/*
 * Private Functions
 */

static void values_init(coax_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;
  GtkWidget *text;
  GtkTooltips *tips;

  wc_units_gui *ug;

  tips = gtk_tooltips_new();

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Analysis/Synthesis Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (8, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  


  /* Analyze button */
  button = gtk_button_new_with_label (_("Analyze->"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 6, 7, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate electrical characteristics "
		       "from physical parameters"),
		       NULL);

  
  /* 
   * Synthesize buttons 
   */

  /* a */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_a), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize 'a' and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length."),
		       NULL);

  /* b */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_b), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize 'b' and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length."),
		       NULL);

  /* c */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_c), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize 'c' and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length."),
		       NULL);

  /* Len */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_len), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 3, 4, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_set_sensitive (button, FALSE);
  gtk_widget_show (button);

  /* er */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_er), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 4, 5, 0,0,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize relative dielectric constant "
		       "and physical length to "
		       "obtain the requested characteristic "
		       "impedance and electrical length."),
		       NULL);



  /* Text labels */
  text = gtk_label_new( "a" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  text = wc_units_menu_new(gui->line->units_abct, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);


  text = gtk_label_new( "b" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  text = gtk_label_new("");
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text),0,0);

  wc_units_attach_units_label(ug, text);

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
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  text = gtk_label_new("");
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);

  wc_units_attach_units_label(ug, text);


  text = gtk_label_new( "t" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 7, 8, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  text = gtk_label_new("");
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 7, 8, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);

  wc_units_attach_units_label(ug, text);


  text = gtk_label_new( _("Length") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  text = wc_units_menu_new(gui->line->units_len, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 3, 4, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  text = gtk_label_new( _("Er") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0, 0, WC_XPAD, WC_YPAD);

  text = gtk_label_new( _("Tand") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6, 0, 0, WC_XPAD, WC_YPAD);

  /* ---------------- Max Electric Field -------------- */

  text = gtk_label_new( "Emax" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 6, 7, 0, 0, WC_XPAD, WC_YPAD);

  gui->text_emax = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_emax, 1, 2, 6, 7,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_emax),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_emax), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_emax), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_set_sensitive(gui->text_emax,FALSE);
  gtk_widget_show(gui->text_emax);


  text = wc_units_menu_new(gui->line->units_emax, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   2, 3, 6, 7, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_entry(ug, gui->text_emax, &(gui->line->emax), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- Characteristic impedance -------------- */

  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  text = gtk_label_new( "ohms" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);

  /* ---------------- Electrical length -------------- */

  text = gtk_label_new( "Elen" );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 1, 2,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  text = gtk_label_new( "degrees" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 1, 2,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);

  /* ---------------- TE10 Cutoff -------------- */

  text = gtk_label_new( _("TE10 Cutoff") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 2, 3, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_fc = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_fc, 6, 7, 2, 3,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_fc),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_fc), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_fc), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_set_sensitive(gui->text_fc,FALSE);
  gtk_widget_show(gui->text_fc);

  text = wc_units_menu_new(gui->line->units_fc, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   7, 8, 2, 3, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_entry(ug, gui->text_fc, &(gui->line->fc), NULL, NULL, WC_FMT_G, 1);


  /* ---------------- Frequency label/entry/units menu -------------- */
  text = gtk_label_new( _("Frequency") );
  gtk_table_attach(GTK_TABLE(table), text,
		   5, 6, 3, 4, 0,0,WC_XPAD,WC_YPAD);

  text = wc_units_menu_new(gui->line->units_freq, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 3, 4, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  
  /* ---------------- RHO_a label/entry/units menu -------------- */

  text = gtk_label_new( _("RHO_a") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 4, 5, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  text = wc_units_menu_new(gui->line->units_rho, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 4, 5, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ---------------- RHO_b label/entry/units menu -------------- */

  text = gtk_label_new( _("RHO_b") );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 5, 6, 
		   GTK_EXPAND|GTK_FILL,0,WC_XPAD,WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text),0,0);
  
  wc_units_attach_units_label(ug,text);


  text = gtk_label_new( _("RHO_b") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 5, 6, 0,0,WC_XPAD,WC_YPAD);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 5, 6, 0,0,WC_XPAD,WC_YPAD);


  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  
  gui->text_a = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_a),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_a, 1, 2, 0, 1,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_a),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_a), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_a), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_a);

  gui->text_b = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_b, 1, 2, 1, 2,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_b),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_b), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_b), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_b);

  gui->text_c = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_c, 1, 2, 2, 3,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_c),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_c), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_c), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_c);

  gui->text_len = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_len, 1, 2, 3, 4,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_len),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_len);

  gui->text_er = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_er, 1, 2, 4, 5,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_er),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_er);

  gui->text_tand = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tand, 1, 2, 5, 6,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tand),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tand);

  gui->text_tshield = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tshield, 1, 2, 7, 8,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tshield),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tshield), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tshield), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tshield);

  gui->text_z0 = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0, 6, 7, 0, 1,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0);

  gui->text_elen = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_elen, 6, 7, 1, 2,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);


  gui->text_freq = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq, 6, 7, 3, 4,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);

  gui->text_rho_a = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho_a, 6, 7, 4, 5,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho_a),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho_a), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho_a), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho_a);

  gui->text_rho_b = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho_b, 6, 7, 5, 6,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho_b),WC_WIDTH,0);
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
  wc_units_gui *ug;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Output Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (5, 7, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* Text labels */

  /* DELAY */
  text = gtk_label_new( _("delay") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_delay = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_delay,
		    1, 2, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_delay);

  text = wc_units_menu_new(gui->line->units_delay, WC_WCALC(gui), &ug);
  wc_units_attach_label(ug, gui->label_delay, &(gui->line->delay), NULL, NULL, WC_FMT_G, 1);
  gtk_table_attach(GTK_TABLE(table), text, 
		   2, 3, 0, 1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* Loss */
  text = gtk_label_new( _("loss") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,WC_XPAD,WC_YPAD);

  gui->label_loss = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss,
		    1,2,1,2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_loss);

  text = wc_units_menu_new(gui->line->units_loss, WC_WCALC(gui), &ug);
  wc_units_attach_label(ug, gui->label_loss, &(gui->line->loss), NULL, NULL, WC_FMT_G, 1);
  gtk_table_attach(GTK_TABLE(table), text, 
		   2, 3, 1, 2, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* Loss/Length */
  text = gtk_label_new( "loss/length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3, 0,0,WC_XPAD,WC_YPAD);

  gui->label_losslen = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen, 
		    1,2,2,3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_losslen);


  text = wc_units_menu_new(gui->line->units_losslen, WC_WCALC(gui), &ug);
  wc_units_attach_label(ug, gui->label_losslen, &(gui->line->losslen), NULL, NULL, WC_FMT_G, 1);
  gtk_table_attach(GTK_TABLE(table), text, 
		   2, 3, 2, 3, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* conductor loss */
  text = gtk_label_new( _("Conductor loss") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4, 0,0,WC_XPAD,WC_YPAD);

  gui->label_closs = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_closs,
		    1,2,3,4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_closs);

  wc_units_attach_label(ug, gui->label_closs, &(gui->line->alpha_c), NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 3, 4, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  wc_units_attach_units_label(ug, text);

  /* dielectric loss */
  text = gtk_label_new( _("Dielectric loss") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5, 0,0,WC_XPAD,WC_YPAD);

  gui->label_dloss = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_dloss,
		    1,2,4,5, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_dloss);

  wc_units_attach_label(ug, gui->label_dloss, &(gui->line->alpha_d), NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 4, 5, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  wc_units_attach_units_label(ug, text);

  /* ---------------- L -------------- */
  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,WC_XPAD,WC_YPAD);

  text = wc_units_menu_new(gui->line->units_L, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 
		   GTK_EXPAND|GTK_FILL,0,WC_XPAD,WC_YPAD);

  gui->label_L = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_L, 
		    5, 6, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_L);

  /* attach inductance label to the units gui */
  wc_units_attach_label(ug, gui->label_L, 
			&(gui->line->L), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- R -------------- */
  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0, 0, WC_XPAD, WC_YPAD);

  text = wc_units_menu_new(gui->line->units_R, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 1, 2, 
		   GTK_EXPAND|GTK_FILL,0,WC_XPAD,WC_YPAD);

  gui->label_R = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_R, 
		    5, 6, 1, 2, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_R);

  wc_units_attach_label(ug, gui->label_R, 
			&(gui->line->R), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- C -------------- */
  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, 
		   4, 5, 2, 3, 0, 0, WC_XPAD, WC_YPAD);

  text = wc_units_menu_new(gui->line->units_C, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  gui->label_C = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_C, 
		    5, 6, 2, 3, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_C);

  wc_units_attach_label(ug, gui->label_C, 
			&(gui->line->C), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- G -------------- */
  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 3, 4, 0, 0, WC_XPAD, WC_YPAD);

  text = wc_units_menu_new(gui->line->units_G, WC_WCALC(gui), &ug);

  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 3, 4, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);


  gui->label_G = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_G, 
		    5, 6, 3, 4, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_G);

  wc_units_attach_label(ug, gui->label_G, 
			&(gui->line->G), NULL, NULL, WC_FMT_G, 1);

  /* spacer */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

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
    

  WC_WCALC(gui)->text_status = gtk_label_new( _("Values Out Of Sync") );
  gtk_box_pack_start (GTK_BOX (my_hbox), WC_WCALC(gui)->text_status, FALSE, FALSE, 0);
  gtk_widget_show (WC_WCALC(gui)->text_status);
  

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
  const char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_a) ); 
  gui->line->a=atof(vstr)*wc_units_to_sf(gui->line->units_abct);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_abct);
  g_print(_("coax_gui.c:calculate():  a = %g m (%g %s)\n"),
	  gui->line->a,
	  gui->line->a/wc_units_to_sf(gui->line->units_abct),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_b) ); 
  gui->line->b=atof(vstr)*wc_units_to_sf(gui->line->units_abct);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_abct);
  g_print(_("coax_gui.c:calculate():  b = %g m (%g %s)\n"),
	  gui->line->b,
	  gui->line->b/wc_units_to_sf(gui->line->units_abct),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_c) ); 
  gui->line->c=atof(vstr)*wc_units_to_sf(gui->line->units_abct);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_abct);
  g_print(_("coax_gui.c:calculate():  c = %g m (%g %s)\n"),
	  gui->line->c,
	  gui->line->c/wc_units_to_sf(gui->line->units_abct),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_len) ); 
  gui->line->len=atof(vstr)*wc_units_to_sf(gui->line->units_len);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_len);
  g_print(_("coax_gui.c:calculate():  len = %g m (%g %s)\n"),
	  gui->line->len,
	  gui->line->len/wc_units_to_sf(gui->line->units_len),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) ); 
  gui->line->er=atof(vstr);
#ifdef DEBUG
  g_print(_("coax_gui.c:calculate():  er = %g\n"),gui->line->er);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) ); 
  gui->line->tand=atof(vstr);
#ifdef DEBUG
  g_print(_("coax_gui.c:calculate():  tand = %g\n"),gui->line->tand);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_emax) ); 
  gui->line->emax=atof(vstr)*wc_units_to_sf(gui->line->units_emax);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_emax);
  g_print(_("coax_gui.c:calculate():  emax = %g V/m (%g %s)\n"),
	  gui->line->emax,
	  gui->line->emax/wc_units_to_sf(gui->line->units_emax),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tshield) ); 
  gui->line->tshield=atof(vstr)*wc_units_to_sf(gui->line->units_abct);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_abct);
  g_print(_("coax_gui.c:calculate():  tshield = %g m (%g %s)\n"),
	  gui->line->tshield,
	  gui->line->tshield/wc_units_to_sf(gui->line->units_abct),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0) ); 
  gui->line->z0=atof(vstr);
#ifdef DEBUG
  g_print(_("coax_gui.c:calculate():  z0 = %g Ohms\n"),gui->line->z0);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
  gui->line->elen=atof(vstr);
#ifdef DEBUG
  g_print(_("coax_gui.c:calculate():  elen = %g Degrees\n"),gui->line->elen);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->line->freq=atof(vstr)*wc_units_to_sf(gui->line->units_freq);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_freq);
  g_print(_("coax_gui.c:calculate():  freq = %g Hz (%g %s)\n"),
	  gui->line->freq,
	  gui->line->freq/wc_units_to_sf(gui->line->units_freq),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_fc) ); 
  gui->line->fc=atof(vstr)*wc_units_to_sf(gui->line->units_fc);
#ifdef DEBUG
  g_print("coax_gui.c:calculate():  fc = %g\n",gui->line->fc);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho_a) ); 
  gui->line->rho_a=atof(vstr)*wc_units_to_sf(gui->line->units_rho);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_rho);
  g_print("coax_gui.c:calculate():  rho_a = %g Ohm-m (%g %s)\n",
	  gui->line->rho_a,
	  gui->line->rho_a/wc_units_to_sf(gui->line->units_rho),
	  tmps);
  free(tmps);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho_b) ); 
  gui->line->rho_b=atof(vstr)*wc_units_to_sf(gui->line->units_rho);
#ifdef DEBUG
  tmps = wc_units_to_str(gui->line->units_rho);
  g_print(_("coax_gui.c:calculate():  rho_b = %g Ohm-m (%g %s)\n"),
	  gui->line->rho_b,
	  gui->line->rho_b/wc_units_to_sf(gui->line->units_rho),
	  tmps);
  free(tmps);
#endif


  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt = coax_calc(gui->line, gui->line->freq);
  }
  else if( strcmp(data,"synthesize_a")==0) {
    rslt = coax_syn(gui->line,gui->line->freq, COAXSYN_A);
  }
  else if( strcmp(data,"synthesize_b")==0) {
    rslt = coax_syn(gui->line,gui->line->freq, COAXSYN_B);
  }
  else if( strcmp(data,"synthesize_c")==0) {
    rslt = coax_syn(gui->line,gui->line->freq, COAXSYN_C);
  }
  else if( strcmp(data,"synthesize_er")==0) {
    rslt = coax_syn(gui->line,gui->line->freq, COAXSYN_ER);
  }
  else{
    g_print(_("error in coax callback.  data=\"%s\""),(char *)data);
    exit(1);
  }
  
#ifdef DEBUG
  g_print(_("coax_gui.c:calculate():  finished calculation\n"));
#endif

  update_display(gui);

  /*
   * if the calculation completed with no errors, then clear the
   * "values out of sync" field
   */
  if (rslt==0) {
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "");
    WC_WCALC(gui)->values_in_sync = TRUE;
  } else {
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "Values out of\nrange.");
    WC_WCALC(gui)->values_in_sync = FALSE;
  }

}

static void update_display(coax_gui *gui)
{
  char str[80];

  /* ---------------- a -------------- */
  sprintf(str,WC_FMT_G,gui->line->a/wc_units_to_sf(gui->line->units_abct));
  gtk_entry_set_text( GTK_ENTRY(gui->text_a), str );

  /* ---------------- b -------------- */
  sprintf(str,WC_FMT_G,gui->line->b/wc_units_to_sf(gui->line->units_abct));
  gtk_entry_set_text( GTK_ENTRY(gui->text_b), str );

  /* ---------------- c -------------- */
  sprintf(str,WC_FMT_G,gui->line->c/wc_units_to_sf(gui->line->units_abct));
  gtk_entry_set_text( GTK_ENTRY(gui->text_c), str );
  
  /* ---------------- length -------------- */
  sprintf(str,WC_FMT_G,gui->line->len/wc_units_to_sf(gui->line->units_len));
  gtk_entry_set_text( GTK_ENTRY(gui->text_len), str );

  /* ---------------- Er -------------- */
  sprintf(str,WC_FMT_G,gui->line->er);
  gtk_entry_set_text( GTK_ENTRY(gui->text_er), str );

  /* ---------------- TanD -------------- */
  sprintf(str,WC_FMT_G,gui->line->tand);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tand), str );

  /* ---------------- Emax  -------------- */
  sprintf(str,WC_FMT_G,gui->line->emax/wc_units_to_sf(gui->line->units_emax));
  gtk_entry_set_text( GTK_ENTRY(gui->text_emax), str );

  /* ---------------- Tshield -------------- */
  sprintf(str,WC_FMT_G,gui->line->tshield/wc_units_to_sf(gui->line->units_abct));
  gtk_entry_set_text( GTK_ENTRY(gui->text_tshield), str );

  /* ---------------- Z0 -------------- */
  sprintf(str,WC_FMT_G,gui->line->z0);
  gtk_entry_set_text( GTK_ENTRY(gui->text_z0), str );

  /* ---------------- Electrical length -------------- */
  sprintf(str,WC_FMT_G,gui->line->elen);
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );

  /* ---------------- TE10 cutoff -------------- */
  sprintf(str,WC_FMT_G,gui->line->fc/wc_units_to_sf(gui->line->units_fc));
  gtk_entry_set_text( GTK_ENTRY(gui->text_fc), str );

  /* ---------------- freq -------------- */
  sprintf(str,WC_FMT_G,gui->line->freq/wc_units_to_sf(gui->line->units_freq));
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );

  /* ---------------- rho-a -------------- */
  sprintf(str,WC_FMT_G,gui->line->rho_a/wc_units_to_sf(gui->line->units_rho));
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho_a), str );

  /* ---------------- rho-b -------------- */
  sprintf(str,WC_FMT_G,gui->line->rho_b/wc_units_to_sf(gui->line->units_rho));
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho_b), str );

  /* ---------------- delay -------------- */
  sprintf(str,WC_FMT_G,gui->line->delay/wc_units_to_sf(gui->line->units_delay));
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );
  
  /* ---------------- loss -------------- */
  sprintf(str,WC_FMT_G,gui->line->loss/wc_units_to_sf(gui->line->units_loss));
  gtk_label_set_text( GTK_LABEL(gui->label_loss), str );
  
  /* ---------------- loss/length -------------- */
  sprintf(str,WC_FMT_G,gui->line->losslen/wc_units_to_sf(gui->line->units_losslen));
  gtk_label_set_text( GTK_LABEL(gui->label_losslen), str );

  /* ---------------- conductor loss -------------- */
  sprintf(str,WC_FMT_G,gui->line->alpha_c/wc_units_to_sf(gui->line->units_loss));
  gtk_label_set_text( GTK_LABEL(gui->label_closs), str );
  
  /* ---------------- dielectric loss -------------- */
  sprintf(str,WC_FMT_G,gui->line->alpha_d/wc_units_to_sf(gui->line->units_loss));
  gtk_label_set_text( GTK_LABEL(gui->label_dloss), str );

  /* ---------------- L -------------- */
  sprintf(str,WC_FMT_G,gui->line->L/gui->line->units_L->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_L), str );

  /* ---------------- R -------------- */
  sprintf(str,WC_FMT_G,gui->line->R/gui->line->units_R->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_R), str );

  /* ---------------- C -------------- */
  sprintf(str,WC_FMT_G,gui->line->C/gui->line->units_C->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_C), str );

  /* ---------------- G -------------- */
  sprintf(str,WC_FMT_G,gui->line->G/gui->line->units_G->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_G), str );

}


static void tooltip_init(coax_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_a, _("Radius of inner conductor"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_b, _("Inner radius of outer conductor"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_c, _("Offset of inner conductor"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_len, _("Physical length of line"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_er, _("Dielectric relative permitivitty"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_tand, _("Dielectric loss tangent"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_emax, _("Breakdown field strength "
		       "in the dielectric"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_tshield, _("Thickness of outer conductor"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_z0, _("Characteristic impedance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, _("Electrical length of line"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, _("Frequency of operation"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_fc, _("Cutoff frequency for"
		       " TE10 mode"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho_a, _("Resistivity of inner conductor"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho_b, _("Resistivity of outer conductor"), NULL);

  
}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("coax_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
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
  fprintf(fp,"/col2x %g 2 div inch def\n", global_print_config->paperwidth);
  fprintf(fp,"/coly currentpoint exch pop def\n");
  fprintf(fp,"/linespace 1.5 def\n");
  fprintf(fp,"\n");
  fprintf(fp,"col1x coly moveto\n");
  fprintf(fp,"/leftcol col1x  def\n");

  fprintf(fp,"(a) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->a/gui->line->units_abct->sf, gui->line->units_abct->name);
  fprintf(fp,"(2a) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  2*gui->line->a/gui->line->units_abct->sf, gui->line->units_abct->name);
  fprintf(fp,"(b) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->b/gui->line->units_abct->sf, gui->line->units_abct->name);
  fprintf(fp,"(2b) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  2*gui->line->b/gui->line->units_abct->sf, gui->line->units_abct->name);
  fprintf(fp,"(c) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->c/gui->line->units_abct->sf, gui->line->units_abct->name);
  fprintf(fp,"(t) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->tshield/gui->line->units_abct->sf, gui->line->units_abct->name);

  fprintf(fp,"(length) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->len/gui->line->units_len->sf, gui->line->units_len->name);
  fprintf(fp,"(e) symbolshow (r) subshow tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->line->er);
  fprintf(fp,"(tan) show (d) symbolshow tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->line->tand);
  fprintf(fp,"(r) symbolshow (a) subshow tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->rho_a/gui->line->units_rho->sf, gui->line->units_rho->name);
  fprintf(fp,"(r) symbolshow (b) subshow tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->rho_b/gui->line->units_rho->sf, gui->line->units_rho->name);
  fprintf(fp,"(frequency) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->freq/gui->line->units_freq->sf, gui->line->units_freq->name);
  fprintf(fp,"newline\n");

  /* Second column of the output */
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");


  fprintf(fp,"(Z0) show tab1 (=) show tab2 (" WC_FMT_G " ) show (W) symbolshow newline\n",
	  gui->line->z0);
  fprintf(fp,"(elen) show tab1 (=) show tab2 (" WC_FMT_G " deg) show newline\n",
	  gui->line->elen);
  fprintf(fp,"(delay) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->delay/gui->line->units_delay->sf, gui->line->units_delay->name);
  fprintf(fp,"(TE) show (10) subshow newlineclose (  cutoff) show "
	  "tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->fc/gui->line->units_fc->sf,gui->line->units_fc->name);
  fprintf(fp,"(Loss) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->loss/gui->line->units_loss->sf, gui->line->units_loss->name);
  fprintf(fp,"(Conductor) show newlineclose (  loss) show "
	  "tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->alpha_c/gui->line->units_losslen->sf, gui->line->units_losslen->name);
  fprintf(fp,"(Dielectric) show newlineclose (  loss) show "
	  "tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->alpha_d/gui->line->units_losslen->sf, gui->line->units_losslen->name);

  fprintf(fp,"(Loss/Len) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->losslen/gui->line->units_losslen->sf, gui->line->units_losslen->name);

  fprintf(fp,"(Maximum) show  newlineclose (  E-field) show "
	  "tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->emax/gui->line->units_emax->sf, gui->line->units_emax->name);

  fprintf(fp,"newline\n");

  fprintf(fp,"(L) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->L/gui->line->units_L->sf, gui->line->units_L->name);
  fprintf(fp,"(R) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->R/gui->line->units_R->sf, gui->line->units_R->name);
  fprintf(fp,"(C) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->C/gui->line->units_C->sf, gui->line->units_C->name);
  fprintf(fp,"(G) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->G/gui->line->units_G->sf, gui->line->units_G->name);

  fprintf(fp,"newline\n");

}

