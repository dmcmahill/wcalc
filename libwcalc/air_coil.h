/* $Id: air_coil.h,v 1.4 2002/06/12 11:30:22 dan Exp $ */

/*
 * Copyright (c) 2001, 2002, 2004 Dan McMahill
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

#ifndef __AIR_COIL_H__
#define __AIR_COIL_H__

#include "units.h"

typedef struct AIR_COIL_COIL
{

  /* Number of turns */
  double Nf;

  /* length of coil (meters) */
  double len;

  /* wire gauge */
  double AWGf;

  /* resistivity  (ohm-meters) */
  double rho;

  /* inside diameter of coil (meters) */
  double dia;

  /* inductance (Henries) */
  double L;

  /* inductance when the length is at a minimum (Henries) */
  double Lmax;

  /* ratio of length to minimum length */
  double fill;

  /* Q at freq (Hertz) */
  double Q;
  double freq;


  /* Self resonant frequency (Hertz) */
  double SRF;

  /* 
   * use the fill to calculate length instead of length to calculate
   * fill 
   */
  int use_fill;

  /* user units */
  wc_units *units_len, *units_dia;
  wc_units *units_L, *units_SRF;
  wc_units *units_rho;
  wc_units *units_freq;

} air_coil_coil;


int air_coil_calc(air_coil_coil *coil, double f);
int air_coil_syn(air_coil_coil *coil, double f, int flag);

void air_coil_free(air_coil_coil * coil);
air_coil_coil *air_coil_new(void);


/*
 * Flags for synthesis
 */

#define AIRCOILSYN_NMIN    0    /* Synthesize for minimum N         */
#define AIRCOILSYN_NFIX    1    /* Synthesize length with fixed N   */


#endif /*__AIR_COIL_H__*/
