/*      $Id: gtk-units.h,v 1.4 2002/06/25 21:19:27 dan Exp $ */

/*
 * Copyright (c) 2002 Dan McMahill
 * All rights reserved.
 *
 * This code is derived from software written by Dan McMahill
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by Dan McMahill
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
#include "wcalc.h"

typedef struct _WC_UNITS_GUI
{
  /* the composite units */
  composite_units_data *units;

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

  /* ist of the names of the various models */
  GList *menu_num, *menu_den;

} wc_units_gui;
#define WC_UNITS_GUI(x)      ((wc_units_gui *) (x))

typedef struct _WC_LABEL_UNITS
{
  wc_units_gui *units_gui;
  GtkWidget *label;

  double mks_val, disp_val;

} wc_label_units;

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

GtkWidget *units_menu_new(const units_data *units, 
			  int initial,
			  Wcalc *gui,
			  void (*callback)(GtkWidget *, gpointer));

void  set_sf_menu(GtkWidget *menu,
		  const units_data units[],
		  double sf);

GtkWidget *wc_composite_units_menu_new(const composite_units_data *units, 
				       Wcalc *gui,
				       wc_units_gui *ug,
				       void (*callback)(GtkWidget *, gpointer));

wc_label_units *wc_label_units_new(const composite_units_data *units, 
				   Wcalc *gui,
				   void (*callback)(GtkWidget *, gpointer));

void wc_composite_units_menu_changed( GtkWidget *w, gpointer data);

#endif /* __GTK_UNITS_H__ */
