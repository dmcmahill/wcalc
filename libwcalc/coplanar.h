/*      $Id: coplanar.h,v 1.1 2006/01/06 15:08:51 dan Exp $ */

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
 
#ifndef __COPLANAR_H_
#define __COPLANAR_H_


#include "units.h"

typedef struct COPLANAR_SUBS
{
  double h, er, tmet, rho, rough, tand;

} coplanar_subs;

typedef struct COPLANAR_LINE
{

  /* length and width */
  double l;
  double w;

  /* spacing */
  double s;

  /* characteristic impedance (ohms) */
  double z0;

  /* effective relative dielectric constant */
  double keff;

  /* electrical length (degrees) */
  double len;

  /* delay (seconds) */
  double delay;

  /* open end length correction */
  double deltal;

  double loss, losslen, skindepth;
  double lc, ld;

  /* incremental circuit model */
  double Ls, Rs, Cs, Gs;

  /* the actual characteristic impedance is Ro + j Xo */
  double Ro, Xo;

  /* frequency of analysis (Hertz) */
  double freq;

  wc_units *units_lwht;
  wc_units *units_L, *units_R, *units_C, *units_G;
  wc_units *units_len;
  wc_units *units_freq;
  wc_units *units_loss;
  wc_units *units_losslen;
  wc_units *units_rho;
  wc_units *units_rough;
  wc_units *units_delay;
  wc_units *units_depth;
  wc_units *units_deltal;

  coplanar_subs *subs;

} coplanar_line;


int coplanar_calc(coplanar_line *line, double f);
int coplanar_syn(coplanar_line *line, double f, int flag);

void coplanar_line_free(coplanar_line * line);
coplanar_line *coplanar_line_new(void);
coplanar_subs *coplanar_subs_new(void);

/*
 * Flags for synthesis
 */

#define CPWSYN_W    0    /* Synthesize the metal width         */
#define CPWSYN_H    1    /* Synthesize the substrate thickness */
#define CPWSYN_ER   2    /* Synthesize the dielectric constant */
#define CPWSYN_S    3    /* Synthesize the spacing */
#define CPWSYN_L    4    /* Synthesize the length */



#endif /*__COPLANAR_H_*/
