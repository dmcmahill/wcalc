/* $Id: microstrip_gui.h,v 1.5 2004/07/24 04:07:24 dan Exp $ */

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

#ifndef __MICROSTRIP_GUI_H__
#define __MICROSTRIP_GUI_H__

#include "microstrip.h"
#include "wcalc.h"

typedef struct MICROSTRIP_GUI 
{
  /* The parent */
  Wcalc wcalc;

  /* Model Dependent Elements */

  /*
   * the microstrip which is being analyzed
   */
  microstrip_line *line;

  /* 
   * Frequency/Units/Model vbox and its contents 
   */
  GtkWidget *units_vbox;

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
  GtkWidget *text_w,*text_l,*text_h,*text_er,*text_tand;
  GtkWidget *text_z0,*text_elen,*text_tmet,*text_rho,*text_rough;
  GtkWidget *text_freq;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_delay,*label_loss,*label_losslen,*label_depth;
  GtkWidget *label_deltal,*label_Ls,*label_Rs,*label_Cp,*label_Gp;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;
  GtkWidget *text_status;

  /*
   * list of labels which change when we change physical units
   */
  GList *phys_units_text;

} microstrip_gui;

microstrip_gui *microstrip_gui_new(void);
void microstrip_gui_init(Wcalc *wcalc, GtkWidget *main_vbox, FILE *fp);


#define WC_MICROSTRIP_GUI(x)      ((microstrip_gui *) (x))

#endif /*__MICROSTRIP_GUI_H__*/

