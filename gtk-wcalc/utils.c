/*
 * Copyright (c) 2009, 2021 Dan McMahill
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
#include "gtk-units.h"

#include "misc.h"
#include "units.h"

#include "utils.h"

#include "wcalc.h"

#ifdef DMALLOC
#include <dmalloc.h>
#endif

void wc_table_add_button(GtkWidget *table, const char *text, const char *tip, gpointer cb, gpointer data, int x, int y)
{
  GtkWidget *button;
  
  button = gtk_button_new_with_label (text);
  gtk_widget_set_tooltip_text( button, tip);
  wc_button_connect( button, cb, data);
  gtk_table_attach(GTK_TABLE(table), button, x, x+1, y, y+1, 0, 0, WC_XPAD, WC_YPAD);
  gtk_widget_show(button);
}

void wc_table_add(GtkWidget *table, void * gui, const char *text,
                  GtkWidget *wtext,
                  const char *utext,
                  GtkWidget **label, wc_units *units,
                  wc_units_gui **ug, double *mks_val,
                  int *x, int *y, wc_table_row_type type)
{
    GtkWidget *name;
    int isentry = 0;

    /* The static label on the left (i.e. "Width" or "Height") */
    if(text != NULL) {
      name = gtk_label_new( text );
    } else {
      name = wtext;
    }
    assert(name != NULL);
    gtk_table_attach(GTK_TABLE(table), name, *x, *x+1, *y, *y+1,
		     0, 0, WC_XPAD, WC_YPAD);
    gtk_widget_show(name);

    /* The text entry or dynamic label */
    switch(type) {
    case WC_ENTRY_NO_UG:
    case WC_ENTRY_NEW_UG:
    case WC_ENTRY_USE_UG:
        *(label) = wc_new_entry(gui);
	gtk_entry_set_text(GTK_ENTRY(*label), WC_OUTPUT_TEXT);
	isentry = 1;
	break;

    case WC_LABEL_NO_UG:
    case WC_LABEL_NEW_UG:
    case WC_LABEL_USE_UG:
	*(label) = gtk_label_new( WC_OUTPUT_TEXT );
	break;

    default:
	fprintf(stderr, "%s():  type = %d which is invalid.  Please report this bug\n",
		__FUNCTION__, type);
	exit(1);
    }

    /* add the entry or dynamic label to the table */
    gtk_table_attach (GTK_TABLE(table), *label, *x+1, *x+2, *y, *y+1,
		      0, 0, WC_XPAD, WC_YPAD);
    gtk_widget_show(*label);


    /* add the units if needed */
    switch(type) {

    case WC_ENTRY_NO_UG:
    case WC_LABEL_NO_UG:
	/* no units */
      if( utext != NULL) {
	name = gtk_label_new(utext);
	gtk_table_attach(GTK_TABLE(table), name, *x+2, *x+3, *y, *y+1,
			 GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
	gtk_misc_set_alignment(GTK_MISC(name),0,0);
        gtk_widget_show(name);
      }
	break;

    case WC_ENTRY_NEW_UG:
    case WC_LABEL_NEW_UG:
	/* new units */
	assert(units != NULL);
	name = wc_units_menu_new(units, WC_WCALC(gui), ug);
	gtk_table_attach(GTK_TABLE(table), name,
			 *x+2, *x+3, *y, *y+1,
			 GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
        gtk_widget_show(name);

	/* now attach to our numeric value */
	if(isentry) {
	    wc_units_attach_entry(*ug, *label, mks_val,
				  NULL, NULL, WC_FMT_G, 0);
	} else {
	    wc_units_attach_label(*ug, *label, mks_val,
				  NULL, NULL, WC_FMT_G, 1);
	}
	break;


    case WC_ENTRY_USE_UG:
    case WC_LABEL_USE_UG:
	/* use existing units */

	/* create the label (Henries/Meter for example) */
	name = gtk_label_new("existing UG");
	gtk_table_attach(GTK_TABLE(table), name, *x+2, *x+3, *y, *y+1,
			 GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
	gtk_misc_set_alignment(GTK_MISC(name),0,0);

	/* attach it so that it gets updated */
	wc_units_attach_units_label(*ug, name);
        gtk_widget_show(name);
        
	/* now attach to our numeric value */
	if(isentry) {
	    wc_units_attach_entry(*ug, *label, mks_val,
				  NULL, NULL, WC_FMT_G, 0);
	} else {
	    wc_units_attach_label(*ug, *label, mks_val,
				  NULL, NULL, WC_FMT_G, 1);
	}
	break;

    default:
	fprintf(stderr, "%s():  type = %d which is invalid.  Please report this bug\n",
		__FUNCTION__, type);
	exit(1);

    }

    (*y)++;
}

GtkWidget *wc_new_entry(gpointer data)
{
    GtkWidget *new;

    new = gtk_entry_new();
    gtk_entry_set_max_length (GTK_ENTRY(new), WC_ENTRYLENGTH);
    gtk_widget_set_size_request( new, WC_WIDTH, -1);

    wc_entry_connect( new, data);

    return new;
}

void wc_entry_connect( GtkWidget *w, gpointer data)
{
    g_signal_connect( G_OBJECT( w ), "changed",
                    G_CALLBACK(wcalc_save_needed), data);
    g_signal_connect( G_OBJECT( w ), "changed",
                    G_CALLBACK(vals_changedCB), data);
}

void wc_button_connect( GtkWidget *w, gpointer cb, gpointer data)
{
  g_signal_connect( G_OBJECT( w ), "clicked",
                    G_CALLBACK(wcalc_save_needed),
                    data);
  g_signal_connect( G_OBJECT( w ), "clicked",
                    G_CALLBACK(cb),
                    data);
}

void wc_picture_init(Wcalc *wcalc, GtkWidget *parent, const char **xpm)
{
  GtkWidget *my_hbox;
  GtkWidget *frame;
  GdkPixbuf *pixbuf;
  GtkWidget *image;

  frame = gtk_frame_new(NULL);
  gtk_container_add(GTK_CONTAINER(parent), frame);
  gtk_frame_set_label_align( GTK_FRAME(frame), 1.0, 0.0);
  gtk_frame_set_shadow_type( GTK_FRAME(frame), GTK_SHADOW_ETCHED_OUT);
  gtk_widget_show(frame);


  my_hbox = gtk_hbox_new (FALSE, 1);
  gtk_container_set_border_width (GTK_CONTAINER (my_hbox), 1);
  gtk_container_add (GTK_CONTAINER (frame), my_hbox);
  gtk_widget_show (my_hbox);



  /* now for the pixmap from gdk */
  pixbuf = gdk_pixbuf_new_from_xpm_data(xpm);
  image = gtk_image_new_from_pixbuf(pixbuf);

  gtk_box_pack_start (GTK_BOX (my_hbox), image, FALSE, FALSE, 0);
  gtk_widget_show( image );


  wcalc->text_status = gtk_label_new( _("Values Out Of Sync") );
  gtk_box_pack_start (GTK_BOX (my_hbox), wcalc->text_status, FALSE, FALSE, 0);
  gtk_widget_show (wcalc->text_status);

}

