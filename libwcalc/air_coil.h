/* $Id: air_coil.h,v 1.5 2004/07/26 01:09:16 dan Exp $ */

/*
 * Copyright (C) 2001, 2002, 2004 Dan McMahill
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
