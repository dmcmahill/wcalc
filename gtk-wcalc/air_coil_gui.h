/* $Id: air_coil_gui.h,v 1.1 2001/10/05 00:50:20 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001 Dan McMahill
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
 *        This product includes software developed Dan McMahill
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

  /* frequency units */
  GtkWidget *combo_funits;

  /* model selection */
  GtkWidget *combo_model;

  /* physical units */
  GtkWidget *combo_punits;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_Nf,*text_dia,*text_len,*text_fill,*text_AWGf,*text_rho,*text_L;

  /* the radio buttons for fill/length selection */
  GtkWidget *len_button,*fill_button;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_Q,*label_Qfreq,*label_SRF,*label_SRF_units;
  GtkWidget *label_Lmax,*label_fill;
  GtkWidget *label_Lmax_units;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;
  GtkWidget *text_status;

  /*
   * list of labels which change when we change physical units
   */
  GList *phys_units_text;

} air_coil_gui;

air_coil_gui *air_coil_gui_new(void);
void air_coil_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_AIR_COIL_GUI(x)      ((air_coil_gui *) (x))

#endif /*__AIR_COIL_GUI_H__*/

