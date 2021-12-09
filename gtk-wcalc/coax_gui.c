/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2009,
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
#include "gtk-units.h"

#include "misc.h"
#include "symbols.h"
#include "units.h"

#include "pixmaps/coax.xpm"
#include "pixmaps/figure_coax.h"
#include "coax.h"
#include "coax_gui.h"
#include "coax_loadsave.h"
#include "newprint.h"
#include "physconst.h"
#include "utils.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static GList * dump_values(Wcalc *wcalc);

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
  Wcalc_init(wcalc);

  /*
   * Supply info for this particular GUI
   */
  wcalc->init = coax_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;

  wcalc->model_name = name;
  wcalc->model_version = version;

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
  wc_picture_init(wcalc, picture_vbox, (const char **) coax);

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

  wc_units_gui *ug = NULL;
  wc_units_gui *abct_ug = NULL;
  wc_units_gui *rho_ug = NULL;

  /* keeps track of current table position */
  int y = 0;
  int x = 0;
  int xb = 3;
  
  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Analysis/Synthesis Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (8, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);


  /* ---------------- a -------------- */
  wc_table_add_entry_new_units(table, gui, "a",
                               &(gui->text_a), gui->line->units_abct, &abct_ug,
                               &(gui->line->a), &x, &y);
  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize 'a' and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_a, gui, xb, y-1);

  /* ---------------- b -------------- */
  wc_table_add_entry_attach_units(table, gui, "b",
                                  &(gui->text_b), gui->line->units_abct, &abct_ug,
                                  &(gui->line->b), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize 'b' and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_b, gui, xb, y-1);

  /* ---------------- c -------------- */
  wc_table_add_entry_attach_units(table, gui, "c",
                                  &(gui->text_c), gui->line->units_abct, &abct_ug,
                                  &(gui->line->c), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize 'c' and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_c, gui, xb, y-1);

  /* ---------------- length -------------- */
  wc_table_add_entry_new_units(table, gui, _("Length"),
                                  &(gui->text_len), gui->line->units_len, &ug,
                                  &(gui->line->len), &x, &y);
  button = gtk_button_new_with_label (_("<-Synthesize"));
  wc_button_connect( button, synthesize_len, gui);
  gtk_table_attach(GTK_TABLE(table), button, xb, xb+1, y-1, y, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_set_sensitive (button, FALSE);


  /* ---------------- er -------------- */
  wc_table_add_entry_no_units(table, gui, _("Er"),
                                  &(gui->text_er),
                                  &(gui->line->er), &x, &y);
  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize relative dielectric constant and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_er, gui, xb, y-1);

  /* ---------------- tand -------------- */
  wc_table_add_entry_no_units(table, gui, _("Tand"),
                                  &(gui->text_tand),
                                  &(gui->line->tand), &x, &y);

  /* ---------------- Max Electric Field -------------- */
  wc_table_add_entry_new_units(table, gui, _("Emax"),
                               &(gui->text_emax), gui->line->units_emax, &ug,
                               &(gui->line->emax), &x, &y);

  wc_table_add_button(table, _("Analyze->"),
                      _("Calculate electrical characteristics "
                        "from physical parameters"),
                      analyze, gui, xb, y-1);

  
  /* ---------------- t -------------- */
  wc_table_add_entry_attach_units(table, gui, "t",
                                  &(gui->text_tshield), gui->line->units_abct, &abct_ug,
                                  &(gui->line->tshield), &x, &y);


  /* new column */
  x = 5;
  y = 0;

  /* spacer between columns */
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, x-1, x, 0, 1,
                   GTK_EXPAND|GTK_FILL, 0,
                   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  /* Text labels */

  /* ---------------- Characteristic impedance -------------- */
  wc_table_add_entry_fixed_units(table, gui, "Z0", "Ohms",
                                 &(gui->text_z0), 
                                  &(gui->line->z0), &x, &y);

  
  /* ---------------- Electrical length -------------- */

  wc_table_add_entry_fixed_units(table, gui, "Elen", _("Degrees"),
                                 &(gui->text_elen), 
                                  &(gui->line->elen), &x, &y);

  /* ---------------- TE10 Cutoff -------------- */
  wc_table_add_entry_new_units(table, gui, _("TE10 Cutoff"),
                               &(gui->text_fc), gui->line->units_fc, &ug,
                               &(gui->line->fc), &x, &y);
  
  gtk_widget_set_sensitive(gui->text_fc,FALSE);
  wc_units_attach_entry(ug, gui->text_fc, &(gui->line->fc), NULL, NULL, WC_FMT_G, 1);

  /* ---------------- Frequency label/entry/units menu -------------- */
  wc_table_add_entry_new_units(table, gui, _("Frequency"),
                               &(gui->text_freq), gui->line->units_freq, &ug,
                               &(gui->line->freq), &x, &y);

  /* ---------------- RHO_a label/entry/units menu -------------- */
  wc_table_add_entry_new_units(table, gui, _("RHO_a"),
                               &(gui->text_rho_a), gui->line->units_rho, &rho_ug,
                               &(gui->line->rho_a), &x, &y);

  /* ---------------- RHO_b label/entry/units menu -------------- */
  wc_table_add_entry_attach_units(table, gui, _("RHO_b"),
                               &(gui->text_rho_b), gui->line->units_rho, &rho_ug,
                               &(gui->line->rho_b), &x, &y);


  gtk_widget_show (table);
}

static void outputs_init(coax_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  wc_units_gui *ug = NULL, *losslen_ug = NULL;

  int y = 0;
  int x = 0;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Output Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (5, 7, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);

  /* ---------------- delay -------------- */
  wc_table_add_label_new_units(table, gui, _("delay"),
                               &(gui->label_delay), gui->line->units_delay, &ug,
                               &(gui->line->delay), &x, &y);
  
  /* ---------------- total loss -------------- */
  wc_table_add_label_new_units(table, gui, _("loss"),
                               &(gui->label_loss), gui->line->units_loss, &ug,
                               &(gui->line->loss), &x, &y);

  /* ---------------- total loss / length -------------- */
  wc_table_add_label_new_units(table, gui, _("loss/length"),
                               &(gui->label_losslen), gui->line->units_losslen, &losslen_ug,
                               &(gui->line->losslen), &x, &y);

  /* ---------------- conductor loss -------------- */
  wc_table_add_label_attach_units(table, gui, _("Conductor loss"),
                               &(gui->label_closs), gui->line->units_losslen, &losslen_ug,
                               &(gui->line->alpha_c), &x, &y);

  /* ---------------- dielectric loss -------------- */
  wc_table_add_label_attach_units(table, gui, _("Conductor loss"),
                               &(gui->label_dloss), gui->line->units_losslen, &losslen_ug,
                               &(gui->line->alpha_d), &x, &y);
  
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
                               &(gui->label_L), gui->line->units_L, &ug,
                               &(gui->line->L), &x, &y);

  /* ---------------- R -------------- */
  wc_table_add_label_new_units(table, gui, "R",
                               &(gui->label_R), gui->line->units_R, &ug,
                               &(gui->line->R), &x, &y);

  /* ---------------- C -------------- */
  wc_table_add_label_new_units(table, gui, "C",
                               &(gui->label_C), gui->line->units_C, &ug,
                               &(gui->line->C), &x, &y);

  /* ---------------- G -------------- */
  wc_table_add_label_new_units(table, gui, "G",
                               &(gui->label_G), gui->line->units_G, &ug,
                               &(gui->line->G), &x, &y);


  gtk_widget_show(table);

}

/* FIXME -- no reason we couldn't pass in what we wanted via a field
   in data and collapse all of these */
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

  gtk_widget_set_tooltip_text( gui->text_a, _("Radius of inner conductor") );
  gtk_widget_set_tooltip_text( gui->text_b, _("Inner radius of outer conductor") );
  gtk_widget_set_tooltip_text( gui->text_c, _("Offset of inner conductor") );
  gtk_widget_set_tooltip_text( gui->text_len, _("Physical length of line") );
  gtk_widget_set_tooltip_text( gui->text_er, _("Dielectric relative permitivitty") );
  gtk_widget_set_tooltip_text( gui->text_tand, _("Dielectric loss tangent") );
  gtk_widget_set_tooltip_text( gui->text_emax, _("Breakdown field strength "
		       "in the dielectric") );
  gtk_widget_set_tooltip_text( gui->text_tshield, _("Thickness of outer conductor") );

  gtk_widget_set_tooltip_text( gui->text_z0, _("Characteristic impedance") );
  gtk_widget_set_tooltip_text( gui->text_elen, _("Electrical length of line") );
  gtk_widget_set_tooltip_text( gui->text_freq, _("Frequency of operation") );
  gtk_widget_set_tooltip_text( gui->text_fc, _("Cutoff frequency for"
		       " TE10 mode") );
  gtk_widget_set_tooltip_text( gui->text_rho_a, _("Resistivity of inner conductor") );
  gtk_widget_set_tooltip_text( gui->text_rho_b, _("Resistivity of outer conductor") );

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("coax_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
	  wcalc,fp,name);
#endif
  coax_save(WC_COAX_GUI(wcalc)->line,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
  coax_gui *gui;
  coax_line *l;

  gui = WC_COAX_GUI(wcalc);
  l = gui->line;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_coax_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }

  list = wc_print_add_cairo(figure_coax_render[0], figure_coax_width[0],
                            figure_coax_height[0], list);

  list = wc_print_add_double("Radius of inner conductor (a)", l->a, l->units_abct, list);
  list = wc_print_add_double("Diameter of inner conductor (2a)", 2.0*l->a, l->units_abct, list);
  list = wc_print_add_double("Inside radius of outer conductor (b)", l->b, l->units_abct, list);
  list = wc_print_add_double("Inside diameter of outer conductor (2b)", 2.0*l->b, l->units_abct, list);
  list = wc_print_add_double("Center conductor offset (c)", l->c, l->units_abct, list);
  list = wc_print_add_double("Shield thickness (t)", l->tshield, l->units_abct, list);
  list = wc_print_add_double("Line physical length (len)", l->len, l->units_abct, list);

  list = wc_print_add_double("Center conductor resistivity (" WC_SYM_RHO_LC "<sub>a</sub>)",
                             l->rho_a, l->units_rho, list);

  list = wc_print_add_double("Shield conductor resistivity (" WC_SYM_RHO_LC "<sub>b</sub>)",
                             l->rho_b, l->units_rho, list);
  list = wc_print_add_double("Relative dielectric contant (" WC_SYM_EPSILON_LC "<sub>r</sub>)", l->er, NULL, list);
  list = wc_print_add_double("Dielectric loss tangent (tan" WC_SYM_DELTA_UC ")", l->tand, NULL, list);

  list = wc_print_add_double("Dielectric breakdown field strength (E<sub>max</sub>)",
                             l->emax, l->units_emax, list);

  list = wc_print_add_double("Analysis Frequency", l->freq, l->units_freq, list);

  list = wc_print_add_double("Characteristic Impedance", l->z0, NULL, list);
  list = wc_print_add_double("Electrical length", l->elen, l->units_len, list);
  list = wc_print_add_double("Delay", l->delay, l->units_delay, list);

  list = wc_print_add_double("TE<sub>10</sub> mode cutoff frequency", l->fc, l->units_fc, list);

  list = wc_print_add_double("Conductor loss", l->alpha_c, l->units_loss, list);
  list = wc_print_add_double("Dielectric loss", l->alpha_d, l->units_loss, list);
  list = wc_print_add_double("Total loss", l->loss, l->units_loss, list);
  list = wc_print_add_double("Total loss per length", l->losslen, l->units_losslen, list);

  list = wc_print_add_double("Incremental Inductance", l->L, l->units_L, list);
  list = wc_print_add_double("Incremental Capacitance", l->C, l->units_C, list);
  list = wc_print_add_double("Incremental Resistance", l->R, l->units_R, list);
  list = wc_print_add_double("Incremental Conductance", l->G, l->units_G, list);

  return list;
}

