/* $Id: newprint.c,v 1.2 2009/01/13 20:35:18 dan Exp $ */

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
#include "units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

 	
/* In points */
#define HEADER_HEIGHT (10*72/25.4)
#define HEADER_GAP (3*72/25.4)
 	
typedef struct
{
  Wcalc *wcalc;

  gchar *filename;
  gdouble font_size;
 	
  gint lines_per_page;
  gchar **lines;
  gint num_lines;
  gint num_pages;
} PrintData;
 	
static void
begin_print (GtkPrintOperation *operation,
	     GtkPrintContext *context,
	     gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;
  char *contents;
  int i;
  double height;
  Wcalc *w;
  GList *list;
  guint nv, j;
  PrintValue *pv;

  height = gtk_print_context_get_height (context) - HEADER_HEIGHT - HEADER_GAP;
 	
  data->lines_per_page = floor (height / data->font_size);
 	
  w = data->wcalc;
  list = NULL;
  if(w->dump_values != NULL) {
    list = w->dump_values(w);
  }

  g_print("%s():  list = %p\n", __FUNCTION__, list);

  nv = g_list_length( list );
  data->lines = (gchar **) g_malloc((nv + 1) * sizeof(gchar *));
  if( data->lines == NULL ) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  data->lines[nv] = NULL;
  for(j = 0 ; j < nv; j++) {
    pv = g_list_nth_data(list, j);
    g_print("list[%d]. Type = %d = %p\n", j, pv->type, pv);
    switch(pv->type) {
    case FLOAT:
      data->lines[j] = g_strdup_printf("%s = %g", pv->name, pv->val.fval);
      break;

    case INT:
      data->lines[j] = g_strdup_printf("%s = %d", pv->name, pv->val.ival);
      break;

    case STRING:
      data->lines[j] = g_strdup_printf("%s = %s", pv->name, pv->val.sval);
      break;
    }
  }
  //g_file_get_contents (data->filename, &contents, NULL, NULL);
  //contents = g_strdup_printf("hello\nthis is a test\nof gtkprint\nI hope I can use it\n");
  
  //data->lines = g_strsplit (contents, "\n", 0);
  //g_free (contents);
 	
  //i = 0;
  //while (data->lines[i] != NULL)
  //i++;
 	
  //data->num_lines = i;

  data->num_lines = nv;
  data->num_pages = (data->num_lines - 1) / data->lines_per_page + 1;
 	
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
  gint text_width, text_height;
  gdouble width, height;
  gint line, i;
  PangoFontDescription *desc;
  gchar *page_str;
 	
  cr = gtk_print_context_get_cairo_context (context);
  width = gtk_print_context_get_width (context);
  height = gtk_print_context_get_height (context);
  
  g_print("%s():  width = %g, height = %g\n", __FUNCTION__, width, height);

  cairo_rectangle (cr, 0, 0, width, HEADER_HEIGHT);
 	
  cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
  cairo_fill_preserve (cr);
 	
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1);
  cairo_stroke (cr);
 	
  layout = gtk_print_context_create_pango_layout (context);
 	
  desc = pango_font_description_from_string ("sans 14");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
 	
  pango_layout_set_text (layout, data->filename, -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
 	
  if (text_width > width)
    {
      pango_layout_set_width (layout, width);
      pango_layout_set_ellipsize (layout, PANGO_ELLIPSIZE_START);
      pango_layout_get_pixel_size (layout, &text_width, &text_height);
    }
 	
  g_print("%s():  text_width = %d, text_height = %d\n", __FUNCTION__, text_width, text_height);

  cairo_move_to (cr, (width - text_width) / 2, 
		 (HEADER_HEIGHT - text_height) / 2);
  pango_cairo_show_layout (cr, layout);
 	
  page_str = g_strdup_printf ("%d/%d", page_nr + 1, data->num_pages);
  pango_layout_set_text (layout, page_str, -1);
  g_free (page_str);
 	
  pango_layout_set_width (layout, -1);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);
  cairo_move_to (cr, width - text_width - 4, (HEADER_HEIGHT - text_height) / 2);
  pango_cairo_show_layout (cr, layout);
 	
  g_object_unref (layout);
 	

  /* Now set up the pango layout for the text portion of the output */
  layout = gtk_print_context_create_pango_layout (context);

  /* pick the font and font size */
  desc = pango_font_description_from_string ("monospace");
  pango_font_description_set_size (desc, data->font_size * PANGO_SCALE);
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);
 	

  /* now print out each line of text for this page */
  cairo_move_to (cr, 0, HEADER_HEIGHT + HEADER_GAP);
  line = page_nr * data->lines_per_page;
  for (i = 0; i < data->lines_per_page && line < data->num_lines; i++)
    {
      pango_layout_set_text (layout, data->lines[line], -1);
      pango_cairo_show_layout (cr, layout);

      /* moves back to x=y and move down by one line */
      cairo_rel_move_to (cr, 0, data->font_size);
      line++;
    }
 	
  g_object_unref (layout);
}
 	
static void
end_print (GtkPrintOperation *operation,
	   GtkPrintContext *context,
	   gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;
 	
  g_free (data->filename);
  g_strfreev (data->lines);
  g_free (data);
}
 	



GtkWidget *
do_printing (GtkWidget *do_widget, Wcalc *wcalc)
{
  GtkPrintOperation *operation;
  PrintData *data;
  GError *error = NULL;
 	
  operation = gtk_print_operation_new ();
  data = g_new0 (PrintData, 1);
  //data->filename = demo_find_file ("printing.c", NULL);
  data->filename = g_strdup(wcalc->model_name);
  data->wcalc = wcalc;

  data->font_size = 12.0;
 	
  g_signal_connect (G_OBJECT (operation), "begin-print",
		    G_CALLBACK (begin_print), data);
  g_signal_connect (G_OBJECT (operation), "draw-page",
		    G_CALLBACK (draw_page), data);
  g_signal_connect (G_OBJECT (operation), "end-print",
		    G_CALLBACK (end_print), data);
 	
  gtk_print_operation_set_use_full_page (operation, FALSE);
  gtk_print_operation_set_unit (operation, GTK_UNIT_POINTS);
 	
  gtk_print_operation_run (operation, 
			   GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, 
			   GTK_WINDOW (do_widget), &error);
 	
  g_object_unref (operation);
 	
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

  printf("%s(%p, %d, %p)\n", __FUNCTION__, data, action, widget);

  do_printing(wcalc->window, wcalc);
  
}


GList * wc_add_double_print(gchar * name, double val, wc_units *units, GList *list)
{
  PrintValue *v;

  g_print("%s(%s, %g, %p, %p)\n", __FUNCTION__, name, val, units, list);

  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( name );
  v->val.fval = val;
  v->type = FLOAT;
  v->units = units;
  
  return g_list_append(list, v);
}


GList * wc_add_int_print(gchar * name, int val, wc_units *units, GList *list)
{
  PrintValue *v;

  g_print("%s(%s, %d, %p, %p)\n", __FUNCTION__, name, val, units, list);
  
  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( name );
  v->val.ival = val;
  v->type = INT;
  v->units = units;
  
  return g_list_append(list, v);
}

GList * wc_add_string_print(gchar * name, gchar * val, wc_units *units, GList *list)
{
  PrintValue *v;

  g_print("%s(\"%s\", \"%s\", %p, %p)\n", __FUNCTION__, name, val, units, list);

  v = (PrintValue *) g_malloc(sizeof(PrintValue));
  if (v == NULL) {
    fprintf(stderr, "%s():  malloc failed\n", __FUNCTION__);
    exit(1);
  }

  v->name = g_strdup( name );
  v->val.sval = g_strdup( val );
  v->type = STRING;
  v->units = units;
  
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

