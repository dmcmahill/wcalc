/*      $Id: wcalc.h,v 1.3 2001/09/15 23:56:40 dan Exp $ */

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

#ifndef __WCALC_H_
#define __WCALC_H_

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "microstrip.h"

typedef struct WCALC
{
  /* 
   * Flag that says all initialization is done.  This is used to mask the effects
   * of certain callbacks which get triggered during initialization.
   */
  int init_done;

  /*
   * The main window
   */
  GtkWidget *window;

  /* 
   * Frequency/Units/Model vbox and its contents 
   */
  GtkWidget *units_vbox;

  /* frequency entry */
  GtkWidget *entry_freq;

  /* frequency units */
  GtkWidget *combo_funits;

  /* model selection */
  GtkWidget *combo_model;

  /* physical units */
  GtkWidget *combo_punits;


  /*
   * Frequency vbox and its contents
   */
  GtkWidget *text_freq;

  /*
   * Values vbox and its contents
   */
  GtkWidget *values_vbox;
  GtkWidget *text_W,*text_L,*text_Z0,*text_elen;

  /*
   * Outputs vbox and its contents
   */
  GtkWidget *outputs_vbox;
  GtkWidget *label_keff,*label_loss,*label_losslen,*label_skindepth;


  /*
   * Substrate vbox and its contents
   */
  GtkWidget *substrate_vbox;
  GtkWidget *text_H,*text_er,*text_rho,*text_rough,*text_tmet,*text_tand;

  /*
   * Picture vbox and its contents
   */
  GtkWidget *picture_vbox;
  GtkWidget *text_status;

  /*
   * list of labels which change when we change physical units
   */
  GList *phys_units_text;


  /*
   * the microstrip line which is being analyzed
   */
  microstrip_line *line;

} Wcalc;



#endif __WCALC_H_
