/*
 * Copyright (C) 2009, 2021 Dan McMahill
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

#include "pixmaps/rc.xpm"
#include "pixmaps/figure_rc.h"
#include "parallel_rc.h"
#include "parallel_rc_gui.h"
#include "parallel_rc_loadsave.h"
#include "newprint.h"
#include "physconst.h"

#include "utils.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static GList * dump_values(Wcalc *wcalc);

static void analyze_p2s( GtkWidget *w, gpointer data );
static void analyze_s2p( GtkWidget *w, gpointer data );
static void calculate( parallel_rc_gui *gui, GtkWidget *w, gpointer data );
static void update_display( parallel_rc_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(parallel_rc_gui *gui, GtkWidget *parent);
static void tooltip_init(parallel_rc_gui *gui);


static char *name="Parallel/Series RC Equivalents";
static char *version="v1.0";
/*
 * Public Functions
 */

parallel_rc_gui *parallel_rc_gui_new(void)
{
  parallel_rc_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (parallel_rc_gui *) malloc(sizeof(parallel_rc_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,_("parallel_rc_gui_new: malloc() failed\n"));
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
  wcalc->init = parallel_rc_gui_init;
  wcalc->save = gui_save;
  wcalc->dump_values = dump_values;

  wcalc->model_name = name;
  wcalc->model_version = version;

  /*
   * Initialize the model dependent portions
   */

  /* create the parallel_rc b which will be used */
  new_gui->b = parallel_rc_new();

  return new_gui;
}

void parallel_rc_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *picture_vbox;

  parallel_rc_gui *gui;

  gui = WC_PARALLEL_RC_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print(_("parallel_rc_gui.c:parallel_rc_gui_init():  calling parallel_rc_load\n"));
#endif
    if (parallel_rc_load(gui->b,fp) != 0) {
      alert(_("ERROR:  Could not load the parallel_rc b\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n"));
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print(_("parallel_rc_gui.c:parallel_rc_gui_init():  new using defaults\n"));
#endif
  }

  /* create the other vbox's and pack them into the main vbox */
  values_vbox = gtk_vbox_new (FALSE, 1);
  picture_vbox = gtk_vbox_new (FALSE, 1);

  gtk_container_set_border_width (GTK_CONTAINER (values_vbox), 5);
  gtk_container_set_border_width (GTK_CONTAINER (picture_vbox), 5);

  gtk_box_pack_start (GTK_BOX (main_vbox), values_vbox, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_vbox), picture_vbox, FALSE, TRUE, 0);

  gtk_widget_show (values_vbox);
  gtk_widget_show (picture_vbox);

  values_init(gui,values_vbox);
  wc_picture_init(wcalc, picture_vbox, (const char **) rc);

  tooltip_init(gui);

  gtk_widget_show_all (wcalc->window);

  wcalc->init_done=1;

  update_display(gui);


  /* run the analysis once since we've changed input units */
  wc_units_menu_init( wcalc );
  if( gui->b->series_to_parallel ) {
    analyze_s2p(NULL, gui);
  } else {
    analyze_p2s(NULL, gui);
  }

  wc_units_menu_init( wcalc );
}

/*
 * Private Functions
 */

static void use_Q_pressed(GtkWidget *widget, gpointer data )
{
  parallel_rc_gui *gui;

  gui = WC_PARALLEL_RC_GUI(data);
  gtk_widget_set_sensitive (gui->text_Qs, TRUE);
  gtk_widget_set_sensitive (gui->text_Qp, TRUE);
  gtk_widget_set_sensitive (gui->text_Rs, FALSE);
  gtk_widget_set_sensitive (gui->text_Rp, FALSE);
  gui->b->use_Q = 1;
}

static void use_R_pressed(GtkWidget *widget, gpointer data )
{
  parallel_rc_gui *gui;

  gui = WC_PARALLEL_RC_GUI(data);
  gtk_widget_set_sensitive (gui->text_Qs, FALSE);
  gtk_widget_set_sensitive (gui->text_Qp, FALSE);
  gtk_widget_set_sensitive (gui->text_Rs, TRUE);
  gtk_widget_set_sensitive (gui->text_Rp, TRUE);
  gui->b->use_Q = 0;
}

static void values_init(parallel_rc_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;

  /* the use_Q radio button group */
  GSList *use_Q_group;

  wc_units_gui *C_ug = NULL, *R_ug, *freq_ug = NULL;

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


  /* ---------------- Series C, R, Q -------------- */
  wc_table_add_entry_new_units(table, gui, "Series capacitance (Cs)",
			       &(gui->text_Cs), gui->b->units_C, &C_ug,
			       &(gui->b->Cs), &x, &y);

  wc_table_add_entry_new_units(table, gui, "Series resistance (Rs)",
			       &(gui->text_Rs), gui->b->units_Rs, &R_ug,
			       &(gui->b->Rs), &x, &y);

  wc_table_add_entry_no_units(table, gui, "Series quality factor (Qs)",
			      &(gui->text_Qs),
			      &(gui->b->Qs), &x, &y);



  /* Series to Parallel Analyze button */
  button = gtk_button_new_with_label (_("Series to Parallel"));
  wc_button_connect( button, analyze_s2p, gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_set_tooltip_text( button,
		       _("Calculate equivalent parallel circuit "
		       "from given series circuit") );
  y++;

  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, "Frequency",
			       &(gui->text_freq), gui->b->units_freq, &freq_ug,
			       &(gui->b->freq), &x, &y);




  /* Column #2 */
  x += 4;
  y = 0;

  wc_table_add_entry_attach_units(table, gui, "Parallel capacitance (Cp)",
				  &(gui->text_Cp), gui->b->units_C, &C_ug,
				  &(gui->b->Cp), &x, &y);


  wc_table_add_entry_new_units(table, gui, "Parallel resistance (Rp)",
			       &(gui->text_Rp), gui->b->units_Rp, &R_ug,
			       &(gui->b->Rp), &x, &y);

  wc_table_add_entry_no_units(table, gui, "Parallel quality factor (Qs)",
			      &(gui->text_Qp),
			      &(gui->b->Qp), &x, &y);


  /* Parallel to Series button */
  button = gtk_button_new_with_label (_("Parallel to Series"));
  wc_button_connect( button, analyze_p2s, gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_widget_set_tooltip_text( button,
		       _("Calculate equivalent series circuit "
		       "from given parallel circuit") );
  y++;


  if(gui->b->use_Q){
    gtk_widget_set_sensitive (gui->text_Rs, FALSE);
    gtk_widget_set_sensitive (gui->text_Rp, FALSE);
    gtk_widget_set_sensitive (gui->text_Qs, TRUE);
    gtk_widget_set_sensitive (gui->text_Qp, TRUE);
  }
  else{
    gtk_widget_set_sensitive (gui->text_Rs, TRUE);
    gtk_widget_set_sensitive (gui->text_Rp, TRUE);
    gtk_widget_set_sensitive (gui->text_Qs, FALSE);
    gtk_widget_set_sensitive (gui->text_Qp, FALSE);
  }


  /* QC button */
  gui->button_use_Q = gtk_radio_button_new_with_label (NULL, _("Q/C"));
  gtk_table_attach(GTK_TABLE(table), gui->button_use_Q, x+1, x+2, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  g_signal_connect( G_OBJECT( gui->button_use_Q ), "clicked",
                    G_CALLBACK(use_Q_pressed),
                    gui);
  gtk_widget_show (gui->button_use_Q);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gui->button_use_Q), TRUE);
  use_Q_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (gui->button_use_Q));

  /* RC button */
  gui->button_use_R = gtk_radio_button_new_with_label (use_Q_group, _("R/C"));
  gtk_table_attach(GTK_TABLE(table), gui->button_use_R, x+2, x+3, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  g_signal_connect( G_OBJECT( gui->button_use_R ), "clicked",
                    G_CALLBACK(use_R_pressed),
                    gui);
  if (gui->b->use_Q == 0) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gui->button_use_R), TRUE);
  }
  gtk_widget_show (gui->button_use_R);



  gtk_widget_show(table);

}


static void analyze_s2p( GtkWidget *w, gpointer data )
{

  WC_PARALLEL_RC_GUI(data)->b->series_to_parallel = 1;
  calculate(WC_PARALLEL_RC_GUI(data), w, "analyze");
}

static void analyze_p2s( GtkWidget *w, gpointer data )
{

  WC_PARALLEL_RC_GUI(data)->b->series_to_parallel = 0;
  calculate(WC_PARALLEL_RC_GUI(data), w, "analyze");
}


static void calculate( parallel_rc_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  /* Series */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Cs) );
  gui->b->Cs=atof(vstr)*wc_units_to_sf(gui->b->units_C);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Rs) );
  gui->b->Rs=atof(vstr)*wc_units_to_sf(gui->b->units_Rs);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Qs) );
  gui->b->Qs=atof(vstr);


  /* Series */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Cp) );
  gui->b->Cp=atof(vstr)*wc_units_to_sf(gui->b->units_C);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Rp) );
  gui->b->Rp=atof(vstr)*wc_units_to_sf(gui->b->units_Rp);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Qp) );
  gui->b->Qp=atof(vstr);

  /* Frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) );
  gui->b->freq=atof(vstr)*wc_units_to_sf(gui->b->units_freq);

#ifdef DEBUG
  g_print("parallel_rc_gui.c:calculate(): --------------- Series/Parallel RC Network -----------\n");
  g_print("parallel_rc_gui.c:calculate(): Cs = %g %s\n",
	  gui->b->Cs/gui->b->units_C->sf, gui->b->units_C->name);
  g_print("parallel_rc_gui.c:calculate(): Rs = %g %s\n",
	  gui->b->Rs/gui->b->units_Rs->sf, gui->b->units_Rs->name);
  g_print("parallel_rc_gui.c:calculate(): Qs = %g\n",
	  gui->b->Qs);

  g_print("parallel_rc_gui.c:calculate(): Cp = %g %s\n",
	  gui->b->Cp/gui->b->units_C->sf, gui->b->units_C->name);
  g_print("parallel_rc_gui.c:calculate(): Rp = %g %s\n",
	  gui->b->Rp/gui->b->units_Rp->sf, gui->b->units_Rp->name);
  g_print("parallel_rc_gui.c:calculate(): Qp = %g\n",
	  gui->b->Qp);

  g_print("parallel_rc_gui.c:calculate(): Frequency = %g %s\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name);

  g_print("parallel_rc_gui.c:calculate(): use_Q = %d\n", gui->b->use_Q);
  g_print("parallel_rc_gui.c:calculate(): series_to_parallel = %d\n", gui->b->series_to_parallel);

  g_print("parallel_rc_gui.c:calculate(): data = \"%s\"\n", (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
      rslt = parallel_rc_calc(gui->b, gui->b->freq);
  } else {
      g_print(_("error in parallel_rc callback.  data=\"%s\""),(char *)data);
      exit(1);
  }

#ifdef DEBUG
  g_print("parallel_rc_gui.c:calculate():  finished calculation, got %d\n", rslt);
  g_print("parallel_rc_gui.c:calculate(): Cs = %g %s\n",
	  gui->b->Cs/gui->b->units_C->sf, gui->b->units_C->name);
  g_print("parallel_rc_gui.c:calculate(): Rs = %g %s\n",
	  gui->b->Rs/gui->b->units_Rs->sf, gui->b->units_Rs->name);
  g_print("parallel_rc_gui.c:calculate(): Qs = %g\n",
	  gui->b->Qs);

  g_print("parallel_rc_gui.c:calculate(): Cp = %g %s\n",
	  gui->b->Cp/gui->b->units_C->sf, gui->b->units_C->name);
  g_print("parallel_rc_gui.c:calculate(): Rp = %g %s\n",
	  gui->b->Rp/gui->b->units_Rp->sf, gui->b->units_Rp->name);
  g_print("parallel_rc_gui.c:calculate(): Qp = %g\n",
	  gui->b->Qp);

  g_print("parallel_rc_gui.c:calculate(): Frequency = %g %s\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name);

  g_print("parallel_rc_gui.c:calculate(): use_Q = %d\n", gui->b->use_Q);
  g_print("parallel_rc_gui.c:calculate(): series_to_parallel = %d\n", gui->b->series_to_parallel);

  g_print("parallel_rc_gui.c:calculate(): data = \"%s\"\n", (char *) data);
  g_print("parallel_rc_gui.c:calculate(): -------------- ---------------------- ----------\n");
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

static void update_display(parallel_rc_gui *gui)
{
  char str[80];

  /* ---------------- Series -------------- */
  sprintf(str,WC_FMT_G,gui->b->Cs/wc_units_to_sf(gui->b->units_C));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Cs), str );

  sprintf(str,WC_FMT_G,gui->b->Rs/wc_units_to_sf(gui->b->units_Rs));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Rs), str );

  sprintf(str,WC_FMT_G,gui->b->Qs);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Qs), str );

  /* ---------------- Parallel -------------- */

  sprintf(str,WC_FMT_G,gui->b->Cp/wc_units_to_sf(gui->b->units_C));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Cp), str );

  sprintf(str,WC_FMT_G,gui->b->Rp/wc_units_to_sf(gui->b->units_Rp));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Rp), str );

  sprintf(str,WC_FMT_G,gui->b->Qp);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Qp), str );

  /* ---------------- Global -------------- */
  sprintf(str,WC_FMT_G,gui->b->freq/wc_units_to_sf(gui->b->units_freq));
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );


}


static void tooltip_init(parallel_rc_gui *gui)
{

  gtk_widget_set_tooltip_text( gui->text_Cs, _("Series capacitance") );
  gtk_widget_set_tooltip_text( gui->text_Rs, _("Series resistance") );
  gtk_widget_set_tooltip_text( gui->text_Qs, _("Series quality factor") );

  gtk_widget_set_tooltip_text( gui->text_Cp, _("Parallel capacitance") );
  gtk_widget_set_tooltip_text( gui->text_Rp, _("Parallel resistance") );
  gtk_widget_set_tooltip_text( gui->text_Qp, _("Parallel quality factor") );


  gtk_widget_set_tooltip_text( gui->text_freq, _("Frequency of operation") );

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("parallel_rc_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
	  wcalc,fp,name);
#endif
  parallel_rc_save(WC_PARALLEL_RC_GUI(wcalc)->b,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
  parallel_rc_gui *gui;
  parallel_rc * b;

  gui = WC_PARALLEL_RC_GUI(wcalc);
  b = gui->b;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_rc_init();
  } else {
    g_list_free_full(list, (GDestroyNotify) wc_print_value_free);
    list = NULL;
  }
  
  list = wc_print_add_cairo(figure_rc_render[0], figure_rc_width[0],
                            figure_rc_height[0], list);

  list = wc_print_add_double("Series capacitance (Cs)", b->Cs, b->units_C, list);
  list = wc_print_add_double("Series resistance (Rs)", b->Rs, b->units_Rs, list);
  list = wc_print_add_double("Series quality factor (Qs)", b->Qs, NULL, list);

  list = wc_print_add_double("Parallel capacitance (Cp)", b->Cp, b->units_C, list);
  list = wc_print_add_double("Parallel resistance (Rp)", b->Rp, b->units_Rp, list);
  list = wc_print_add_double("Parallel quality factor (Qp)", b->Qp, NULL, list);

  list = wc_print_add_double("Operation frequency (freq)", b->freq, b->units_freq, list);

  return list;
}


