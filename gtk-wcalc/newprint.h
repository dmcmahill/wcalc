/* $Id: newprint.h,v 1.2 2009/01/13 20:35:18 dan Exp $ */

/*
 * Copyright (C) 2009 Dan McMahill
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

#ifndef __NEWPRINT_H__
#define __NEWPRINT_H__

#include "units.h"

typedef struct printValue
{
  gchar *name;
  union {
    gdouble fval;
    gint ival;
    gchar *sval;
  } val;
  
  enum {FLOAT, INT, STRING} type;

  wc_units *units;
} PrintValue;

GList * wc_add_double_print(gchar * name, double val, wc_units *units, GList *list);
GList * wc_add_int_print(gchar * name, int val, wc_units *units, GList *list);
GList * wc_add_string_print(gchar * name, gchar * val, wc_units *units, GList *list);
void wc_print_value_free(PrintValue * val);

/* data should be a pointer to the current wcalc */
void newprint_popup(gpointer data,
		    guint action,
		    GtkWidget *widget);


#endif /*__NEWPRINT_H__*/




