/* $Id: coupled_microstrip_gui.h,v 1.5 2004/08/30 22:59:18 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2004 Dan McMahill
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

#ifndef __COUPLED_MICROSTRIP_GUI_H__
#define __COUPLED_MICROSTRIP_GUI_H__

#include "coupled_microstrip.h"
#include "wcalc.h"

typedef struct COUPLED_MICROSTRIP_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the coupled_microstrip which is being analyzed
   */
  coupled_microstrip_line *line;

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
  GtkWidget *label_keffe, *label_keffo;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;

  /* the radio buttons for z0/k vs z0e/z0o selection */
  GtkWidget *button_z0k, *button_z0ez0o;

} coupled_microstrip_gui;

coupled_microstrip_gui *coupled_microstrip_gui_new(void);
void coupled_microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_COUPLED_MICROSTRIP_GUI(x)      ((coupled_microstrip_gui *) (x))

#endif /*__COUPLED_MICROSTRIP_GUI_H__*/

