/* $Id: coax_gui.h,v 1.13 2004/07/23 22:11:55 dan Exp $ */

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

