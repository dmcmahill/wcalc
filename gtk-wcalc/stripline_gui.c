/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2005, 2006, 2009,
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

#include "pixmaps/stripline.xpm"
#include "pixmaps/figure_stripline.h"
#include "newprint.h"
#include "stripline.h"
#include "stripline_gui.h"
#include "stripline_loadsave.h"
#include "physconst.h"
#include "units.h"

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
static void synthesize_er( GtkWidget *w, gpointer data );
static void calculate( stripline_gui *gui, GtkWidget *w, gpointer data );
static void update_display( stripline_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(stripline_gui *gui, GtkWidget *parent);
static void outputs_init(stripline_gui *gui, GtkWidget *parent);
static void tooltip_init(stripline_gui *gui);

static char *name="Stripline Analysis/Synthesis";
static char *version="v0.1";
/*
 * Public Functions
 */

stripline_gui *stripline_gui_new(void)
{
  stripline_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (stripline_gui *) malloc(sizeof(stripline_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,"stripline_gui_new: malloc() failed\n");
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
  wcalc->init = stripline_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;

  wcalc->model_name = name;
  wcalc->model_version = version;

  /*
   * Initialize the model dependent portions
   */

  /* create the stripline line which will be used */
  new_gui->line = stripline_line_new();
  new_gui->phys_units_text = NULL;

  return new_gui;
}

void stripline_gui_init(Wcalc *wcalc, GtkWidget *main_vbox,FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  stripline_gui *gui;

  gui = WC_STRIPLINE_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print("stripline_gui.c:stripline_gui_init():  calling stripline_load\n");
#endif
    if (stripline_load(gui->line,fp) != 0) {
      alert("ERROR:  Could not load the stripline\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n");
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print("stripline_gui.c:stripline_gui_init():  new using defaults\n");
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
  wc_picture_init(wcalc, picture_vbox, (const char **) stripline);

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

static void values_init(stripline_gui *gui, GtkWidget *parent)
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

  /* ---------------- Height  -------------- */
  wc_table_add_entry_attach_units(table, gui, _("Height (H)"),
                               &(gui->text_h), gui->line->units_lwht, &lwht_ug,
                               &(gui->line->subs->h), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize substrate dielectric thickness and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_h, gui, xb, y-1);

  /* ---------------- Dielectric Constant -------------- */
  wc_table_add_entry_no_units(table, gui, "Er",
                              &(gui->text_er),
                              &(gui->line->subs->er), &x, &y);

  wc_table_add_button(table, _("<-Synthesize"),
                      _("Synthesize substrate dielectric constant and physical length to "
                        "obtain the requested characteristic "
                        "impedance and electrical length."),
                      synthesize_er, gui, xb, y-1);

  /* ---------------- Analyze Button  -------------- */
  wc_table_add_button_wh(table, _("Analyze->"),
                         _("Calculate electrical characteristics "
                           "from physical parameters"),
                         analyze, gui, xb+1, 1, 0, y, NULL);


  /* ---------------- Loss Tangent -------------- */
  wc_table_add_entry_no_units(table, gui, _("Tand"),
                              &(gui->text_tand),
                              &(gui->line->subs->tand), &x, &y);

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
                                 &(gui->text_z0),
                                 &(gui->line->z0), &x, &y);


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

static void outputs_init(stripline_gui *gui, GtkWidget *parent)
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

  /* ----------------  Skin Depth -------------- */
  wc_table_add_label_new_units(table, gui, _("Skin Depth"),
                               &(gui->label_depth), gui->line->units_depth, &ug,
                               &(gui->line->skindepth), &x, &y);

  /* ----------------  End correction -------------- */
  wc_table_add_label_new_units(table, gui, _("Delta L"),
                               &(gui->label_deltal), gui->line->units_deltal, &ug,
                               &(gui->line->deltal), &x, &y);

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
                               &(gui->label_Ls), gui->line->units_L, &ug,
                               &(gui->line->Ls), &x, &y);

  /* ---------------- R -------------- */
  wc_table_add_label_new_units(table, gui, "R",
                               &(gui->label_Rs), gui->line->units_R, &ug,
                               &(gui->line->Rs), &x, &y);

  /* ---------------- C -------------- */
  wc_table_add_label_new_units(table, gui, "C",
                               &(gui->label_Cp), gui->line->units_C, &ug,
                               &(gui->line->Cs), &x, &y);

  /* ---------------- G -------------- */
  wc_table_add_label_new_units(table, gui, "G",
                               &(gui->label_Gp), gui->line->units_G, &ug,
                               &(gui->line->Gs), &x, &y);


  gtk_widget_show(table);

}


static void analyze( GtkWidget *w, gpointer data )
{
  calculate(WC_STRIPLINE_GUI(data), w, "analyze");
}

static void synthesize_w( GtkWidget *w, gpointer data )
{
  calculate(WC_STRIPLINE_GUI(data), w, "synthesize_w");
}

static void synthesize_h( GtkWidget *w, gpointer data )
{
  calculate(WC_STRIPLINE_GUI(data), w, "synthesize_h");
}

static void synthesize_er( GtkWidget *w, gpointer data )
{
  calculate(WC_STRIPLINE_GUI(data), w, "synthesize_er");
}


static void calculate( stripline_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_w) );
  gui->line->w=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  w = %g\n", gui->line->w);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l) );
  gui->line->l=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  l = %g\n", gui->line->l);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_h) );
  gui->line->subs->h=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  h = %g\n", gui->line->subs->h);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_er) );
  gui->line->subs->er=atof(vstr);
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  er = %g\n", gui->line->subs->er);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tand) );
  gui->line->subs->tand=atof(vstr);
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  tand = %g\n", gui->line->subs->tand);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_z0) );
  gui->line->Ro=atof(vstr);
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  z0 = %g\n", gui->line->z0);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_elen) );
  gui->line->len=atof(vstr);
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  elen = %g\n", gui->line->len);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_tmet) );
  gui->line->subs->tmet=atof(vstr)*gui->line->units_lwht->sf;
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  tmet = %g\n", gui->line->subs->tmet);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) );
  gui->line->subs->rho=atof(vstr)*gui->line->units_rho->sf;
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  rho = %g\n", gui->line->subs->rho);
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rough) );
  gui->line->subs->rough=atof(vstr)*gui->line->units_rough->sf;
#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  rough = %g\n",
	  gui->line->subs->rough);
#endif


  /* get the frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) );
  gui->line->freq=atof(vstr)*gui->line->units_freq->sf;

#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  freq = %g\n",
	  gui->line->freq);
#endif

#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  calculation with \"%s\"\n",
	  (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt=stripline_calc(gui->line, gui->line->freq);
  }
  else if( strcmp(data,"synthesize_w")==0) {
    rslt=stripline_syn(gui->line, gui->line->freq, SLISYN_W);
  }
  else if( strcmp(data,"synthesize_h")==0) {
    rslt=stripline_syn(gui->line, gui->line->freq, SLISYN_H);
  }
  else if( strcmp(data,"synthesize_er")==0) {
    rslt=stripline_syn(gui->line, gui->line->freq, SLISYN_ER);
  }
  else{
    fprintf(stderr,"stripline_gui.c:  error in stripline callback\n"
	    "Please report this bug and how you triggered it\n");
    exit(1);
  }

#ifdef DEBUG
  g_print("stripline_gui.c:calculate():  finished calculation\n");
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

static void update_display(stripline_gui *gui)
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


static void tooltip_init(stripline_gui *gui)
{

  gtk_widget_set_tooltip_text( gui->text_w, "Width of stripline" );
  gtk_widget_set_tooltip_text( gui->text_l, "Length of stripline" );
  gtk_widget_set_tooltip_text( gui->text_h, "Total substrate thickness" );
  gtk_widget_set_tooltip_text( gui->text_er, "Substrate relative"
		       " dielectric constant" );
  gtk_widget_set_tooltip_text( gui->text_tand, "Substrate loss tangent" );

  gtk_widget_set_tooltip_text( gui->text_z0, "Characteristic impedance" );
  gtk_widget_set_tooltip_text( gui->text_elen, "Electrical length" );
  gtk_widget_set_tooltip_text( gui->text_tmet, "Line metal thickness" );
  gtk_widget_set_tooltip_text( gui->text_rho, "Metal resistivity" );
  gtk_widget_set_tooltip_text( gui->text_rough, "Metal surface roughness" );
  gtk_widget_set_tooltip_text( gui->text_freq, "Frequency of operation" );

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print("stripline_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n",
	  wcalc,fp,name);
#endif
  stripline_save(WC_STRIPLINE_GUI(wcalc)->line,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
  stripline_gui *gui;
  stripline_line *l;

  gui = WC_STRIPLINE_GUI(wcalc);
  l = gui->line;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_stripline_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }

  list = wc_print_add_cairo(figure_stripline_render[0], figure_stripline_width[0],
                            figure_stripline_height[0], list);

  list = wc_print_add_double("Width of line (W)", l->w, l->units_lwht, list);
  list = wc_print_add_double("Length of line (L)", l->l, l->units_lwht, list);

  list = wc_print_add_double("Dielectric thickness (H)", l->subs->h, l->units_lwht, list);
  list = wc_print_add_double("Relative dielectric contant ("
                             WC_SYM_EPSILON_LC "<sub>r</sub>)", l->subs->er, NULL, list);
  list = wc_print_add_double("Dielectric loss tangent (tan"
                             WC_SYM_DELTA_LC ")", l->subs->tand, NULL, list);
  list = wc_print_add_double("Metal thickness (T<sub>met</sub>)", l->subs->tmet, l->units_lwht, list);
  list = wc_print_add_double("Metal resistivity ("
                             WC_SYM_RHO_LC ")", l->subs->rho, l->units_rho, list);
  list = wc_print_add_double("Metal surface roughness (rough)", l->subs->rough,
                             l->units_rough, list);

  list = wc_print_add_double("Analysis Frequency", l->freq, l->units_freq, list);

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

  list = wc_print_add_double("Incremental Inductance", l->Ls, l->units_L, list);
  list = wc_print_add_double("Incremental Capacitance", l->Cs, l->units_C, list);
  list = wc_print_add_double("Incremental Resistance", l->Rs, l->units_R, list);
  list = wc_print_add_double("Incremental Conductance", l->Gs, l->units_G, list);

  return list;
}

