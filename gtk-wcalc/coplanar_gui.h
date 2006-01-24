/* $Id: coplanar_gui.h,v 1.1 2006/01/09 20:30:45 dan Exp $ */

/*
 * Copyright (c) 2006 Dan McMahill
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
  GtkWidget *text_freq;

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

