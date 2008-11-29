/* $Id: coupled_stripline_gui.h,v 1.2 2006/02/13 19:20:22 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004, 2006 Dan McMahill
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

#ifndef __COUPLED_STRIPLINE_GUI_H__
#define __COUPLED_STRIPLINE_GUI_H__

#include "coupled_stripline.h"
#include "wcalc.h"

typedef struct COUPLED_STRIPLINE_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the coupled_stripline which is being analyzed
   */
  coupled_stripline_line *line;

  /* 
   * Frequency/Units/Model vbox and its contents 
   */
  GtkWidget *units_vbox;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_w,*text_s,*text_l,*text_h,*text_er,*text_tand;
  GtkWidget *text_z0, *text_k;
  GtkWidget *text_z0e, *text_z0o;
  GtkWidget *text_elen,*text_tmet,*text_rho,*text_rough;
  GtkWidget *text_freq;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_delay;
  GtkWidget *label_loss_ev, *label_losslen_ev;
  GtkWidget *label_loss_od, *label_losslen_od;
  GtkWidget *label_depth;
  GtkWidget *label_deltal_ev, *label_deltal_od;
  GtkWidget *label_Lev, *label_Rev, *label_Cev, *label_Gev;
  GtkWidget *label_Lodd, *label_Rodd, *label_Codd, *label_Godd;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

  /* the radio buttons for z0/k vs z0e/z0o selection */
  GtkWidget *button_z0k, *button_z0ez0o;

} coupled_stripline_gui;

coupled_stripline_gui *coupled_stripline_gui_new(void);
void coupled_stripline_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_COUPLED_STRIPLINE_GUI(x)      ((coupled_stripline_gui *) (x))

#endif /*__COUPLED_STRIPLINE_GUI_H__*/

