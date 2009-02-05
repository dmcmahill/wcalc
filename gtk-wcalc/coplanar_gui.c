/* $Id: coplanar_gui.c,v 1.9 2009/02/05 05:42:50 dan Exp $ */

/*
 * Copyright (C) 2006, 2007, 2009 Dan McMahill
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
#include "pixmaps/figure_coplanar.h"
#endif
#include "coplanar.h"
#include "coplanar_gui.h"
#include "coplanar_loadsave.h"
#include "newprint.h"
#include "physconst.h"
#include "units.h"

#include "gtk-units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);
static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_w( GtkWidget *w, gpointer data );
static void synthesize_s( GtkWidget *w, gpointer data );
static void synthesize_h( GtkWidget *w, gpointer data );
static void synthesize_er( GtkWidget *w, gpointer data );
static void calculate( coplanar_gui *gui, GtkWidget *w, gpointer data );
static void update_display( coplanar_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(coplanar_gui *gui, GtkWidget *parent);
static void outputs_init(coplanar_gui *gui, GtkWidget *parent);
static void picture_init(coplanar_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(coplanar_gui *gui);

static char *name="Coplanar Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

coplanar_gui *coplanar_gui_new(void)
{
  coplanar_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (coplanar_gui *) malloc(sizeof(coplanar_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"coplanar_gui_new: malloc() failed\n");
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = coplanar_gui_init;
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

  /* create the coplanar line which will be used */
  new_gui->line = coplanar_line_new();
  new_gui->phys_units_text = NULL;

  return new_gui;
}

void coplanar_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  coplanar_gui *gui;

  gui = WC_COPLANAR_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("coplanar_gui.c:coplanar_gui_init():  calling coplanar_load\n");
#endif
    if (coplanar_load(gui->line,fp) != 0) {
      alert("ERROR:  Could not load the coplanar\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("coplanar_gui.c:coplanar_gui_init():  new using defaults\n");
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

static void values_init(coplanar_gui *gui, GtkWidget *parent)
{
  GtkTooltips *tips;
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *lwht;
  GtkWidget *button;
  GtkWidget *frame;
  wc_units_gui *ug;

  /* position in the table */
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
  

  /* Analyze button */
  button = gtk_button_new_with_label (_("Analyze->"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 4, 5, 0, 4, 0,GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate electrical characteristics "
		       "from physical parameters"),
		       NULL);
  gtk_widget_show (button);
  

  /* ---------------- Width  -------------- */

  text = gtk_label_new( "Width (W)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_w = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_w),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_w, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_w),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_w), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_w);

  text = wc_units_menu_new(gui->line->units_lwht, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+2, x+3, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);


  /* synthesize width */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_w), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button,
		   x+3, x+4, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize width and length to obtain the specified "
			 "characteristic impedance and electrical length"),
		       NULL);
  gtk_widget_show (button);

  y++;

  /* ---------------- Spacing (gap)  -------------- */

  text = gtk_label_new( "Spacing (S)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_s = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(gui->text_s),"      ");
  gtk_table_attach (GTK_TABLE(table), gui->text_s, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
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

  /* synthesize spacing */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_s), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button,
		   x+3, x+4, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize spacing and length to obtain the specified "
			 "characteristic impedance and electrical length"),
		       NULL);
  gtk_widget_show (button);

  y++;

  /* ---------------- Length  -------------- */
  text = gtk_label_new( "Length (L)" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_l = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_l, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
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
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_h = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_h, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
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

  /* synthesize height */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_h), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button,
		   x+3, x+4, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize substrate thickness and length to"
			 " obtain the specified "
			 "characteristic impedance and electrical length"),
		       NULL);
  gtk_widget_show (button);

  y++;

  /* ---------------- Dielectric Constant -------------- */

  text = gtk_label_new( "Er" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_er = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_er, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_er),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_er), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_er);

  /* dielectric constant */
  button = gtk_button_new_with_label ("<-Synthesize");
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (synthesize_er), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button,
		   x+3, x+4, y, y+1, 0, GTK_EXPAND|GTK_FILL, WC_XPAD, WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Synthesize substrate dielectric constant"
			 " and length to obtain the specified "
			 "characteristic impedance and electrical length"),
		       NULL);
  gtk_widget_show (button);

  y++;

  /* ---------------- Loss Tangent -------------- */

  text = gtk_label_new( "Tand" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->text_tand = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tand, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_tand),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_tand), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_tand);

  y++;

  /* ---------------- Use bottom side ground -------------- */
  gui->button_gnd = 
    gtk_check_button_new_with_label(_("With bottom side ground"));
  gtk_table_attach(GTK_TABLE(table), gui->button_gnd,
		   x+1, x+3, y, y+1, 0,0,WC_XPAD,WC_YPAD);  
  gtk_widget_show(gui->button_gnd);

  gtk_signal_connect (GTK_OBJECT (gui->button_gnd), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->button_gnd), "clicked",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);

  y++;

  /* ----------------  Impedance -------------- */
  x = 5;
  y = 0;

  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_z0 = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_z0, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_z0),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_z0), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_z0);

  y++;

  /* ----------------  Electrical length -------------- */

  text = gtk_label_new( "Elec. Len." );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_elen = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_elen, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_elen),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_elen), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_elen);

  y++;

  /* ---------------- Metal thickness -------------- */

  text = gtk_label_new( "Tmet" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);


  gui->text_tmet = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_tmet,
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
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
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_rho  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rho,
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rho),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rho), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rho);

  text = wc_units_menu_new(gui->line->units_rho, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+2, x+3, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  y++;

  /* ---------------- Surface roughness -------------- */

  text = gtk_label_new( "Rough" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_rough  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_rough,
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_rough),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_rough), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_rough);

  text = wc_units_menu_new(gui->line->units_rough, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+2, x+3, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  y++;

  /* ---------------- Frequency -------------- */

  text = gtk_label_new( "Frequency" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->text_freq  = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_table_attach (GTK_TABLE(table), gui->text_freq,
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(gui->text_freq),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (gui->text_freq), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(gui->text_freq);

  text = wc_units_menu_new(gui->line->units_freq, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+2, x+3, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  text = gtk_label_new( " " );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gtk_widget_show (table);
}

static void outputs_init(coplanar_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  wc_units_gui *ug;

  int x = 0;
  int y = 0;

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
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_delay = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_delay, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_delay);

  text = wc_units_menu_new(gui->line->units_delay, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+3, x+4, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_delay, &(gui->line->delay), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- Loss -------------- */
  text = gtk_label_new( "Loss" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_loss = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_loss);

  text = wc_units_menu_new(gui->line->units_loss, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+3, x+4, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_loss, &(gui->line->loss), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ----------------  Loss/Length -------------- */
  text = gtk_label_new( "Loss/Length" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_losslen = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_losslen);

  text = wc_units_menu_new(gui->line->units_losslen, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+3, x+4, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_losslen, &(gui->line->losslen), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ----------------  Skin Depth -------------- */
  text = gtk_label_new( "Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_depth = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_depth, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_depth);

  text = wc_units_menu_new(gui->line->units_depth, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+3, x+4, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_depth, &(gui->line->skindepth), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ----------------  End correction -------------- */
  text = gtk_label_new( "Delta L" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_deltal = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_deltal, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_deltal);

  text = wc_units_menu_new(gui->line->units_deltal, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 
		   x+3, x+4, y, y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_deltal, &(gui->line->deltal), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ----------------  Effective dielectric constant -------------- */
  text = gtk_label_new( "Keff" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_keff = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_keff, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_keff);


  /* ---------------- L -------------- */
  x = 4;
  y = 0;

  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gui->label_Ls = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Ls, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Ls);

  text = wc_units_menu_new(gui->line->units_L, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Ls, &(gui->line->Ls), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- R -------------- */
  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Rs = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Rs, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Rs);

  text = wc_units_menu_new(gui->line->units_R, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Rs, &(gui->line->Rs), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- C -------------- */
  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Cp = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Cp, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Cp);

  text = wc_units_menu_new(gui->line->units_C, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Cp, &(gui->line->Cs), 
			NULL, NULL, WC_FMT_G, 1);

  y++;

  /* ---------------- G -------------- */
  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Gp = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Gp, 
		    x+1, x+2, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Gp);

  text = wc_units_menu_new(gui->line->units_G, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1, 
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


#include "pixmaps/coplanar.xpm"

static void picture_init(coplanar_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) coplanar);
					
  
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
  calculate(WC_COPLANAR_GUI(data), w, "analyze");
}

static void synthesize_w( GtkWidget *w, gpointer data )
{
  calculate(WC_COPLANAR_GUI(data), w, "synthesize_w");
}

static void synthesize_s( GtkWidget *w, gpointer data )
{
  calculate(WC_COPLANAR_GUI(data), w, "synthesize_s");
}

static void synthesize_h( GtkWidget *w, gpointer data )
{
  calculate(WC_COPLANAR_GUI(data), w, "synthesize_h");
}

static void synthesize_er( GtkWidget *w, gpointer data )
{
  calculate(WC_COPLANAR_GUI(data), w, "synthesize_er");
}


static void calculate( coplanar_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_w) ); 
  gui->line->w=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  w = %g\n", gui->line->w);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_s) ); 
  gui->line->s=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  s = %g\n", gui->line->s);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l) ); 
  gui->line->l=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  l = %g\n", gui->line->l);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_h) ); 
  gui->line->subs->h=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  h = %g\n", gui->line->subs->h);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) ); 
  gui->line->subs->er=atof(vstr);
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  er = %g\n", gui->line->subs->er);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) ); 
  gui->line->subs->tand=atof(vstr);
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  tand = %g\n", gui->line->subs->tand);
#endif

  gui->line->with_ground = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui->button_gnd));
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  with_gnd = %d\n", 
	  gui->line->with_ground);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0) ); 
  gui->line->Ro=atof(vstr);
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  z0 = %g\n", gui->line->z0);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) ); 
  gui->line->len=atof(vstr);
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  elen = %g\n", gui->line->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) ); 
  gui->line->subs->tmet=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  tmet = %g\n", gui->line->subs->tmet);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) ); 
  gui->line->subs->rho=atof(vstr)*gui->line->units_rho->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  rho = %g\n", gui->line->subs->rho);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) ); 
  gui->line->subs->rough=atof(vstr)*gui->line->units_rough->sf;
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  rough = %g\n",
	  gui->line->subs->rough);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->line->freq=atof(vstr)*gui->line->units_freq->sf;

#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  freq = %g\n",
	  gui->line->freq);
#endif

#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  calculation with \"%s\"\n",
	  (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt=coplanar_calc(gui->line, gui->line->freq);
  }
  else if( strcmp(data,"synthesize_w")==0) {
    rslt=coplanar_syn(gui->line, gui->line->freq, CPWSYN_W);
  }
  else if( strcmp(data,"synthesize_s")==0) {
    rslt=coplanar_syn(gui->line, gui->line->freq, CPWSYN_S);
  }
  else if( strcmp(data,"synthesize_h")==0) {
    rslt=coplanar_syn(gui->line, gui->line->freq, CPWSYN_H);
  }
  else if( strcmp(data,"synthesize_er")==0) {
    rslt=coplanar_syn(gui->line, gui->line->freq, CPWSYN_ER);
  }
  else{
    fprintf(stderr,"coplanar_gui.c:  error in coplanar callback\n"
	    "Please report this bug and how you triggered it\n");
    exit(1);
  }
  
#ifdef DEBUG
  g_print("coplanar_gui.c:calculate():  finished calculation\n");
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

static void update_display(coplanar_gui *gui)
{
  char str[80];

  /* the entries */

  /* ---------------- w -------------- */
  sprintf(str,WC_FMT_G,gui->line->w/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_w), str );

  /* ---------------- s -------------- */
  sprintf(str,WC_FMT_G,gui->line->s/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_s), str );

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

  /* ---------------- with ground -------------- */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui->button_gnd),
			       gui->line->with_ground);
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


static void tooltip_init(coplanar_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_w, "Width of coplanar", NULL);
  gtk_tooltips_set_tip(tips, gui->text_s, "Spacing to ground plane", NULL);
  gtk_tooltips_set_tip(tips, gui->text_l, "Length of coplanar", NULL);
  gtk_tooltips_set_tip(tips, gui->text_h, "Total substrate thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_er, "Substrate relative"
		       " dielectric constant",NULL);
  gtk_tooltips_set_tip(tips, gui->text_tand, "Substrate loss tangent", NULL);
  gtk_tooltips_set_tip(tips, gui->button_gnd, 
		       "Check to include the effects of a bottom side "
		       "ground plane", NULL);

  gtk_tooltips_set_tip(tips, gui->text_z0, "Characteristic impedance", NULL);
  gtk_tooltips_set_tip(tips, gui->text_elen, "Electrical length", NULL);
  gtk_tooltips_set_tip(tips, gui->text_tmet, "Line metal thickness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rho, "Metal resistivity", NULL);
  gtk_tooltips_set_tip(tips, gui->text_rough, "Metal surface roughness", NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, "Frequency of operation", NULL);
  
}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("coplanar_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  coplanar_save(WC_COPLANAR_GUI(wcalc)->line,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
#if GTK_CHECK_VERSION(2,10,0)
  coplanar_gui *gui;
  coplanar_line *l;

  gui = WC_COPLANAR_GUI(wcalc);
  l = gui->line;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_coplanar_init();
  }  {
    // FIXME -- free the old list first!!!!
    list = NULL;
    list = wc_print_add_cairo(figure_coplanar_render[0], figure_coplanar_width[0], 
			      figure_coplanar_height[0], list);

    list = wc_print_add_double("Width of line (w)", l->w, l->units_lwht, list);
    list = wc_print_add_double("Space to the coplanar ground lines (s)", l->s, l->units_lwht, list);
    list = wc_print_add_double("Length of line (l)", l->l, l->units_lwht, list);

    list = wc_print_add_string( "With bottom side ground?", l->with_ground ? "Yes" : "No", NULL, list);

    list = wc_print_add_double("Dielectric thickness (h)", l->subs->h, l->units_lwht, list);
    list = wc_print_add_double("Relative dielectric contant (er)", l->subs->er, NULL, list);
    list = wc_print_add_double("Dielectric loss tangent (tand)", l->subs->tand, NULL, list);
    list = wc_print_add_double("Metal thickness (tmet)", l->subs->tmet, l->units_lwht, list);
    list = wc_print_add_double("Metal resistivity (rho)", l->subs->rho, l->units_rho, list);
    list = wc_print_add_double("Metal surface roughness (rough)", l->subs->rough, 
			       l->units_rough, list);

    list = wc_print_add_double("Aanalysis Frequency", l->freq, l->units_freq, list);

    list = wc_print_add_double("Characteristic Impedance", l->z0, NULL, list);
    list = wc_print_add_double("Characteristic Impedance (real part)", l->Ro, NULL, list);
    list = wc_print_add_double("Characteristic Impedance (imaginary part)", l->Xo, NULL, list);
    list = wc_print_add_double("Electrical length", l->len, NULL, list);
    list = wc_print_add_double("Delay", l->delay, l->units_delay, list);

    list = wc_print_add_double("Conductor loss", l->lc, l->units_loss, list);
    list = wc_print_add_double("Dielectric loss", l->ld, l->units_loss, list);
    list = wc_print_add_double("Total loss", l->loss, l->units_loss, list);
    list = wc_print_add_double("Total loss per length", l->losslen, l->units_losslen, list);
    list = wc_print_add_double("Open end length correction", l->deltal, l->units_deltal, list);
     
    list = wc_print_add_double("Incremental Inductance (L)", l->Ls, l->units_L, list);
    list = wc_print_add_double("Incremental Capacitance (C)", l->Cs, l->units_C, list);
    list = wc_print_add_double("Incremental Resistance (R)", l->Rs, l->units_R, list);
    list = wc_print_add_double("Incremental Conductance (G)", l->Gs, l->units_G, list);
    
  }
#endif

  return list;
}

static void print_ps(Wcalc *wcalc, FILE *fp)
{
  coplanar_gui *gui;
  char *file;

  gui = WC_COPLANAR_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("coplanar.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "coplanar.eps");
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
  fprintf(fp,"(S) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->s/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"(H) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->subs->h/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"(L) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->line->l/gui->line->units_lwht->sf,
	  gui->line->units_lwht->name);
  fprintf(fp,"newline\n");
  fprintf(fp,"(%s bottom side ground plane) show newline\n",
	  gui->line->with_ground == 1 ? "With" : "Without");
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

  fprintf(fp,"(freq) show tab1 (=) show tab2 (" 
	  WC_FMT_G " %s) show newline\n",
	  gui->line->freq/gui->line->units_freq->sf,
	  gui->line->units_freq->name);

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

