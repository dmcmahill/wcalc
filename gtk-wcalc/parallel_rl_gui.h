/* $Id: parallel_rl_gui.h,v 1.1 2009/02/10 05:00:15 dan Exp $ */

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

#ifndef __PARALLEL_RL_GUI_H__
#define __PARALLEL_RL_GUI_H__

#include "parallel_rl.h"
#include "wcalc.h"
#include "gtk-units.h"

typedef struct PARALLEL_RL_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the parallel_rl which is being analyzed
   */
  parallel_rl *b;

  /* 
   * Frequency/Units/Model vbox and its contents 
   */
  GtkWidget *units_vbox;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;

  GtkWidget *text_Rs,*text_Ls,*text_Qs;
  GtkWidget *text_Rp,*text_Lp,*text_Qp;

  GtkWidget *text_freq;

  /* the radio buttons for Q vs R selection */
  GtkWidget *button_use_Q, *button_use_R;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;


  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

} parallel_rl_gui;

parallel_rl_gui *parallel_rl_gui_new(void);
void parallel_rl_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_PARALLEL_RL_GUI(x)      ((parallel_rl_gui *) (x))

#endif /*__PARALLEL_RL_GUI_H__*/

