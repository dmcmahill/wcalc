/* $Id: bars_gui.c,v 1.4 2009/01/10 20:56:16 dan Exp $ */

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

#ifndef __UTILS_H__
#define __UTILS_H__


/*
 * table = table that we are adding cells to
 * gui = the top wcalc gui
 * text = the static text label
 * label = the entry box
 * units = the units
 * ug = units gui
 * x, y = current x,y location
 */

/*
 * We need to be able to do the following:
 *
 * add an entry with 
 *    - no units
 *    - a new units menu
 *    - attach to an existing units menu
 *
 * add a label with
 *    - no units
 *    - a new units menu
 *    - attach to an existing units menu
 *
 * Labels get updated by the units menus.  Entries do not.
 *
 *
 */

typedef  enum {
    WC_ENTRY_NO_UG, WC_ENTRY_NEW_UG, WC_ENTRY_USE_UG,
    WC_LABEL_NO_UG, WC_LABEL_NEW_UG, WC_LABEL_USE_UG
} wc_table_row_type;

#define wc_table_add_entry_no_units(table, gui, text, entry, x, y) \
    (wc_table_add( (table), (gui), (text), (entry), NULL, NULL, NULL, \
		   (x), (y), WC_ENTRY_NO_UG))

#define wc_table_add_entry_new_units(table, gui, text, entry, units, ug, mks, x, y) \
    (wc_table_add( (table), (gui), (text), (entry), (units), (ug), (mks), \
		   (x), (y), WC_ENTRY_NEW_UG))

#define wc_table_add_entry_attach_units(table, gui, text, entry, units, ug, mks, x, y) \
    (wc_table_add( (table), (gui), (text), (entry), (units), (ug), (mks), \
		   (x), (y), WC_ENTRY_USE_UG))

#define wc_table_add_label_no_units(table, gui, text, entry, x, y) \
    (wc_table_add( (table), (gui), (text), (entry), NULL, NULL, NULL, \
		   (x), (y), WC_LABEL_NO_UG))

#define wc_table_add_label_new_units(table, gui, text, entry, units, ug, mks, x, y) \
    (wc_table_add( (table), (gui), (text), (entry), (units), (ug), (mks), \
		   (x), (y), WC_LABEL_NEW_UG))

#define wc_table_add_label_attach_units(table, gui, text, entry, units, ug, mks, x, y) \
    (wc_table_add( (table), (gui), (text), (entry), (units), (ug), (mks), \
		   (x), (y), WC_LABEL_USE_UG))




void wc_table_add(GtkWidget *table, void * gui, const char *text, 
		  GtkWidget **label, wc_units *units,
		  wc_units_gui **ug, double *mks_val, 
		  int *x, int *y, wc_table_row_type type);

#endif /* __UTILS_H__ */
