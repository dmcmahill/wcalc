/* $Id: air_coil_gui.h,v 1.8 2008/11/29 20:41:40 dan Exp $ */

/*
 * Copyright (C) 1999, 2000, 2001, 2002, 2005, 2012 Dan McMahill
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

#ifndef __AIR_COIL_GUI_H__
#define __AIR_COIL_GUI_H__

#include "air_coil.h"
#include "wcalc.h"

typedef struct AIR_COIL_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the air_coil  which is being analyzed
   */
  air_coil_coil *coil;

  /* 
   * Frequency/Units/Model vbox and its contents 
   */
  GtkWidget *units_vbox;

  /* frequency entry */
  GtkWidget *text_freq;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_Nf,*text_dia,*text_len,*text_fill;
  GtkWidget *text_AWGf,*text_wire_diameter, *text_rho,*text_L;

  /* the radio buttons for fill/length selection */
  GtkWidget *len_button,*fill_button;

  /* the radio buttons for AWG/wire diameter selection */
  GtkWidget *awg_button, *wire_diameter_button;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_Q, *label_Qfreq, *label_SRF;
  GtkWidget *label_Lmax,*label_fill;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;


  /*
   * The buttons
   */
  GtkWidget *button_analyze, *button_synth_N, *button_synth_ID, *button_synth_L;

} air_coil_gui;

air_coil_gui *air_coil_gui_new(void);
void air_coil_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_AIR_COIL_GUI(x)      ((air_coil_gui *) (x))

#endif /*__AIR_COIL_GUI_H__*/

