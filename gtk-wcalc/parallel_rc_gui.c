/* $Id: bars_gui.c,v 1.13 2009/02/05 22:15:16 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
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
#include "epscat.h"
#include "menus.h"
#include "gtk-units.h"

#include "misc.h"
#include "units.h"

#if GTK_CHECK_VERSION(2,10,0)
#include "pixmaps/figure_rc.h"
#endif
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

static void print_ps(Wcalc *wcalc,FILE *fp);
static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void calculate( parallel_rc_gui *gui, GtkWidget *w, gpointer data );
static void update_display( parallel_rc_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(parallel_rc_gui *gui, GtkWidget *parent);
static void picture_init(parallel_rc_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
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
  wcalc->init_done=0;

  wcalc->init = parallel_rc_gui_init;
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
  picture_init(gui,wcalc->window,picture_vbox);

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


static void values_init(parallel_rc_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;
  GtkTooltips *tips;

  wc_units_gui *C_ug = NULL, *R_ug, *freq_ug = NULL;

  /* keeps track of current table position */
  int y = 0;
  int x = 0;

  tips = gtk_tooltips_new();

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
			       &(gui->text_Rs), gui->b->units_R, &R_ug, 
			       &(gui->b->Rs), &x, &y);

  wc_table_add_entry_no_units(table, gui, "Series quality factor (Qs)", 
			      &(gui->text_Qs),
			      &(gui->b->Qs), &x, &y);


  y++;
  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, "Frequency", 
			       &(gui->text_freq), gui->b->units_freq, &freq_ug, 
			       &(gui->b->freq), &x, &y);


  /* Analyze button */
  button = gtk_button_new_with_label (_("Series to Parallel"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate equivalent parallel circuit "
		       "from series circuit"),
		       NULL);
  y++;


  /* Column #2 */
  x += 4;
  y = 0;

  wc_table_add_entry_attach_units(table, gui, "Parallel capacitance (Cp)", 
				  &(gui->text_Cp), gui->b->units_C, &C_ug, 
				  &(gui->b->Cp), &x, &y);


  wc_table_add_entry_attach_units(table, gui, "Parallel resistance (Rp)", 
				  &(gui->text_Rp), gui->b->units_R, &R_ug, 
				  &(gui->b->Rp), &x, &y);


  wc_table_add_entry_no_units(table, gui, "Parallel quality factor (Qs)", 
			      &(gui->text_Qp),
			      &(gui->b->Qp), &x, &y);


  gtk_widget_show(table);

}

#include "pixmaps/rc.xpm"

static void picture_init(parallel_rc_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) rc);
					
  
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (my_hbox), pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    

  WC_WCALC(gui)->text_status = gtk_label_new( _("Values Out Of Sync") );
  gtk_box_pack_start (GTK_BOX (my_hbox), WC_WCALC(gui)->text_status, FALSE, FALSE, 0);
  gtk_widget_show (WC_WCALC(gui)->text_status);
  

}


static void analyze( GtkWidget *w, gpointer data )
{
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
  gui->b->Rs=atof(vstr)*wc_units_to_sf(gui->b->units_R);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Qs) ); 
  gui->b->Qs=atof(vstr);


  /* Series */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Cp) ); 
  gui->b->Cp=atof(vstr)*wc_units_to_sf(gui->b->units_C);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Rp) ); 
  gui->b->Rp=atof(vstr)*wc_units_to_sf(gui->b->units_R);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Qp) ); 
  gui->b->Qp=atof(vstr);

  /* Frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->b->freq=atof(vstr)*wc_units_to_sf(gui->b->units_freq);
  
  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
      rslt = parallel_rc_calc(gui->b, gui->b->freq);
  } else {
      g_print(_("error in parallel_rc callback.  data=\"%s\""),(char *)data);
      exit(1);
  }
  
#ifdef DEBUG
  g_print(_("parallel_rc_gui.c:calculate():  finished calculation\n"));
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

  sprintf(str,WC_FMT_G,gui->b->Rs/wc_units_to_sf(gui->b->units_R));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Rs), str );

  sprintf(str,WC_FMT_G,gui->b->Qs);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Qs), str );

  /* ---------------- Parallel -------------- */

  sprintf(str,WC_FMT_G,gui->b->Cp/wc_units_to_sf(gui->b->units_C));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Cp), str );

  sprintf(str,WC_FMT_G,gui->b->Rp/wc_units_to_sf(gui->b->units_R));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Rp), str );

  sprintf(str,WC_FMT_G,gui->b->Qp);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Qp), str );

}


static void tooltip_init(parallel_rc_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_Cs, _("Series capacitance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Rs, _("Series resistance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Qs, _("Series quality factor"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_Cp, _("Parallel capacitance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Rp, _("Parallel resistance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Qp, _("Parallel quality factor"), NULL);


  gtk_tooltips_set_tip(tips, gui->text_freq, _("Frequency of operation"), NULL);

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
#if GTK_CHECK_VERSION(2,10,0)
  parallel_rc_gui *gui;
  parallel_rc * b;

  gui = WC_PARALLEL_RC_GUI(wcalc);
  b = gui->b;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_rc_init();
  }  {
    // FIXME -- free the old list first!!!!
    list = NULL;
    list = wc_print_add_cairo(figure_rc_render[0], figure_rc_width[0], 
			      figure_rc_height[0], list);
    
    list = wc_print_add_double("Series capacitance (Cs)", b->Cs, b->units_C, list);
    list = wc_print_add_double("Series resistance (Rs)", b->Rs, b->units_R, list);
    list = wc_print_add_double("Series quality factor (Qs)", b->Qs, NULL, list);
    
    list = wc_print_add_double("Parallel capacitance (Cp)", b->Cp, b->units_C, list);
    list = wc_print_add_double("Parallel resistance (Rp)", b->Rp, b->units_R, list);
    list = wc_print_add_double("Parallel quality factor (Qp)", b->Qp, NULL, list);
    
    list = wc_print_add_double("Operation frequency (freq)", b->freq, b->units_freq, list);

  }
#endif

  return list;
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  parallel_rc_gui *gui;
  char *file;

  gui = WC_PARALLEL_RC_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("rc.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "rc.eps");
  eps_cat(file,fp);

  /* print the data */

  fprintf(fp,"%% spit out the numbers\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"newline\n");
  fprintf(fp,"/col1x currentpoint pop def\n");
  fprintf(fp,"/col2x %g 2 div inch def\n", global_print_config->paperwidth);
  fprintf(fp,"/coly currentpoint exch pop def\n");
  fprintf(fp,"/bspace 1.5 def\n");
  fprintf(fp,"\n");
  fprintf(fp,"col1x coly moveto\n");
  fprintf(fp,"/leftcol col1x  def\n");

  fprintf(fp,"(Cs) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Cs/gui->b->units_C->sf, gui->b->units_C->name);
  fprintf(fp,"(Rs) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Rs/gui->b->units_R->sf, gui->b->units_R->name);
  fprintf(fp,"(Qs) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->b->Qs);

  fprintf(fp,"(frequency) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name);
  fprintf(fp,"newline\n");

  /* Second column of the output */
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");


  fprintf(fp,"(Cp) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Cp/gui->b->units_C->sf, gui->b->units_C->name);
  fprintf(fp,"(Rp) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Rp/gui->b->units_R->sf, gui->b->units_R->name);
  fprintf(fp,"(Qp) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->b->Qp);


  fprintf(fp,"newline\n");

}

