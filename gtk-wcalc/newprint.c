/* $Id: newprint.c,v 1.8 2009/02/04 01:55:23 dan Exp $ */

/*
 * Copyright (c) 2009 Dan McMahill
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

/*
 * Note:  The gtkprint example from the gtk-demo program was used
 * as a starting point for the code here.  
 */

/* #define DEBUG */

#include "config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <gtk/gtk.h>

#include "newprint.h"
#include "render_figures.h"
#include "units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

 	
/* In points */
#define HEADER_BORDER 8
#define HEADER_GAP (3*72/25.4)
 	
typedef struct
{
  Wcalc *wcalc;

  gdouble font_size;
 	
  gint num_pages;

  GList * print_list;
  GList * paginations;

} PrintData;


static GtkPrintSettings *settings = NULL;
	
static void
begin_print (GtkPrintOperation *operation,
	     GtkPrintContext *context,
	     gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;
  gdouble height;
  Wcalc *w;

  /* 
   * get the height in points (we set the units to points in
   * do_print())
   */

  /* FIXME -- how do we know what the header height is??? */
  #define HEADER_HEIGHT 150
  height = gtk_print_context_get_height (context) - HEADER_HEIGHT - HEADER_GAP;
  #undef HEADER_HEIGHT


  /*
   * Next we build up a list of strings where each string
   * represents 1 line of text on our page.
   *
   * We get the list of strings by first obtaining a GList of
   * a combination of a string like "width of trace", a numerical
   * value (double, int, or string), and a pointer to wcalc units
   */
  w = data->wcalc;
  data->print_list = NULL;
  if(w->dump_values != NULL) {
    data->print_list = w->dump_values(w);
  }

  data->paginations = NULL;
  data->paginations = g_list_append(data->paginations, 0);
  data->paginations = g_list_append(data->paginations, g_list_length(data->print_list) );


  /*
   *  figure out the number of pages (probably always 1 for this
   * program) FIXME
   */
  data->num_pages = 1;
  
  /*
   * Tell the print_operation how many pages because it will
   *   a) fill in the # of pages in the dialog and
   *   b) call draw_page() once for each page.
   */
  gtk_print_operation_set_n_pages (operation, data->num_pages);
}

 	
static void
draw_page (GtkPrintOperation *operation,
	   GtkPrintContext *context,
	   gint page_nr,
	   gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;
  cairo_t *cr;
  PangoLayout *layout;
  gint text_width, text_height, header_height;
  gdouble width, height;
  gint i;
  PangoFontDescription *desc;
  gchar *page_str;
  PrintValue *pv;
  double sf;
  char *units_name;
  gchar *text = NULL, *text2 = NULL;
  double x, y;

  /* get the cairo context for our page */
  cr = gtk_print_context_get_cairo_context (context);

  /* figure out width and height */
  width = gtk_print_context_get_width (context);
  height = gtk_print_context_get_height (context);
  

  /*
   * *********************************************************************
   *  print out the name and version of this program 
   * *********************************************************************
   */
  layout = gtk_print_context_create_pango_layout (context);

  /* pick the font and set the font */
  desc = pango_font_description_from_string ("sans 18");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
 	

  /* create the string and put it in our layout */
  page_str = g_strdup_printf ("<markup><b>Wcalc</b> Transmission Line Analysis/Synthesis\n"
			      "<span size=\"smaller\">Version %s</span>\n"
			      "%s\n"
			      "<span size=\"smaller\">Model Version %s</span>"
			      "</markup>", VERSION, data->wcalc->model_name,
			      data->wcalc->model_version);


  pango_layout_set_markup(layout, page_str, -1);
  g_free(page_str);

  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  header_height = text_height + 2.0 * HEADER_BORDER;


  /*
   * Create a shaded rectangle across the top of the page.  It is the
   * full width of the printable area.  
   *
   */
  

  /* define the rectangle */
  cairo_rectangle (cr, 0, 0, width, header_height);

  /* fill it */
  cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
  cairo_fill_preserve (cr);
 	
  /* and put a black line around the rectangle */
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1);
  cairo_stroke (cr);

  /* and finally add the header text */
  cairo_move_to (cr, (width - text_width) / 2, HEADER_BORDER);
  pango_cairo_show_layout (cr, layout);


  page_str = g_strdup_printf ("%d/%d", page_nr + 1, data->num_pages);
  pango_layout_set_text (layout, page_str, -1);
  g_free (page_str);
 	
  pango_layout_set_width (layout, -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  cairo_move_to (cr, width - text_width - 4, (header_height - text_height) / 2);
  pango_cairo_show_layout (cr, layout);

  g_object_unref (layout);
 	

  /*
   * *********************************************************************
   *  put down an advertisement at the bottom
   * *********************************************************************
   */
  layout = gtk_print_context_create_pango_layout (context);

  /* pick the font and set the font */
  desc = pango_font_description_from_string ("sans 10");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
 	

  /* create the string and put it in our layout */
  page_str = g_strdup_printf ("<markup><i>http://wcalc.sf.net</i>\n%s</markup>",
			      data->wcalc->file_name != NULL ?
			      data->wcalc->file_name : "untitled");

  pango_layout_set_markup(layout, page_str, -1);
  g_free(page_str);

  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);

  /* define the rectangle */
  cairo_rectangle (cr, 0, height - text_height, width, text_height);

  /* fill it */
  cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
  cairo_fill_preserve (cr);
 	
  /* and put a black line around the rectangle */
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1);
  cairo_stroke (cr);

  /* and finally add the header text */
  cairo_move_to (cr, (width - text_width) / 2, height - text_height);
  pango_cairo_show_layout (cr, layout);

  g_object_unref (layout);
 	
  cairo_move_to (cr, x, y);

  /*
   * *********************************************************************
   *  print out the data
   * *********************************************************************
   */

  /* Now set up the pango layout for the text portion of the output */
  layout = gtk_print_context_create_pango_layout (context);

  /* pick the font and font size */
  /* monospace, sans-serif, serif */
  desc = pango_font_description_from_string ("serif");

  /* A size value of 10 * PANGO_SCALE is a 10 point font. */
  pango_font_description_set_size (desc, data->font_size * PANGO_SCALE);
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
 	
  pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);

  /* FIXME -- smarter scaling here */
#define TEXT_COLUMN_WIDTH 5.0
#define TEXT_COLUMN_INDENT -0.5

  pango_layout_set_width (layout, pango_units_from_double(TEXT_COLUMN_WIDTH*72.0));
  pango_layout_set_indent (layout, pango_units_from_double(TEXT_COLUMN_INDENT*72.0));

  /* turn on wrapping of long lines */
  pango_layout_set_wrap (layout, PANGO_WRAP_WORD_CHAR);

  /* Position ourselves below the header */
  cairo_move_to (cr, 0, header_height + HEADER_GAP);

  /* now print out each line for this page */
  for(i = g_list_nth_data(data->paginations, page_nr) ;
      i < g_list_nth_data(data->paginations, page_nr + 1) ;
      i++) {

    /* Get the next bit of data to print */
    pv = g_list_nth_data(data->print_list, i);
    if( pv->units != NULL ) {
      sf = pv->units->sf;
      units_name = wc_units_to_str(pv->units);
    } else {
      sf = 1.0;
      units_name = strdup( "" );
    }

    /* 
     * FIXME -- Some sort of
     * tabs to align all the "=" would be nice as well as a way to
     * use greek letters for some of the units.
     */
    switch(pv->type) {
    case FLOAT:
      text = g_strdup (pv->name);
      text2 = g_strdup_printf("= %g %s", pv->val.fval/sf, units_name);
      break;

    case INT:
      text = g_strdup (pv->name);
      text2 = g_strdup_printf("= %d %s", (int) (pv->val.ival/sf), units_name);
      break;

    case STRING:
      text = g_strdup (pv->name);
      text2 = g_strdup_printf("= %s %s", pv->val.sval, units_name);
      break;

    case CAIRO:
      text = g_strdup_printf("[CAIRO FIGURE HERE]");
      break;

    }

    free(units_name);

    if(pv->type == CAIRO) {
      cairo_get_current_point (cr, &x, &y);
      cairo_save (cr);
      /*
       * The bounding box for our figure has its top left corner at
       * 0,0 so we need to translate over by half the width of the
       * page minus half the figure width and down by the current
       * position on our page.  This results in a figure which is
       * in line vertically and centered horizontally.
       */
      cairo_translate (cr, width/2 - pv->width/2, y);

      /* render the figure */
      pv->val.cairoval(NULL, cr);

      /* 
       * restore to undo the translation and move our current point
       * down
       */
      cairo_restore (cr);
      cairo_move_to (cr, 0, y + pv->height);

    } else {
      cairo_get_current_point (cr, &x, &y);

      pango_layout_set_text (layout, text, -1);
      pango_cairo_show_layout (cr, layout);
      pango_layout_get_pixel_size (layout, &text_width, &text_height);

      cairo_rel_move_to (cr, (TEXT_COLUMN_WIDTH+0.25)*72.0, 0);
      pango_layout_set_text (layout, text2, -1);
      pango_cairo_show_layout (cr, layout);
      cairo_move_to (cr, x, y);

      /* moves back to x=y and move down by one line */
      cairo_rel_move_to (cr, 0, text_height);
      cairo_get_current_point (cr, &x, &y);

    }
    g_free(text);
      
  }

  g_object_unref (layout);
}
 	
static void
end_print (GtkPrintOperation *operation,
	   GtkPrintContext *context,
	   gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;
 	
  g_free (data);
}
 	



GtkWidget *
do_printing (GtkWidget *do_widget, Wcalc *wcalc)
{
  GtkPrintOperation *operation;
  GtkPrintOperationResult res;
  PrintData *data;
  GError *error = NULL;
 	
  operation = gtk_print_operation_new ();

  
  /* load any print settings from the last time */
  if (settings != NULL)
    gtk_print_operation_set_print_settings (operation, settings);
 

  /* 
   * store some user data, in particular a pointer to our current
   * wcalc gui.  That will let the callbacks get at all of the
   * data needed to render the outputs.
   */
  data = g_new0 (PrintData, 1);
  data->wcalc = wcalc;
  data->font_size = 12.0;
 	
  /* Hook up the callbacks */


  /* 
   * Emitted after the user has finished changing print settings in
   * the dialog, before the actual rendering starts.  Use for pagination.
   */
  g_signal_connect (G_OBJECT (operation), "begin-print",
		    G_CALLBACK (begin_print), data);


  /*
   * Emitted for each page to be rendered
   */
  g_signal_connect (G_OBJECT (operation), "draw-page",
		    G_CALLBACK (draw_page), data);

  /*
   * Emitted after all pages have been rendered.  Use to clean
   * up after begin-print.
   */
  g_signal_connect (G_OBJECT (operation), "end-print",
		    G_CALLBACK (end_print), data);

 	
  /* only use the imagable area (inside the margins */
  gtk_print_operation_set_use_full_page (operation, FALSE);

  /*
   * sets the transformation for the cairo context so we use these
   * units
   */
  gtk_print_operation_set_unit (operation, GTK_UNIT_POINTS);


  /* Run the print dialog */
  res = gtk_print_operation_run (operation, 
				 GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, 
				 GTK_WINDOW (do_widget), &error);
 	

  /*
   * If the user didn't just cancel the print, then store the print
   * settings for next time.
   */
  if (res == GTK_PRINT_OPERATION_RESULT_APPLY) {
    if (settings != NULL) {
      g_object_unref (settings); 
    }
    settings = g_object_ref (gtk_print_operation_get_print_settings (operation));
  }

  g_object_unref (operation);
 	

  /* If there was an error printing, display it */
  if (error) {
    GtkWidget *dialog;
    
    dialog = gtk_message_dialog_new (GTK_WINDOW (do_widget),
				     GTK_DIALOG_DESTROY_WITH_PARENT,
				     GTK_MESSAGE_ERROR,
				     GTK_BUTTONS_CLOSE,
				     "%s", error->message);
    g_error_free (error);
    
    g_signal_connect (dialog, "response",
		      G_CALLBACK (gtk_widget_destroy), 
		      NULL);
    
    gtk_widget_show (dialog);
  }
  
  
  return NULL;
}


/*
 * data is the Wcalc *
 * action is whats specified in menus.c
 * widget is the GtkMenuItem *
 */
void newprint_popup(gpointer data,
		    guint action,
		    GtkWidget *widget)
{
  Wcalc * wcalc;

  wcalc = WC_WCALC(data);

  if(wcalc->values_in_sync == FALSE) {

    alert("The input and output values are out of sync\n"
	  "Before printing, please press the analyze button\n"
	  "or one of the synthesis buttons in the calculator\n"
	  "to make the input and output values be consistent\n");
  } else {
    do_printing(wcalc->window, wcalc);
  }
}


GList * wc_print_add_double(gchar * name, double val, wc_units *units, GList *list)
{
  PrintValue *v;

  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( name );
  v->val.fval = val;
  v->type = FLOAT;
  v->units = units;
  v->width = 0;
  v->height = 0;

  return g_list_append(list, v);
}


GList * wc_print_add_int(gchar * name, int val, wc_units *units, GList *list)
{
  PrintValue *v;

  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( name );
  v->val.ival = val;
  v->type = INT;
  v->units = units;
  v->width = 0;
  v->height = 0;
  
  return g_list_append(list, v);
}

GList * wc_print_add_string(gchar * name, gchar * val, wc_units *units, GList *list)
{
  PrintValue *v;

  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( name );
  v->val.sval = g_strdup( val );
  v->type = STRING;
  v->units = units;
  v->width = 0;
  v->height = 0;
  
  return g_list_append(list, v);
}

GList * wc_print_add_cairo(cairo_t * (*fn)(cairo_surface_t *cs, cairo_t *cr), 
			   int width, int height, GList *list)
{
  PrintValue *v;

  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( "Cairo Figure" );
  v->val.cairoval = fn;
  v->type = CAIRO;
  v->units = NULL;
  v->width = width;
  v->height = height;

  return g_list_append(list, v);
}

void wc_print_value_free(PrintValue * val)
{
  if( val == NULL) {
    return;
  }

  if( val->name != NULL ) {
    g_free( val->name );
  }

  if( val->type == STRING && val->val.sval != NULL ) {
    g_free(val->val.sval);
  }

  free( val );
}

