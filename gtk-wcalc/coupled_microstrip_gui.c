/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2009,
 * 2021 Dan McMahill
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

#include "pixmaps/coupled_microstrip.xpm"
#include "pixmaps/figure_coupled_microstrip.h"
#include "coupled_microstrip.h"
#include "coupled_microstrip_gui.h"
#include "coupled_microstrip_loadsave.h"
#include "newprint.h"
#include "physconst.h"
#include "units.h"

#include "symbols.h"
#include "gtk-units.h"
#include "utils.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void use_z0k_pressed(GtkWidget *widget, gpointer data );
static void use_z0ez0o_pressed(GtkWidget *widget, gpointer data );

static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize( GtkWidget *w, gpointer data );
static void calculate( coupled_microstrip_gui *gui, GtkWidget *w, gpointer data );
static void update_display( coupled_microstrip_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(coupled_microstrip_gui *gui, GtkWidget *parent);
static void outputs_init(coupled_microstrip_gui *gui, GtkWidget *parent);
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

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  Wcalc_init(wcalc);

  /*
   * Supply info for this particular GUI
   */
  wcalc->init = coupled_microstrip_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;
  wcalc->model_name = name;
  wcalc->model_version = version;

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
  wc_picture_init(wcalc, picture_vbox, (const char **) coupled_microstrip);

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
  wc_units_gui *ug = NULL, *lwht_ug = NULL;

  /* position in the table */
  int y = 0;
  int x = 0;
  int xb = 3;

  /* the "z0/k vs z0e/z0o" radio button group */
  GSList *z0k_group;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Analysis/Synthesis Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (4, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  wc_table_add_button_wh(table, _("<-Synthesize"),
                         _("Calculate physical characteristics "
                           "from electrical parameters"),
                         synthesize, gui, xb, 1, 0, 4, NULL);

  wc_table_add_button_wh(table, _("Analyze->"),
                         _("Calculate electrical characteristics "
                           "from physical parameters"),
                         analyze, gui, xb+1, 1, 0, 4, NULL);


  /* ---------------- Width  -------------- */

  wc_table_add_entry_new_units(table, gui, _("Width (W)"),
                               &(gui->text_w), gui->line->units_lwst, &lwht_ug,
                               &(gui->line->w), &x, &y);


  /* ---------------- Spacing (gap)  -------------- */
  wc_table_add_entry_attach_units(table, gui, _("Spacing (S)"),
                               &(gui->text_s), gui->line->units_lwst, &lwht_ug,
                               &(gui->line->s), &x, &y);

  /* ---------------- Length  -------------- */
  wc_table_add_entry_attach_units(table, gui, _("Length (L)"),
                               &(gui->text_l), gui->line->units_lwst, &lwht_ug,
                               &(gui->line->l), &x, &y);

  /* ---------------- Height  -------------- */
  wc_table_add_entry_attach_units(table, gui, _("Height (H)"),
                               &(gui->text_h), gui->line->units_lwst, &lwht_ug,
                               &(gui->line->subs->h), &x, &y);

  /* ---------------- Dielectric Constant -------------- */
  wc_table_add_entry_no_units(table, gui, "Er",
                              &(gui->text_er),
                              &(gui->line->subs->er), &x, &y);

  /* ---------------- Loss Tangent -------------- */
  wc_table_add_entry_no_units(table, gui, _("Tand"),
                              &(gui->text_tand),
                              &(gui->line->subs->tand), &x, &y);

  /* ---------------- Metal thickness -------------- */
  wc_table_add_entry_attach_units(table, gui, "Tmet",
                               &(gui->text_tmet), gui->line->units_lwst, &lwht_ug,
                               &(gui->line->subs->tmet), &x, &y);

  /* ---------------- Resistivity -------------- */
  wc_table_add_entry_new_units(table, gui, _("Rho"),
                               &(gui->text_rho), gui->line->units_rho, &ug,
                               &(gui->line->subs->rho), &x, &y);

  /* ---------------- Surface roughness -------------- */
  wc_table_add_entry_new_units(table, gui, _("Rough"),
                               &(gui->text_rough), gui->line->units_rough, &ug,
                               &(gui->line->subs->rough), &x, &y);

  /* ---------------- new column -------------- */
  x = 5;
  y = 0;

  /* spacer between columns */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, x-1, x, 0, 1,
                   GTK_EXPAND|GTK_FILL, 0,
                   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

   /* ---------------- Z0/K -------------- */
  button = gtk_radio_button_new_with_label (NULL, _("Z0"));
  wc_table_add_wentry_fixed_units(table, gui, button, "Ohms",
                                  &(gui->text_z0),
                                  &(gui->line->z0), &x, &y);
  g_signal_connect( G_OBJECT( button ), "clicked",
                    G_CALLBACK(use_z0k_pressed),
                    gui);
  gui->button_z0k = button;
  gtk_widget_show (button);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE);
  z0k_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));

  wc_table_add_entry_no_units(table, gui, "k",
                              &(gui->text_k),
                              &(gui->line->k), &x, &y);


   /* ---------------- Z0e/Z0o -------------- */
  button = gtk_radio_button_new_with_label (z0k_group, _("Z0e"));
  wc_table_add_wentry_fixed_units(table, gui, button, "Ohms",
                                  &(gui->text_z0e),
                                  &(gui->line->z0e), &x, &y);
  g_signal_connect( G_OBJECT( button ), "clicked",
                    G_CALLBACK(use_z0ez0o_pressed),
                    gui);
  gui->button_z0ez0o = button;
  gtk_widget_show (button);

  wc_table_add_entry_fixed_units(table, gui, _("Z0o"), "Ohms",
                                 &(gui->text_z0o),
                                 &(gui->line->z0o), &x, &y);


  /* ---------------- Electrical length -------------- */
  wc_table_add_entry_fixed_units(table, gui, _("Elec. Len."), _("Degrees"),
                                 &(gui->text_elen),
                                 &(gui->line->len), &x, &y);


  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, _("Frequency"),
                               &(gui->text_freq), gui->line->units_freq, &ug,
                               &(gui->line->freq), &x, &y);


  /* ---------------- Update the radio buttons -------------- */

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

  /* ----------------  - -------------- */

  gtk_widget_show (table);
}

static void outputs_init(coupled_microstrip_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  int x = 0;
  int y = 0;
  int x2;
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
  wc_table_add_label_no_units(table, gui, "Keff-e",
                              &(gui->label_keffe),
                              &x, &y);

  /* ---------------- Keff-o -------------- */
  wc_table_add_label_no_units(table, gui, "Keff-o",
                              &(gui->label_keffo),
                              &x, &y);

  /* ---------------- total loss -------------- */
  wc_table_add_label_new_units(table, gui, _("Even Mode Loss"),
                               &(gui->label_loss_ev), gui->line->units_loss, &ug,
                               &(gui->line->loss_ev), &x, &y);
  wc_table_add_label_attach_units(table, gui, _("Odd Mode Loss"),
                                  &(gui->label_loss_od), gui->line->units_loss, &ug,
                                  &(gui->line->loss_odd), &x, &y);

  /* ---------------- total loss / length -------------- */
  wc_table_add_label_new_units(table, gui, _("Even Mode Loss/Length"),
                               &(gui->label_losslen_ev), gui->line->units_losslen, &ug,
                               &(gui->line->losslen_ev), &x, &y);
  wc_table_add_label_attach_units(table, gui, _("Odd Mode Loss/Length"),
                               &(gui->label_losslen_od), gui->line->units_losslen, &ug,
                               &(gui->line->losslen_odd), &x, &y);

  /* ----------------  Skin Depth -------------- */
  wc_table_add_label_new_units(table, gui, _("Skin Depth"),
                               &(gui->label_depth), gui->line->units_depth, &ug,
                               &(gui->line->skindepth), &x, &y);

  /* ---------------- delay -------------- */
  /*
  wc_table_add_label_new_units(table, gui, _("Delay"),
                               &(gui->label_delay), gui->line->units_delay, &ug,
                               &(gui->line->delay), &x, &y);
  */

  /* ---------------- new column -------------- */
  x = 4;
  x2 = x + 2;
  y = 0;

  /* spacer between columns */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, x-1, x, 0, 1,
                   GTK_EXPAND|GTK_FILL, 0,
                   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  /* ----------------  Even mode End correction -------------- */
  wc_table_add_label_new_units(table, gui, _("Even Mode Delta L"),
                               &(gui->label_deltal_ev), gui->line->units_deltal, &ug,
                               &(gui->line->deltale), &x2, &y);

  wc_table_add_label_attach_units(table, gui, _("Odd Mode Delta L"),
                               &(gui->label_deltal_od), gui->line->units_deltal, &ug,
                               &(gui->line->deltalo), &x2, &y);

  /* ---------------- L -------------- */
  wc_table_add_label_no_units(table, gui, "Lev",
                               &(gui->label_Lev),
                               &x, &y);
  y--;
  wc_table_add_label_new_units(table, gui, "Lodd",
                               &(gui->label_Lodd), gui->line->units_L, &ug,
                               &(gui->line->Lodd), &x2, &y);

  wc_units_attach_label(ug, gui->label_Lev, &(gui->line->Lev), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- R -------------- */
  wc_table_add_label_no_units(table, gui, "Rev",
                               &(gui->label_Rev),
                               &x, &y);
  y--;
  wc_table_add_label_new_units(table, gui, "Rodd",
                               &(gui->label_Rodd), gui->line->units_R, &ug,
                               &(gui->line->Rodd), &x2, &y);
  wc_units_attach_label(ug, gui->label_Rev, &(gui->line->Rev), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- C -------------- */
  wc_table_add_label_no_units(table, gui, "Cev",
                               &(gui->label_Cev),
                               &x, &y);
  y--;
  wc_table_add_label_new_units(table, gui, "Codd",
                               &(gui->label_Codd), gui->line->units_C, &ug,
                               &(gui->line->Codd), &x2, &y);
  wc_units_attach_label(ug, gui->label_Cev, &(gui->line->Cev), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- G -------------- */
  wc_table_add_label_no_units(table, gui, "G",
                               &(gui->label_Gev),
                               &x, &y);
  y--;
  wc_table_add_label_new_units(table, gui, "Godd",
                               &(gui->label_Godd), gui->line->units_G, &ug,
                               &(gui->line->Godd), &x2, &y);
  wc_units_attach_label(ug, gui->label_Gev, &(gui->line->Gev), NULL, NULL, WC_FMT_G, 1);


  gtk_widget_show(table);

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

  gtk_widget_set_tooltip_text( gui->text_w, "Width of each trace" );
  gtk_widget_set_tooltip_text( gui->text_l, "Length of the traces" );
  gtk_widget_set_tooltip_text( gui->text_h, "Total substrate thickness" );
  gtk_widget_set_tooltip_text( gui->text_er, "Substrate relative"
		       " dielectric constant" );
  gtk_widget_set_tooltip_text( gui->text_tand, "Substrate loss tangent" );

  gtk_widget_set_tooltip_text( gui->text_z0, "Characteristic impedance of the line" );
  gtk_widget_set_tooltip_text( gui->text_k, "Coupling coefficient of the line" );
  gtk_widget_set_tooltip_text( gui->text_z0e, "Even mode characteristic impedance of the line" );
  gtk_widget_set_tooltip_text( gui->text_z0o, "Odd mode characteristic impedance of the line" );
  gtk_widget_set_tooltip_text( gui->text_elen, "Electrical length" );
  gtk_widget_set_tooltip_text( gui->text_tmet, "Line metal thickness" );
  gtk_widget_set_tooltip_text( gui->text_rho, "Metal resistivity" );
  gtk_widget_set_tooltip_text( gui->text_rough, "Metal surface roughness" );
  gtk_widget_set_tooltip_text( gui->text_freq, "Frequency of operation" );

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
  coupled_microstrip_gui *gui;
  coupled_microstrip_line *l;

  gui = WC_COUPLED_MICROSTRIP_GUI(wcalc);
  l = gui->line;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_coupled_microstrip_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }

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

  return list;
}

