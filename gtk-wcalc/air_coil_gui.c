/* $Id: air_coil_gui.c,v 1.10 2004/07/28 03:25:24 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004 Dan McMahill
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
#include "units.h"

#include "air_coil.h"
#include "air_coil_gui.h"
#include "air_coil_loadsave.h"
#include "physconst.h"

#include "gtk-units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);

static void use_len_pressed(GtkWidget *widget, gpointer data );
static void use_fill_pressed(GtkWidget *widget, gpointer data );

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_N( GtkWidget *w, gpointer data );
static void synthesize_dia( GtkWidget *w, gpointer data );
static void synthesize_len( GtkWidget *w, gpointer data );
static void calculate( air_coil_gui *gui, GtkWidget *w, gpointer data );
static void update_display( air_coil_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void vals_changedCB( GtkWidget *widget, gpointer data );

static void values_init(air_coil_gui *gui, 
			GtkWidget *value_parent,
			GtkWidget *output_parent);
static void picture_init(air_coil_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(air_coil_gui *gui);

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
      fprintf(stderr,_("air_coil_gui_new: malloc() failed\n"));
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
  wcalc->units_menu_list = NULL;

  /*
   * Initialize the model dependent portions
   */

  /* create the air_coil line which will be used */
  new_gui->coil = air_coil_new();

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
    g_print(_("air_coil_gui.c:air_coil_gui_init():  calling air_coil_load\n"));
#endif
    if (air_coil_load(gui->coil,fp) != 0) {
      alert(_("ERROR:  Could not load the air coil\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n"));
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print(_("air_coil_gui.c:air_coil_gui_init():  new using defaults\n"));
#endif
  }

#ifdef DEBUG
    g_print(_("air_coil_gui.c:air_coil_gui_init():  creating main vboxes\n"));
#endif

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


#ifdef DEBUG
    g_print(_("air_coil_gui.c:air_coil_gui_init():  calling values init\n"));
#endif
  values_init(gui, values_vbox, outputs_vbox);
  picture_init(gui,wcalc->window,picture_vbox);

  tooltip_init(gui);

  gtk_widget_show (wcalc->window);

  wcalc->init_done=1;

  update_display(gui);

  
  /* run the analysis once since we've changed input units */
#ifdef DEBUG
    g_print(_("air_coil_gui.c:air_coil_gui_init():  running analysis\n"));
#endif
  wc_units_menu_init( wcalc );
  analyze(NULL, gui);
  wc_units_menu_init( wcalc );

}

/*
 * Private Functions
 */

static void values_init(air_coil_gui *gui, 
			GtkWidget *value_parent,
			GtkWidget *output_parent)
{
  GtkWidget *hbox;
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *button;
  GtkWidget *frame;
  wc_units_gui *ug;
  wc_units_gui *f_ug;
  wc_units_gui *L_ug;

  /* the "Len/Fill" radio button group */
  GSList *len_group;


  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(value_parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Analysis/Synthesis Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (4, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);
  

  /* Analyze button */
  button = gtk_button_new_with_label (_("Analyze"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 0, 4, 
		   0, GTK_EXPAND|GTK_FILL, WC_XPAD, WC_YPAD);
  gtk_widget_show (button);
  
  /* 
   * Synthesize buttons 
   */

  /* ---------------- Number of Turns -------------- */

  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_N), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show (button);

  /* I.D. */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_dia), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_sensitive (button, FALSE);
  gtk_widget_show (button);

  /* Len */
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_len), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 2, 4, 0, GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_show (button);



  /* ---------------- Number of Turns -------------- */

  text = gtk_label_new( "N" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_Nf = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_Nf),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_Nf, 1, 2, 0, 1,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_Nf),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_Nf), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_Nf), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_Nf);

  /* ---------------- Coil Diameter -------------- */
  text = gtk_label_new( "I.D." );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_dia = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_dia, 1, 2, 1, 2,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_dia),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_dia), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_dia), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_dia);

  text = wc_units_menu_new(gui->coil->units_dia, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ---------------- Wire Size  -------------- */

  text = gtk_label_new( _("Wire Dia.") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_AWGf = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_AWGf, 6, 7, 0, 1,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_AWGf),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_AWGf), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_AWGf), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_AWGf);

  hbox = gtk_hbox_new(FALSE,1);
  gtk_table_attach(GTK_TABLE(table), hbox, 7, 8, 0, 1, 
		   GTK_EXPAND|GTK_FILL,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(hbox);
  text = gtk_label_new( _("AWG") );
  gtk_box_pack_start (GTK_BOX (hbox), text, FALSE, FALSE, 0);
  gtk_widget_show(text);

  /* ---------------- Resistivity  -------------- */

  text = gtk_label_new( _("Resistivity") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_rho = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho, 
		    6, 7, 1, 2, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho);

  text = wc_units_menu_new(gui->coil->units_rho, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 1, 2, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ----------------  Inductance  -------------- */

  text = gtk_label_new( _("Inductance") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_L = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_L, 6, 7, 2, 3,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_L),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_L), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_L), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_L);


  text = wc_units_menu_new(gui->coil->units_L, WC_WCALC(gui), &L_ug);
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ----------------  Frequency  -------------- */

  text = gtk_label_new( _("Frequency") );
  gtk_table_attach(GTK_TABLE(table), text, 5, 6, 3, 4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  gui->text_freq = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq,
		    6, 7, 3, 4, 0, 0, WC_XPAD, WC_YPAD);  
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);


  text = wc_units_menu_new(gui->coil->units_freq, WC_WCALC(gui), &f_ug);
  gtk_table_attach(GTK_TABLE(table), text, 7, 8, 3, 4, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ----------------    -------------- */

  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  /* ----------------  Coil Length  -------------- */
  /* the 2 text entries */
  gui->text_len = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_len, 1, 2, 2, 3,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_len),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_len), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_len);

  gui->text_fill = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_fill, 1, 2, 3, 4,0,0,WC_XPAD,WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_fill),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_fill), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_fill), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_fill);

  /* */
  button = gtk_radio_button_new_with_label (NULL, _("Len."));
  gtk_table_attach(GTK_TABLE(table), button, 0, 1, 2, 3, 0,0,WC_XPAD,WC_YPAD);
  gtk_signal_connect(GTK_OBJECT(button), "clicked",
		     GTK_SIGNAL_FUNC(use_len_pressed),
		     gui);
  gui->len_button = button;
  gtk_widget_show (button);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);

  len_group = gtk_radio_button_group (GTK_RADIO_BUTTON (button));
  button = gtk_radio_button_new_with_label(len_group, "Fill");
  gtk_table_attach(GTK_TABLE(table), button, 0, 1, 3, 4, 0,0,WC_XPAD,WC_YPAD);
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

  text = wc_units_menu_new(gui->coil->units_len, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 2, 3, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  /* ----------------    -------------- */


  gtk_entry_set_text( GTK_ENTRY(gui->text_Nf),"10" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_dia),"0.25" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_len),"1.0" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_AWGf),"22" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho),"1" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_L),"50" );
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq),"10" );

  gtk_widget_show (table);


  /* ----------------  Outputs  -------------- */

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(output_parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Output Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (2, 7, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* ---------------- Q -------------- */
  text = gtk_label_new( "Q" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Q = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Q, 
		    1,2,0,1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_Q);

  gui->label_Qfreq = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), gui->label_Qfreq, 2, 3, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Qfreq);
  gtk_misc_set_alignment(GTK_MISC(gui->label_Qfreq), 0, 0);
  wc_units_attach_units_label(f_ug, gui->label_Qfreq);

  
  /* ---------------- Self resonant freq.  -------------- */
  text = gtk_label_new( _("Self Resonant Freq.") );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_SRF = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_SRF, 
		    1, 2, 1, 2, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_SRF);

  text = wc_units_menu_new(gui->coil->units_SRF, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_SRF, &(gui->coil->SRF), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- closewound inductance -------------- */

  text = gtk_label_new( _("Closewound Inductance") );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Lmax = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lmax, 
		    5, 6, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Lmax);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  wc_units_attach_units_label(L_ug, text);
  wc_units_attach_label(L_ug, gui->label_Lmax, &(gui->coil->Lmax), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- Fill factor -------------- */

  text = gtk_label_new( _("Len/Closewound Len") );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);



  gui->label_fill = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_fill, 5,6,1,2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_fill);

  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  /* spacer */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

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
    

  gui->text_status = gtk_label_new( _("Values Out Of Sync") );
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
  gui->coil->Nf = atof(vstr);
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  Nf = %g\n"), gui->coil->Nf);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_dia) ); 
  gui->coil->dia = atof(vstr)*gui->coil->units_dia->sf;
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  I.D. = %g = %g %s\n"), 
	  gui->coil->dia, 
	  gui->coil->dia/gui->coil->units_dia->sf,
	  gui->coil->units_dia->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_len) ); 
  gui->coil->len = atof(vstr)*gui->coil->units_len->sf;
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  len = %g = %g %s\n"),
	  gui->coil->len,
	  gui->coil->len/gui->coil->units_len->sf,
	  gui->coil->units_len->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_fill) ); 
  gui->coil->fill = atof(vstr);
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  fill = %g\n"), 
	  gui->coil->fill);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_AWGf) ); 
  gui->coil->AWGf = atof(vstr);
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  AWGf = %g\n"), 
	  gui->coil->AWGf);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->coil->rho = atof(vstr)*gui->coil->units_rho->sf;
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  rho = %g = %g %s\n"), 
	  gui->coil->rho,
	  gui->coil->rho/gui->coil->units_rho->sf,
	  gui->coil->units_rho->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_L) ); 
  gui->coil->L = atof(vstr)*gui->coil->units_L->sf;
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  L = %g = %g %s\n"), 
	  gui->coil->L,
	  gui->coil->L/gui->coil->units_L->sf,
	  gui->coil->units_L->name);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->coil->freq=atof(vstr)*gui->coil->units_freq->sf;

#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  freq = %g = %g %s\n"),
	  gui->coil->freq,
	  gui->coil->freq/gui->coil->units_freq->sf,
	  gui->coil->units_freq->name);
#endif

  // XXX should use an enum and switch...
  if( strcmp(data,"analyze")==0) {
    rslt = air_coil_calc(gui->coil, gui->coil->freq);
  }
  else if( strcmp(data,"synthesize_N")==0) {
    rslt = air_coil_syn(gui->coil, gui->coil->freq, AIRCOILSYN_NMIN);
  }
  else if( strcmp(data,"synthesize_len")==0) {
    rslt = air_coil_syn(gui->coil, gui->coil->freq, AIRCOILSYN_NFIX);
  }
  else{
    g_print(_("error in aircoil callback"));
  }
  
#ifdef DEBUG
  g_print(_("air_coil_gui.c:calculate():  finished calculation\n"));
#endif

  update_display(gui);

  /*
   * if the calculation completed with no errors, then clear the
   * "values out of sync" field
   */
  if (rslt==0)
    gtk_label_set_text(GTK_LABEL(gui->text_status), "");
  else
    gtk_label_set_text(GTK_LABEL(gui->text_status), _("Values out of\nrange."));

}

static void update_display(air_coil_gui *gui)
{
  char str[80];

  /* ----------------  N  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->Nf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Nf), str );

  /* ----------------  Diameter  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->dia/gui->coil->units_dia->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_dia), str );

  /* ----------------  length  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->len/gui->coil->units_len->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_len), str );

  /* ----------------  fill  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->fill);
  gtk_entry_set_text( GTK_ENTRY(gui->text_fill), str );

  /* ----------------  AWG  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->AWGf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_AWGf), str );

  /* ----------------  resistivity  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->rho/gui->coil->units_rho->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );
  
  /* ----------------  inductance  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->L/gui->coil->units_L->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_L), str );
  
  /* ----------------  frequency  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->freq/gui->coil->units_freq->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );
  
  /* ----------------  Q  -------------- */
  sprintf(str, WC_FMT_G " @ " WC_FMT_G, gui->coil->Q, 
	  gui->coil->freq/gui->coil->units_freq->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Q), str );
    
  /* ----------------  SRF  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->SRF/gui->coil->units_SRF->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_SRF), str );

  /* ----------------  Lmax  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->Lmax/gui->coil->units_L->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Lmax), str );
  
  /* ----------------  fill (output)  -------------- */
  sprintf(str, WC_FMT_G, gui->coil->fill);
  gtk_label_set_text( GTK_LABEL(gui->label_fill), str );

}


static void tooltip_init(air_coil_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_Nf, _("Number of turns"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_dia, _("Inside diameter of the coil"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_len, _("Length of the coil"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_fill, _("Ratio of the length of the coil"
		       " to the length of a closewound coil"),NULL);
  gtk_tooltips_set_tip(tips, gui->text_AWGf, _("Wire diameter"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_rho, _("Resistivity of the wire"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_L, _("Inductance of the coil"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, _("Frequency of operation"), NULL);
  
}

static void vals_changedCB(GtkWidget *widget, gpointer data )
{
  air_coil_gui *gui;

  gui = WC_AIR_COIL_GUI(data);

  if(WC_WCALC(gui)->init_done)
    gtk_label_set_text(GTK_LABEL(gui->text_status), _("Values Out Of Sync"));
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


static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("air_coil_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
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
  fprintf(fp,"/col2x %g 2 div inch def\n", global_print_config->paperwidth);
  fprintf(fp,"/coly currentpoint exch pop def\n");
  fprintf(fp,"/linespace 1.5 def\n");
  fprintf(fp,"\n");
  fprintf(fp,"col1x coly moveto\n");
  fprintf(fp,"/leftcol col1x  def\n");
  fprintf(fp,"(N) show tab1 (=) show tab2 (%g turns) show newline\n",
	  gui->coil->Nf);
  fprintf(fp,"(I.D.) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->coil->dia/gui->coil->units_dia->sf,
	  gui->coil->units_dia->name);
  fprintf(fp,"(Len.) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->coil->len/gui->coil->units_len->sf,
	  gui->coil->units_len->name);
  fprintf(fp,"newline\n");
  fprintf(fp,"(Wire Size) show tab1 (=) show tab2 (" WC_FMT_G " AWG) show newline\n",
	  gui->coil->AWGf);
  fprintf(fp,"(Rho) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->coil->rho/gui->coil->units_rho->sf,
	  gui->coil->units_rho->name);
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");
  fprintf(fp,"(L) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->coil->L/gui->coil->units_L->sf,
	  gui->coil->units_L->name);
  fprintf(fp,"(Q) show tab1 (=) show tab2 (" WC_FMT_G " at " WC_FMT_G " %s) show newline\n",
	  gui->coil->Q, gui->coil->freq/gui->coil->units_freq->sf,
	  gui->coil->units_freq->name);
  fprintf(fp,"(SRF) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->coil->SRF/gui->coil->units_SRF->sf,
	  gui->coil->units_SRF->name);
  fprintf(fp,"(Closewound) show\n");
  fprintf(fp,"linespace /linespace 1.0 def newline /linespace exch def \n");
  fprintf(fp,"(inductance) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->coil->Lmax/gui->coil->units_L->sf,
	  gui->coil->units_L->name);
  fprintf(fp,"(fill) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->coil->fill);
  fprintf(fp,"newline\n");
  
}

