
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


void wc_table_add(GtkWidget *table, void * gui, const char *text,
			 GtkWidget **label, wc_units *units,
			 wc_units_gui **ug, double *mks_val,
			 int *x, int *y, wc_table_row_type type)
{
    GtkWidget *name;
    int isentry = 0;

    /* The static label on the left (i.e. "Width" or "Height") */
    name = gtk_label_new( text );
    gtk_table_attach(GTK_TABLE(table), name, *x, *x+1, *y, *y+1,
		     0, 0, WC_XPAD, WC_YPAD);
    gtk_widget_show(name);

    /* The text entry or dynamic label */
    switch(type) {
    case WC_ENTRY_NO_UG:
    case WC_ENTRY_NEW_UG:
    case WC_ENTRY_USE_UG:
	*(label) = gtk_entry_new_with_max_length( WC_ENTRYLENGTH );
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
    gtk_widget_set_usize(GTK_WIDGET(*label),WC_WIDTH,0);

    if( isentry ) {
	gtk_signal_connect (GTK_OBJECT (*label), "changed",
			    GTK_SIGNAL_FUNC (wcalc_save_needed), gui);
	gtk_signal_connect (GTK_OBJECT (*label), "changed",
			    GTK_SIGNAL_FUNC (vals_changedCB), gui);
    }
    gtk_widget_show(*label);


    /* add the units if needed */
    switch(type) {

    case WC_ENTRY_NO_UG:
    case WC_LABEL_NO_UG:
	/* no units */
	break;

    case WC_ENTRY_NEW_UG:
    case WC_LABEL_NEW_UG:
	/* new units */
	assert(units != NULL);
	name = wc_units_menu_new(units, WC_WCALC(gui), ug);
	gtk_table_attach(GTK_TABLE(table), name,
			 *x+2, *x+3, *y, *y+1,
			 GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);

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
	name = gtk_label_new("");
	gtk_table_attach(GTK_TABLE(table), name, *x+2, *x+3, *y, *y+1,
			 GTK_EXPAND|GTK_FILL, 0, WC_XPAD, WC_YPAD);
	gtk_misc_set_alignment(GTK_MISC(name),0,0);

	/* attach it so that it gets updated */
	wc_units_attach_units_label(*ug, name);

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

