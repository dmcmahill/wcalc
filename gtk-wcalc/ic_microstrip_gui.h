
/*
 * Copyright (C) 1999, 2000, 2001, 2002 Dan McMahill
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

#ifndef __IC_MICROSTRIP_GUI_H__
#define __IC_MICROSTRIP_GUI_H__

#include "ic_microstrip.h"
#include "wcalc.h"

typedef struct IC_MICROSTRIP_GUI
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the ic_microstrip which is being analyzed
   */
  ic_microstrip_line *line;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_w,*text_l,*text_tox,*text_eox;
  GtkWidget *text_h,*text_es,*text_sigmas;
  GtkWidget *text_Ro,*text_elen,*text_tmet,*text_rho,*text_rough;
  GtkWidget *text_freq;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_delay,*label_loss,*label_losslen;
  GtkWidget *label_met_depth,*label_subs_depth;
  GtkWidget *label_keff;
  GtkWidget *label_Lmis,*label_Rmis,*label_Cmis,*label_Gmis;
  GtkWidget *label_z0;
  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

} ic_microstrip_gui;

ic_microstrip_gui *ic_microstrip_gui_new(void);
void ic_microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_IC_MICROSTRIP_GUI(x)      ((ic_microstrip_gui *) (x))

#endif /*__IC_MICROSTRIP_GUI_H__*/

