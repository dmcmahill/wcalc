/* $Id: coax_gui.h,v 1.14 2004/08/30 22:59:17 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2004 Dan McMahill
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

#ifndef __COAX_GUI_H__
#define __COAX_GUI_H__

#include "coax.h"
#include "wcalc.h"
#include "gtk-units.h"

typedef struct COAX_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the coax which is being analyzed
   */
  coax_line *line;

  /* 
   * Frequency/Units/Model vbox and its contents 
   */
  GtkWidget *units_vbox;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_a,*text_b,*text_c,*text_tshield;

  GtkWidget *text_len;

  GtkWidget *text_er,*text_tand,*text_emax;
  GtkWidget *text_z0,*text_elen;

  GtkWidget *text_fc;

  GtkWidget *text_freq;
  GtkWidget *text_rho_a,*text_rho_b;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_delay;
  GtkWidget *label_loss,*label_losslen;
  GtkWidget *label_closs, *label_dloss;
  GtkWidget *label_L,*label_R,*label_C,*label_G;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

} coax_gui;

coax_gui *coax_gui_new(void);
void coax_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_COAX_GUI(x)      ((coax_gui *) (x))

#endif /*__COAX_GUI_H__*/

