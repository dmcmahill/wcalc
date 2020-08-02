
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
#include "pixmaps/figure_rl.h"
#endif
#include "parallel_rl.h"
#include "parallel_rl_gui.h"
#include "parallel_rl_loadsave.h"
#include "newprint.h"
#include "physconst.h"

#include "utils.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);
static GList * dump_values(Wcalc *wcalc);

static void analyze_p2s( GtkWidget *w, gpointer data );
static void analyze_s2p( GtkWidget *w, gpointer data );
static void calculate( parallel_rl_gui *gui, GtkWidget *w, gpointer data );
static void update_display( parallel_rl_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(parallel_rl_gui *gui, GtkWidget *parent);
static void picture_init(parallel_rl_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(parallel_rl_gui *gui);


static char *name="Parallel/Series RL Equivalents";
static char *version="v1.0";
/*
 * Public Functions
 */

parallel_rl_gui *parallel_rl_gui_new(void)
{
  parallel_rl_gui *new_gui;
  Wcalc *wcalc;

  /* allocate memory */
  new_gui = (parallel_rl_gui *) malloc(sizeof(parallel_rl_gui));
  if(new_gui == NULL)
    {
      fprintf(stderr,_("parallel_rl_gui_new: malloc() failed\n"));
      exit(1);
    }

  /* Wcalc is the parent */
  wcalc = (Wcalc *) new_gui;

  /*
   * Initialize the parent
   */
  wcalc->init_done=0;

  wcalc->init = parallel_rl_gui_init;
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

  /* create the parallel_rl b which will be used */
  new_gui->b = parallel_rl_new();

  return new_gui;
}

void parallel_rl_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp)
{
  GtkWidget *values_vbox;
  GtkWidget *picture_vbox;

  parallel_rl_gui *gui;

  gui = WC_PARALLEL_RL_GUI(wcalc);

  /* if this is an open from file calculator, then open the file */
  if (fp != NULL){
#ifdef DEBUG
    g_print(_("parallel_rl_gui.c:parallel_rl_gui_init():  calling parallel_rl_load\n"));
#endif
    if (parallel_rl_load(gui->b,fp) != 0) {
      alert(_("ERROR:  Could not load the parallel_rl b\n"
	    "data from the chosen file.  The values\n"
	    "may be corrupted.\n"));
    }
  }
  else{
    /* put any default values here */
#ifdef DEBUG
    g_print(_("parallel_rl_gui.c:parallel_rl_gui_init():  new using defaults\n"));
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
  parallel_rl_gui *gui;

  gui = WC_PARALLEL_RL_GUI(data);
  gtk_widget_set_sensitive (gui->text_Qs, TRUE);
  gtk_widget_set_sensitive (gui->text_Qp, TRUE);
  gtk_widget_set_sensitive (gui->text_Rs, FALSE);
  gtk_widget_set_sensitive (gui->text_Rp, FALSE);
  gui->b->use_Q = 1;
}

static void use_R_pressed(GtkWidget *widget, gpointer data )
{
  parallel_rl_gui *gui;

  gui = WC_PARALLEL_RL_GUI(data);
  gtk_widget_set_sensitive (gui->text_Qs, FALSE);
  gtk_widget_set_sensitive (gui->text_Qp, FALSE);
  gtk_widget_set_sensitive (gui->text_Rs, TRUE);
  gtk_widget_set_sensitive (gui->text_Rp, TRUE);
  gui->b->use_Q = 0;
}

static void values_init(parallel_rl_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;
  GtkTooltips *tips;

  /* the use_Q radio button group */
  GSList *use_Q_group;

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
  wc_table_add_entry_new_units(table, gui, "Series inductance (Ls)", 
			       &(gui->text_Ls), gui->b->units_L, &C_ug, 
			       &(gui->b->Ls), &x, &y);

  wc_table_add_entry_new_units(table, gui, "Series resistance (Rs)", 
			       &(gui->text_Rs), gui->b->units_Rs, &R_ug, 
			       &(gui->b->Rs), &x, &y);

  wc_table_add_entry_no_units(table, gui, "Series quality factor (Qs)", 
			      &(gui->text_Qs),
			      &(gui->b->Qs), &x, &y);



  /* Series to Parallel Analyze button */
  button = gtk_button_new_with_label (_("Series to Parallel"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze_s2p), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate equivalent parallel circuit "
		       "from given series circuit"),
		       NULL);
  y++;

  /* ---------------- Frequency -------------- */
  wc_table_add_entry_new_units(table, gui, "Frequency", 
			       &(gui->text_freq), gui->b->units_freq, &freq_ug, 
			       &(gui->b->freq), &x, &y);




  /* Column #2 */
  x += 4;
  y = 0;

  wc_table_add_entry_attach_units(table, gui, "Parallel inductance (Lp)", 
				  &(gui->text_Lp), gui->b->units_L, &C_ug, 
				  &(gui->b->Lp), &x, &y);


  wc_table_add_entry_new_units(table, gui, "Parallel resistance (Rp)", 
			       &(gui->text_Rp), gui->b->units_Rp, &R_ug, 
			       &(gui->b->Rp), &x, &y);

  wc_table_add_entry_no_units(table, gui, "Parallel quality factor (Qs)", 
			      &(gui->text_Qp),
			      &(gui->b->Qp), &x, &y);


  /* Parallel to Series button */
  button = gtk_button_new_with_label (_("Parallel to Series"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze_p2s), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate equivalent series circuit "
		       "from given parallel circuit"),
		       NULL);
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


  /* QL button */
  gui->button_use_Q = gtk_radio_button_new_with_label (NULL, _("Q/L"));
  gtk_table_attach(GTK_TABLE(table), gui->button_use_Q, x+1, x+2, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_signal_connect(GTK_OBJECT(gui->button_use_Q), "clicked",
		     GTK_SIGNAL_FUNC(use_Q_pressed),
		     gui);
  gtk_widget_show (gui->button_use_Q);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gui->button_use_Q), TRUE);
  use_Q_group = gtk_radio_button_group (GTK_RADIO_BUTTON (gui->button_use_Q));

  /* RL button */
  gui->button_use_R = gtk_radio_button_new_with_label (use_Q_group, _("R/L"));
  gtk_table_attach(GTK_TABLE(table), gui->button_use_R, x+2, x+3, y, y+1,
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_signal_connect(GTK_OBJECT(gui->button_use_R), "clicked",
		     GTK_SIGNAL_FUNC(use_R_pressed),
		     gui);
  if (gui->b->use_Q == 0) {
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gui->button_use_R), TRUE);
  }
  gtk_widget_show (gui->button_use_R);



  gtk_widget_show(table);

}

#include "pixmaps/rl.xpm"

static void picture_init(parallel_rl_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) rl);
					
  
  /* a pixmap widget to contain the pixmap */
  pixmapwid = gtk_pixmap_new( pixmap , mask);
  gtk_box_pack_start (GTK_BOX (my_hbox), pixmapwid, FALSE, FALSE, 0);
  gtk_widget_show( pixmapwid );
    

  WC_WCALC(gui)->text_status = gtk_label_new( _("Values Out Of Sync") );
  gtk_box_pack_start (GTK_BOX (my_hbox), WC_WCALC(gui)->text_status, FALSE, FALSE, 0);
  gtk_widget_show (WC_WCALC(gui)->text_status);
  

}


static void analyze_s2p( GtkWidget *w, gpointer data )
{

  WC_PARALLEL_RL_GUI(data)->b->series_to_parallel = 1;
  calculate(WC_PARALLEL_RL_GUI(data), w, "analyze");
}

static void analyze_p2s( GtkWidget *w, gpointer data )
{

  WC_PARALLEL_RL_GUI(data)->b->series_to_parallel = 0;
  calculate(WC_PARALLEL_RL_GUI(data), w, "analyze");
}


static void calculate( parallel_rl_gui *gui, GtkWidget *w, gpointer data )
{
  const char *vstr;
  int rslt=0;

  /* Series */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Ls) ); 
  gui->b->Ls=atof(vstr)*wc_units_to_sf(gui->b->units_L);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Rs) ); 
  gui->b->Rs=atof(vstr)*wc_units_to_sf(gui->b->units_Rs);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Qs) ); 
  gui->b->Qs=atof(vstr);


  /* Series */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Lp) ); 
  gui->b->Lp=atof(vstr)*wc_units_to_sf(gui->b->units_L);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Rp) ); 
  gui->b->Rp=atof(vstr)*wc_units_to_sf(gui->b->units_Rp);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_Qp) ); 
  gui->b->Qp=atof(vstr);

  /* Frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->b->freq=atof(vstr)*wc_units_to_sf(gui->b->units_freq);
  
#ifdef DEBUG
  g_print("parallel_rl_gui.c:calculate(): --------------- Series/Parallel RL Network -----------\n");
  g_print("parallel_rl_gui.c:calculate(): Ls = %g %s\n",
	  gui->b->Ls/gui->b->units_L->sf, gui->b->units_L->name);
  g_print("parallel_rl_gui.c:calculate(): Rs = %g %s\n",
	  gui->b->Rs/gui->b->units_Rs->sf, gui->b->units_Rs->name);
  g_print("parallel_rl_gui.c:calculate(): Qs = %g\n",
	  gui->b->Qs);

  g_print("parallel_rl_gui.c:calculate(): Lp = %g %s\n",
	  gui->b->Lp/gui->b->units_L->sf, gui->b->units_L->name);
  g_print("parallel_rl_gui.c:calculate(): Rp = %g %s\n",
	  gui->b->Rp/gui->b->units_Rp->sf, gui->b->units_Rp->name);
  g_print("parallel_rl_gui.c:calculate(): Qp = %g\n",
	  gui->b->Qp);

  g_print("parallel_rl_gui.c:calculate(): Frequency = %g %s\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name); 

  g_print("parallel_rl_gui.c:calculate(): use_Q = %d\n", gui->b->use_Q);
  g_print("parallel_rl_gui.c:calculate(): series_to_parallel = %d\n", gui->b->series_to_parallel);
  
  g_print("parallel_rl_gui.c:calculate(): data = \"%s\"\n", (char *) data);
#endif

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
      rslt = parallel_rl_calc(gui->b, gui->b->freq);
  } else {
      g_print(_("error in parallel_rl callback.  data=\"%s\""),(char *)data);
      exit(1);
  }
  
#ifdef DEBUG
  g_print("parallel_rl_gui.c:calculate():  finished calculation, got %d\n", rslt);
  g_print("parallel_rl_gui.c:calculate(): Ls = %g %s\n",
	  gui->b->Ls/gui->b->units_L->sf, gui->b->units_L->name);
  g_print("parallel_rl_gui.c:calculate(): Rs = %g %s\n",
	  gui->b->Rs/gui->b->units_Rs->sf, gui->b->units_Rs->name);
  g_print("parallel_rl_gui.c:calculate(): Qs = %g\n",
	  gui->b->Qs);

  g_print("parallel_rl_gui.c:calculate(): Lp = %g %s\n",
	  gui->b->Lp/gui->b->units_L->sf, gui->b->units_L->name);
  g_print("parallel_rl_gui.c:calculate(): Rp = %g %s\n",
	  gui->b->Rp/gui->b->units_Rp->sf, gui->b->units_Rp->name);
  g_print("parallel_rl_gui.c:calculate(): Qp = %g\n",
	  gui->b->Qp);

  g_print("parallel_rl_gui.c:calculate(): Frequency = %g %s\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name); 

  g_print("parallel_rl_gui.c:calculate(): use_Q = %d\n", gui->b->use_Q);
  g_print("parallel_rl_gui.c:calculate(): series_to_parallel = %d\n", gui->b->series_to_parallel);
  
  g_print("parallel_rl_gui.c:calculate(): data = \"%s\"\n", (char *) data);
  g_print("parallel_rl_gui.c:calculate(): -------------- ---------------------- ----------\n");
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

static void update_display(parallel_rl_gui *gui)
{
  char str[80];

  /* ---------------- Series -------------- */
  sprintf(str,WC_FMT_G,gui->b->Ls/wc_units_to_sf(gui->b->units_L));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Ls), str );

  sprintf(str,WC_FMT_G,gui->b->Rs/wc_units_to_sf(gui->b->units_Rs));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Rs), str );

  sprintf(str,WC_FMT_G,gui->b->Qs);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Qs), str );

  /* ---------------- Parallel -------------- */

  sprintf(str,WC_FMT_G,gui->b->Lp/wc_units_to_sf(gui->b->units_L));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Lp), str );

  sprintf(str,WC_FMT_G,gui->b->Rp/wc_units_to_sf(gui->b->units_Rp));
  gtk_entry_set_text( GTK_ENTRY(gui->text_Rp), str );

  sprintf(str,WC_FMT_G,gui->b->Qp);
  gtk_entry_set_text( GTK_ENTRY(gui->text_Qp), str );

  /* ---------------- Global -------------- */
  sprintf(str,WC_FMT_G,gui->b->freq/wc_units_to_sf(gui->b->units_freq));
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );

  
}


static void tooltip_init(parallel_rl_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_Ls, _("Series inductance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Rs, _("Series resistance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Qs, _("Series quality factor"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_Lp, _("Parallel inductance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Rp, _("Parallel resistance"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_Qp, _("Parallel quality factor"), NULL);


  gtk_tooltips_set_tip(tips, gui->text_freq, _("Frequency of operation"), NULL);

}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("parallel_rl_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
	  wcalc,fp,name);
#endif
  parallel_rl_save(WC_PARALLEL_RL_GUI(wcalc)->b,fp,name);
}

static GList * dump_values(Wcalc *wcalc)
{
  static GList * list = NULL;
#if GTK_CHECK_VERSION(2,10,0)
  parallel_rl_gui *gui;
  parallel_rl * b;

  gui = WC_PARALLEL_RL_GUI(wcalc);
  b = gui->b;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_rl_init();
  }  {
    // FIXME -- free the old list first!!!!
    list = NULL;
    list = wc_print_add_cairo(figure_rl_render[0], figure_rl_width[0], 
			      figure_rl_height[0], list);
    
    list = wc_print_add_double("Series inductance (Ls)", b->Ls, b->units_L, list);
    list = wc_print_add_double("Series resistance (Rs)", b->Rs, b->units_Rs, list);
    list = wc_print_add_double("Series quality factor (Qs)", b->Qs, NULL, list);
    
    list = wc_print_add_double("Parallel inductance (Lp)", b->Lp, b->units_L, list);
    list = wc_print_add_double("Parallel resistance (Rp)", b->Rp, b->units_Rp, list);
    list = wc_print_add_double("Parallel quality factor (Qp)", b->Qp, NULL, list);
    
    list = wc_print_add_double("Operation frequency (freq)", b->freq, b->units_freq, list);

  }
#endif

  return list;
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  parallel_rl_gui *gui;
  char *file;

  gui = WC_PARALLEL_RL_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("rl.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "rl.eps");
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

  fprintf(fp,"(Ls) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Ls/gui->b->units_L->sf, gui->b->units_L->name);
  fprintf(fp,"(Rs) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Rs/gui->b->units_Rs->sf, gui->b->units_Rs->name);
  fprintf(fp,"(Qs) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->b->Qs);

  fprintf(fp,"(frequency) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name);
  fprintf(fp,"newline\n");

  /* Second column of the output */
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");


  fprintf(fp,"(Lp) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Lp/gui->b->units_L->sf, gui->b->units_L->name);
  fprintf(fp,"(Rp) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->Rp/gui->b->units_Rp->sf, gui->b->units_Rp->name);
  fprintf(fp,"(Qp) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->b->Qp);


  fprintf(fp,"newline\n");

}

