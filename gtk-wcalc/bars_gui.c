/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009,
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

#include <assert.h>

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
#include "units.h"

#include "pixmaps/bars_fig.xpm"
#include "pixmaps/figure_bars_fig.h"
#include "bars.h"
#include "bars_gui.h"
#include "bars_loadsave.h"
#include "newprint.h"
#include "physconst.h"

#include "utils.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void calculate( bars_gui *gui, GtkWidget *w, gpointer data );
static void update_display( bars_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(bars_gui *gui, GtkWidget *parent);
static void outputs_init(bars_gui *gui, GtkWidget *parent);
static void tooltip_init(bars_gui *gui);


static char *name="Rectangular Bar Inductance Analysis";
static char *version="v0.1";
/*
 * Public Functions
 */

bars_gui *bars_gui_new(void)
{
  bars_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (bars_gui *) malloc(sizeof(bars_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,_("bars_gui_new: malloc() failed\n"));
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

  wcalc->init = bars_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;

  wcalc->model_name = name;
  wcalc->model_version = version;

  /*
   * Initialize the model dependent portions
   */

  /* create the bars b which will be used */
  new_gui->b = bars_new();

  return new_gui;
}

void bars_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  bars_gui *gui;

  gui = WC_BARS_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print(_("bars_gui.c:bars_gui_init():  calling bars_load\n"));
#endif
    if (bars_load(gui->b,fp) != 0) {
      alert(_("ERROR:  Could not load the bars b\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n"));
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print(_("bars_gui.c:bars_gui_init():  new using defaults\n"));
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
  wc_picture_init(wcalc, picture_vbox, (const char **) bars_fig);

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


static void values_init(bars_gui *gui, GtkWidget *parent)
{
  GtkWidget *frame;
  GtkWidget *table;

  wc_units_gui *xy_ug = NULL, *freq_ug = NULL;

  /* keeps track of current table position */
  int y = 0;
  int x = 0;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), _("Analysis/Synthesis Values") );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  /* Setup the values_vbox contents */
  table = gtk_table_new (8, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);


  /* ---------------- Bar 1, width, height, length  -------------- */
  wc_table_add_entry_new_units(table, gui, "Bar 1 Width (a)",
			       &(gui->text_a), gui->b->units_xy, &xy_ug,
			       &(gui->b->a), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Bar 1 Thickness (b)",
				  &(gui->text_b), gui->b->units_xy, &xy_ug,
				  &(gui->b->b), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Bar 1 Length (l1)",
				  &(gui->text_l1), gui->b->units_xy, &xy_ug,
				  &(gui->b->l1), &x, &y);

  y++;
  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, "Frequency",
			       &(gui->text_freq), gui->b->units_freq, &freq_ug,
			       &(gui->b->freq), &x, &y);


  /* ---------------- Analyze Button -------------- */
  wc_table_add_button(table, _("Analyze"),
                      _("Calculate electrical characteristics "
                        "from physical parameters"),
                      analyze, gui, x, y);
  y++;

  /* Column #2 */
  x += 4;
  y = 0;

  /* ---------------- Bar 2, width, height, length  -------------- */
  wc_table_add_entry_attach_units(table, gui, "Bar 2 Width (d)",
				  &(gui->text_d), gui->b->units_xy, &xy_ug,
				  &(gui->b->d), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Bar 2 Thickness (c)",
				  &(gui->text_c), gui->b->units_xy, &xy_ug,
				  &(gui->b->c), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Bar 2 Length (l2)",
				  &(gui->text_l2), gui->b->units_xy, &xy_ug,
				  &(gui->b->l2), &x, &y);

  /* ---------------- Bar 2, position  -------------- */
  wc_table_add_entry_attach_units(table, gui, "X offset (E)",
				  &(gui->text_E), gui->b->units_xy, &xy_ug,
				  &(gui->b->E), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Y offset (P)",
				  &(gui->text_P), gui->b->units_xy, &xy_ug,
				  &(gui->b->P), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Z offset (l3)",
				  &(gui->text_l3), gui->b->units_xy, &xy_ug,
				  &(gui->b->l3), &x, &y);

  gtk_widget_show(table);

}


static void outputs_init(bars_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  int x=0;
  int y=0;
  wc_units_gui *ug = NULL;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (5, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);


  wc_table_add_label_new_units(table, gui, "L1",
			       &(gui->label_L1), gui->b->units_L, &ug,
			       &(gui->b->L1), &x, &y);

  wc_table_add_label_attach_units(table, gui, "L2",
				  &(gui->label_L2), gui->b->units_L, &ug,
				  &(gui->b->L2), &x, &y);

  wc_table_add_label_attach_units(table, gui, "M",
				  &(gui->label_M), gui->b->units_L, &ug,
				  &(gui->b->M), &x, &y);

  wc_table_add_label_no_units(table, gui, "k",
				  &(gui->label_k), &x, &y);

  /* spacer */

  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gtk_widget_show(table);

}


static void analyze( GtkWidget *w, gpointer data )
{
  calculate(WC_BARS_GUI(data), w, "analyze");
}


static void calculate( bars_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  /* Bar #1 */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_a) );
  gui->b->a=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_b) );
  gui->b->b=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l1) );
  gui->b->l1=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  /* Bar #2 */

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_d) );
  gui->b->d=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_c) );
  gui->b->c=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l2) );
  gui->b->l2=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  /* Bar #2 position */

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_E) );
  gui->b->E=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_P) );
  gui->b->P=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l3) );
  gui->b->l3=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  /* Frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) );
  gui->b->freq=atof(vstr)*wc_units_to_sf(gui->b->units_freq);

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
      rslt = bars_calc(gui->b, gui->b->freq);
  } else {
      g_print(_("error in bars callback.  data=\"%s\""),(char *)data);
      exit(1);
  }

#ifdef DEBUG
  g_print(_("bars_gui.c:calculate():  finished calculation\n"));
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

static void update_display(bars_gui *gui)
{
  char str[80];

  /* ---------------- a -------------- */
  sprintf(str,WC_FMT_G,gui->b->a/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_a), str );

  /* ---------------- b -------------- */
  sprintf(str,WC_FMT_G,gui->b->b/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_b), str );

  /* ---------------- l1 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l1/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l1), str );


  /* ---------------- d -------------- */
  sprintf(str,WC_FMT_G,gui->b->d/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_d), str );

  /* ---------------- c -------------- */
  sprintf(str,WC_FMT_G,gui->b->c/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_c), str );

  /* ---------------- l2 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l2/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l2), str );


  /* ---------------- E -------------- */
  sprintf(str,WC_FMT_G,gui->b->E/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_E), str );

  /* ---------------- P -------------- */
  sprintf(str,WC_FMT_G,gui->b->P/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_P), str );

  /* ---------------- l3 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l3/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l3), str );

  /* ---------------- freq -------------- */
  sprintf(str,WC_FMT_G,gui->b->freq/wc_units_to_sf(gui->b->units_freq));
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );

  /* ---------------- L1 -------------- */
  sprintf(str,WC_FMT_G,gui->b->L1/wc_units_to_sf(gui->b->units_L));
  gtk_label_set_text( GTK_LABEL(gui->label_L1), str );

  /* ---------------- L2 -------------- */
  sprintf(str,WC_FMT_G,gui->b->L2/wc_units_to_sf(gui->b->units_L));
  gtk_label_set_text( GTK_LABEL(gui->label_L2), str );

  /* ---------------- M -------------- */
  sprintf(str,WC_FMT_G,gui->b->M/wc_units_to_sf(gui->b->units_L));
  gtk_label_set_text( GTK_LABEL(gui->label_M), str );

  /* ---------------- k -------------- */
  sprintf(str,WC_FMT_G,gui->b->k);
  gtk_label_set_text( GTK_LABEL(gui->label_k), str );


}


static void tooltip_init(bars_gui *gui)
{

  gtk_widget_set_tooltip_text( gui->text_a, _("Width of bar #1"));

  gtk_widget_set_tooltip_text( gui->text_b, _("Thickness of bar #1"));
  gtk_widget_set_tooltip_text( gui->text_l1, _("Length of bar #1"));

  gtk_widget_set_tooltip_text( gui->text_d, _("Width of bar #2"));
  gtk_widget_set_tooltip_text( gui->text_c, _("Thickness of bar #2"));
  gtk_widget_set_tooltip_text( gui->text_l2, _("Length of bar #2"));

  gtk_widget_set_tooltip_text( gui->text_E, _("Offset position of bar #2 in the width direction"));
  gtk_widget_set_tooltip_text( gui->text_P, _("Offset position of bar #2 in the vertical direction"));
  gtk_widget_set_tooltip_text( gui->text_l3, _("Offset position of bar #2 in the length direction"));

  gtk_widget_set_tooltip_text( gui->text_freq, _("Frequency of operation"));

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("bars_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
	  wcalc,fp,name);
#endif
  bars_save(WC_BARS_GUI(wcalc)->b,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
  bars_gui *gui;
  bars * b;

  gui = WC_BARS_GUI(wcalc);
  b = gui->b;

  if( list == NULL ) {
    figure_bars_fig_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }

  list = wc_print_add_cairo(figure_bars_fig_render[0], figure_bars_fig_width[0],
                            figure_bars_fig_height[0], list);

  list = wc_print_add_double("Width of bar #1 (a)", b->a, b->units_xy, list);
  list = wc_print_add_double("Thickness of bar #1 (b)", b->b, b->units_xy, list);
  list = wc_print_add_double("Length of bar #1 (l1)", b->l1, b->units_xy, list);

  list = wc_print_add_double("Width of bar #2 (d)", b->d, b->units_xy, list);
  list = wc_print_add_double("Thickness of bar #2 (c)", b->c, b->units_xy, list);
  list = wc_print_add_double("Length of bar #2 (l2)", b->l2, b->units_xy, list);


  list = wc_print_add_double("Bar #2 position in the width direction (E)", b->E, b->units_xy, list);
  list = wc_print_add_double("Bar #2 position in the thickness direction (P)", b->P, b->units_xy, list);
  list = wc_print_add_double("Bar #2 position in the length direction (l3)", b->l3, b->units_xy, list);

  list = wc_print_add_double("Bar #1 Self Inductance (L1)", b->L1, b->units_L, list);
  list = wc_print_add_double("Bar #2 Self Inductance (L2)", b->L2, b->units_L, list);
  list = wc_print_add_double("Mutual Inductance (M)", b->M, b->units_L, list);
  list = wc_print_add_double("Coupling Coefficient (k)", b->k, NULL, list);

  return list;
}


