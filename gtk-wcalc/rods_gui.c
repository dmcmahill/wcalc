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
#include "epscat.h"
#include "menus.h"
#include "gtk-units.h"

#include "misc.h"
#include "units.h"

#if GTK_CHECK_VERSION(2,10,0)
#include "pixmaps/figure_rods_fig.h"
#endif
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

static void print_ps(Wcalc *wcalc,FILE *fp);
static GList * dump_values(Wcalc *wcalc);

static void analyze( GtkWidget *w, gpointer data );
static void calculate( rods_gui *gui, GtkWidget *w, gpointer data );
static void update_display( rods_gui *gui);

static void gui_save(Wcalc *wcalc, FILE *fp, char *name);

static void values_init(rods_gui *gui, GtkWidget *parent);
static void outputs_init(rods_gui *gui, GtkWidget *parent);
static void picture_init(rods_gui *gui,
			 GtkWidget *window,
			 GtkWidget *parent);
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
  wcalc->init_done=0;

  wcalc->init = rods_gui_init;
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
  GtkTooltips *tips;

  wc_units_gui *xy_ug = NULL, *freq_ug = NULL, *rho_ug = NULL;

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
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
  gtk_signal_connect (GTK_OBJECT (button), "clicked",
		      GTK_SIGNAL_FUNC (analyze), (gpointer)
		      gui);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0,
		   GTK_EXPAND|GTK_FILL,WC_XPAD,WC_YPAD);
  gtk_tooltips_set_tip(tips, button, 
		       _("Calculate electrical characteristics "
		       "from physical parameters"),
		       NULL);
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
  gtk_signal_connect (GTK_OBJECT (gui->text_l1), "changed",
		      GTK_SIGNAL_FUNC (sync_l2_to_l1), (gpointer) gui);

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


  wc_table_add_label_new_units(table, gui, "R1", 
			       &(gui->label_R1), gui->b->units_R, &ug_R, 
			       &(gui->b->R1), &x, &y);

  wc_table_add_label_attach_units(table, gui, "R2", 
				  &(gui->label_R2), gui->b->units_R, &ug_R, 
				  &(gui->b->R2), &x, &y);
  /* spacer */

  text = gtk_label_new( "                " );
  gtk_table_attach(GTK_TABLE(table), text, 3, 4, 0, 1, 
		   GTK_EXPAND|GTK_FILL, 0,
		   WC_XPAD,WC_YPAD);
  gtk_widget_show(text);

  gtk_widget_show(table);
  
}


#include "pixmaps/rods_fig.xpm"

static void picture_init(rods_gui *gui, GtkWidget *window,GtkWidget *parent)
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
					 (gchar **) rods_fig);
					
  
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
  GtkTooltips *tips;

  tips = gtk_tooltips_new();

  gtk_tooltips_set_tip(tips, gui->text_d1, _("Diameter of wire #1"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_l1, _("Length of wire #1"), NULL);

  gtk_tooltips_set_tip(tips, gui->text_d2, _("Diameter of wire #2"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_l2, _("Length of wire #2\nConstrained to be the same as wire #1 in this release."), NULL);

  gtk_tooltips_set_tip(tips, gui->text_distance, _("Offset position of wire #2 in the radial direction"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_offset, _("Offset position of wire #2 in the axial direction.\nNot supported yet."), NULL);

  gtk_tooltips_set_tip(tips, gui->text_rho, _("Bulk resistivity of wires"), NULL);
  gtk_tooltips_set_tip(tips, gui->text_freq, _("Frequency of operation"), NULL);

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
#if GTK_CHECK_VERSION(2,10,0)
  rods_gui *gui;
  rods * b;

  gui = WC_RODS_GUI(wcalc);
  b = gui->b;

  /* Initialize the graphics */
  if( list == NULL ) {
    figure_rods_fig_init();
  }
  // FIXME -- free the old list first!!!!
  list = NULL;
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

#endif

  return list;
}


static void print_ps(Wcalc *wcalc, FILE *fp)
{
  rods_gui *gui;
  char *file;

  gui = WC_RODS_GUI(wcalc);

  /* print the EPS file */

  file=g_malloc( (strlen(global_print_config->eps_dir)+strlen("rods_fig.eps")+2)*sizeof(char));
  sprintf(file,"%s%c%s",global_print_config->eps_dir,
	  global_print_config->dir_sep,
	  "rods_fig.eps");
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

  fprintf(fp,"(d1) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->d1/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(l1) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->l1/gui->b->units_xy->sf, gui->b->units_xy->name);

  fprintf(fp,"(d2) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->d2/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(l2) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->l2/gui->b->units_xy->sf, gui->b->units_xy->name);

  fprintf(fp,"(distance) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->distance/gui->b->units_xy->sf, gui->b->units_xy->name);
  fprintf(fp,"(offset) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->offset/gui->b->units_xy->sf, gui->b->units_xy->name);

  fprintf(fp,"(rho) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->rho/gui->b->units_rho->sf, gui->b->units_rho->name);

  fprintf(fp,"(frequency) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->freq/gui->b->units_freq->sf, gui->b->units_freq->name);
  fprintf(fp,"newline\n");

  /* Second column of the output */
  fprintf(fp,"\n");
  fprintf(fp,"col2x coly moveto \n");
  fprintf(fp,"/leftcol col2x def\n");


  fprintf(fp,"(L1) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->L1/gui->b->units_L->sf, gui->b->units_L->name);
  fprintf(fp,"(L2) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->L2/gui->b->units_L->sf, gui->b->units_L->name);
  fprintf(fp,"(M) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->M/gui->b->units_L->sf, gui->b->units_L->name);

  fprintf(fp,"(k) show tab1 (=) show tab2 (" WC_FMT_G ") show newline\n",
	  gui->b->k);

  fprintf(fp,"(R1) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->R1/gui->b->units_R->sf, gui->b->units_R->name);
  fprintf(fp,"(R2) show tab1 (=) show tab2 (" WC_FMT_G " %s) show newline\n",
	  gui->b->R2/gui->b->units_R->sf, gui->b->units_R->name);

  fprintf(fp,"newline\n");

}

