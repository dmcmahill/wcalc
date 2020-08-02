
/*
 * Copyright (C) 2006 Dan McMahill
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

#ifndef __COPLANAR_GUI_H__
#define __COPLANAR_GUI_H__

#include "coplanar.h"
#include "wcalc.h"

typedef struct COPLANAR_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the coplanar which is being analyzed
   */
  coplanar_line *line;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_w, *text_s, *text_l, *text_h, *text_er, *text_tand;
  GtkWidget *text_z0, *text_elen, *text_tmet, *text_rho, *text_rough;
  GtkWidget *text_freq, *button_gnd;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_delay,*label_loss,*label_losslen,*label_depth;
  GtkWidget *label_deltal,*label_Ls,*label_Rs,*label_Cp,*label_Gp;
  GtkWidget *label_keff;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

  /*
   * list of labels which change when we change physical units
   */
  GList *phys_units_text;

} coplanar_gui;

coplanar_gui *coplanar_gui_new(void);
void coplanar_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_COPLANAR_GUI(x)      ((coplanar_gui *) (x))

#endif /*__COPLANAR_GUI_H__*/

