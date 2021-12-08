/*
 * Copyright (c) 2009, 2020, 2021 Dan McMahill
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

#include <glib.h>
#include <gtk/gtk.h>

#include "alert.h"
#include "newprint.h"
#include "units.h"
#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

/* In points */
#define HEADER_BORDER 8
#define HEADER_GAP (3*72/25.4)

#define MARGIN_LEFT (0.5 * 72.0)
#define MARGIN_RIGHT (0.5 * 72.0)
#define MARGIN_TOP (0.5 * 72.0)
#define MARGIN_BOT (0.5 * 72.0)

typedef struct
{
  Wcalc *wcalc;

  gdouble font_size;

  gint num_pages;

  GList * print_list;
  GList * paginations;

  gchar *date_string;

} PrintData;


static GtkPrintSettings *settings = NULL;
static GtkPageSetup *page_setup = NULL;

static void
begin_print (GtkPrintOperation *operation,
	     GtkPrintContext *context,
	     gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;
  Wcalc *w;
  time_t currenttime;

  currenttime = time (NULL);
  data->date_string = g_strdup (asctime (localtime (&currenttime)));

  w = data->wcalc;
  data->print_list = NULL;
  if(w->dump_values != NULL) {
    data->print_list = w->dump_values(w);
  }

  data->paginations = NULL;
  data->paginations = g_list_append(data->paginations, GINT_TO_POINTER(0));
  data->paginations = g_list_append(data->paginations, GINT_TO_POINTER(g_list_length(data->print_list)) );


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
  double xx = 0.0, yy = 0.0;

#ifdef DEBUG
  g_print ("%s(%p, %p, %d, %p)\n", __FUNCTION__, operation, context, page_nr, user_data);
#endif

  /* get the cairo context for our page */
  cr = gtk_print_context_get_cairo_context (context);
#ifdef DEBUG
  g_print ("%s():  cairo context is %p\n", __FUNCTION__, cr);
#endif

  /*
   * figure out width and height
   * in points (we set the units to points in
   * do_print())
   */
  width = gtk_print_context_get_width (context);
  height = gtk_print_context_get_height (context);


#ifdef DEBUG
  g_print ("gtk_print_context is %g x %g = %g x %g inches\n",
	   width, height, width / 72.0, height / 72.0);
#endif
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
  cairo_rectangle (cr, MARGIN_LEFT, 0, width - MARGIN_LEFT - MARGIN_RIGHT, header_height);

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
  cairo_move_to (cr, width - text_width - 4 - MARGIN_RIGHT, (header_height - text_height) / 2);
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
  page_str = g_strdup_printf ("<markup><b>%s</b>\n%s<i>http://wcalc.sf.net</i></markup>",
			      data->wcalc->file_filename != NULL ?
			      data->wcalc->file_filename : "untitled",
			      data->date_string);

  pango_layout_set_markup(layout, page_str, -1);
  g_free(page_str);

  pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
  pango_layout_get_pixel_size (layout, &text_width, &text_height);

  /* define the rectangle */
  cairo_rectangle (cr, MARGIN_LEFT, height - text_height, width - MARGIN_LEFT - MARGIN_RIGHT, text_height);

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

  cairo_move_to (cr, xx, yy);

  /*
   * *********************************************************************
   *  print out the data
   * *********************************************************************
   */

  /* pick the font and font size */
  /* monospace, sans-serif, serif */
  desc = pango_font_description_from_string ("serif");

  /* A size value of 10 * PANGO_SCALE is a 10 point font. */
  pango_font_description_set_size (desc, data->font_size * PANGO_SCALE);

  /* Position ourselves below the header */
  cairo_move_to (cr, MARGIN_LEFT, header_height + HEADER_GAP);

  /* now print out each line for this page */
  for(i = GPOINTER_TO_INT(g_list_nth_data(data->paginations, page_nr)) ;
      i < GPOINTER_TO_INT(g_list_nth_data(data->paginations, page_nr + 1)) ;
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
      text = g_strdup_printf ("<markup>%s</markup>", pv->name);
      text2 = g_strdup_printf("= %g %s", pv->val.fval/sf, units_name);
      break;

    case INT:
      text = g_strdup_printf ("<markup>%s</markup>", pv->name);
      text2 = g_strdup_printf("= %d %s", (int) (pv->val.ival/sf), units_name);
      break;

    case STRING:
      text = g_strdup_printf ("<markup>%s</markup>", pv->name);
      text2 = g_strdup_printf("= %s %s", pv->val.sval, units_name);
      break;

    case CAIRO:
      text = g_strdup_printf("<markup>[CAIRO FIGURE HERE]</markup>");
      break;

    }

    free(units_name);

    if(pv->type == CAIRO) {
      cairo_get_current_point (cr, &xx, &yy);
      cairo_save (cr);
      /*
       * The bounding box for our figure has its top left corner at
       * 0,0 so we need to translate over by half the width of the
       * page minus half the figure width and down by the current
       * position on our page.  This results in a figure which is
       * in line vertically and centered horizontally.
       */
      cairo_translate (cr, width/2 - pv->width/2, yy);

      /* render the figure */
      pv->val.cairoval(NULL, cr);

      /*
       * restore to undo the translation and move our current point
       * down
       */
      cairo_restore (cr);
      cairo_move_to (cr, MARGIN_LEFT, yy + pv->height);

    } else {
      cairo_get_current_point (cr, &xx, &yy);
      layout = gtk_print_context_create_pango_layout (context);
      pango_layout_set_font_description (layout, desc);
      pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);

      /* FIXME -- smarter scaling here */
#define TEXT_COLUMN_WIDTH 4.0
#define TEXT_COLUMN_INDENT -0.5

      pango_layout_set_width (layout,
			      pango_units_from_double(TEXT_COLUMN_WIDTH*72.0));
      pango_layout_set_indent (layout,
			       pango_units_from_double(TEXT_COLUMN_INDENT*72.0));

      pango_layout_set_wrap (layout, PANGO_WRAP_WORD_CHAR);


      pango_layout_set_markup (layout, text, -1);
      pango_layout_get_pixel_size (layout, &text_width, &text_height);
      pango_cairo_show_layout (cr, layout);
      g_object_unref (layout);

      cairo_rel_move_to (cr, MARGIN_LEFT + (TEXT_COLUMN_WIDTH+0.25)*72.0, 0);
      layout = gtk_print_context_create_pango_layout (context);
      pango_layout_set_font_description (layout, desc);
      pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
      pango_layout_set_text (layout, text2, -1);
      pango_cairo_show_layout (cr, layout);
      g_object_unref (layout);
      cairo_move_to (cr, xx, yy);

      /* moves back to x=y and move down by one line */
      cairo_rel_move_to (cr, 0, text_height);
      cairo_get_current_point (cr, &xx, &yy);

    }
    g_free(text);

  }

  pango_font_description_free (desc);

#if 0
  /*
   * This puts a rectangle around the entire imagable area and a full
   * size "X" across it.  It is just here to help with debugging page
   * setup stuff.
   */
  cairo_set_line_width (cr, 5.0);

  cairo_move_to (cr, 0, 0);
  cairo_line_to (cr, width, height);
  cairo_stroke (cr);

  cairo_move_to (cr, width, 0);
  cairo_line_to (cr, 0, height);
  cairo_stroke (cr);

  cairo_rectangle (cr, 0, 0, width, height);
  cairo_stroke (cr);
#endif


#ifdef DEBUG
  g_print ("%s():  all done with page #%d\n", __FUNCTION__, page_nr);
#endif

}

static void
end_print (GtkPrintOperation *operation,
	   GtkPrintContext *context,
	   gpointer user_data)
{
  PrintData *data = (PrintData *)user_data;

#ifdef DEBUG
  g_print ("%s():  freeing data\n", __FUNCTION__);
#endif

  g_free (data->date_string);
  g_free (data);

#ifdef DEBUG
  g_print ("%s():  all done\n", __FUNCTION__);
#endif
}




static void
do_page_setup (GtkWidget *do_widget, Wcalc *wcalc)
{
  GtkPageSetup *new_page_setup;

  if (settings == NULL) {
    settings = gtk_print_settings_new ();
  }

  /* FIXME -- this code is duplicated in do_printing */
  if (page_setup == NULL) {
    GtkPaperSize *letter_size;

#ifdef DEBUG
    g_print ("%s():  Creating new default page setup\n", __FUNCTION__);
#endif

    page_setup = gtk_page_setup_new();
    letter_size = gtk_paper_size_new (GTK_PAPER_NAME_LETTER);
    gtk_page_setup_set_orientation (page_setup, GTK_PAGE_ORIENTATION_PORTRAIT);
    gtk_page_setup_set_paper_size (page_setup, letter_size);
    gtk_paper_size_free (letter_size);
  }

  new_page_setup = gtk_print_run_page_setup_dialog (GTK_WINDOW (do_widget),
                                                    page_setup, settings);
  if (page_setup) {
    g_object_unref (page_setup);
  }

  page_setup = new_page_setup;
}

static GtkWidget *
do_printing (GtkWidget *do_widget, Wcalc *wcalc, gchar *filename)
{
  GtkPrintOperation *operation;
  GtkPrintOperationResult res;
  PrintData *data;
  GError *error = NULL;

  operation = gtk_print_operation_new ();


  /* load any print settings from the last time */
  if (settings == NULL) {
    settings = gtk_print_settings_new ();
  }
  gtk_print_operation_set_print_settings (operation, settings);

  /* set the default page setup */
  if (page_setup == NULL) {
    GtkPaperSize *letter_size;

#ifdef DEBUG
    g_print ("%s():  Creating new default page setup\n", __FUNCTION__);
#endif

    page_setup = gtk_page_setup_new();
    letter_size = gtk_paper_size_new (GTK_PAPER_NAME_LETTER);
    gtk_page_setup_set_orientation (page_setup, GTK_PAGE_ORIENTATION_PORTRAIT);
    gtk_page_setup_set_paper_size (page_setup, letter_size);
    gtk_paper_size_free (letter_size);
  }
  gtk_print_operation_set_default_page_setup(operation, page_setup);


  /*
   * store some user data, in particular a pointer to our current
   * wcalc gui.  That will let the callbacks get at all of the
   * data needed to render the outputs.
   */
  data = g_new0 (PrintData, 1);
  data->wcalc = wcalc;
  data->font_size = 10.0;

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


  /* only use the imagable area (inside the margins) */
  gtk_print_operation_set_use_full_page (operation, FALSE);

  /*
   * sets the transformation for the cairo context so we use these
   * units
   */
  gtk_print_operation_set_unit (operation, GTK_UNIT_POINTS);

  /* Run the print dialog */
  if (filename == NULL) {
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

#ifdef DEBUG
    g_print ("%s():  Unreferencing the print operation\n", __FUNCTION__);
#endif
    g_object_unref (operation);

  } else {
#ifdef DEBUG
    g_print ("Exporting PDF to \"%s\"\n", filename);
#endif
    gtk_print_operation_set_export_filename (operation, filename);
    res = gtk_print_operation_run (operation,
				   GTK_PRINT_OPERATION_ACTION_EXPORT,
				   GTK_WINDOW (do_widget), &error);
#ifdef DEBUG
    g_print ("Finished print operation that exported PDF to \"%s\"\n", filename);
#endif

  }


#ifdef DEBUG
  g_print ("%s():  After running the print operation, error = %p\n", __FUNCTION__, error);
#endif

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
    do_printing(wcalc->window, wcalc, NULL);
  }
}


/*
 * data is the Wcalc *
 * action is whats specified in menus.c
 * widget is the GtkMenuItem *
 */
void newprint_pdf_popup(gpointer data,
			guint action,
			GtkWidget *widget)
{
  Wcalc * wcalc;
  GtkWidget *dia;
  gchar *tmps;

  wcalc = WC_WCALC(data);

  if(wcalc->values_in_sync == FALSE) {

    alert("The input and output values are out of sync\n"
	  "Before printing, please press the analyze button\n"
	  "or one of the synthesis buttons in the calculator\n"
	  "to make the input and output values be consistent\n");
    return;
  }

  dia = gtk_file_chooser_dialog_new (_("Export to PDF"),
				     GTK_WINDOW (wcalc->window),
				     GTK_FILE_CHOOSER_ACTION_SAVE,
				     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				     GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
				     NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dia), TRUE);

  if (wcalc->pdf_filename != NULL) {
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dia), wcalc->pdf_filename);
  } else if (wcalc->file_basename != NULL) {

    /* this makes sure we give a default filename if the pdf doesn't
       exist */
    tmps = g_strconcat(wcalc->file_basename, ".pdf", NULL);
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dia), tmps);
    g_free(tmps);

    /* this makes sure we give a default directory */
    tmps = g_strconcat(".", G_DIR_SEPARATOR_S, wcalc->file_basename, ".pdf", NULL);
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dia), tmps);
    g_free(tmps);
  } else {
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dia), "Untitled.pdf");

    tmps = g_strconcat(".", G_DIR_SEPARATOR_S, "Untitled.pdf", NULL);
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER (dia), tmps);
    g_free(tmps);
  }

  if (gtk_dialog_run (GTK_DIALOG (dia)) == GTK_RESPONSE_ACCEPT)
    {
      gchar *filename;
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dia));
      do_printing(wcalc->window, wcalc, filename);
      if(wcalc->pdf_filename != NULL)
        g_free (wcalc->pdf_filename);
      wcalc->pdf_filename = filename;
    }
  gtk_widget_destroy (dia);

}

/*
 * data is the Wcalc *
 * action is whats specified in menus.c
 * widget is the GtkMenuItem *
 */
void page_setup_popup(gpointer data,
		      guint action,
		      GtkWidget *widget)
{
  Wcalc * wcalc;

  wcalc = WC_WCALC(data);

  do_page_setup(wcalc->window, wcalc);

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

