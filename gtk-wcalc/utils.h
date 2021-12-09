
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

/* 
 * Entry
 */

#define wc_table_add_entry_no_units(table, gui, text, entry, mks, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, NULL, (entry), NULL, NULL, (mks), \
		   (x), (y), WC_ENTRY_NO_UG))

#define wc_table_add_entry_fixed_units(table, gui, text, utext, entry, mks, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, (utext), (entry), NULL, NULL, (mks), \
		   (x), (y), WC_ENTRY_NO_UG))

#define wc_table_add_entry_new_units(table, gui, text, entry, units, ug, mks, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, NULL, (entry), (units), (ug), (mks), \
		   (x), (y), WC_ENTRY_NEW_UG))

#define wc_table_add_entry_attach_units(table, gui, text, entry, units, ug, mks, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, NULL, (entry), (units), (ug), (mks), \
		   (x), (y), WC_ENTRY_USE_UG))

/* 
 * The _wentry_ ones use a widget instead of text for the label on the
 * left.  Think radio button.
 */
#define wc_table_add_wentry_no_units(table, gui, wtext, entry, mks, x, y) \
  (wc_table_add( (table), (gui), NULL, (wtext), NULL, (entry), NULL, NULL, (mks), \
		   (x), (y), WC_ENTRY_NO_UG))

#define wc_table_add_wentry_fixed_units(table, gui, wtext, utext, entry, mks, x, y) \
  (wc_table_add( (table), (gui), NULL, (wtext), (utext), (entry), NULL, NULL, (mks), \
		   (x), (y), WC_ENTRY_NO_UG))

#define wc_table_add_wentry_new_units(table, gui, wtext, entry, units, ug, mks, x, y) \
  (wc_table_add( (table), (gui), NULL, (wtext), NULL, (entry), (units), (ug), (mks), \
		   (x), (y), WC_ENTRY_NEW_UG))

#define wc_table_add_wentry_attach_units(table, gui, wtext, entry, units, ug, mks, x, y) \
  (wc_table_add( (table), (gui), NULL, (wtext), NULL, (entry), (units), (ug), (mks), \
		   (x), (y), WC_ENTRY_USE_UG))

/* 
 * Labels
 */

#define wc_table_add_label_no_units(table, gui, text, entry, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, NULL, (entry), NULL, NULL, NULL, \
		   (x), (y), WC_LABEL_NO_UG))

#define wc_table_add_label_fixed_units(table, gui, text, utext, entry, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, (utext), (entry), NULL, NULL, NULL, \
		   (x), (y), WC_LABEL_NO_UG))

#define wc_table_add_label_new_units(table, gui, text, entry, units, ug, mks, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, NULL, (entry), (units), (ug), (mks), \
		   (x), (y), WC_LABEL_NEW_UG))

#define wc_table_add_label_attach_units(table, gui, text, entry, units, ug, mks, x, y) \
  (wc_table_add( (table), (gui), (text), NULL, NULL, (entry), (units), (ug), (mks), \
		   (x), (y), WC_LABEL_USE_UG))




void wc_table_add(GtkWidget *table, void * gui, const char *text,
                  GtkWidget *wtext,
                  const char *utext,
		  GtkWidget **label, wc_units *units,
		  wc_units_gui **ug, double *mks_val,
		  int *x, int *y, wc_table_row_type type);

/*
 * create a new entry with fixed width and hook up the standard
 * wcalc_save_needed and vals_changedCB event handler functions
 */
GtkWidget *wc_new_entry(gpointer data);

/*
 * attach wcalc_save_needed and vals_changedCB
 * to the "changed" event for an entry (or other widget)
 */

void wc_entry_connect( GtkWidget *w, gpointer data);
/*
 * attach wcalc_save_needed and the specified callback to the
 * "clicked" event for a button
 */
void wc_button_connect( GtkWidget *w, gpointer cb, gpointer data);

/*
 * use to init the picture region in each GUI
 * example:
 * wc_picture_init(wcalc, picture_vbox, air_coil);
 */
void wc_picture_init(Wcalc *wcalc, GtkWidget *parent, const char **xpm);

/*
 * used to create/attach things like the analyze or synthesize buttons
 */
void wc_table_add_button(GtkWidget *table, const char *text, const char *tip, gpointer cb, gpointer data, int x, int y);
 
#endif /* __UTILS_H__ */
