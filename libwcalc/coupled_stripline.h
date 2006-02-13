/* $Id: coupled_stripline.h,v 1.1 2006/02/12 06:16:26 dan Exp $ */

/*
 * Copyright (c) 1999, 2000, 2001, 2002, 2004, 2006 Dan McMahill
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

#ifndef __COUPLED_STRIPLINE_H_
#define __COUPLED_STRIPLINE_H_

#include "stripline.h"
#include "units.h"


typedef struct COUPLED_STRIPLINE_LINE
{

  /* length, width, and spacing */
  double l;
  double w;
  double s;

  /* characteristic impedance (ohms) and coupling factor */
  double z0, k;

  /* even and odd mode impedance */
  double z0e;
  double z0o;

  /* flag which says to use z0/k instead of z0e/z0o for synthesis */
  int use_z0k;

  /* electrical length (degrees) */
  double len;


  /* open circuit end line correction */
  double deltale, deltalo;

  double loss_ev, loss_odd, losslen_ev, losslen_odd, skindepth;

  /* incremental circuit model */
  double Lev, Rev, Cev, Gev;
  double Lodd, Rodd, Codd, Godd;

  /* analysis frequency */
  double freq;

  stripline_subs *subs;

  /* user units */
  wc_units *units_lwst;
  wc_units *units_len;

  wc_units *units_freq;
  wc_units *units_loss;
  wc_units *units_losslen;
  wc_units *units_rho;
  wc_units *units_rough;
  wc_units *units_delay;
  wc_units *units_depth;
  wc_units *units_deltal;

  wc_units *units_L, *units_R, *units_C, *units_G;

} coupled_stripline_line;


int coupled_stripline_calc(coupled_stripline_line *line, double f);
int coupled_stripline_syn(coupled_stripline_line *line, double f);

void coupled_stripline_line_free(coupled_stripline_line * line);
coupled_stripline_line *coupled_stripline_line_new(void);


#endif /*__COUPLED_STRIPLINE_H_*/
