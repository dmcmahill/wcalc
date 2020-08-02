
/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2009 Dan McMahill
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
#include "misc.h"

#if GTK_CHECK_VERSION(2,10,0)
#include "pixmaps/figure_coupled_microstrip.h"
#endif
#include "coupled_microstrip.h"
#include "coupled_microstrip_gui.h"
#include "coupled_microstrip_loadsave.h"
#include "newprint.h"
#include "physconst.h"
#include "units.h"

#include "symbols.h"
#include "gtk-units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void use_z0k_pressed(GtkWidget *widget, gpointer data );
static void use_z0ez0o_pressed(GtkWidget *widget, gpointer data );

static void print_ps(Wcalc *wcalc,FILE *fp);
static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize( GtkWidget *w, gpointer data );
static void calculate( coupled_microstrip_gui *gui, GtkWidget *w, gpointer data );
static void update_display( coupled_microstrip_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

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
  wcalc->dump_values = dump_values;

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
  GtkTooltips *tips;
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

  tips = gtk_tooltips_new();

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
  button = gtk_button_new_with_label (_("<-Synthesize"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 4, 
		   0, GTK_EXPAND|GTK_FILL, WC_XPAD, WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize width, spacing, and length to obtain the specified "
			 "impedances and electrical length"),
		       NULL);
  gtk_widget_show (button);

  /* Analyze button */
  button = gtk_button_new_with_label (_("Analyze->"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 0, 4, 
		   0, GTK_EXPAND|GTK_FILL, WC_XPAD, WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate electrical characteristics "
		       "from physical parameters"),
		       NULL);
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

  /* ---------------- Spacing  -------------- */

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
  gtk_table_attach(GTK_TABLE(table), text, x+4, x+5, y, y+1,
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
  gtk_table_attach(GTK_TABLE(table), text, x+4, x+5, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  gtk_widget_show(text);
  wc_units_attach_units_label(ug, text);

  y++;

  /* ----------------  Incremental circuit model -------------- */

  /* ---------------- L ----------------- */
  text = gtk_label_new( "Lev" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Lev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Lev);

  text = wc_units_menu_new(gui->line->units_L, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+4, x+5, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Lev, &(gui->line->Lev), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "Lodd" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Lodd = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lodd, x+3, x+4, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Lodd);

  wc_units_attach_label(ug, gui->label_Lodd, &(gui->line->Lodd), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- R ----------------- */
  text = gtk_label_new( "Rev" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Rev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Rev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Rev);

  text = wc_units_menu_new(gui->line->units_R, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+4, x+5, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Rev, &(gui->line->Rev), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "Rodd" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Rodd = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Rodd, x+3, x+4, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Rodd);

  wc_units_attach_label(ug, gui->label_Rodd, &(gui->line->Rodd), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- C ----------------- */
  text = gtk_label_new( "Cev" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Cev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Cev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Cev);

  text = wc_units_menu_new(gui->line->units_C, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+4, x+5, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Cev, &(gui->line->Cev), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "Codd" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Codd = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Codd, x+3, x+4, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Codd);

  wc_units_attach_label(ug, gui->label_Codd, &(gui->line->Codd), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- G ----------------- */
  text = gtk_label_new( "Gev" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Gev = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Gev, x+1, x+2, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Gev);

  text = wc_units_menu_new(gui->line->units_G, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+4, x+5, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Gev, &(gui->line->Gev), 
			NULL, NULL, WC_FMT_G, 1);

  text = gtk_label_new( "Godd" );
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Godd = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Godd, x+3, x+4, y, y+1,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Godd);

  wc_units_attach_label(ug, gui->label_Godd, &(gui->line->Godd), 
			NULL, NULL, WC_FMT_G, 1);

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
    

  WC_WCALC(gui)->text_status = gtk_label_new( "Values Out Of Sync" );
  gtk_box_pack_start (GTK_BOX (my_hbox), WC_WCALC(gui)->text_status, FALSE, FALSE, 0);
  gtk_widget_show (WC_WCALC(gui)->text_status);
  

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
  if (rslt==0) {
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "");
    WC_WCALC(gui)->values_in_sync = TRUE;
  } else {
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "Values out of\nrange.");
    WC_WCALC(gui)->values_in_sync = FALSE;
  }

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

  /* ---------------- L -------------- */
  sprintf(str,WC_FMT_G, gui->line->Lev/gui->line->units_L->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Lev), str );
  sprintf(str,WC_FMT_G, gui->line->Lodd/gui->line->units_L->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Lodd), str );
    
  /* ---------------- R -------------- */
  sprintf(str,WC_FMT_G, gui->line->Rev/gui->line->units_R->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Rev), str );
  sprintf(str,WC_FMT_G, gui->line->Rodd/gui->line->units_R->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Rodd), str );
    
  /* ---------------- C -------------- */
  sprintf(str,WC_FMT_G, gui->line->Cev/gui->line->units_C->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Cev), str );
  sprintf(str,WC_FMT_G, gui->line->Codd/gui->line->units_C->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Codd), str );
    
  /* ---------------- G -------------- */
  sprintf(str,WC_FMT_G, gui->line->Gev/gui->line->units_G->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Gev), str );
  sprintf(str,WC_FMT_G, gui->line->Godd/gui->line->units_G->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Godd), str );

  /* XXX */
#ifdef notdef
  /* ---------------- delay -------------- */
  sprintf(str,WC_FMT_G, gui->line->delay/gui->line->units_delay->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );
  
#endif

}


static void tooltip_init(coupled_microstrip_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_w, "Width of each trace", NULL);
  gtk_tooltips_set_tip(tips, gui->text_l, "Length of the traces", NULL);
  gtk_tooltips_set_tip(tips, gui->text_h, "Total substrate thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_er, "Substrate relative"
		       " dielectric constant",NULL);
  gtk_tooltips_set_tip(tips, gui->text_tand, "Substrate loss tangent", NULL);

  gtk_tooltips_set_tip(tips, gui->text_z0, "Characteristic impedance of the line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_k, "Coupling coefficient of the line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_z0e, "Even mode characteristic impedance of the line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_z0o, "Odd mode characteristic impedance of the line", NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, "Electrical length", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tmet, "Line metal thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho, "Metal resistivity", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rough, "Metal surface roughness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);
  
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

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
#if GTK_CHECK_VERSION(2,10,0)
  coupled_microstrip_gui *gui;
  coupled_microstrip_line *l;

  gui = WC_COUPLED_MICROSTRIP_GUI(wcalc);
  l = gui->line;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_coupled_microstrip_init();
  }  {
    // FIXME -- free the old list first!!!!
    list = NULL;
    list = wc_print_add_cairo(figure_coupled_microstrip_render[0], figure_coupled_microstrip_width[0], 
			      figure_coupled_microstrip_height[0], list);

    list = wc_print_add_double("Width of lines (W)", l->w, l->units_lwst, list);
    list = wc_print_add_double("Length of lines (L)", l->l, l->units_lwst, list);
    list = wc_print_add_double("Gap between lines (S)", l->s, l->units_lwst, list);

    list = wc_print_add_double("Dielectric thickness (H)", l->subs->h, l->units_lwst, list);
    list = wc_print_add_double("Relative dielectric contant ("
			       WC_SYM_EPSILON_LC "<sub>r</sub>)", l->subs->er, NULL, list);
    list = wc_print_add_double("Dielectric loss tangent (tan"
			       WC_SYM_DELTA_LC ")", l->subs->tand, NULL, list);
    list = wc_print_add_double("Metal thickness (t<sub>met</sub>)", l->subs->tmet, l->units_lwst, list);
    list = wc_print_add_double("Metal resistivity ("
			       WC_SYM_RHO_LC ")", l->subs->rho, l->units_rho, list);
    list = wc_print_add_double("Metal surface roughness (rough)", l->subs->rough, 
			       l->units_rough, list);

    list = wc_print_add_double("Analysis Frequency", l->freq, l->units_freq, list);

    list = wc_print_add_double("Characteristic Impedance", l->z0, NULL, list);
    list = wc_print_add_double("Coupling coefficient", l->k, NULL, list);
    list = wc_print_add_double("Even mode impedance", l->z0e, NULL, list);
    list = wc_print_add_double("Odd mode impedance", l->z0o, NULL, list);

    list = wc_print_add_double("Electrical length", l->len, NULL, list);

    list = wc_print_add_double("Even mode loss", l->loss_ev, l->units_loss, list);
    list = wc_print_add_double("Odd mode loss", l->loss_odd, l->units_loss, list);
    list = wc_print_add_double("Even mode loss per length", l->losslen_ev, l->units_losslen, list);
    list = wc_print_add_double("Odd mode loss per length", l->losslen_odd, l->units_losslen, list);

    list = wc_print_add_double("Even mode open end length correction",
			       l->deltale, l->units_deltal, list);
    list = wc_print_add_double("Odd mode open end length correction",
			       l->deltalo, l->units_deltal, list);
     
    list = wc_print_add_double("Even mode incremental Inductance", l->Lev, l->units_L, list);
    list = wc_print_add_double("Even mode incremental Capacitance", l->Cev, l->units_C, list);
    list = wc_print_add_double("Even mode incremental Resistance", l->Rev, l->units_R, list);
    list = wc_print_add_double("Even mode incremental Conductance", l->Gev, l->units_G, list);

    list = wc_print_add_double("Odd mode incremental Inductance", l->Lodd, l->units_L, list);
    list = wc_print_add_double("Odd mode incremental Capacitance", l->Codd, l->units_C, list);
    list = wc_print_add_double("Odd mode incremental Resistance", l->Rodd, l->units_R, list);
    list = wc_print_add_double("Odd mode incremental Conductance", l->Godd, l->units_G, list);
    
  }
#endif

  return list;
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
  fprintf(fp,"/col2x %g 2 div inch def\n", global_print_config->paperwidth);
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


  fprintf(fp,"newline\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"(Lev) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Lev/gui->line->units_L->sf,
	  gui->line->units_L->name);
  fprintf(fp,"(Rev) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Rev/gui->line->units_R->sf,
	  gui->line->units_R->name);
  fprintf(fp,"(Cev) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Cev/gui->line->units_C->sf,
	  gui->line->units_C->name);
  fprintf(fp,"(Gev) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Gev/gui->line->units_G->sf,
	  gui->line->units_G->name);


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

  fprintf(fp,"newline newlineclose\n");
  fprintf(fp,"(Lodd) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Lodd/gui->line->units_L->sf,
	  gui->line->units_L->name);
  fprintf(fp,"(Rodd) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Rodd/gui->line->units_R->sf,
	  gui->line->units_R->name);
  fprintf(fp,"(Codd) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Codd/gui->line->units_C->sf,
	  gui->line->units_C->name);
  fprintf(fp,"(Godd) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->Godd/gui->line->units_G->sf,
	  gui->line->units_G->name);


}

