/* $Id: bars_gui.c,v 1.32 2005/10/29 19:07:30 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2008 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
#include "gtk-units.h"

#include "misc.h"
#include "units.h"

#include "bars.h"
#include "bars_gui.h"
#include "bars_loadsave.h"
#include "physconst.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

static void print_ps(Wcalc *wcalc,FILE *fp);

static void analyze( GtkWidget *w, gpointer data );
static void synthesize_a( GtkWidget *w, gpointer data );
static void synthesize_b( GtkWidget *w, gpointer data );
static void synthesize_c( GtkWidget *w, gpointer data );
static void synthesize_er( GtkWidget *w, gpointer data );
static void synthesize_len( GtkWidget *w, gpointer data );
static void calculate( bars_gui *gui, GtkWidget *w, gpointer data );
static void update_display( bars_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(bars_gui *gui, GtkWidget *parent);
static void outputs_init(bars_gui *gui, GtkWidget *parent);
static void picture_init(bars_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
static void tooltip_init(bars_gui *gui);


static char *name="Coaxial Transmission Line Analysis/Synthesis";
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
  wcalc->init_done=0;

  wcalc->init = bars_gui_init;
  wcalc->print_ps = print_ps;
  wcalc->load = NULL;
  wcalc->save = gui_save;
  wcalc->analyze = NULL;
  wcalc->synthesize = NULL;
  wcalc->display = NULL;

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

static void wc_table_add(GtkWidget *table, void * gui, const char *text, 
			 GtkWidget **label, wc_units *units, wc_units_gui *ug, int *x, int *y)
{
  GtkWidget *t;

  t = gtk_label_new( text );
  gtk_table_attach(GTK_TABLE(table), t, *x, *x+1, *y, *y+1, 
		   0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(t);
  
  *(label) = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
  gtk_entry_set_text(GTK_ENTRY(*label),"      ");
  gtk_table_attach (GTK_TABLE(table), *label, *x+1, *x+2, *y, *y+1, 
		    0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_set_usize(GTK_WIDGET(*label),WC_WIDTH,0);
  gtk_signal_connect (GTK_OBJECT (*label), "changed",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (*label), "changed",
		      GTK_SIGNAL_FUNC (vals_changedCB), gui);
  gtk_widget_show(*label);

#if 0  
  if( units != NULL) {
    t = wc_units_menu_new(units, WC_WCALC(gui), &ug);
    gtk_table_attach(GTK_TABLE(table), t, 
		     *x+2, *x+3, *y, *y+1, GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
  }
#endif

  (*y)++;
}

static void values_init(bars_gui *gui, GtkWidget *parent)
{
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *table;
  GtkTooltips *tips;
  GtkWidget *text;

  wc_units_gui *ug;

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
  


  /* Analyze button */
  button = gtk_button_new_with_label (_("Analyze->"));
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 6, 7, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate electrical characteristics "
		       "from physical parameters"),
		       NULL);

  
  /* 
   * Synthesize buttons 
   */

  /*
  text = wc_units_menu_new(gui->line->units_xy, WC_WCALC(gui), &ug);
  gtk_table_attach(GTK_TABLE(table), text, x+2, x+3, y, y+1,
		   GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

  wc_units_attach_label(ug, gui->label_loss_ev, &(gui->line->loss_ev), 
			NULL, NULL, WC_FMT_G, 1);
  */
  ug = NULL;

  /* ---------------- Bar 1, width, height, length  -------------- */
  wc_table_add(table, gui, "Bar 1 Width (a)", &(gui->text_a), gui->b->units_xy, ug, &x, &y);
  wc_table_add(table, gui, "Bar 1 Thickness (b)", &(gui->text_b), gui->b->units_xy, ug, &x, &y);
  wc_table_add(table, gui, "Bar 1 Length (l1)", &(gui->text_l1), gui->b->units_z, ug, &x, &y);

  /* ---------------- Bar 2, width, height, length  -------------- */
  wc_table_add(table, gui, "Bar 2 Width (d)", &(gui->text_d), gui->b->units_xy, ug, &x, &y);
  wc_table_add(table, gui, "Bar 2 Thickness (c)", &(gui->text_c), gui->b->units_xy, ug, &x, &y);
  wc_table_add(table, gui, "Bar 2 Length (l2)", &(gui->text_l2), gui->b->units_z, ug, &x, &y);

  /* ---------------- Bar 2, position  -------------- */
  wc_table_add(table, gui, "Bar 2 Width (E)", &(gui->text_E), gui->b->units_xy, ug, &x, &y);
  wc_table_add(table, gui, "Bar 2 Thickness (P)", &(gui->text_P), gui->b->units_xy, ug, &x, &y);
  wc_table_add(table, gui, "Bar 2 Length (l3)", &(gui->text_l3), gui->b->units_z, ug, &x, &y);

  /* ---------------- Frequency -------------- */
  wc_table_add(table, gui, "Frequency", &(gui->text_freq), gui->b->units_freq, ug, &x, &y);

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


  wc_table_add(table, gui, "L1", &(gui->label_L1), gui->b->units_L, ug, &x, &y);
  wc_table_add(table, gui, "L2", &(gui->label_L2), gui->b->units_L, ug, &x, &y);
  wc_table_add(table, gui, "M", &(gui->label_M), gui->b->units_L, ug, &x, &y);
  wc_table_add(table, gui, "k", &(gui->label_k), NULL, NULL, &x, &y);

  /* spacer */

  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);


  gtk_widget_show(table);
  
}


#include "pixmaps/bars_fig.xpm"

static void picture_init(bars_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) bars_fig);
					
  
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
  calculate(WC_BARS_GUI(data), w, "analyze");
}

static void synthesize_a( GtkWidget *w, gpointer data )
{
  calculate(WC_BARS_GUI(data), w, "synthesize_a");
}

static void synthesize_b( GtkWidget *w, gpointer data )
{
  calculate(WC_BARS_GUI(data), w, "synthesize_b");
}

static void synthesize_c( GtkWidget *w, gpointer data )
{
  calculate(WC_BARS_GUI(data), w, "synthesize_c");
}

static void synthesize_er( GtkWidget *w, gpointer data )
{
  calculate(WC_BARS_GUI(data), w, "synthesize_er");
}

static void synthesize_len( GtkWidget *w, gpointer data )
{
  calculate(WC_BARS_GUI(data), w, "synthesize_len");
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
  gui->b->l1=atof(vstr)*wc_units_to_sf(gui->b->units_z);

  /* Bar #2 */

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_d) ); 
  gui->b->d=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_c) ); 
  gui->b->c=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l2) ); 
  gui->b->l2=atof(vstr)*wc_units_to_sf(gui->b->units_z);

  /* Bar #2 position */

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_E) ); 
  gui->b->E=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_P) ); 
  gui->b->P=atof(vstr)*wc_units_to_sf(gui->b->units_xy);

  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_l3) ); 
  gui->b->l3=atof(vstr)*wc_units_to_sf(gui->b->units_z);

  /* Frequency */
  vstr = gtk_entry_get_text( GTK_ENTRY(gui->text_freq) ); 
  gui->b->freq=atof(vstr)*wc_units_to_sf(gui->b->units_freq);

  /* XXX should use an enum and switch... */
  if( strcmp(data,"analyze")==0) {
    rslt = bars_calc(gui->b, gui->b->freq);
  }
#ifdef FIXEM
  else if( strcmp(data,"synthesize_a")==0) {
    rslt = bars_syn(gui->b,gui->b->freq, BARSSYN_A);
  }
  else if( strcmp(data,"synthesize_b")==0) {
    rslt = bars_syn(gui->b,gui->b->freq, BARSSYN_B);
  }
  else if( strcmp(data,"synthesize_c")==0) {
    rslt = bars_syn(gui->b,gui->b->freq, BARSSYN_C);
  }
  else if( strcmp(data,"synthesize_er")==0) {
    rslt = bars_syn(gui->b,gui->b->freq, BARSSYN_ER);
  }
#endif
  else{
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
  if (rslt==0)
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), "");
  else
    gtk_label_set_text(GTK_LABEL(WC_WCALC(gui)->text_status), _("Values out of\nrange."));

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
  sprintf(str,WC_FMT_G,gui->b->l1/wc_units_to_sf(gui->b->units_z));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l1), str );


  /* ---------------- d -------------- */
  sprintf(str,WC_FMT_G,gui->b->d/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_d), str );
  
  /* ---------------- c -------------- */
  sprintf(str,WC_FMT_G,gui->b->c/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_c), str );
  
  /* ---------------- l2 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l2/wc_units_to_sf(gui->b->units_z));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l2), str );


  /* ---------------- E -------------- */
  sprintf(str,WC_FMT_G,gui->b->E/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_E), str );
  
  /* ---------------- P -------------- */
  sprintf(str,WC_FMT_G,gui->b->P/wc_units_to_sf(gui->b->units_xy));
  gtk_entry_set_text( GTK_ENTRY(gui->text_P), str );
  
  /* ---------------- l3 -------------- */
  sprintf(str,WC_FMT_G,gui->b->l3/wc_units_to_sf(gui->b->units_z));
  gtk_entry_set_text( GTK_ENTRY(gui->text_l3), str );

  /* ---------------- freq -------------- */
  sprintf(str,WC_FMT_G,gui->b->freq/wc_units_to_sf(gui->b->units_freq));
  gtk_entry_set_text( GTK_ENTRY(gui->text_freq), str );

}


static void tooltip_init(bars_gui *gui)
{
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_a, _("Width of bar #1"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_b, _("Thickness of bar #1"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_l1, _("Length of bar #1"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_d, _("Width of bar #2"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_c, _("Thickness of bar #2"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_l2, _("Length of bar #2"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_E, _("Offset position of bar #2 in the width direction"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_P, _("Offset position of bar #2 in the vertical direction"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_l3, _("Offset position of bar #2 in the length direction"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_freq, _("Frequency of operation"), NULL);
}

static void gui_save(Wcalc *wcalc, FILE *fp, char *name)
{
#ifdef DEBUG
  g_print(_("bars_gui.c:gui_save():  wcalc=%p, fp=%p, name=%p\n"),
	  wcalc,fp,name);
#endif
  bars_save(WC_BARS_GUI(wcalc)->b,fp,name);
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  bars_gui *gui;
  char *file;

  gui = WC_BARS_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("bars.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "bars.eps");
  eps_cat(file,fp);

  /* print the data */

  fprintf(fp,"%% spit out the numbers\n");
  fprintf(fp,"newb\n");
  fprintf(fp,"newb\n");
  fprintf(fp,"newb\n");
  fprintf(fp,"/col1x currentpoint pop def\n");
  fprintf(fp,"/col2x %g 2 div inch def\n", global_print_config->paperwidth);
  fprintf(fp,"/coly currentpoint exch pop def\n");
  fprintf(fp,"/bspace 1.5 def\n");
  fprintf(fp,"\n");
  fprintf(fp,"col1x coly moveto\n");
  fprintf(fp,"/leftcol col1x  def\n");

  fprintf(fp,"(a) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->a/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(b) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->b/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(l1) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->l1/gui->b->units_z->sf, gui->b->units_z->name);

  fprintf(fp,"(d) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->d/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(c) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->c/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(l2) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->l2/gui->b->units_z->sf, gui->b->units_z->name);

  fprintf(fp,"(E) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->E/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(P) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->P/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(l3) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->l3/gui->b->units_z->sf, gui->b->units_z->name);

  fprintf(fp,"(frequency) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name);
  fprintf(fp,"newb\n");

  /* Second column of the output */
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");


  fprintf(fp,"(L1) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->L1/gui->b->units_L->sf, gui->b->units_L->name);
  fprintf(fp,"(L2) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->L2/gui->b->units_L->sf, gui->b->units_L->name);
  fprintf(fp,"(M) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newb\n",
	  gui->b->M/gui->b->units_L->sf, gui->b->units_L->name);

  fprintf(fp,"(k) show tab1 (=) show tab2 (" WC_FMT_G ") show newb\n",
	  gui->b->k);


  fprintf(fp,"newb\n");

}

