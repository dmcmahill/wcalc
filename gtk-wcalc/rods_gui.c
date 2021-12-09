/*
 * Copyright (C) 2020, 2021 Dan McMahill
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

#include "pixmaps/rods_fig.xpm"
#include "pixmaps/figure_rods_fig.h"
#include "rods.h"
#include "rods_gui.h"
#include "rods_loadsave.h"
#include "newprint.h"
#include "physconst.h"

#include "utils.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void calculate( rods_gui *gui, GtkWidget *w, gpointer data );
static void update_display( rods_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(rods_gui *gui, GtkWidget *parent);
static void outputs_init(rods_gui *gui, GtkWidget *parent);
static void tooltip_init(rods_gui *gui);


static char *name="Circular Parallel Wires Inductance Analysis";
static char *version="v1.0";
/*
 * Public Functions
 */

rods_gui *rods_gui_new(void)
{
  rods_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (rods_gui *) malloc(sizeof(rods_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,_("rods_gui_new: malloc() failed\n"));
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
  wcalc->init = rods_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;

  wcalc->model_name = name;
  wcalc->model_version = version;

  /*
   * Initialize the model dependent portions
   */

  /* create the rods b which will be used */
  new_gui->b = rods_new();

  return new_gui;
}

void rods_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *outputs_vbox;
  GtkWidget *picture_vbox;

  rods_gui *gui;

  gui = WC_RODS_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print(_("rods_gui.c:rods_gui_init():  calling rods_load\n"));
#endif
    if (rods_load(gui->b,fp) != 0) {
      alert(_("ERROR:  Could not load the rods b\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n"));
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print(_("rods_gui.c:rods_gui_init():  new using defaults\n"));
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
  wc_picture_init(wcalc, picture_vbox, (const char **) rods_fig);

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

static void sync_l2_to_l1(GtkWidget *widget, gpointer data)
{
  rods_gui *gui;

  gui = WC_RODS_GUI(data);
  gtk_entry_set_text( GTK_ENTRY(gui->text_l2), gtk_entry_get_text( GTK_ENTRY(gui->text_l1) ) );
}

static void values_init(rods_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;

  wc_units_gui *xy_ug = NULL, *freq_ug = NULL, *rho_ug = NULL;

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


  /* ---------------- Wire 1, diameter, length  -------------- */
  wc_table_add_entry_new_units(table, gui, "Wire 1 Diameter (d1)",
			       &(gui->text_d1), gui->b->units_xy, &xy_ug,
			       &(gui->b->d1), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Wire 1 Length (l1)",
				  &(gui->text_l1), gui->b->units_xy, &xy_ug,
				  &(gui->b->l1), &x, &y);

  y++;
  /* ---------------- Resistivity -------------- */
  wc_table_add_entry_new_units(table, gui, "Resistivity",
			       &(gui->text_rho), gui->b->units_rho, &rho_ug,
			       &(gui->b->rho), &x, &y);

  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, "Frequency",
			       &(gui->text_freq), gui->b->units_freq, &freq_ug,
			       &(gui->b->freq), &x, &y);


  /* Analyze button */
  button = gtk_button_new_with_label (_("Analyze"));
  wc_button_connect( button, analyze, gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_set_tooltip_text( button,
		       _("Calculate electrical characteristics "
                         "from physical parameters") );
  y++;


  /* Column #2 */
  x += 4;
  y = 0;

  /* ---------------- Wire 2, diameter, length  -------------- */
  wc_table_add_entry_attach_units(table, gui, "Wire 2 Diameter (d2)",
				  &(gui->text_d2), gui->b->units_xy, &xy_ug,
				  &(gui->b->d2), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Wire 2 Length (l2)",
				  &(gui->text_l2), gui->b->units_xy, &xy_ug,
				  &(gui->b->l2), &x, &y);
  /* don't support l2 != l1 yet */
  gtk_widget_set_sensitive (gui->text_l2, FALSE);
  g_signal_connect( G_OBJECT( gui->text_l1 ), "changed",
                    G_CALLBACK(sync_l2_to_l1), gui);

  /* ---------------- Wire 2, position  -------------- */
  wc_table_add_entry_attach_units(table, gui, "Radial Distance (distance)",
				  &(gui->text_distance), gui->b->units_xy, &xy_ug,
				  &(gui->b->distance), &x, &y);

  wc_table_add_entry_attach_units(table, gui, "Axial offset (offset)",
				  &(gui->text_offset), gui->b->units_xy, &xy_ug,
				  &(gui->b->offset), &x, &y);
  /* don't support offset != 0 yet */
  gtk_widget_set_sensitive (gui->text_offset, FALSE);

  gtk_widget_show(table);

}


static void outputs_init(rods_gui *gui, GtkWidget *parent)
{
  GtkWidget *table;
  GtkWidget *text;
  GtkWidget *frame;
  int x=0;
  int y=0;
  wc_units_gui *ug_L = NULL, *ug_R;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label( GTK_FRAME(frame), "Output Values" );
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);

  table = gtk_table_new (5, 8, FALSE);
  gtk_container_add (GTK_CONTAINER (frame), table);


  wc_table_add_label_new_units(table, gui, "L1",
			       &(gui->label_L1), gui->b->units_L, &ug_L,
			       &(gui->b->L1), &x, &y);

  wc_table_add_label_attach_units(table, gui, "L2",
				  &(gui->label_L2), gui->b->units_L, &ug_L,
				  &(gui->b->L2), &x, &y);

  wc_table_add_label_attach_units(table, gui, "M",
				  &(gui->label_M), gui->b->units_L, &ug_L,
				  &(gui->b->M), &x, &y);

  wc_table_add_label_no_units(table, gui, "k",
				  &(gui->label_k), &x, &y);

  /* Column #2 */
  x += 4;
  y = 0;

  /* spacer between columns*/
  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, x-1, x, 0, 1,
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  wc_table_add_label_new_units(table, gui, "R1 (LF)",
			       &(gui->label_R1), gui->b->units_R, &ug_R,
			       &(gui->b->R1), &x, &y);

  wc_table_add_label_attach_units(table, gui, "R2 (LF)",
				  &(gui->label_R2), gui->b->units_R, &ug_R,
				  &(gui->b->R2), &x, &y);

  
  gtk_widget_show(table);

}


static void analyze( GtkWidget *w, gpointer data )
{
  calculate(WC_RODS_GUI(data), w, "analyze");
}


static void calculate( rods_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  /* Wire #1 */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_d1) );
  gui->b->d1=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l1) );
  gui->b->l1=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  /* Wire #2 */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_d2) );
  gui->b->d2=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

#ifdef notdef
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l2) );
  gui->b->l2=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  /* Wire #2 position */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_offset) );
  gui->b->offset=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

#else
  gui->b->l2 = gui->b->l1;
  gui->b->offset = 0.0;
#endif

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_distance) );
  gui->b->distance=atof(vstr)*wc_units_to_sf(gui->b->units_xy);


  /* Resistivity */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_rho) );
  gui->b->rho=atof(vstr)*wc_units_to_sf(gui->b->units_rho);

  /* Frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) );
  gui->b->freq=atof(vstr)*wc_units_to_sf(gui->b->units_freq);

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
      rslt = rods_calc(gui->b, gui->b->freq);
  } else {
      g_print(_("error in rods callback.  data=\"%s\""),(char *)data);
      exit(1);
  }

#ifdef DEBUG
  g_print(_("rods_gui.c:calculate():  finished calculation\n"));
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

static void update_display(rods_gui *gui)
{
  char str[80];

  /* ---------------- d1 -------------- */
  sprintf(str,WC_FMT_G,gui->b->d1/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_d1), str );

  /* ---------------- l1 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l1/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l1), str );

  /* ---------------- d2 -------------- */
  sprintf(str,WC_FMT_G,gui->b->d2/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_d2), str );

  /* ---------------- l2 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l2/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l2), str );

  /* ---------------- offset --------- */
  sprintf(str,WC_FMT_G,gui->b->offset/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_offset), str );

  /* ---------------- distance -------- */
  sprintf(str,WC_FMT_G,gui->b->distance/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_distance), str );

  /* ---------------- rho -------------- */
  sprintf(str,WC_FMT_G,gui->b->rho/wc_units_to_sf(gui->b->units_rho));
  gtk_entry_set_text( GTK_ENTRY(gui->text_rho), str );

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

  /* ---------------- R1 -------------- */
  sprintf(str,WC_FMT_G,gui->b->R1/wc_units_to_sf(gui->b->units_R));
  gtk_label_set_text( GTK_LABEL(gui->label_R1), str );

  /* ---------------- R2 -------------- */
  sprintf(str,WC_FMT_G,gui->b->R2/wc_units_to_sf(gui->b->units_R));
  gtk_label_set_text( GTK_LABEL(gui->label_R2), str );

}


static void tooltip_init(rods_gui *gui)
{

  gtk_widget_set_tooltip_text( gui->text_d1, _("Diameter of wire #1") );
  gtk_widget_set_tooltip_text( gui->text_l1, _("Length of wire #1") );

  gtk_widget_set_tooltip_text( gui->text_d2, _("Diameter of wire #2") );
  gtk_widget_set_tooltip_text( gui->text_l2, _("Length of wire #2\nConstrained to be the same as wire #1 in this release.") );

  gtk_widget_set_tooltip_text( gui->text_distance, _("Offset position of wire #2 in the radial direction") );
  gtk_widget_set_tooltip_text( gui->text_offset, _("Offset position of wire #2 in the axial direction.\nNot supported yet.") );

  gtk_widget_set_tooltip_text( gui->text_rho, _("Bulk resistivity of wires") );
  gtk_widget_set_tooltip_text( gui->text_freq, _("Frequency of operation") );

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("rods_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
	  wcalc,fp,name);
#endif
  rods_save(WC_RODS_GUI(wcalc)->b,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
  rods_gui *gui;
  rods * b;

  gui = WC_RODS_GUI(wcalc);
  b = gui->b;

  if( list == NULL ) {
    figure_rods_fig_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }

  list = wc_print_add_cairo(figure_rods_fig_render[0], figure_rods_fig_width[0],
                            figure_rods_fig_height[0], list);

  list = wc_print_add_double("Diameter of wire #1 (d1)", b->d1, b->units_xy, list);
  list = wc_print_add_double("Length of wire #1 (l1)", b->l1, b->units_xy, list);

  list = wc_print_add_double("Diameter of wire #2 (d2)", b->d2, b->units_xy, list);
  list = wc_print_add_double("Length of wire #2 (l2)", b->l2, b->units_xy, list);

  list = wc_print_add_double("Wire #2 position in the radial direction (distance)", b->distance, b->units_xy, list);
  list = wc_print_add_double("Wire #2 position in the axial direction (offset)", b->offset, b->units_xy, list);

  list = wc_print_add_double("Wire #1 Self Inductance (L1)", b->L1, b->units_L, list);
  list = wc_print_add_double("Wire #2 Self Inductance (L2)", b->L2, b->units_L, list);
  list = wc_print_add_double("Mutual Inductance (M)", b->M, b->units_L, list);
  list = wc_print_add_double("Coupling Coefficient (k)", b->k, NULL, list);

  list = wc_print_add_double("Wire #1 DC Resistance (R1)", b->R1, b->units_R, list);
  list = wc_print_add_double("Wire #2 DC Resistance (R2)", b->R2, b->units_R, list);

  list = wc_print_add_double("Wire Resistivity (rho)", b->rho, b->units_rho, list);

  return list;
}


