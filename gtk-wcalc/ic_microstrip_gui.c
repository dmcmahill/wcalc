/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2021 Dan McMahill
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
#include "menus.h"
#include "misc.h"
#include "units.h"

#include "pixmaps/ic_microstrip.xpm"
#include "pixmaps/figure_ic_microstrip.h"
#include "ic_microstrip.h"
#include "ic_microstrip_gui.h"
#include "ic_microstrip_loadsave.h"
#include "newprint.h"
#include "physconst.h"

#include "symbols.h"
#include "gtk-units.h"
#include "utils.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_w( GtkWidget *w, gpointer data );
static void synthesize_h( GtkWidget *w, gpointer data );
static void synthesize_tox( GtkWidget *w, gpointer data );
static void calculate( ic_microstrip_gui *gui, GtkWidget *w, gpointer data );
static void update_display( ic_microstrip_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(ic_microstrip_gui *gui, GtkWidget *parent);
static void outputs_init(ic_microstrip_gui *gui, GtkWidget *parent);
static void tooltip_init(ic_microstrip_gui *gui);

static char *name="IC_Microstrip Analysis/Synthesis";
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
  Wcalc_init(wcalc);

  /*
   * Supply info for this particular GUI
   */
  wcalc->init = ic_microstrip_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;

  wcalc->model_name = name;
  wcalc->model_version = version;

  /*
   * Initialize the model dependent portions
   */

  /* create the ic_microstrip line which will be used */
  new_gui->line = ic_microstrip_line_new();

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
  wc_picture_init(wcalc, picture_vbox, (const char **) ic_microstrip);

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

static void values_init(ic_microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  wc_units_gui *ug = NULL, *lwht_ug = NULL;

  /* position in the table */
  int y = 0;
  int x = 0;
  int xb = 3;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (4, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* ---------------- Analyze Button  -------------- */
  wc_table_add_button_wh(table, _("Analyze"),
                         _("Calculate electrical characteristics "
                           "from physical parameters"),
                         analyze, gui, xb+1, 1, 0, 3, NULL);

  /* ---------------- Width  -------------- */
  wc_table_add_entry_new_units(table, gui, _("Width (W)"),
                               &(gui->text_w), gui->line->units_lwht, &lwht_ug,
                               &(gui->line->w), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize width and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_w, gui, xb, y-1);

  /* ---------------- Length  -------------- */
  wc_table_add_entry_attach_units(table, gui, _("Length (L)"),
                               &(gui->text_l), gui->line->units_lwht, &lwht_ug,
                               &(gui->line->l), &x, &y);

  /* ---------------- Substrate Height  -------------- */
  wc_table_add_entry_attach_units(table, gui, _("Substrate Height (H)"),
                               &(gui->text_h), gui->line->units_lwht, &lwht_ug,
                               &(gui->line->subs->h), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize substrate thickness and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_h, gui, xb, y-1);

  /* ---------------- Substrate Dielectric Constant -------------- */
  wc_table_add_entry_no_units(table, gui, "Es",
                              &(gui->text_es),
                              &(gui->line->subs->es), &x, &y);

  /* ---------------- Substrate conductivity -------------- */
  wc_table_add_entry_new_units(table, gui, _("sigmas"),
                                  &(gui->text_sigmas), gui->line->units_sigmas, &ug,
                                  &(gui->line->subs->sigmas), &x, &y);

  /* ---------------- Oxide Thickness  -------------- */
  wc_table_add_entry_attach_units(table, gui, "Tox",
                               &(gui->text_tox), gui->line->units_lwht, &lwht_ug,
                               &(gui->line->subs->tox), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize oxide thickness and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_tox, gui, xb, y-1);

  /* ---------------- Oxide Dielectric Constant -------------- */
  wc_table_add_entry_no_units(table, gui, "Eox",
                              &(gui->text_eox),
                              &(gui->line->subs->eox), &x, &y);


  /* ---------------- new column -------------- */
  x = 5;
  y = 0;

  /* spacer between columns */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, x-1, x, 0, 1,
                   GTK_EXPAND|GTK_FILL, 0,
                   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

   /* ---------------- Characteristic impedance -------------- */
  wc_table_add_entry_fixed_units(table, gui, "Z0", "Ohms",
                                 &(gui->text_Ro),
                                 &(gui->line->Ro), &x, &y);


  /* ---------------- Electrical length -------------- */
  wc_table_add_entry_fixed_units(table, gui, _("Elec. Len."), _("Degrees"),
                                 &(gui->text_elen),
                                 &(gui->line->len), &x, &y);


  /* ---------------- Metal thickness -------------- */
  wc_table_add_entry_attach_units(table, gui, "Tmet",
                               &(gui->text_tmet), gui->line->units_lwht, &lwht_ug,
                               &(gui->line->subs->tmet), &x, &y);

  /* ---------------- Resistivity -------------- */
  wc_table_add_entry_new_units(table, gui, _("Rho"),
                               &(gui->text_rho), gui->line->units_rho, &ug,
                               &(gui->line->subs->rho), &x, &y);

  /* ---------------- Surface roughness -------------- */
  wc_table_add_entry_new_units(table, gui, _("Rough"),
                               &(gui->text_rough), gui->line->units_rough, &ug,
                               &(gui->line->subs->rough), &x, &y);

  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, _("Frequency"),
                               &(gui->text_freq), gui->line->units_freq, &ug,
                               &(gui->line->freq), &x, &y);

  gtk_widget_show (table);
}

static void outputs_init(ic_microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  wc_units_gui *ug=NULL, *depth_ug=NULL;

  int x = 0;
  int y = 0;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (6, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* ---------------- delay -------------- */
  wc_table_add_label_new_units(table, gui, _("Delay"),
                               &(gui->label_delay), gui->line->units_delay, &ug,
                               &(gui->line->delay), &x, &y);

  /* ---------------- total loss -------------- */
  wc_table_add_label_new_units(table, gui, _("Loss"),
                               &(gui->label_loss), gui->line->units_loss, &ug,
                               &(gui->line->loss), &x, &y);

  /* ---------------- total loss / length -------------- */
  wc_table_add_label_new_units(table, gui, _("Loss/Length"),
                               &(gui->label_losslen), gui->line->units_losslen, &ug,
                               &(gui->line->losslen), &x, &y);

  /* ----------------  Skin Depth in Metal -------------- */
  wc_table_add_label_new_units(table, gui, _("Metal Skin Depth"),
                               &(gui->label_met_depth), gui->line->units_depth, &depth_ug,
                               &(gui->line->met_skindepth), &x, &y);

  /* ----------------  Skin Depth in Substrate -------------- */
  wc_table_add_label_new_units(table, gui, _("Substrate Skin Depth"),
                               &(gui->label_subs_depth), gui->line->units_depth, &depth_ug,
                               &(gui->line->subs_skindepth), &x, &y);


  /* ----------------  Effective dielectric constant -------------- */
  wc_table_add_label_no_units(table, gui, _("Keff"),
                               &(gui->label_keff),
                               &x, &y);

  /* ---------------- new column -------------- */
  x = 4;
  y = 0;

  /* spacer between columns */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, x-1, x, 0, 1,
                   GTK_EXPAND|GTK_FILL, 0,
                   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  /* ---------------- L -------------- */
  wc_table_add_label_new_units(table, gui, "L",
                               &(gui->label_Lmis), gui->line->units_L, &ug,
                               &(gui->line->Lmis), &x, &y);

  /* ---------------- R -------------- */
  wc_table_add_label_new_units(table, gui, "R",
                               &(gui->label_Rmis), gui->line->units_R, &ug,
                               &(gui->line->Rmis), &x, &y);

  /* ---------------- C -------------- */
  wc_table_add_label_new_units(table, gui, "C",
                               &(gui->label_Cmis), gui->line->units_C, &ug,
                               &(gui->line->Cmis), &x, &y);

  /* ---------------- G -------------- */
  wc_table_add_label_new_units(table, gui, "G",
                               &(gui->label_Gmis), gui->line->units_G, &ug,
                               &(gui->line->Gmis), &x, &y);

  /* ---------------- G -------------- */
  wc_table_add_label_fixed_units(table, gui, "Z0", "Ohms",
                                 &(gui->label_z0),
                                 &x, &y);


  #ifdef notdef
  /* ---------------- Delay -------------- */
  text = gtk_label_new( "Delay" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 0, 1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_delay = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_delay,
		    1,2,0,1, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_delay);

  text = wc_units_menu_new(gui->line->units_delay, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_delay, &(gui->line->delay),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- Loss -------------- */
  text = gtk_label_new( "Loss" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 1, 2,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_loss = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_loss,
		    1,2,1,2, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_loss);

  text = wc_units_menu_new(gui->line->units_loss, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 1, 2,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_loss, &(gui->line->loss),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- loss/len -------------- */
  text = gtk_label_new( "Loss/Length" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 2, 3,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_losslen = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_losslen,
		    1,2,2,3, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_losslen);

  text = wc_units_menu_new(gui->line->units_losslen, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 2, 3,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_losslen, &(gui->line->losslen),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- metal depth -------------- */
  text = gtk_label_new( "Metal Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 3, 4,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_met_depth = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_met_depth,
		    1,2,3,4, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_met_depth);

  text = wc_units_menu_new(gui->line->units_depth, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 3, 4,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_met_depth, &(gui->line->met_skindepth),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- substrate skin depth -------------- */
  text = gtk_label_new( "Substrate Skin Depth" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 4, 5,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_subs_depth = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_subs_depth,
		    1,2,4,5, 0,0,WC_XPAD,WC_YPAD);
  gtk_widget_show(gui->label_subs_depth);

  text = gtk_label_new( "" );
  gtk_table_attach(GTK_TABLE(table), text, 2, 3, 4, 5,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  gtk_misc_set_alignment(GTK_MISC(text), 0, 0);
  gtk_widget_show(text);
  wc_units_attach_units_label(ug, text);

  /* ----------------  -------------- */
  text = gtk_label_new( "Keff" );
  gtk_table_attach(GTK_TABLE(table), text, 0, 1, 5, 6,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_keff = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_keff, 1, 2, 5, 6,
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_keff);

  /* ---------------- L -------------- */
  text = gtk_label_new( "L" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 0, 1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Lmis = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Lmis, 5, 6, 0, 1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Lmis);

  text = wc_units_menu_new(gui->line->units_L, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Lmis, &(gui->line->Lmis),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- R -------------- */
  text = gtk_label_new( "R" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 1, 2,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Rmis = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Rmis,
		    5,6,1,2,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Rmis);

  text = wc_units_menu_new(gui->line->units_R, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 1, 2,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Rmis, &(gui->line->Rmis),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- C -------------- */
  text = gtk_label_new( "C" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 2, 3,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Cmis = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Cmis, 5, 6, 2, 3,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Cmis);

  text = wc_units_menu_new(gui->line->units_C, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 2, 3,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Cmis, &(gui->line->Cmis),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- G -------------- */
  text = gtk_label_new( "G" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 3, 4,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_Gmis = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_Gmis,
		    5,6,3,4,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_Gmis);

  text = wc_units_menu_new(gui->line->units_G, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, 6, 7, 3, 4,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_Gmis, &(gui->line->Gmis),
			NULL, NULL, WC_FMT_G, 1);

  /* ---------------- Z0 -------------- */
  text = gtk_label_new( "Z0" );
  gtk_table_attach(GTK_TABLE(table), text, 4, 5, 4, 5,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(text);

  gui->label_z0 = gtk_label_new( WC_OUTPUT_TEXT );
  gtk_table_attach (GTK_TABLE(table), gui->label_z0,
		    5,6,4,5,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(gui->label_z0);

  /* ---------------- - -------------- */

  /* spacer */

  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);
#endif

  gtk_widget_show(table);

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
  const char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_w) );
  gui->line->w = atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  w = %g %s\n",
	  gui->line->w, gui->line->units_lwht->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l) );
  gui->line->l = atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  l = %g %s\n",
	  gui->line->l, gui->line->units_lwht->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_h) );
  gui->line->subs->h = atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  h = %g %s\n",
	  gui->line->subs->h, gui->line->units_lwht->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_es) );
  gui->line->subs->es = atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  es = %g\n",
	  gui->line->subs->es);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_sigmas) );
  gui->line->subs->sigmas = atof(vstr)*gui->line->units_sigmas->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  sigmas = %g %s\n",
	  gui->line->subs->sigmas, gui->line->units_sigmas->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tox) );
  gui->line->subs->tox = atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  tox = %g %s\n",
	  gui->line->subs->tox, gui->line->units_lwht->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_eox) );
  gui->line->subs->eox = atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  eox = %g\n",
	  gui->line->subs->eox);
#endif


  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Ro) );
  gui->line->Ro = atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  Ro = %g Ohms\n",
	  gui->line->Ro);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) );
  gui->line->len = atof(vstr);
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  elen = %g deg.\n",
	  gui->line->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) );
  gui->line->subs->tmet = atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  tmet = %g %s\n",
	  gui->line->subs->tmet, gui->line->units_lwht->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) );
  gui->line->subs->rho = atof(vstr)*gui->line->units_rho->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  rho = %g %s\n",
	  gui->line->subs->rho, gui->line->units_rho->name);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) );
  gui->line->subs->rough = atof(vstr)*gui->line->units_rough->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  rough = %g %s-RMS\n",
	  gui->line->subs->rough, gui->line->units_rough->name);
#endif


  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) );
  gui->line->freq = atof(vstr)*gui->line->units_freq->sf;
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  freq = %g %s\n",
	  gui->line->freq, gui->line->units_freq->name);
#endif

#ifdef DEBUG
  g_print("ic_microstrip_gui.c:calculate():  calculation with \"%s\"\n",
	  (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt=ic_microstrip_calc(gui->line, gui->line->freq);
  }
  else if( strcmp(data,"synthesize_w")==0) {
    rslt=ic_microstrip_syn(gui->line, gui->line->freq, IC_MLISYN_W);
  }
  else if( strcmp(data,"synthesize_h")==0) {
    rslt=ic_microstrip_syn(gui->line, gui->line->freq, IC_MLISYN_H);
  }
  else if( strcmp(data,"synthesize_tox")==0) {
    rslt=ic_microstrip_syn(gui->line, gui->line->freq, IC_MLISYN_TOX);
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
  if (rslt==0) {
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "");
    WC_WCALC(gui)->values_in_sync = TRUE;
  } else {
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "Values out of\nrange.");
    WC_WCALC(gui)->values_in_sync = FALSE;
  }

}

static void update_display(ic_microstrip_gui *gui)
{
  char str[80];

  /* the entries */

  /* ---------------- width  -------------- */
  sprintf(str, WC_FMT_G, gui->line->w/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_w), str );

  /* ---------------- length  -------------- */
  sprintf(str, WC_FMT_G, gui->line->l/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_l), str );

  /* ---------------- substrate height -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->h/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_h), str );

  /* ---------------- substrate permitivity -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->es);
  gtk_entry_set_text( GTK_ENTRY(gui->text_es), str );

  /* ---------------- substrate conductivity -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->sigmas/gui->line->units_sigmas->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_sigmas), str );

  /* ---------------- oxide thickness -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->tox/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tox), str );

  /* ---------------- oxide permitivity  -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->eox);
  gtk_entry_set_text( GTK_ENTRY(gui->text_eox), str );

  /* ---------------- Characteristic resistance -------------- */
  sprintf(str, WC_FMT_G, gui->line->Ro);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Ro), str );

  /* ---------------- Electrical length -------------- */
  sprintf(str, WC_FMT_G, gui->line->len);
  gtk_entry_set_text( GTK_ENTRY(gui->text_elen), str );

  /* ---------------- metal thickness -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->tmet/gui->line->units_lwht->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_tmet), str );

  /* ---------------- metal resistivity -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->rho);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );

  /* ---------------- metal surface roughness -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs->rough/gui->line->units_rough->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_rough), str );

  /* ---------------- frequency -------------- */
  sprintf(str, WC_FMT_G, gui->line->freq/gui->line->units_freq->sf);
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );


  /* the labels */

  /* ---------------- delay -------------- */
  sprintf(str, WC_FMT_G, gui->line->delay/gui->line->units_delay->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_delay), str );

  /* ---------------- loss -------------- */
  sprintf(str,WC_FMT_G, gui->line->loss/gui->line->units_loss->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_loss), str );

  /* ---------------- loss/len -------------- */
  sprintf(str, WC_FMT_G, gui->line->losslen/gui->line->units_losslen->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_losslen), str );

  /* ---------------- metal skindepth  -------------- */
  sprintf(str, WC_FMT_G, gui->line->met_skindepth/gui->line->units_lwht->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_met_depth), str );

  /* ---------------- substrate skindepth -------------- */
  sprintf(str, WC_FMT_G, gui->line->subs_skindepth/gui->line->units_lwht->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_subs_depth), str );

  /* ---------------- Keff -------------- */
  sprintf(str, WC_FMT_G, gui->line->keff);
  gtk_label_set_text( GTK_LABEL(gui->label_keff), str );

  /* ---------------- L -------------- */
  sprintf(str, WC_FMT_G, gui->line->Lmis/gui->line->units_L->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Lmis), str );

  /* ---------------- R -------------- */
  sprintf(str, WC_FMT_G, gui->line->Rmis/gui->line->units_R->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Rmis), str );

  /* ---------------- C -------------- */
  sprintf(str, WC_FMT_G, gui->line->Cmis/gui->line->units_C->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Cmis), str );

  /* ---------------- G -------------- */
  sprintf(str, WC_FMT_G, gui->line->Gmis/gui->line->units_G->sf);
  gtk_label_set_text( GTK_LABEL(gui->label_Gmis), str );

  /* ---------------- Ro + j Xo -------------- */
  sprintf(str, WC_FMT_G " + j" WC_FMT_G, gui->line->Ro, gui->line->Xo);
  gtk_label_set_text( GTK_LABEL(gui->label_z0), str );


}


static void tooltip_init(ic_microstrip_gui *gui)
{

  gtk_widget_set_tooltip_text( gui->text_w, "Width of metal trace" );
  gtk_widget_set_tooltip_text( gui->text_l, "Length of metal trace" );
  gtk_widget_set_tooltip_text( gui->text_h, "Substrate thickness" );
  gtk_widget_set_tooltip_text( gui->text_es, "Substrate relative"
		       " dielectric constant" );
  gtk_widget_set_tooltip_text( gui->text_sigmas, "Substrate conductivity" );
  gtk_widget_set_tooltip_text( gui->text_eox, "Oxide relative"
		       " dielectric constant" );
  gtk_widget_set_tooltip_text( gui->text_tox, "Oxide thickness" );

  gtk_widget_set_tooltip_text( gui->text_Ro, "Characteristic impedance" );
  gtk_widget_set_tooltip_text( gui->text_elen, "Electrical length" );
  gtk_widget_set_tooltip_text( gui->text_tmet, "Thickness of the metal trace" );
  gtk_widget_set_tooltip_text( gui->text_rho, "Metal resistivity" );
  gtk_widget_set_tooltip_text( gui->text_rough, "Metal surface roughness" );
  gtk_widget_set_tooltip_text( gui->text_freq, "Frequency of operation" );

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("ic_microstrip_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  ic_microstrip_save(WC_IC_MICROSTRIP_GUI(wcalc)->line,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
  ic_microstrip_gui *gui;
  ic_microstrip_line *l;

  gui = WC_IC_MICROSTRIP_GUI(wcalc);
  l = gui->line;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_ic_microstrip_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }

  list = wc_print_add_cairo(figure_ic_microstrip_render[0], figure_ic_microstrip_width[0],
                            figure_ic_microstrip_height[0], list);

  list = wc_print_add_double("Width of line (w)", l->w, l->units_lwht, list);
  list = wc_print_add_double("Length of line (l)", l->l, l->units_lwht, list);

  list = wc_print_add_double("Oxide thickness (t<sub>ox</sub>)", l->subs->tox, l->units_lwht, list);
  list = wc_print_add_double("Oxide relative dielectric contant ("
                             WC_SYM_EPSILON_LC "<sub>ox</sub>)", l->subs->eox, NULL, list);

  list = wc_print_add_double("Substrate thickness (h)", l->subs->h, l->units_lwht, list);
  list = wc_print_add_double("Substrate relative dielectric contant ("
                             WC_SYM_EPSILON_LC "<sub>s</sub>)",
                             l->subs->es, NULL, list);
  list = wc_print_add_double("Substrate conductivity ("
                             WC_SYM_SIGMA_LC "<sub>s</sub>)",
                             l->subs->sigmas, l->units_sigmas, list);

  list = wc_print_add_double("Metal thickness (t<sub>met</sub>)", l->subs->tmet, l->units_lwht, list);
  list = wc_print_add_double("Metal resistivity ("
                             WC_SYM_RHO_LC ")", l->subs->rho, l->units_rho, list);
  list = wc_print_add_double("Metal surface roughness (rough)", l->subs->rough,
                             l->units_rough, list);

  list = wc_print_add_double("Analysis Frequency", l->freq, l->units_freq, list);

  list = wc_print_add_double("Characteristic Impedance (real part)", l->Ro, NULL, list);
  list = wc_print_add_double("Characteristic Impedance (imaginary part)", l->Xo, NULL, list);
  list = wc_print_add_double("Electrical length", l->len, NULL, list);
  list = wc_print_add_double("Delay", l->delay, l->units_delay, list);


  list = wc_print_add_double("Metal skin depth", l->met_skindepth, l->units_depth, list);
  list = wc_print_add_double("Substrate skin depth", l->subs_skindepth, l->units_depth, list);

  list = wc_print_add_double("Total loss", l->loss, l->units_loss, list);
  list = wc_print_add_double("Total loss per length", l->losslen, l->units_losslen, list);
  list = wc_print_add_double("Total loss per wavelength", l->losslambda, NULL, list);

  list = wc_print_add_double("Incremental Inductance", l->Lmis, l->units_L, list);
  list = wc_print_add_double("Incremental Capacitance", l->Cmis, l->units_C, list);
  list = wc_print_add_double("Incremental Resistance", l->Rmis, l->units_R, list);
  list = wc_print_add_double("Incremental Conductance", l->Gmis, l->units_G, list);

  return list;
}

