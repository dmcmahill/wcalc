/*
 * Copyright (C) 2020 Dan McMahill
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

#ifndef __RODS_GUI_H__
#define __RODS_GUI_H__

#include "rods.h"
#include "wcalc.h"
#include "gtk-units.h"

typedef struct RODS_GUI
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the rods which is being analyzed
   */
  rods *b;

  /*
   * Frequency/Units/Model vbox and its contents
   */
  GtkWidget *units_vbox;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;

  GtkWidget *text_d1,*text_l1;
  GtkWidget *text_d2,*text_l2;
  GtkWidget *text_offset,*text_distance;

  GtkWidget *text_rho;
  GtkWidget *text_freq;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;

  GtkWidget *label_L1, *label_L2, *label_R1, *label_R2;
  GtkWidget *label_M, *label_k;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

} rods_gui;

rods_gui *rods_gui_new(void);
void rods_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_RODS_GUI(x)      ((rods_gui *) (x))

#endif /*__RODS_GUI_H__*/

