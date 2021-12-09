/*
 * Copyright (C) 2002, 2003, 2004, 2021 Dan McMahill
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

#ifndef __GTK_UNITS_H__
#define __GTK_UNITS_H__

#include "config.h"

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "misc.h"
#include "units.h"
#include "wcalc.h"

typedef struct _WC_UNITS_GUI
{
  /* the actual units */
  wc_units *units;

  /* the single scale factor */
  double *sf;

  /* the complete units string */
  char *name;

  /*
   * flag that says we should update the displayed value to reflect
   * new units.
   */
  int sync_value;

  /*
   * list of options menus for the various components of the units
   */
  GtkWidget **opt_menus;

  /* list of the names of the various models */
  GList *menu_num, *menu_den;

  /*
   * list of gtk_label's and gtk_entry's which we want to update when
   * the units are changed
   */
  GList *update_list;

} wc_units_gui;
#define WC_UNITS_GUI(x)      ((wc_units_gui *) (x))

typedef struct _WC_UNITS_MENU_DATA
{
  /* Index into the units array indicating if this is, for example,
   *   nH, uH, mH, etc.
   */
  int ind;

  /* The options menu that this is part of */
  GtkWidget *opt_menu;

} wc_units_menu_data;

#define WC_UNITS_MENU_DATA(x)      ((wc_units_menu_data *) (x))

typedef struct _WC_UNITS_UPDATE_ITEM
{
  /* pointer to the widget that need updating */
  GtkWidget *widget;

  /* what sort of GtkWidget is it? */
  enum {LABEL, ENTRY, UNITS_LABEL, NONE} type;

  /* pointer to the value in mks */
  double *mks_val;

  /* pointer to the scale factor */
  double *sf;

  /* pointer to the units string for the value */
  char **units_str;

  /* sprintf() format string (typically something like "%6.2f") */
  char *fmt_string;

  /*
   * do we actually update the widget or only the scale factor and scale
   * factor string?
   */
  int update;

} wc_units_update_item;

GtkWidget *wc_units_menu_new(wc_units *units,
			     Wcalc *gui,
			     wc_units_gui **ug);

void wc_units_menu_changed( GtkWidget *w, gpointer data);


void  wc_units_attach(wc_units_gui *ug,
		      GtkWidget *widget,
		      double *mks_val,
		      double *sf,
		      char **units_str,
		      const char *fmt_string,
		      int update,
		      int type);

#define wc_units_attach_units(ug, mks_val, sf, units_str)             \
       (wc_units_attach((ug), NULL, (mks_val), (sf), (units_str),     \
       "", 0, NONE))

/* attach a gtk_entry which will display the value of our variable */
#define wc_units_attach_entry(ug, widget, mks_val, sf, units_str,     \
       fmt_string, update)                                            \
       (wc_units_attach((ug), (widget), (mks_val), (sf), (units_str), \
       (fmt_string), update, ENTRY))

/* attach a gtk_label which will display the value of our variable */
#define wc_units_attach_label(ug, widget, mks_val, sf, units_str,     \
       fmt_string, update)                                            \
       (wc_units_attach((ug), (widget), (mks_val), (sf), (units_str), \
       (fmt_string), update, LABEL))

/* attach a gtk_label which will display the units of our variable */
#define wc_units_attach_units_label(ug, widget)                       \
       (wc_units_attach((ug), (widget), NULL, NULL, NULL, NULL, 1,    \
       UNITS_LABEL))

void wc_units_menu_init(Wcalc *);

#endif /* __GTK_UNITS_H__ */
